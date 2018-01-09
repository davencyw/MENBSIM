#ifndef __MENBSIM_HH__
#define __MENBSIM_HH__

#include "global.hh"
#include "inputreader.hh"
#include "simenv.hh"

class Menbsim {
 public:
  Menbsim(SimEnv simenv) : _simenv(simenv){};
  void initialize();

 private:
  SimEnv _simenv;
  datastruct _inputdata;
};

#endif  //__MENBSIM_HH__