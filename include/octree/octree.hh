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
// TODO(dave): use smart pointers
// TODO(dave): add statistics for tree generation
// TODO(dave): add node indices for tree
// TODO(dave): add data object on treenode (see node indices for tree)

class Octree {
 public:
  Octree(const Extent extent, const array_t& xpos, const array_t& ypos,
         const array_t& zpos, const int leafsize)
      : _xpos(xpos), _ypos(ypos), _zpos(zpos), _extent(extent) {
    _treeinfo = new Treeinfo(&_xpos, &_ypos, &_zpos, leafsize);
    assert(leafsize > 0);
  }

  void init();

  const Octreenode* getroot() { return _root; }

  const unsigned int getnumnodes() { return _numnodes; }

  const array_t& _xpos;
  const array_t& _ypos;
  const array_t& _zpos;

 private:
  std::tuple<Eigen::Vector3d, precision_t> getgeometry();

  Eigen::Vector3d _origin;
  const Extent _extent;

  Treeinfo* _treeinfo;
  unsigned int _numnodes = 0;

  Octreenode* _root;
};
}  // namespace oct

#endif  // __OCTREE_HH__
