/*__DECLARATION__
 *
 * 
 *      MENBSIM
 *      N-Body Simulation of a elliptic galaxy using multipole expansions.
 *      This project is done in the lecture of computational astrophysics
 *      in 2017 at University of Zurich (UZH).
 *
 *      author: 
 *      david schmidig         [     david@davencyw.net   ]
 *      ETH Zurich             [ davschmi@student.ethz.ch ]
 *      DAVENCYW CODE          [        davencyw.net      ]
 */
#ifndef __FORCESOLVER_HH__
#define __FORCESOLVER_HH__

#include <global.hh>
#include "octree/octree.hh"

class Forcesolver {
 public:
  Forcesolver() = default;
  virtual void solve(const unsigned int numparticles, const array_t& xpos,
                     const array_t& ypos, const array_t& zpos,
                     const array_t& masses, array_t& forcex, array_t& forcey,
                     array_t& forcez, const precision_t softening,
                     const Extent extent) = 0;

  void setconfig() {
    // TODO(dave): write config
    // leafsize for octree in multipole
  }
};

class Naivesolver : public Forcesolver {
 public:
  Naivesolver() = default;
  void solve(const unsigned int numparticles, const array_t& xpos,
             const array_t& ypos, const array_t& zpos, const array_t& masses,
             array_t& forcex, array_t& forcey, array_t& forcez,
             const precision_t softening, const Extent extent) override;
};

class Multipolesolver : public Forcesolver {
 public:
  Multipolesolver() = default;
  void solve(const unsigned int numparticles, const array_t& xpos,
             const array_t& ypos, const array_t& zpos, const array_t& masses,
             array_t& forcex, array_t& forcey, array_t& forcez,
             const precision_t softening, const Extent extent) override;

 private:
  void createTree(const unsigned int numparticles, const array_t& xpos,
                  const array_t& ypos, const array_t& zpos,
                  const Extent extent);
  oct::Octree* _octree;
};

#endif  //__FORCESOLVER_HH__