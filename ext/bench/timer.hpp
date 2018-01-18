/*__DECLARATION__
 *
 *  DESCRIPTION
 *  ___________________________________________________________
 *
 *  Part of:
 *  Common CPP Library
 *  Common c++ library for everyday use. Always in development.
 *
 *  author: david schmidig [david@davencyw.net]
 *          DAVENCYW CODE [davencyw.net]
 *          Msc CSE ETH Zurich
 *
 *
 */

#ifndef __CCPP_TIMER__
#define __CCPP_TIMER__

#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#include <ctime>

namespace CCPP {

inline long int mach_absolute_time() {
  timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return (long int)(t.tv_sec * 1e9 + t.tv_nsec);
}

class Timer {
 private:
  long int _start, _end;

 public:
  enum Timeres { MICROSECONDS, MILLISECONDS, SECONDS };

  Timer() {
    _start = 0;
    _end = 0;
  }

  void start() {
    _end = 0;
    _start = mach_absolute_time();
  }

  void stop() { _end = mach_absolute_time(); }

  void reset() {
    _start = 0;
    _end = 0;
  }

  long int elapsed(int t) {
    // sanity check, more imprecise
    if (_end == 0) _end = mach_absolute_time();

    switch (t) {
      case MICROSECONDS:
        return (_end - _start) * 1e-3;
        break;
      case MILLISECONDS:
        return (_end - _start) * 1e-6;
        break;
      case SECONDS:
        return (_end - _start) * 1e-9;
        break;
      default:
        return (_end - _start) * 1e-6;
        break;
    }
  }

  // in milliseconds
  long int elapsedAndReset() {
    if (_end == 0) _end = mach_absolute_time();
    long int t = _end - _start;
    _start = _end;
    _end = 0;
    return t * 1e-6;
  }
};

}  // namespace CCPP

// end timer.hpp
#endif
