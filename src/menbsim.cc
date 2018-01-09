#include "menbsim.hh"
#include "inputreader.hh"

void Menbsim::initialize() {
  datastruct = Inputreader::readfromfile(_simenv._inputfilepath);
}