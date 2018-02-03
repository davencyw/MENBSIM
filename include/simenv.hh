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

#include <iostream>
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
  unsigned int _outputstep;

  void summarizetooutput() {
    std::cout << "\n\n\n__________________________\n"
              << "simenv_summary:"
              << "\n__________________________\n\n"
              << "_dt          :\t" << _dt << "\n"
              << "_nsteps      :\t" << _nsteps << "\n"
              << "_softening   :\t" << _softeningparam << "\n"
              << "_solvertype  :\t" << _solvertype << "\n"
              << "_leafnodesize:\t" << _octreeleafnodesize << "\n"
              << "\n_nthreads    :\t" << _nthreads << "\n"
              << "_scheduling  :\t" << _scheduling << "\n"
              << "_cuda        :\t" << _cuda << "\n"
              << "\n_outfolder   :\t" << _outfolder << "\n"
              << "_inputfile   :\t" << _inputfilepath << "\n"
              << "_runhash     :\t" << _runhash << "\n"
              << "_nooutput    :\t" << _nooutput << "\n"
              << "_outputstep  :\t" << _outputstep << "\n"
              << "\n__________________________\n\n";
  }
};

#endif  //__SIMENV_HH__
