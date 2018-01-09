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

#include "forcesolver.hh"
#include "global.hh"
#include "inputreader.hh"
#include "simenv.hh"

class Menbsim {
 public:
  Menbsim(SimEnv simenv) : _simenv(simenv){};

  // Initialize data and do verification if wanted
  // checks:
  //    0 - on verification
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

 private:
  Forcesolver* solver;
  SimEnv _simenv;
  datastruct _inputdata;
};

#endif  //__MENBSIM_HH__