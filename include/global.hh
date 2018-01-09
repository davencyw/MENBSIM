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

#endif  //__GLOBAL_HH__