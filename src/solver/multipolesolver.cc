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
#pragma omp parallel for
  for (unsigned particle_i = 0; particle_i < numparticles; ++particle_i) {
    // TODO(dave): go through all highest possible levels of octree
  }
  CCPP::BENCH::stop(B_MULTIPOLE);
}

// TODO(dave): make parameter leafsize nonstatic
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

    for (unsigned int index : *leafnodeindicesinposarray) {
      // leafnode monopole
      _monopole(leafnodedataindex) += _masses(index);
      // TODO(dave): leafnode quadrapole
    }
  }

  const std::array<oct::Octreenode*, 8>* toplevelchildren(
      _octree->getroot()->getchildren());

#pragma omp parallel for
  for (unsigned int toplevelnode_i = 0; toplevelnode_i < 8; toplevelnode_i++) {
    expandmoments((*toplevelchildren)[toplevelnode_i]);
  }
}

void Multipolesolver::expandmoments(const oct::Octreenode* const node) {
  if (node->isleaf()) {
    return;
  } else {
    const unsigned int nodedataindex(node->getdataindex());
    const std::array<oct::Octreenode*, 8>* children(node->getchildren());
    // TODO(dave): OMP parallelize
    for (unsigned int child_i = 0; child_i < 8; child_i++) {
      const oct::Octreenode* childnode_i((*children)[child_i]);
      expandmoments(childnode_i);

      // propagate from child_i to node
      const unsigned int childdataindex(childnode_i->getdataindex());
      // monopole
      _monopole(nodedataindex) += _monopole(childdataindex);
      // TODO(dave): quadrapole
    }
  }
}
