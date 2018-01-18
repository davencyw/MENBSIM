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
#ifndef __MENBSIM_HH__
#define __MENBSIM_HH__

#include "global.hh"
#include "io.hh"
#include "simenv.hh"
#include "solver/forcesolver.hh"

namespace Menbsim {

enum class FORCESOLVERTYPE { NAIVE, MULTIPOLE };
enum VERIFICATION { NOVERIFICATION, VERIFYDENSITY };

class Menbsim {
 public:
  Menbsim(SimEnv simenv) : _simenv(simenv){};
  Menbsim(SimEnv simenv, FORCESOLVERTYPE type) : _simenv(simenv) {
    switchsolver(type);
  };

  // Initialize data and do verification if wanted
  // checks:
  //    0 - no verification
  //    1 - verify inputdensity
  void initialize(int checks);

  // verify inputdensity from file
  // output:
  //    0 - no output
  //    1 - terminal output short
  //    2 - terminal output long
  bool verifyinputdensity(int output);

  void step();
  void steps(int);

  Extent getextent();

  void switchsolver(FORCESOLVERTYPE type) {
    switch (type) {
      case FORCESOLVERTYPE::MULTIPOLE: {
        _solver = new Multipolesolver();
      } break;
      default: { _solver = new Naivesolver; } break;
    }
  }

 private:
  bool _initialized = false;

  precision_t _deltat;

  Forcesolver* _solver;
  SimEnv _simenv;

  Datastruct _inputdata;
  unsigned int _numparticles;
  array_t* _masses;
  array_t* _xposition;
  array_t* _yposition;
  array_t* _zposition;
  array_t* _xvelocity;
  array_t* _yvelocity;
  array_t* _zvelocity;
  array_t* _softening;
  array_t* _potential;

  precision_t _softeningparam;
  array_t _forcex;
  array_t _forcey;
  array_t _forcez;

  unsigned int _step_i = 1;
};

}  // namespace Menbsim

#endif  //__MENBSIM_HH__
