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
#ifndef __OCTREENODE_HH__
#define __OCTREENODE_HH__

#include "global.hh"

#include <array>
#include <cassert>
#include <iostream>
#include <vector>

#include <eigen3/Eigen/Dense>

namespace oct {

struct Treeinfo {
  Treeinfo(const array_t* xpos, const array_t* ypos, const array_t* zpos,
           const unsigned int leafsize)
      : xpos(xpos), ypos(ypos), zpos(zpos), leafsize(leafsize) {
    assert(leafsize > 0);
  }
  const array_t* xpos;
  const array_t* ypos;
  const array_t* zpos;
  const unsigned int leafsize;
};

// TODO(dave): remove indicesvector on each level and only store on leaf level
// TODO(dave): descructor for children

static const precision_t splitterx[] = {1, 1, 1, 1, -1, -1, -1, -1};
static const precision_t splittery[] = {1, -1, 1, -1, 1, -1, 1, -1};
static const precision_t splitterz[] = {-1, -1, 1, 1, -1, -1, 1, 1};

class Octreenode {
 public:
  Octreenode(Eigen::Vector3d midpoint, precision_t halfwidth,
             Treeinfo* treeinfo)
      : _midpoint(midpoint), _halfwidth(halfwidth), _treeinfo(treeinfo) {
      }
  Octreenode(Eigen::Vector3d midpoint, bool root, precision_t halfwidth,
             Treeinfo* treeinfo)
      : _midpoint(midpoint),
        _root(root),
        _halfwidth(halfwidth),
        _treeinfo(treeinfo) {
    if (_root) {
      // root is never a leaf
      _leaf = false;
      // create children
      createchildren();
    }
  }

  const std::array<Octreenode*, 8>* getchildren() const { return &_children; }
  const std::vector<unsigned int>* getindices() const {
    return &_indexinposarray;
  }

  const unsigned int addpoint(const unsigned int indexinposarray,
                              const precision_t x, const precision_t y,
                              const precision_t z);

  bool isleaf() { return _leaf; }
  bool isroot() { return _root; }
  void setroot(bool root) { _root = root; }
  void setleaf(bool leaf) { _leaf = leaf; }

 private:
  void splitleaf();
  void createchildren();
  inline const unsigned int getchildindex(const precision_t x,
                                          const precision_t y,
                                          const precision_t z,
                                          const Eigen::Vector3d& midpoint) {
    unsigned int index(0);
    // x axis
    if (x < midpoint(0)) {
      index |= (1u << 2);
    }
    // y axis
    if (y < midpoint(1)) {
      index |= (1u);
    }
    // z axis
    if (z > midpoint(2)) {
      index |= (1u << 1);
    }
    return index;
  }

  std::array<Octreenode*, 8> _children;

  std::vector<unsigned int> _indexinposarray;

  Eigen::Vector3d _midpoint;
  precision_t _halfwidth;

  bool _leaf = true;
  bool _root = false;

  Treeinfo* _treeinfo;
};

}  // oct

#endif  // __OCTREENODE_HH__
