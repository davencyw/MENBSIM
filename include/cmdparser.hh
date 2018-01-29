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
#ifndef __CMDPARSER_HH__
#define __CMDPARSER_HH__

#include "global.hh"
#include "menbsim.hh"
#include "simenv.hh"

#include <cstdlib>
#include <string>
#include "boost/program_options.hpp"

void cmdpars(const int argc, char const* argv[], SimEnv& simenv) {
  // set default simenv
  simenv._nthreads = __P_DEF_NTHREADS;
  simenv._scheduling = __P_DEF_SCHEDULE;
  simenv._cuda = __P_DEF_CUDA;
  simenv._outfolder = __P_DEF_OUTFOLDER;
  simenv._nooutput = __P_DEF_NOOUTPUT;
  simenv._solvertype = __P_DEF_SOLVERTYPE;
  simenv._octreeleafnodesize = __P_DEF_OCTREELEAFNODESIZE;

  // BOOST PRORGAM OPTIONS
  namespace po = boost::program_options;
  po::options_description desc("Parameters");
  desc.add_options()("help", "Print help messages")(
      "dt,d", po::value<double>(&(simenv._dt)), "force timestep [optional]")(
      "numsteps", po::value<int>(&(simenv._nsteps))->required(),
      "number of timesteps")(
      "softening", po::value<double>(&(simenv._softeningparam))->required(),
      "softening of potential")(
      "solvertype,t", po::value<int>(&(simenv._solvertype)), "type of solver")(
      "leafnodesize, l", po::value<int>(&(simenv._octreeleafnodesize)),
      "octree leafnode binsize")(
      "input,i", po::value<std::string>(&(simenv._inputfilepath))->required(),
      "input folder")("outfolder,o",
                      po::value<std::string>(&(simenv._outfolder)),
                      "output folder [optional]")("nooutput", "no fileoutput")(
      "outputstep,x", po::value<unsigned int>(&(simenv._outputstep)),
      "every outputstep write to output [optional]")(
      "nthreads,n", po::value<int>(&(simenv._nthreads)),
      "number of threads [optional]")(
      "schedule,s", po::value<int>(&(simenv._scheduling)),
      "omp scheduling [optional]")("cuda", "enable cuda support [optional]");

  po::variables_map vm;
  try {
    // can throw
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
      std::cout << __P_NAME << std::endl << desc << std::endl;
      exit(0);
    }

    if (vm.count("cuda")) simenv._cuda = true;
    if (vm.count("nooutput")) simenv._nooutput = true;

    po::notify(vm);  // throws on error, so do after help in case
                     // there are any problems
  } catch (po::error& e) {
    std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
    std::cerr << desc << std::endl;
    exit(1);
  }

  // folder sanitization
  if (simenv._outfolder.back() != '/' && !simenv._outfolder.empty()) {
    simenv._outfolder.append("/");
  }

  // runhash
  simenv._runhash = std::time(0);

  // TODO(dave): write param summary

  // END BOOST PROGRAM OPTIONS
};

#endif  //__CMDPARSER_HH__
