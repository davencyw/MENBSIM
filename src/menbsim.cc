#include "menbsim.hh"
#include "inputreader.hh"

void Menbsim::initialize() {
  _inputdata = Inputreader::readfromfile(_simenv._inputfilepath);
}