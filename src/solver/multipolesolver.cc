#include "solver/forcesolver.hh"

#include "benchdec.hh"
#include "global.hh"
#include "octree/octree.hh"
#include "octree/octreenode.hh"

#include <stack>

void Multipolesolver::solve(const unsigned int numparticles,
                            const array_t& xpos, const array_t& ypos,
                            const array_t& zpos, const array_t& masses,
                            array_t& forcex, array_t& forcey, array_t& forcez,
                            const precision_t softening, const Extent extent) {
  // create tree first
  CCPP::BENCH::start(B_TREEGEN);
  _xpos = xpos;
  _ypos = ypos;
  _zpos = zpos;
  _masses = masses;
  _extent = extent;
  _numparticles = numparticles;

  createTree(_simenv._octreeleafnodesize);
  CCPP::BENCH::stop(B_TREEGEN);

  CCPP::BENCH::start(B_MULTIPOLE);
  const std::array<oct::Octreenode*, 8> rootchildren(
      *_octree->getroot()->getchildren());

#pragma omp parallel for
  for (unsigned particle_i = 0; particle_i < numparticles; ++particle_i) {
    // TODO(dave): get opening angle
    // TODO(dave): iterate over highest possible node
    // TODO(dave): get force
  }
  CCPP::BENCH::stop(B_MULTIPOLE);
}

void Multipolesolver::createTree(const unsigned int leafnodesize) {
  _octree = new oct::Octree(_extent, _xpos, _ypos, _zpos, leafnodesize);
  _octree->init();
  multipoleExpansion();
}

void Multipolesolver::multipoleExpansion() {
  // create multipole datacontainer
  const unsigned int numnodes(_octree->getnumnodes());
  _monopole = array_t(numnodes);
  _quadrapole = array_t(numnodes);
  _nodecomx = array_t(numnodes);
  _nodecomy = array_t(numnodes);
  _nodecomz = array_t(numnodes);
  _monopole.setZero();
  _quadrapole.setZero();

  // get leafnodes
  std::vector<const oct::Octreenode*> leafnodes(_octree->getleafnodes());

// compute leafnode multipole expansions
#pragma omp parallel for
  for (unsigned int leafnode_i = 0; leafnode_i < leafnodes.size();
       ++leafnode_i) {
    const oct::Octreenode* leafnode(leafnodes[leafnode_i]);
    const std::vector<unsigned int>* leafnodeindicesinposarray(
        leafnode->getindices());
    const unsigned int leafnodedataindex(leafnode->getdataindex());

    precision_t comx(0.0);
    precision_t comy(0.0);
    precision_t comz(0.0);
    precision_t totalemass(0.0);

    for (unsigned int index : *leafnodeindicesinposarray) {
      const precision_t mass_i(_masses(index));
      // leafnode monopole and com
      totalemass += mass_i;
      comx += mass_i * _xpos(index);
      comy += mass_i * _ypos(index);
      comz += mass_i * _zpos(index);
      // TODO(dave): leafnode quadrapole
    }
    const precision_t totalmassinverse(1.0 / totalemass);
    _monopole(leafnodedataindex) = totalemass;
    _nodecomx(leafnodedataindex) = comx * totalmassinverse;
    _nodecomy(leafnodedataindex) = comy * totalmassinverse;
    _nodecomz(leafnodedataindex) = comz * totalmassinverse;
  }

  const std::array<oct::Octreenode*, 8>* rootchildren(
      _octree->getroot()->getchildren());

#pragma omp parallel for
  for (unsigned int toplevelnode_i = 0; toplevelnode_i < 8; toplevelnode_i++) {
    expandmoments((*rootchildren)[toplevelnode_i]);
  }
}

void Multipolesolver::expandmoments(const oct::Octreenode* const node) {
  if (node->isleaf()) {
    return;
  } else {
    const unsigned int nodedataindex(node->getdataindex());
    const std::array<oct::Octreenode*, 8>* children(node->getchildren());

    precision_t comx(0.0);
    precision_t comy(0.0);
    precision_t comz(0.0);
    precision_t totalemass(0.0);

    for (unsigned int child_i = 0; child_i < 8; child_i++) {
      const oct::Octreenode* childnode_i((*children)[child_i]);
      expandmoments(childnode_i);

      // propagate from child_i to node
      const unsigned int childdataindex(childnode_i->getdataindex());
      // monopole and com
      const precision_t mass_i(_monopole(childdataindex));
      totalemass += mass_i;
      comx += mass_i * _nodecomx(childdataindex);
      comy += mass_i * _nodecomy(childdataindex);
      comz += mass_i * _nodecomz(childdataindex);
      // com of node

      // TODO(dave): quadrapole
    }
    const precision_t totalmassinverse(1.0 / totalemass);
    _monopole(nodedataindex) = totalemass;
    _nodecomx(nodedataindex) = comx * totalmassinverse;
    _nodecomy(nodedataindex) = comy * totalmassinverse;
    _nodecomz(nodedataindex) = comz * totalmassinverse;
  }
}
