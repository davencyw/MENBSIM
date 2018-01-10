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
    data.numparticles = std::get<0>(headertuple);

    for (int i = 0; i < data.numdata; ++i) {
      data.datavector.push_back(array_t(data.numparticles));
    }

    std::vector<std::pair<precision_t, precision_t>> extents;

    // read values
    for (int i = 0; i < data.numdata; ++i) {
      precision_t extent_min(0);
      precision_t extent_max(0);

      for (int j = 0; j < data.numparticles; ++j) {
        precision_t value;
        filestream >> value;
        data.datavector[i](j) = value;
        extent_min = std::min(extent_min, value);
        extent_max = std::max(extent_max, value);
      }

      extents.push_back(std::make_pair(extent_min, extent_max));
    }

    data.extent.x = extents[1];
    data.extent.y = extents[2];
    data.extent.z = extents[3];

    return data;
  }
};

#endif  //__INPUTREADER_HH__