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

#include <array>
#include <vector>

#include <eigen3/Eigen/Dense>

namespace oct {

class Octreenode {
 public:
  Octreenode(unsigned int indexinposarray)
      : _indexinposarray(indexinposarray) {}

  void setparent(Octreenode* node) { _parent = node; }
  Octreenode* getparent() { return _parent; }
  Octreenode* getchildren() { return _children[0]; }
  unsigned int getindex() { return _indexinposarray; }

  bool isleaf() { return !_numchildren; }

 private:
  Octreenode* _parent;
  std::array<Octreenode*, 8> _children;
  unsigned int _numchildren = 0;
  unsigned int _indexinposarray;
};

class Octree {
 public:
  Octree(Eigen::Vector3d origin, const Extent extent, const array_t& xpos,
         const array_t& ypos, const array_t& zpos)
      : _origin(origin),
        _extent(extent),
        _xpos(xpos),
        _ypos(ypos),
        _zpos(zpos) {}

  void insert(unsigned int _indexinposarray) {}
  Octreenode* getroot() { return _root; }

 private:
  Eigen::Vector3d _origin;
  const Extent _extent;

  const array_t& _xpos;
  const array_t& _ypos;
  const array_t& _zpos;

  Octreenode* _root;
};
}  // oct

#endif  // __OCTREE_HH__