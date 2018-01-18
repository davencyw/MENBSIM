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

  // initialize benchstructure
  CCPP::BENCH::settitle("menbsim");
  // CCPP::BENCH::setinfo("");

  CCPP::BENCH::reg("initialization");
  CCPP::BENCH::reg("steps");
  CCPP::BENCH::regchild("solver", B_STEP);
  CCPP::BENCH::regchild("update", B_STEP);
  CCPP::BENCH::regchild("treegen", B_SOLVER);
  CCPP::BENCH::regchild("multipole", B_SOLVER);

  // start main program
  Menbsim::Menbsim sim(simenv);

  CCPP::BENCH::start(B_INIT);
  sim.initialize(Menbsim::NOVERIFICATION);
  CCPP::BENCH::stop(B_INIT);

  CCPP::BENCH::start(B_STEP);
  sim.steps(10);
  CCPP::BENCH::stop(B_STEP);

  std::cout << "\n\n_____________________________________________________\n\n"
            << "finished simulation\n"
            << "\n\n\n";

  CCPP::BENCH::summarize();

  return 0;
}
