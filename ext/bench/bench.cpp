#include "bench.hpp"

#ifdef CCPP_BENCH

namespace CCPP {

// bench title - optional
std::string BENCH::_title = "";
std::string BENCH::_info = "";

// number of profiled sections
int BENCH::_n = -1;
// name of profiled section
std::vector<std::string> BENCH::_names;
// realtions
std::vector<int> BENCH::_father;
std::vector<int> BENCH::_top;
std::vector<std::vector<int>> BENCH::_children;
// time of profiled section [ms]
std::vector<long int> BENCH::_times;
// timer for every section
std::vector<Timer> BENCH::_timers;
// resolution of timer - milliseconds default
int BENCH::_res = BENCH::MILLISECONDS;
std::string BENCH::_resname = "ms";

}  // namespace CCPP

#endif
