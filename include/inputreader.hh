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
#ifndef __INPUTREADER_HH__
#define __INPUTREADER_HH__

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

struct datastruct {
  datastruct() { datavector.reserve(numdata); }

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
  std::vector<std::pair<precision_t, precision_t>> extent;
};

class Inputreader {
 public:
  static datastruct readfromfile(std::string fullfilepath) {
    // open filestream
    // number of <particles, gasparticles, starparticles>
    std::tuple<int, int, int> headertuple;
    std::ifstream filestream;
    filestream.open(fullfilepath, std::ios::in);

    filestream >> std::get<0>(headertuple) >> std::get<1>(headertuple) >>
        std::get<2>(headertuple);

    // allocate memory
    datastruct data;
    for (int i = 0; i < data.numdata; ++i) {
      data.datavector[i] = array_t(std::get<0>(headertuple));
    }

    // read values
    for (int i = 0; i < data.numdata; ++i) {
      precision_t extent_min(0);
      precision_t extent_max(0);

      for (int j = 0; j < std::get<0>(headertuple); ++j) {
        precision_t value;
        filestream >> value;
        data.datavector[i](j) = value;
        extent_min = std::min(extent_min, value);
        extent_max = std::max(extent_max, value);
      }

      data.extent[i] = std::make_pair(extent_min, extent_max);
    }

    return data;
  }
};

#endif  //__INPUTREADER_HH__