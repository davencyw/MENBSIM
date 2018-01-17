#include "global.hh"
#include "octree/octree.hh"
#include "octree/octreenode.hh"

#include <array>
#include <iostream>
#include <vector>

#include <eigen3/Eigen/Dense>
#include "gtest/gtest.h"

typedef Eigen::Array<precision_t, Eigen::Dynamic, 1> array_t;

TEST(OctreeTest, OneLevel) {
  // test octree implementation with testdata
  const unsigned int leafsize(1);

  // create testdata
  static const unsigned int indexorder[] = {5, 7, 4, 6, 1, 3, 0, 2};

  Extent extent;
  extent.x = extent.y = extent.z = std::make_pair(-1, 1);

  // in each cube below the root one point
  array_t xpos(8), ypos(8), zpos(8);
  xpos << -.25, -.25, -.25, -.25, .75, .75, .75, .75;
  ypos << -.25, -.25, .75, .75, -.25, -.25, .75, .75;
  zpos << -.25, .75, -.25, .75, -.25, .75, -.25, .75;

  // input data
  oct::Octree octree(extent, xpos, ypos, zpos, leafsize);
  octree.init();

  // verify data
  const std::array<oct::Octreenode*, 8>* children(
      octree.getroot()->getchildren());

  for (unsigned child_i = 0; child_i < 8; ++child_i) {
    auto indices((*children)[indexorder[child_i]]->getindices());
    EXPECT_EQ(indices->size(), 1);
    EXPECT_EQ((*indices)[0], child_i);
    auto child_children((*children)[indexorder[child_i]]->getchildren());
    // nullpointer test for uninitialized children
    for (auto child_child_i : *child_children) {
      EXPECT_EQ(nullptr, child_child_i);
    }
  }
}

TEST(OctreeTest, TwoLevels) {
  const unsigned int leafsize(1);

  Extent extent;
  extent.x = extent.y = extent.z = std::make_pair(0, 1);

  // two points in the same cube below root, one more split until level two
  array_t xpos(4), ypos(4), zpos(4);
  xpos << .25, .25, .75, .75;
  ypos << .25, .25, .75, .75;
  zpos << .125, .375, .625, .875;

  oct::Octree octree(extent, xpos, ypos, zpos, leafsize);
  octree.init();

  // TODO(dave): verify data
  const std::array<oct::Octreenode*, 8>* children(
      octree.getroot()->getchildren());

  const std::vector<unsigned int>* indices2((*children)[2]->getindices());
  const std::vector<unsigned int>* indices5((*children)[5]->getindices());
  EXPECT_EQ(indices2->size(), 2);
  EXPECT_EQ(indices5->size(), 2);
}

TEST(OctreeTest, ThreeLevels) {
  const unsigned int leafsize(1);

  Extent extent;
  extent.x = extent.y = extent.z = std::make_pair(0, 1);

  // two points in the same cube below root, one more split until level two
  array_t xpos(2), ypos(2), zpos(2);
  xpos << 0, .124;
  ypos << 0, 0;
  zpos << 0, 0;

  oct::Octree octree(extent, xpos, ypos, zpos, leafsize);
  octree.init();

  // TODO(dave): verify data
  const std::array<oct::Octreenode*, 8>* children(
      octree.getroot()->getchildren());
}

TEST(DISABLED_OctreeTest, AllInOneCube) {}

TEST(OctreeTest, OnEdge) {
  const unsigned int leafsize(2);

  Extent extent;
  extent.x = extent.y = extent.z = std::make_pair(-1, 1);

  // two points on the edge of the first level
  array_t xpos(2), ypos(2), zpos(2);
  xpos << 0, 0;
  ypos << 0, 0;
  zpos << 0, 0;

  oct::Octree octree(extent, xpos, ypos, zpos, leafsize);
  octree.init();

  // verify data
  const std::array<oct::Octreenode*, 8>* children(
      octree.getroot()->getchildren());

  auto indices((*children)[0]->getindices());
  EXPECT_EQ(indices->size(), 2);
  EXPECT_EQ((*indices)[0], 0);
  EXPECT_EQ((*indices)[1], 1);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
