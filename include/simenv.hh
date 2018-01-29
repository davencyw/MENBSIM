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
#ifndef __SIMENV_HH__
#define __SIMENV_HH__

#include <string>

struct SimEnv {
  // physical environment
  //____________________

  // timestep
  double _dt = 0;
  // number of steps
  int _nsteps;
  // softening parameter for potential
  double _softeningparam;

  // computing environment
  //____________________
  int _solvertype;
  int _octreeleafnodesize;

  // number of threads
  int _nthreads;
  // omp scheduling
  int _scheduling;
  // cuda enabled
  bool _cuda;

  // data environment
  //____________________
  std::string _outfolder;
  std::string _inputfilepath;
  unsigned int _runhash;
  bool _nooutput;
  unsigned int _outputstep = 1;
};

#endif  //__SIMENV_HH__
