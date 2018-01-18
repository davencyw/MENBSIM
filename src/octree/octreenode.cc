#include "octree/octreenode.hh"

#include "global.hh"

namespace oct {

const int Octreenode::addpoint(const unsigned int indexinposarray,
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
    if (_indexinposarray.size() > _treeinfo->leafsize) {
      splitleaf();
    }
  }
  // TODO(dave): add index where inserted
  return -1;
}

void Octreenode::splitleaf() {
  /*DEBUG*/ std::cout << "splitted\n";
  assert(_indexinposarray.size() == _treeinfo->leafsize + 1);
  // set to no leaf
  _leaf = false;
  // create children
  createchildren();

  // split all containing points and propagate them down
  for (unsigned int point_indexinposarray : _indexinposarray) {
    // get coordinates from posarrays
    const precision_t x((*_treeinfo->xpos)(point_indexinposarray));
    const precision_t y((*_treeinfo->ypos)(point_indexinposarray));
    const precision_t z((*_treeinfo->zpos)(point_indexinposarray));
    const unsigned int childindex(getchildindex(x, y, z, _midpoint));

    //*DEBUG*/ std::cout << _midpoint << "\n";
    //*DEBUG*/ std::cout << "ci: " << childindex << "\n";
    _children[childindex]->addpoint(point_indexinposarray, x, y, z);
  }
}

void Octreenode::createchildren() {
  // an octreenode has always 8 children
  for (unsigned child_i = 0; child_i < 8; ++child_i) {
    // compute midpoint for child with _halfwidth
    const precision_t quarterwidth(0.5 * _halfwidth);
    Eigen::Vector3d midpoint(splitterx[child_i] * quarterwidth,
                             splittery[child_i] * quarterwidth,
                             splitterz[child_i] * quarterwidth);
    midpoint += _midpoint;
    _children[child_i] = new Octreenode(midpoint, quarterwidth, _treeinfo);
  }
}

}  // namespace oct
