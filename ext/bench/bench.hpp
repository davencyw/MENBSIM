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

#ifndef __CCPP_BENCH__
#define __CCPP_BENCH__

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "timer.hpp"

namespace CCPP {

class BENCH {
 public:
  enum Timeres {
#ifdef CCPP_BENCH
    MICROSECONDS = Timer::MICROSECONDS,
    MILLISECONDS = Timer::MILLISECONDS,
    SECONDS = Timer::SECONDS
#endif
  };

  static void inline settitle(std::string title) {
#ifdef CCPP_BENCH
    _title = title;
#endif
  }

  static void inline setresolution(Timeres res) {
#ifdef CCPP_BENCH
    _res = res;

    // get time resolution unit to string
    switch (_res) {
      case MICROSECONDS:
        _resname = "μs";
        break;
      case MILLISECONDS:
        _resname = "ms";
        break;
      case SECONDS:
        _resname = "s";
        break;
      default:
        _resname = "TIMEERROR";
        break;
    }
#endif
  }

  static void inline reg(std::string name) {
#ifdef CCPP_BENCH
    transform(name.begin(), name.end(), name.begin(), ::toupper);
    _names.emplace_back(name);
    _timers.emplace_back(Timer());
    _times.emplace_back(0);
    ++_n;
    _father.push_back(-1);  // no father
    _top.push_back(_n);
    _children.emplace_back(std::vector<int>());

#endif
  }

  static void inline regchild(std::string name, int father) {
#ifdef CCPP_BENCH

    // TODO eror handling if father > _n
    // TODO check cyclic relations (not allowed)

    transform(name.begin(), name.end(), name.begin(), ::toupper);
    _names.emplace_back(name);
    _timers.emplace_back(Timer());
    _times.emplace_back(0);
    ++_n;
    _father.push_back(father);
    _children[father].push_back(_n);
    _children.emplace_back(std::vector<int>());

#endif
  }

  static void inline start(const int i) {
#ifdef CCPP_BENCH
    _timers[i].start();

#endif
  }

  static void inline stop(const int i) {
#ifdef CCPP_BENCH
    _timers[i].stop();
    _times[i] += _timers[i].elapsed(_res);
    _timers[i].reset();
#endif
  }

  static long int inline gettime(const int i) {
#ifdef CCPP_BENCH
    return _times[i];
#endif
    return 0;
  }

  static void inline summarize() {
#ifdef CCPP_BENCH

    if (_top.size() == 0) {
      // print BENCH error message - no top level section found
      if (!_title.empty()) _title.append("\n");

      std::cout << "\n\n\n_________________\n\n"
                << "P R O F I L I N G\n"
                << _title << "_________________\n\n\n"
                << "E R R O R\n"
                << "no sections registered";
      return;
    }

    if (!_title.empty()) _title.append("\n");

    std::cout << "\n\n\n_________________\n\n"
              << "P R O F I L I N G\n"
              << _title << "_________________\n\n\n";

    for (unsigned int i = 0; i < _top.size(); ++i) {
      // get id of top section
      int s(_top[i]);
      summarizechild(s, 0);
    }

#endif
  }

  static void inline summarizechild(int s, int l) {
#ifdef CCPP_BENCH

    // TODO make loops c++11/14 style
    // TODO make colored

    // box drawing chars
    std::string bfin("╠═════");
    std::string bspacing("");

    // get total time for top level sections
    long int totale(0);
    if (l == 0) {
      for (unsigned int i = 0; i < _top.size(); ++i) totale += _times[_top[i]];
    } else {
      totale = _times[_father[s]];
    }

    // print title and time
    for (int i = 1; i <= l; ++i) {
      bspacing.append("\t");
      bfin.insert(0, "\t");
    }

    bfin.append("  ");

    double p(0);
    if (totale != 0) p = ((double)_times[s] / (double)totale) * 100.0;

    std::cout << bfin << _names[s] << std::endl
              << bspacing << "║"
              << "runtime " << _resname << "\t\t" << _times[s] << std::endl
              << bspacing << "║"
              << "percentage of runtime \t" << p << "%" << std::endl
              << std::endl;

    for (unsigned int i = 0; i < _children[s].size(); ++i) {
      int ss((_children[s])[i]);
      summarizechild(ss, l + 1);
    }

#endif
  }

  static void summarytocsv(std::string filename) {
#ifdef CCPP_BENCH

    if (_title.empty()) _title = "NO TITLE";
    if (_info.empty()) _info = "NO INFO";

    // open file
    std::ofstream fileout;
    fileout.open(filename);

    // write summary
    fileout << _title << std::endl
            << _info << std::endl
            << "resolution: " << _resname << std::endl;

    // write csv description
    fileout << "id;name;time[" << _resname << "];fatherid" << std::endl;

    // write data
    for (int i = 0; i <= _n; ++i) {
      fileout << i << ";" << _names[i] << ";" << _times[i] << ";" << _father[i]
              << std::endl;
    }

    // close filestream
    fileout.close();

#endif
  }

 private:
#ifdef CCPP_BENCH

  // bench title - optional
  static std::string _title;

  // info of program for summary - optional
  static std::string _info;

  // number of profiled sections
  static int _n;
  // name of profiled section
  static std::vector<std::string> _names;
  // relations pair(father, child)
  static std::vector<int> _father;
  static std::vector<int> _top;
  static std::vector<std::vector<int>> _children;
  // time of profiled section [ms]
  static std::vector<long int> _times;
  // timer for every section
  static std::vector<Timer> _timers;
  // resolution of timer
  static int _res;
  static std::string _resname;

#endif
};

}  // namespace CCPP

// bench.hpp
#endif
