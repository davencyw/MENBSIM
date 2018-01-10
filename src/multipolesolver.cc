#include "forcesolver.hh"

#include "global.hh"
#include "octree/octree.hh"

void Multipolesolver::solve(const unsigned int numparticles,
                            const array_t& xpos, const array_t& ypos,
                            const array_t& zpos, const array_t& masses,
                            array_t& forcex, array_t& forcey, array_t& forcez,
                            const precision_t softening, const Extent extent) {
  // create tree first
  createTree(numparticles, xpos, ypos, zpos, extent);

  for (unsigned particle_i = 0; particle_i < numparticles; ++particle_i) {
    // TODO(dave): go through all highest possible levels of octree
  }
};

void Multipolesolver::createTree(const unsigned int numparticles,
                                 const array_t& xpos, const array_t& ypos,
                                 const array_t& zpos, const Extent extent) {
  _octree = new oct::Octree(extent, xpos, ypos, zpos, 8);
  _octree->init();
}