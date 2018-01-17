#ifndef __OCTREENODE_HH__
#define __OCTREENODE_HH__

#include "global.hh"

#include <array>
#include <cassert>
#include <iostream>
#include <vector>

#include <eigen3/Eigen/Dense>

namespace oct {

// TODO(dave): Remove indicesvector on each level and only store on leaf level

class Octreenode {
 public:
  Octreenode(Eigen::Vector3d midpoint, unsigned int leafsize)
      : _midpoint(midpoint), _leafsize(leafsize) {}
  Octreenode(Eigen::Vector3d midpoint, unsigned int leafsize, bool root)
      : _midpoint(midpoint), _leafsize(leafsize), _root(root) {
    if (_root) {
      _leaf = false;
      // create children first
      for (unsigned child_i = 0; child_i < 8; ++child_i) {
        // TODO(dave): compute midpoint for child with _halfwidth
        Eigen::Vector3d midpoint(0, 0, 0);
        _children[child_i] = new Octreenode(midpoint, _leafsize);
      }
    }
  }

  const std::array<Octreenode*, 8>* getchildren() const { return &_children; }
  const Octreenode* getchildren(bool t) const { return _children[0]; }

  const std::vector<unsigned int>* getindices() const {
    return &_indexinposarray;
  }

  const unsigned int addpoint(const unsigned int indexinposarray,
                              const precision_t x, const precision_t y,
                              const precision_t z) {
    // if root or not a leaf, propagate down
    if (!_leaf) {
      if (!_root) {
        _indexinposarray.push_back(indexinposarray);
      }
      const unsigned int childindex(getchildindex(x, y, z, _midpoint));
      _children[childindex]->addpoint(indexinposarray, x, y, z);
      return childindex;
    }

    // if leaf
    if (_leaf) {
      _indexinposarray.push_back(indexinposarray);
      // leaf full
      if (_indexinposarray.size() > _leafsize) {
        std::cout << "splitted\n";
        assert(_indexinposarray.size() == _leafsize + 1);
        // set to no leaf
        _leaf = false;
        // create children
        for (unsigned child_i = 0; child_i < 8; ++child_i) {
          // TODO(dave): compute midpoint for child with _halfwidth
          Eigen::Vector3d midpoint(0, 0, 0);
          _children[child_i] = new Octreenode(midpoint, _leafsize);
        }
        // TODO(dave): split all containing points and propagate them down
        for (unsigned point_i = 0; point_i < 9; ++point_i) {
          const unsigned int point_indexinposarray(_indexinposarray[point_i]);
          // TODO(dave): Get coordinates from posarrays
          const precision_t x(0);
          const precision_t y(0);
          const precision_t z(0);
          const unsigned int childindex(getchildindex(x, y, z, _midpoint));
          _children[childindex]->addpoint(indexinposarray, x, y, z);
        }
      }
    }
  };

  bool isleaf() { return _leaf; }
  bool isroot() { return _root; }
  void setroot(bool root) { _root = root; }
  void setleaf(bool leaf) { _leaf = leaf; }

 private:
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
  unsigned int _leafsize = 8;
};

}  // oct

#endif  // __OCTREENODE_HH__
