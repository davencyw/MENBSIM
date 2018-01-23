#include "octree/octree.hh"

#include "global.hh"
#include "octree/octreenode.hh"

#include <array>
#include <cassert>
#include <iostream>
#include <stack>
#include <tuple>
#include <vector>

#include <eigen3/Eigen/Dense>

namespace oct {
void Octree::init() {
  std::tuple<Eigen::Vector3d, precision_t> geometry_tuple(getgeometry());
  _origin = std::get<0>(geometry_tuple);

  const precision_t halfwidth(std::get<1>(geometry_tuple));

  _root = new Octreenode(_origin, nullptr, halfwidth, _treeinfo);
  _numnodes = 8;

  const unsigned int numpoints(_xpos.size());

  // insert each point into tree
  for (unsigned point_i = 0; point_i < numpoints; ++point_i) {
    const precision_t x(_xpos(point_i));
    const precision_t y(_ypos(point_i));
    const precision_t z(_zpos(point_i));

    const unsigned int numnewnodes(_root->addpoint(point_i, x, y, z));
    _numnodes += numnewnodes;
    //*DEBUG*/ std::cout << "point " << point_i << '\n';
  }
}

std::tuple<Eigen::Vector3d, precision_t> Octree::getgeometry() {
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

// TODO(dave): rewrite tree to get leafnodes instantly (though its not too
// slow for affordable treesizes)
// TODO(dave): rewrite nodeindexing
std::vector<const Octreenode*> Octree::getleafnodes() {
  std::vector<const Octreenode*> leafnodes;
  std::stack<Octreenode*> nodestack;
  // children of root;
  for (auto child : (*_root->getchildren())) {
    nodestack.push(child);
  }

  // create indices along
  unsigned int nodeindex(0);

  while (!nodestack.empty()) {
    Octreenode* currentnode(nodestack.top());
    nodestack.pop();

    currentnode->setdataindex(nodeindex);
    ++nodeindex;

    if (currentnode->isleaf()) {
      leafnodes.push_back(currentnode);
    } else {
      for (auto child : (*currentnode->getchildren())) {
        nodestack.push(child);
      }
    }
  }
  return leafnodes;
}

}  // namespace oct
