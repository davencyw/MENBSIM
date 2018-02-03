#include "menbsim.hh"

#include "benchdec.hh"
#include "const.hh"
#include "io.hh"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <parallel/algorithm>
#include <string>

namespace Menbsim {

void Menbsim::initialize(int checks) {
  std::cout << "read inputfile " << _simenv._inputfilepath << "\n\n";
  _inputdata = io::Reader::readfromfile(_simenv._inputfilepath);
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
    CCPP::BENCH::start(B_VERIFICATION);
    verifyinputdensity(1);
    CCPP::BENCH::stop(B_VERIFICATION);
  }

  _initialized = true;
}

void Menbsim::verifyinputdensity(int output) {
  // creates histogram of current density in 3d
  const unsigned int numbins(4000);

  const precision_t xdist(_inputdata.extent.x.second -
                          _inputdata.extent.x.first + 0.01);
  const precision_t ydist(_inputdata.extent.y.second -
                          _inputdata.extent.y.first + 0.01);
  const precision_t zdist(_inputdata.extent.z.second -
                          _inputdata.extent.z.first + 0.01);
  const precision_t binwidthx(xdist / static_cast<precision_t>(numbins));
  const precision_t binwidthy(ydist / static_cast<precision_t>(numbins));
  const precision_t binwidthz(zdist / static_cast<precision_t>(numbins));
  const precision_t hernquistbinwidth((binwidthx + binwidthy + binwidthz) / 3);

  array_t xbins = array_t::Zero(numbins);
  array_t ybins = array_t::Zero(numbins);
  array_t zbins = array_t::Zero(numbins);
  array_t binshernquist = array_t::Zero(numbins);

  const precision_t xextentfirst(std::abs(_inputdata.extent.x.first));
  const precision_t yextentfirst(std::abs(_inputdata.extent.y.first));
  const precision_t zextentfirst(std::abs(_inputdata.extent.z.first));
  precision_t totalemass(0.0);

#pragma omp parallel for
  for (size_t particle_i = 0; particle_i < _numparticles; particle_i++) {
    const precision_t xpos((*_xposition)(particle_i));
    const precision_t ypos((*_yposition)(particle_i));
    const precision_t zpos((*_zposition)(particle_i));

    const unsigned int xbinsindex(
        static_cast<unsigned int>((xpos + xextentfirst) / binwidthx));
    const unsigned int ybinsindex(
        static_cast<unsigned int>((ypos + yextentfirst) / binwidthy));
    const unsigned int zbinsindex(
        static_cast<unsigned int>((zpos + zextentfirst) / binwidthz));
    const precision_t mass_i((*_masses)(particle_i));

#pragma omp critical
    {
      xbins(xbinsindex) += 1;
      ybins(ybinsindex) += 1;
      zbins(zbinsindex) += 1;
      totalemass += mass_i;
    }
  }

  xbins /= _numparticles;
  ybins /= _numparticles;
  zbins /= _numparticles;

  // hernquist density distribution
  precision_t r(0.0);
  const unsigned int halfbins(numbins / 2);
  // TODO(dave): specify scale length a
  const precision_t a(0.0);

  for (size_t bin_i = 0; bin_i < halfbins; bin_i++) {
    binshernquist(bin_i) = binshernquist(bin_i + halfbins) =
        totalemass / (2 * __SC_PI) * a / r / std::pow(r + a, 3);
    r += hernquistbinwidth;
  }

  // write densities to files
  // write to file
  std::string filename("menbsim_" + std::to_string(_simenv._runhash));
  std::string fullfilepath(_simenv._outfolder + filename);
  io::Writer::write1ddensitytofile(fullfilepath + "_x.denshist", xbins,
                                   "xdensity");
  io::Writer::write1ddensitytofile(fullfilepath + "_y.denshist", ybins,
                                   "ydensity");
  io::Writer::write1ddensitytofile(fullfilepath + "_z.denshist", zbins,
                                   "zdensity");
  io::Writer::write1ddensitytofile(fullfilepath + "_hernquist.denshist",
                                   binshernquist, "hernquistdensity");

}  // namespace Menbsim

