#ifndef __BENCHDEC_HH__
#define __BENCHDEC_HH__

// enable or disable
#ifndef CCPP_BENCH
#define DCCPP_BENCH
#endif

#include "bench/bench.hpp"

enum {
  B_INIT,
  B_STEP,
  B_SOLVER,
  B_UPDATE

};

#endif  //__BENCHDEC_HH__
