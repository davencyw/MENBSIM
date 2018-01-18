#ifndef __BENCHDEC_HH__
#define __BENCHDEC_HH__

// enable or disable
//#ifndef CCPP_BENCH
//#define CCPP_BENCH
//#endif

#include "bench/bench.hpp"

enum {
  B_INIT,
  B_STEP,
  B_SOLVER,
  B_UPDATE,
  B_TREEGEN,
  B_MULTIPOLE

};

#endif  //__BENCHDEC_HH__
