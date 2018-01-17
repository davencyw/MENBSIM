#include "global.hh"
#include "octree/octree.hh"
#include "octree/octreenode.hh"

#include <iostream>

#include <eigen3/Eigen/Dense>
#include "gtest/gtest.h"

typedef Eigen::Array<precision_t, Eigen::Dynamic, 1> array_t;

TEST(OctreeTest, OneLevel) {
  // test octree implementation with testdata
  const unsigned int leafsize(1);

  // create testdata
  static const unsigned int indexorder[] = {5, 7, 4, 6, 1, 3, 0, 2};

  Extent extent;
  extent.x = std::make_pair(-1, 1);
  extent.y = std::make_pair(-1, 1);
  extent.z = std::make_pair(-1, 1);

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
  }

  // for (unsigned child_i = 0; child_i < 8; ++child_i) {
  //   auto ptr = (children + child_i)->getchildren();
  //   for (unsigned child_j = 0; child_j < 8; ++child_j) {
  //     EXPECT_EQ(NULL, ptr + child_j);
  //   }
  // }
}

TEST(OctreeTest, Twolevels) {}
TEST(OctreeTest, AllInOneCube) {}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
