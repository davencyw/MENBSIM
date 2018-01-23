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
  createTree(numparticles, xpos, ypos, zpos, masses, extent);
  CCPP::BENCH::stop(B_TREEGEN);

  CCPP::BENCH::start(B_MULTIPOLE);
  for (unsigned particle_i = 0; particle_i < numparticles; ++particle_i) {
    // TODO(dave): go through all highest possible levels of octree
  }
  CCPP::BENCH::stop(B_MULTIPOLE);
}

// TODO(dave): make parameter leafsize nonstatic
void Multipolesolver::createTree(const unsigned int numparticles,
                                 const array_t& xpos, const array_t& ypos,
                                 const array_t& zpos, const array_t& masses,
                                 const Extent extent) {
  _octree = new oct::Octree(extent, xpos, ypos, zpos, 10);
  _octree->init();

  multipoleExpansion(numparticles, xpos, ypos, zpos, masses, extent);
}

void Multipolesolver::multipoleExpansion(
    const unsigned int numparticles, const array_t& xpos, const array_t& ypos,
    const array_t& zpos, const array_t& masses, const Extent extent) {
  // create multipole datacontainer
  const unsigned int numnodes(_octree->getnumnodes());
  _monopole = array_t(numnodes);
  _quadrapole = array_t(numnodes);
  _monopole.setZero();
  _quadrapole.setZero();

  // get leafnodes
  std::vector<const oct::Octreenode*> leafnodes(_octree->getleafnodes());

  // TODO(dave): use openmp to parallelize
  // compute multipole expansions bottom up
  for (auto leafnode : leafnodes) {
    const oct::Octreenode* currentnode = leafnode;

    const std::vector<unsigned int>* leafnodeindicesinposarray(
        leafnode->getindices());
    const unsigned int leafnodedataindex(leafnode->getdataindex());

    for (unsigned int index : *leafnodeindicesinposarray) {
      // leafnode monopole
      _monopole(leafnodedataindex) += masses(index);
      // TODO(dave): leafnode quadrapole
    }

    // propagate mono-/quadrapole upwards in tree
    while (currentnode) {
      // TODO(dave): monopole
      // TODO(dave): quadrapole

      currentnode = currentnode->getparent();
    }
  }
}
