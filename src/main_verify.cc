#include <climits>

#include <omp.h>
#include <iostream>
#include <string>

#include "benchdec.hh"
#include "cmdparser.hh"
#include "menbsim.hh"
#include "simenv.hh"

int main(int argc, char const *argv[]) {
  // parse command line arguments
  SimEnv simenv;
  cmdpars(argc, argv, simenv);

  // create terminal output
  std::cout << "\033[0m\n"
            << "\n"
            << "\n"
            << "\n"
            << "\n"
            << "\n"
            << " __  __ ______ _   _ ____   _____ _____ __  __ \n"
            << "|  \\/  |  ____| \\ | |  _ \\ / ____|_   _|  \\/  |\n"
            << "| \\  / | |__  |  \\| | |_) | (___   | | | \\  / |\n"
            << "| |\\/| |  __| | . ` |  _ < \\___ \\  | | | |\\/| |\n"
            << "| |  | | |____| |\\  | |_) |____) |_| |_| |  | |\n"
            << "|_|  |_|______|_| \\_|____/|_____/|_____|_|  |_|\n"
            << "\n"
            << "\n\n"
            << "author: david schmidig [david@davencyw.net]\n"
            << "        davencyw code  [davencyw.net]\n"
            << "        ETH Zurich\n\n"
            << "_____________________________________________________\n\n\n";

  // initialize environment
  omp_set_num_threads(simenv._nthreads);
  omp_set_dynamic(simenv._scheduling);

  // start main program verification
  Menbsim::Menbsim sim(simenv);

  sim.initialize(Menbsim::VERIFYDENSITY);
  sim.verifydirectforce();

  return 0;
}
