#include "menbsim.hh"
#include "inputreader.hh"

#include <cassert>
#include <iostream>

namespace Menbsim {

void Menbsim::initialize(int checks) {
  std::cout << "read inputfile " << _simenv._inputfilepath << "\n\n";
  _inputdata = Inputreader::readfromfile(_simenv._inputfilepath);
  _numparticles = _inputdata.numparticles;

  // sanitize input
  for (unsigned i = 0; i < _inputdata.numdata; ++i) {
    assert(_inputdata.datavector[i].size() == _numparticles);
  }

  // print info
  std::cout
      << "got " << _numparticles << " particles with extent: \n\n"
      << _inputdata.extent[1].first << "\t" << _inputdata.extent[1].second
      << "\n"
      << _inputdata.extent[2].first << "\t" << _inputdata.extent[2].second
      << "\n"
      << _inputdata.extent[3].first << "\t" << _inputdata.extent[3].second
      << "\n\n_____________________________________________________\n\n\n";

  // create local pointers for easy access and readable code
  _masses = &_inputdata.datavector[0];
  _xposition = &_inputdata.datavector[1];
  _yposition = &_inputdata.datavector[2];
  _zposition = &_inputdata.datavector[3];
  _xvelocity = &_inputdata.datavector[4];
  _yvelocity = &_inputdata.datavector[5];
  _zvelocity = &_inputdata.datavector[6];
  _softening = &_inputdata.datavector[7];
  _potential = &_inputdata.datavector[8];

  // initialize force arrays
  _forcex = array_t(_numparticles);
  _forcey = array_t(_numparticles);
  _forcez = array_t(_numparticles);

  if (checks) {
    verifyinputdensity(1);
  }

  _initialized = true;
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
    std::cout << "\n       ! W A R N I N G !\n"
              << " N O T   I N I T I A L I Z E D!\n"
              << "    S T E P   I G N O R E D\n\n\n";
    return;
  }

  std::cout << '\xd' << "step: " << _step_i++ << " ..." << std::flush;

  // reset forces
  _forcex.setZero();
  _forcey.setZero();
  _forcez.setZero();

  // compute force
  _solver->solve(_numparticles, *_xvelocity, *_yvelocity, *_zvelocity, *_masses,
                 _forcex, _forcey, _forcez);

  // // update particle velocity
  *_xvelocity += *_masses * _forcex;
  *_yvelocity += *_masses * _forcey;
  *_zvelocity += *_masses * _forcez;

  // update particle position
  *_xposition += *_xvelocity;
  *_yposition += *_yvelocity;
  *_zposition += *_zvelocity;
}

}  // namespace menbsim