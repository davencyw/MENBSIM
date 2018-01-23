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

static const precision_t splitterx[] = {-1, 1, -1, 1, -1, 1, -1, 1};
static const precision_t splittery[] = {-1, -1, -1, -1, 1, 1, 1, 1};
static const precision_t splitterz[] = {1, 1, -1, -1, 1, 1, -1, -1};

class Octreenode {
 public:
  Octreenode(Eigen::Vector3d midpoint, precision_t halfwidth,
             Treeinfo* treeinfo)
      : Octreenode(midpoint, nullptr, halfwidth, treeinfo) {}

  Octreenode(Eigen::Vector3d midpoint, Octreenode* parent,
             precision_t halfwidth, Treeinfo* treeinfo)
      : _parent(parent),
        _midpoint(midpoint),
        _halfwidth(halfwidth),
        _treeinfo(treeinfo) {
    if (parent == nullptr) {
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

  const bool isleaf() const { return _leaf; }
  const bool isroot() const { return _parent == nullptr; }
  void setleaf(bool leaf) { _leaf = leaf; }
  const unsigned int getdataindex() const { return _dataindex; }
  void setdataindex(unsigned int dataindex) { _dataindex = dataindex; }
  const Octreenode* getparent() const { return _parent; }

 private:
  const unsigned int splitleaf();
  void createchildren();
  inline const unsigned int getchildindex(const precision_t x,
                                          const precision_t y,
                                          const precision_t z,
                                          const Eigen::Vector3d& midpoint) {
    unsigned int index(0);
    // x axis
    if (x > midpoint(0)) index |= (1u);
    // y axis
    if (y > midpoint(1)) index |= (1u << 2);
    // z axis
    if (z < midpoint(2)) index |= (1u << 1);
    return index;
  }

  Octreenode* _parent = nullptr;
  std::array<Octreenode*, 8> _children = {};
  unsigned int _dataindex;

  std::vector<unsigned int> _indexinposarray;

  Eigen::Vector3d _midpoint;
  precision_t _halfwidth;

  bool _leaf = true;

  Treeinfo* _treeinfo;
};
}  // namespace oct

#endif  // __OCTREENODE_HH__
