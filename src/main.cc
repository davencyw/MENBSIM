#include <climits>

#include <omp.h>
#include <iostream>
#include <string>

#include "cmdparser.hh"
#include "menbsim.hh"
#include "simenv.hh"

int main(int argc, char const *argv[]) {
  // parse command line arguments
  SimEnv simenv;
  cmdpars(argc, argv, simenv);

  // create terminal output
  std::cout << "\n"
            << "\n"
            << "\n"
            << "\n"
            << "\n"
            << "\n"
            << "\n"
            << "\n\n"
            << "author: david schmidig [david@davencyw.net]\n"
            << "        davencyw code  [davencyw.net]\n"
            << "        ETH Zurich\n\n\n";

  // start main program
  Menbsim sim(simenv);

  return 0;
}