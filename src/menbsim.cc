#include "menbsim.hh"
#include "io.hh"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <parallel/algorithm>

namespace Menbsim {

void Menbsim::initialize(int checks) {
  std::cout << "read inputfile " << _simenv._inputfilepath << "\n\n";
  _inputdata = Reader::readfromfile(_simenv._inputfilepath);
  _numparticles = _inputdata.numparticles;

  // sanitize input
  for (unsigned i = 0; i < _inputdata.numdata; ++i) {
    assert(_inputdata.datavector[i].size() == _numparticles);
  }

  // print info
  std::cout
      << "got " << _numparticles << " particles with extent: \n\n"
      << _inputdata.extent.x.first << "\t" << _inputdata.extent.x.second << "\n"
      << _inputdata.extent.y.first << "\t" << _inputdata.extent.y.second << "\n"
      << _inputdata.extent.z.first << "\t" << _inputdata.extent.z.second
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

  // TODO(dave): add poissonian error bars to the numeric density profile and
  // plot
  return true;
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
              << " N O T   I N I T I A L I Z E D\n"
              << "   S T E P S   I G N O R E D\n\n\n";
    exit(EXIT_FAILURE);
    return;
  }

  std::cout << '\xd' << "step: " << _step_i++ << " ..." << std::flush;

  // reset forces
  _forcex.setZero();
  _forcey.setZero();
  _forcez.setZero();

  Extent extent = getextent();

  // compute force
  _solver->solve(_numparticles, *_xvelocity, *_yvelocity, *_zvelocity, *_masses,
                 _forcex, _forcey, _forcez, _softeningparam, extent);

  // // update particle velocity
  *_xvelocity += _forcex / *_masses * _deltat;
  *_yvelocity += _forcey / *_masses * _deltat;
  *_zvelocity += _forcez / *_masses * _deltat;

  // update particle position
  *_xposition += *_xvelocity * _deltat;
  *_yposition += *_yvelocity * _deltat;
  *_zposition += *_zvelocity * _deltat;

  // TODO(dave): write to file
}

Extent Menbsim::getextent() {
  // get extent of all particles in all dimensions
  Extent extent;

  const precision_t xmin(*__gnu_parallel::min_element(
      _xposition->data(), _xposition->data() + _xposition->size()));

  const precision_t xmax(*__gnu_parallel::max_element(
      _xposition->data(), _xposition->data() + _xposition->size()));

  const precision_t ymin(*__gnu_parallel::min_element(
      _xposition->data(), _xposition->data() + _xposition->size()));

  const precision_t ymax(*__gnu_parallel::max_element(
      _xposition->data(), _xposition->data() + _xposition->size()));

  const precision_t zmin(*__gnu_parallel::min_element(
      _xposition->data(), _xposition->data() + _xposition->size()));

  const precision_t zmax(*__gnu_parallel::max_element(
      _xposition->data(), _xposition->data() + _xposition->size()));

  extent.x = std::make_pair(xmin, xmax);
  extent.y = std::make_pair(ymin, ymax);
  extent.z = std::make_pair(zmin, zmax);
}

}  // namespace menbsim