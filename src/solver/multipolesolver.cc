#include "solver/forcesolver.hh"

#include "benchdec.hh"
#include "global.hh"
#include "octree/octree.hh"

void Multipolesolver::solve(const unsigned int numparticles,
                            const array_t& xpos, const array_t& ypos,
                            const array_t& zpos, const array_t& masses,
                            array_t& forcex, array_t& forcey, array_t& forcez,
                            const precision_t softening, const Extent extent) {
  // create tree first
  CCPP::BENCH::start(B_TREEGEN);
  createTree(numparticles, xpos, ypos, zpos, extent);
  CCPP::BENCH::stop(B_TREEGEN);

  CCPP::BENCH::start(B_MULTIPOLE);
  for (unsigned particle_i = 0; particle_i < numparticles; ++particle_i) {
    // TODO(dave): go through all highest possible levels of octree
  }
  CCPP::BENCH::stop(B_MULTIPOLE);
};
// TODO(dave): make parameter leafsize nonstatic
void Multipolesolver::createTree(const unsigned int numparticles,
                                 const array_t& xpos, const array_t& ypos,
                                 const array_t& zpos, const Extent extent) {
  _octree = new oct::Octree(extent, xpos, ypos, zpos, 10);
  _octree->init();
}
