#include "menbsim.hh"
#include "inputreader.hh"

void Menbsim::initialize(int checks) {
  _inputdata = Inputreader::readfromfile(_simenv._inputfilepath);
}

bool Menbsim::verifyinputdensity(int output) {}