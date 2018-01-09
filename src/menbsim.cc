#include "menbsim.hh"
#include "inputreader.hh"

void Menbsim::initialize() {
  Inputreader::readfromfile(_simenv._inputfilepath);
}