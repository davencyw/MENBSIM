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

#include "global.hh"
#include "octree/octree.hh"
#include "simenv.hh"

class Forcesolver {
 public:
  Forcesolver(SimEnv simenv) : _simenv(simenv){};
  virtual void solve(const unsigned int numparticles, const array_t& xpos,
                     const array_t& ypos, const array_t& zpos,
                     const array_t& masses, array_t& forcex, array_t& forcey,
                     array_t& forcez, const precision_t softening,
                     const Extent extent) = 0;

 private:
  SimEnv _simenv;
};

class Naivesolver : public Forcesolver {
 public:
  Naivesolver(SimEnv simenv) : Forcesolver(simenv){};
  void solve(const unsigned int numparticles, const array_t& xpos,
             const array_t& ypos, const array_t& zpos, const array_t& masses,
             array_t& forcex, array_t& forcey, array_t& forcez,
             const precision_t softening, const Extent extent) override;
};

class Multipolesolver : public Forcesolver {
 public:
  Multipolesolver(SimEnv simenv) : Forcesolver(simenv){};
  void solve(const unsigned int numparticles, const array_t& xpos,
             const array_t& ypos, const array_t& zpos, const array_t& masses,
             array_t& forcex, array_t& forcey, array_t& forcez,
             const precision_t softening, const Extent extent) override;

 private:
  void createTree();
  void multipoleExpansion();
  void expandmoments(const oct::Octreenode* const node);
  array_t _xpos;
  array_t _ypos;
  array_t _zpos;
  array_t _masses;
  Extent _extent;
  unsigned int _numparticles;

  oct::Octree* _octree;
  array_t _monopole;
  array_t _quadrapole;
};

#endif  //__FORCESOLVER_HH__