void Menbsim::verifydirectforce() {
  step();
  // verify shells with Newtons second theorem for spherical potentials
  const unsigned int numshells(50);

  array_t analytical_force(numshells);
  array_t averaged_force(numshells);
  averaged_force.setZero();
  analytical_force.setZero();

  array_t particledist = (*_xposition) * (*_xposition) +
                         (*_yposition) * (*_yposition) +
                         (*_zposition) * (*_zposition);
  particledist = particledist.sqrt();
  std::vector<unsigned int> sortedindices(_numparticles);
  iota(sortedindices.begin(), sortedindices.end(), 0);

  // sort according to distance from origin
  // TODO(dave): use __gnu_parallel, way faster!
  sort(sortedindices.begin(), sortedindices.end(),
       [&particledist](unsigned int i1, unsigned int i2) {
         return particledist(i1) < particledist(i2);
       });

  // empiric shell distance for provided input dataset
  const precision_t shelldist(2940.53 / static_cast<precision_t>(numshells));
  const precision_t shelldx(shelldist * 0.1);
  const precision_t shelldxhalf(shelldx);

  for (unsigned i = 0; i < _numparticles; ++i) {
    unsigned sortedindex(sortedindices[i]);
    std::cout << particledist(sortedindex) << "\t" << _forcex(sortedindex)
              << "\n";
  }

  return;

  precision_t massinshell(0.0);
  precision_t currentshelldist(0);
  unsigned int particle_i(0);
  unsigned int sortedindex(sortedindices[0]);

  for (unsigned int shell_i = 0; shell_i < numshells; shell_i++) {
    currentshelldist += shelldist;
    while (particledist(sortedindex) < currentshelldist - shelldxhalf &&
           particle_i < _numparticles - 1) {
      // inside shell
      massinshell += (*_masses)(sortedindex);
      // next particle
      sortedindex = sortedindices[++particle_i];
    }
    precision_t massonshell(0.0);
    precision_t forceonshell(0.0);
    unsigned int particlesonshell(0);
    while (particledist(sortedindex) > currentshelldist - shelldxhalf &&
           particledist(sortedindex) < currentshelldist + shelldxhalf &&
           particle_i < _numparticles - 1) {
      // on shell
      ++particlesonshell;
      massonshell += (*_masses)(sortedindex);
      forceonshell += std::sqrt(_forcex(sortedindex) * _forcex(sortedindex) +
                                _forcey(sortedindex) * _forcey(sortedindex) +
                                _forcez(sortedindex) * _forcez(sortedindex));
      sortedindex = sortedindices[++particle_i];
    }
    averaged_force(shell_i) =
        forceonshell / static_cast<precision_t>(particlesonshell);
    // analytical force magnitude for a spherical potential outside a spherical
    // shell of matter is M^2/r^4
    analytical_force(shell_i) =
        std::pow(massinshell, 2) / std::pow(currentshelldist, 4);
    massinshell += massonshell;
  }

  std::cout
      << "\n\n\nDIRECT FORCEVERIFICATION\n\n"
      << "AV FORCE: \t\t AN FORCE:\n_____________________________________\n";
  for (unsigned int shell_i = 0; shell_i < numshells; shell_i++) {
    std::cout << averaged_force(shell_i) << " \t,\t "
              << analytical_force(shell_i) << "\n";
  }
  std::cout << "\n\n\n";
}

void Menbsim::steps(int numsteps) {
  if (!_initialized) {
    std::cout << "\n       ! W A R N I N G !\n"
              << " N O T   I N I T I A L I Z E D\n"
              << "   S T E P S   I G N O R E D\n\n\n";
    exit(EXIT_FAILURE);
    return;
  }

  for (int i = 0; i < numsteps; ++i) {
    std::cout << '\xd' << "step: " << _step_i << " ..." << std::flush;
    step();
  }
}

void Menbsim::step() {
  // reset forces
  _forcex.setZero();
  _forcey.setZero();
  _forcez.setZero();

  Extent extent = getextent();

  // compute force
  CCPP::BENCH::start(B_SOLVER);
  _solver->solve(_numparticles, *_xposition, *_yposition, *_zposition, *_masses,
                 &_forcex, &_forcey, &_forcez, _softeningparam, extent);
  CCPP::BENCH::stop(B_SOLVER);

  // // update particle velocity
  CCPP::BENCH::start(B_UPDATE);
  // get timestep
  _deltat = _simenv._dt;
  if (!_deltat) {
    _deltat = gettimestep();
  }

  *_xvelocity += _forcex / *_masses * _deltat;
  *_yvelocity += _forcey / *_masses * _deltat;
  *_zvelocity += _forcez / *_masses * _deltat;

  // update particle position
  *_xposition += *_xvelocity * _deltat;
  *_yposition += *_yvelocity * _deltat;
  *_zposition += *_zvelocity * _deltat;
  CCPP::BENCH::stop(B_UPDATE);

  CCPP::BENCH::start(B_OUTPUT);
  writeoutput();
  CCPP::BENCH::stop(B_OUTPUT);
  ++_step_i;
}

Extent Menbsim::getextent() {
  // get extent of all particles in all dimensions
  Extent extent;
  const unsigned int numpoints(_xposition->size());

  const precision_t xmin(*__gnu_parallel::min_element(
      _xposition->data(), _xposition->data() + numpoints));
  const precision_t xmax(*__gnu_parallel::max_element(
      _xposition->data(), _xposition->data() + numpoints));
  const precision_t ymin(*__gnu_parallel::min_element(
      _yposition->data(), _yposition->data() + numpoints));
  const precision_t ymax(*__gnu_parallel::max_element(
      _yposition->data(), _yposition->data() + numpoints));
  const precision_t zmin(*__gnu_parallel::min_element(
      _zposition->data(), _zposition->data() + numpoints));
  const precision_t zmax(*__gnu_parallel::max_element(
      _zposition->data(), _zposition->data() + numpoints));

  extent.x = std::make_pair(xmin, xmax);
  extent.y = std::make_pair(ymin, ymax);
  extent.z = std::make_pair(zmin, zmax);
  return extent;
}

precision_t Menbsim::gettimestep() {
  // TODO(dave): compute biggest possible timestepsize
  return 0.01;
}

void Menbsim::writeoutput() {
  if (_simenv._nooutput) {
    return;
  }
  if (_step_i % _simenv._outputstep == 0) {
    std::string step_i(std::to_string(_step_i));
    step_i.insert(step_i.begin(), 10 - step_i.length(), '0');

    // write to file
    std::string filename("menbsim_" + std::to_string(_simenv._runhash) + "_s" +
                         step_i + ".posdat");
    std::string fullfilepath(_simenv._outfolder + filename);
    io::Writer::writepostofile(fullfilepath, *_xposition, *_yposition,
                               *_zposition);
  }
}

}  // namespace Menbsim
