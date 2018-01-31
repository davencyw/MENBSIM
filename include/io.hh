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

namespace io {

class Reader {
 public:
  static Datastruct readfromfile(std::string fullfilepath) {
    // open filestream
    // number of <particles, gasparticles, starparticles>
    std::tuple<int, int, int> headertuple;
    std::ifstream filestream;
    filestream.open(fullfilepath, std::ios::in);

    filestream >> std::get<0>(headertuple) >> std::get<1>(headertuple) >>
        std::get<2>(headertuple);

    // allocate memory
    Datastruct data;
    data.numparticles = std::get<0>(headertuple);

    for (unsigned int i = 0; i < data.numdata; ++i) {
      data.datavector.push_back(array_t(data.numparticles));
    }

    std::vector<std::pair<precision_t, precision_t>> extents;

    // read values
    for (unsigned int i = 0; i < data.numdata; ++i) {
      precision_t extent_min(0);
      precision_t extent_max(0);

      for (unsigned int j = 0; j < data.numparticles; ++j) {
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

class Writer {
 public:
  // only writes location of entities to a csv file
  static void writepostofile(std::string fullfilepath, array_t& xpos,
                             array_t& ypos, array_t& zpos) {
    const unsigned int numentities(xpos.size());

    std::ofstream filestream;
    filestream.open(fullfilepath, std::ios::out);

    // write header
    filestream << "XPOS,YPOS,ZPOS," << numentities << "\n";

    // write data
    for (unsigned entitiy_i = 0; entitiy_i < numentities; ++entitiy_i) {
      filestream << xpos(entitiy_i) << "," << ypos(entitiy_i) << ","
                 << zpos(entitiy_i) << "\n";
    }

    // finalize
    filestream.close();
  }

  static void write1ddensitytofile(std::string fullfilepath, array_t& bins,
                                   std::string header) {
    const unsigned int numbins(bins.size());

    std::ofstream filestream;
    filestream.open(fullfilepath, std::ios::out);

    // write header
    filestream << header << "\n";

    // write data
    for (unsigned bin_i = 0; bin_i < numbins - 1; ++bin_i) {
      filestream << bins(bin_i) << " , ";
    }

    // finalize
    filestream << bins(numbins - 1);
    filestream.close();
  }
};
}  // namespace io

#endif  //__INPUTREADER_HH__
