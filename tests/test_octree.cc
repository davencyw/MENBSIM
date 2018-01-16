#include "global.hh"
#include "octree/octree.hh"

#include <iostream>

#include <eigen3/Eigen/Dense>
#include "gtest/gtest.h"

typedef Eigen::Array<precision_t, Eigen::Dynamic, 1> array_t;

TEST(OctreeTest, StandardData) {
  // test octree implementation with testdata
  const unsigned int leafsize(1);

  // create testdata
  Extent extent;
  array_t xpos, ypos, zpos;

  // input data
  oct::Octree octree(extent, xpos, ypos, zpos, leafsize);
  octree.init();

  // verify data
  EXPECT_TRUE(true);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
  return 0;
}
