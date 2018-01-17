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
#ifndef __OCTREE_HH__
#define __OCTREE_HH__

#include "global.hh"
#include "octree/octreenode.hh"

#include <array>
#include <cassert>
#include <iostream>
#include <tuple>
#include <vector>

#include <eigen3/Eigen/Dense>

namespace oct {

// TODO(dave): write destructor

class Octree {
 public:
  Octree(const Extent extent, const array_t& xpos, const array_t& ypos,
         const array_t& zpos, const int leafsize)
      : _extent(extent), _xpos(xpos), _ypos(ypos), _zpos(zpos) {
    _treeinfo = new Treeinfo(&_xpos, &_ypos, &_zpos, leafsize);
    assert(leafsize > 0);
  }

  void init() {
    std::tuple<Eigen::Vector3d, precision_t> geometry_tuple(getgeometry());
    _origin = std::get<0>(geometry_tuple);

    const precision_t halfwidth(std::get<1>(geometry_tuple));

    _root = new Octreenode(_origin, true, halfwidth, _treeinfo);

    const unsigned int numpoints(_xpos.size());

    // insert each point into tree
    for (unsigned point_i = 0; point_i < numpoints; ++point_i) {
      const precision_t x(_xpos(point_i));
      const precision_t y(_ypos(point_i));
      const precision_t z(_zpos(point_i));

      _root->addpoint(point_i, x, y, z);
    }
  }

  const Octreenode* getroot() { return _root; }

  const array_t& _xpos;
  const array_t& _ypos;
  const array_t& _zpos;

 private:
  std::tuple<Eigen::Vector3d, precision_t> getgeometry() {
    const precision_t xorigin((_extent.x.second + _extent.x.first) / 2.0);
    const precision_t yorigin((_extent.y.second + _extent.y.first) / 2.0);
    const precision_t zorigin((_extent.z.second + _extent.z.first) / 2.0);

    const precision_t xwidth((_extent.x.second - _extent.x.first));
    const precision_t ywidth((_extent.y.second - _extent.y.first));
    const precision_t zwidth((_extent.z.second - _extent.z.first));

    const precision_t width(std::max(std::max(xwidth, ywidth), zwidth));

    Eigen::Vector3d midpoint(xorigin, yorigin, zorigin);
    auto geometry_tuple = std::make_tuple(midpoint, width * .5);

    return geometry_tuple;
  }

  Eigen::Vector3d _origin;
  const Extent _extent;

  Treeinfo* _treeinfo;

  Octreenode* _root;
};
}  // oct

#endif  // __OCTREE_HH__
