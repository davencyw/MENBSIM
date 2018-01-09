#include "menbsim.hh"
#include "inputreader.hh"

#include <iostream>

void Menbsim::initialize(int checks) {
  std::cout << "read inputfile " << _simenv._inputfilepath << "\n\n";
  _inputdata = Inputreader::readfromfile(_simenv._inputfilepath);

  std::cout << "got " << _inputdata.numparticles << " particles with extent: \n"
            << _inputdata.extent[1].first << "\t" << _inputdata.extent[1].second
            << "\n"
            << _inputdata.extent[2].first << "\t" << _inputdata.extent[2].second
            << "\n"
            << _inputdata.extent[3].first << "\t" << _inputdata.extent[3].second
            << "\n\n\n";
}

bool Menbsim::verifyinputdensity(int output) {
  // TODO(dave): write verification of rho(r) comparing to the analyitcal
  // density funcitno described by Hernquist

  // TODO(dave): add poissonian error bars to the numeric density profile
}

void Menbsim::steps(int numsteps) {
  for (int i = 0; i < numsteps; ++i) {
    step();
  }
}

void Menbsim::step() {
  // TODO(dave): do primitive step
}