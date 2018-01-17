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
#include <vector>

#include <eigen3/Eigen/Dense>

namespace oct {

struct Infoarrays {
  array_t* xpos;
  array_t* ypos;
  array_t* zpos;
};

class Octree {
 public:
  Octree(const Extent extent, const array_t& xpos, const array_t& ypos,
         const array_t& zpos, const int leafsize)
      : _extent(extent),
        _xpos(xpos),
        _ypos(ypos),
        _zpos(zpos),
        _leafsize(leafsize) {}

  void init() {
    _origin = getmidpoint();
    _root = new Octreenode(_origin, _leafsize, true);

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
  Eigen::Vector3d getmidpoint() {
    precision_t xorigin((_extent.x.second + _extent.x.first) / 2.0);
    precision_t yorigin((_extent.y.second + _extent.y.first) / 2.0);
    precision_t zorigin((_extent.z.second + _extent.z.first) / 2.0);
    return Eigen::Vector3d(xorigin, yorigin, zorigin);
  }

  Eigen::Vector3d _origin;
  const Extent _extent;

  const int _leafsize;

  Octreenode* _root;
};
}  // oct

#endif  // __OCTREE_HH__
