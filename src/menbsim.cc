#include "menbsim.hh"
#include "inputreader.hh"

#include <iostream>

namespace Menbsim {

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

  // create local pointers for easy access and readable code
  *_masses = _inputdata.datavector[0];
  *_xposition = _inputdata.datavector[1];
  *_yposition = _inputdata.datavector[2];
  *_zposition = _inputdata.datavector[3];
  *_xvelocity = _inputdata.datavector[4];
  *_yvelocity = _inputdata.datavector[5];
  *_zvelocity = _inputdata.datavector[6];
  *_softening = _inputdata.datavector[7];
  *_potential = _inputdata.datavector[8];

  if (checks) {
    verifyinputdensity(1);
  }
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
  if (!_initialized) {
    // TODO(dave): throw error
    return;
  }

  // compute force
  _solver->solve(_numparticles, *_xvelocity, *_yvelocity, *_zvelocity, *_masses,
                 _forcex, _forcey, _forcez);

  // update particle velocity
  *_xvelocity += *_masses * _forcex;
  *_yvelocity += *_masses * _forcey;
  *_zvelocity += *_masses * _forcez;

  // update particle position
  *_xposition += *_xvelocity;
  *_yposition += *_yvelocity;
  *_zposition += *_zvelocity;
}

}  // namespace menbsim