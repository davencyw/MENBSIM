/*__DECLARATION__
 *
 *
 *      MENBSIM
 *      N-Body Simulation of a elliptic galaxy using multipole expansions.
 *      This project is done in the lecture of computational astrophysics
 *      in 2017 at University of Zurich (UZH).
 *
 *      author:
 *      david schmidig         [     david@davencyw.net   ]
 *      ETH Zurich             [ davschmi@student.ethz.ch ]
 *      DAVENCYW CODE          [        davencyw.net      ]
 */
#ifndef __GLOBAL_HH__
#define __GLOBAL_HH__

#include <vector>

#include <eigen3/Eigen/Dense>

// program properties
#define __P_NAME "MENBSIM"
#define __P_DEF_NTHREADS 4
#define __P_DEF_SCHEDULE 1
#define __P_DEF_CUDA false
#define __P_DEF_OUTFOLDER "./"

// typedefs
typedef double precision_t;
typedef Eigen::Array<precision_t, Eigen::Dynamic, 1> array_t;

// structs
struct Extent {
  std::pair<precision_t, precision_t> x;
  std::pair<precision_t, precision_t> y;
  std::pair<precision_t, precision_t> z;
};

struct Datastruct {
  unsigned int numparticles;
  static const int numdata = 9;

  // 0 array_t masses;
  // 1 array_t xposition;
  // 2 array_t yposition;
  // 3 array_t zposition;
  // 4 array_t xvelocity;
  // 5 array_t yvelocity;
  // 6 array_t zvelocity;
  // 7 array_t softening;
  // 8 array_t potential;
  std::vector<array_t> datavector;
  Extent extent;
};

#endif  //__GLOBAL_HH__
