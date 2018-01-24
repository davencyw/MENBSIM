#include "global.hh"
#include "octree/octree.hh"
#include "octree/octreenode.hh"

#include <array>
#include <iostream>
#include <vector>

#include <gtest/gtest.h>
#include <eigen3/Eigen/Dense>

TEST(OctreeTest, OneLevelXYZ) {
  // test octree implementation with testdata
  const unsigned int leafsize(1);

  // create testdata

  Extent extent;
  extent.x = extent.y = extent.z = std::make_pair(-1, 1);

  // in each cube below the root one point
  array_t xpos(8), ypos(8), zpos(8);
  xpos << -.25, .75, -.25, .75, -.25, .75, -.25, .75;
  ypos << -.25, -.25, -.25, -.25, .75, .75, .75, .75;
  zpos << .75, .75, -.25, -.25, .75, .75, -.25, -.25;

  // input data
  oct::Octree octree(extent, xpos, ypos, zpos, leafsize);
  octree.init();

  // verify data
  const std::array<oct::Octreenode*, 8>* children(
      octree.getroot()->getchildren());

  for (unsigned child_i = 0; child_i < 8; ++child_i) {
    auto indices((*children)[child_i]->getindices());
    EXPECT_EQ(indices->size(), 1);
    EXPECT_EQ((*indices)[0], child_i);
    auto child_children((*children)[child_i]->getchildren());
    // nullpointer test for uninitialized children
    for (auto child_child_i : *child_children) {
      EXPECT_EQ(nullptr, child_child_i);
    }
  }

  EXPECT_EQ(octree.getnumnodes(), 8);
}

TEST(OctreeTest, TwoLevelsX) {
  const unsigned int leafsize(1);

  Extent extent;
  extent.x = extent.y = extent.z = std::make_pair(0, 1);

  // two points in the same cube below root, one more split until level two
  // negative x
  {
    array_t xpos(2), ypos(2), zpos(2);
    xpos << .125, .375;
    ypos << .25, .25;
    zpos << .25, .25;

    oct::Octree octree(extent, xpos, ypos, zpos, leafsize);
    octree.init();

    // verify data
    const std::array<oct::Octreenode*, 8>* children(
        octree.getroot()->getchildren());

    const std::vector<unsigned int>* indices((*children)[2]->getindices());
    EXPECT_EQ(indices->size(), 2);
    const std::array<oct::Octreenode*, 8>* child_children =
        (*children)[2]->getchildren();

    // 4, 0
    const std::vector<unsigned int>* child_indices0 =
        (*child_children)[0]->getindices();
    EXPECT_EQ(child_indices0->size(), 1);
    EXPECT_EQ((*child_indices0)[0], 0);

    const std::vector<unsigned int>* child_indices1 =
        (*child_children)[1]->getindices();
    EXPECT_EQ(child_indices1->size(), 1);
    EXPECT_EQ((*child_indices1)[0], 1);

    EXPECT_EQ(octree.getnumnodes(), 16);
  }
  // two points in the same cube below root, one more split until level two
  // positive x
  {
    array_t xpos(2), ypos(2), zpos(2);

    xpos << .625, .875;
    ypos << .75, .75;
    zpos << .75, .75;

    oct::Octree octree(extent, xpos, ypos, zpos, leafsize);
    octree.init();

    // verify data
    const std::array<oct::Octreenode*, 8>* children(
        octree.getroot()->getchildren());

    const std::vector<unsigned int>* indices((*children)[5]->getindices());
    EXPECT_EQ(indices->size(), 2);
    const std::array<oct::Octreenode*, 8>* child_children =
        (*children)[5]->getchildren();

    const std::vector<unsigned int>* child_indices0 =
        (*child_children)[0]->getindices();
    EXPECT_EQ(child_indices0->size(), 1);
    EXPECT_EQ((*child_indices0)[0], 0);

    const std::vector<unsigned int>* child_indices2 =
        (*child_children)[1]->getindices();
    EXPECT_EQ(child_indices2->size(), 1);
    EXPECT_EQ((*child_indices2)[0], 1);

    EXPECT_EQ(octree.getnumnodes(), 16);
  }
}

TEST(OctreeTest, TwoLevelsY) {
  const unsigned int leafsize(1);

  Extent extent;
  extent.x = extent.y = extent.z = std::make_pair(0, 1);

  // two points in the same cube below root, one more split until level two
  // negative y
  {
    array_t xpos(2), ypos(2), zpos(2);
    xpos << .25, .25;
    ypos << .125, .375;
    zpos << .25, .25;

    oct::Octree octree(extent, xpos, ypos, zpos, leafsize);
    octree.init();

    // verify data
    const std::array<oct::Octreenode*, 8>* children(
        octree.getroot()->getchildren());

    const std::vector<unsigned int>* indices((*children)[2]->getindices());
    EXPECT_EQ(indices->size(), 2);
    const std::array<oct::Octreenode*, 8>* child_children =
        (*children)[2]->getchildren();

    // 4, 0
    const std::vector<unsigned int>* child_indices0 =
        (*child_children)[0]->getindices();
    EXPECT_EQ(child_indices0->size(), 1);
    EXPECT_EQ((*child_indices0)[0], 0);

    const std::vector<unsigned int>* child_indices1 =
        (*child_children)[4]->getindices();
    EXPECT_EQ(child_indices1->size(), 1);
    EXPECT_EQ((*child_indices1)[0], 1);

    EXPECT_EQ(octree.getnumnodes(), 16);
  }
  // two points in the same cube below root, one more split until level two
  // positive y
  {
    array_t xpos(2), ypos(2), zpos(2);

    xpos << .75, .75;
    ypos << .625, .875;
    zpos << .75, .75;

    oct::Octree octree(extent, xpos, ypos, zpos, leafsize);
    octree.init();

    // verify data
    const std::array<oct::Octreenode*, 8>* children(
        octree.getroot()->getchildren());

    const std::vector<unsigned int>* indices((*children)[5]->getindices());
    EXPECT_EQ(indices->size(), 2);
    const std::array<oct::Octreenode*, 8>* child_children =
        (*children)[5]->getchildren();

    const std::vector<unsigned int>* child_indices0 =
        (*child_children)[0]->getindices();
    EXPECT_EQ(child_indices0->size(), 1);
    EXPECT_EQ((*child_indices0)[0], 0);

    const std::vector<unsigned int>* child_indices2 =
        (*child_children)[4]->getindices();
    EXPECT_EQ(child_indices2->size(), 1);
    EXPECT_EQ((*child_indices2)[0], 1);

    EXPECT_EQ(octree.getnumnodes(), 16);
  }
}

TEST(OctreeTest, TwoLevelsZ) {
  const unsigned int leafsize(1);

  Extent extent;
  extent.x = extent.y = extent.z = std::make_pair(0, 1);

  // two points in the same cube below root, one more split until level two
  // negative z
  {
    array_t xpos(2), ypos(2), zpos(2);
    xpos << .25, .25;
    ypos << .25, .25;
    zpos << .125, .375;

    oct::Octree octree(extent, xpos, ypos, zpos, leafsize);
    octree.init();

    // verify data
    const std::array<oct::Octreenode*, 8>* children(
        octree.getroot()->getchildren());

    const std::vector<unsigned int>* indices((*children)[2]->getindices());
    EXPECT_EQ(indices->size(), 2);
    const std::array<oct::Octreenode*, 8>* child_children =
        (*children)[2]->getchildren();

    const std::vector<unsigned int>* child_indices0 =
        (*child_children)[2]->getindices();
    EXPECT_EQ(child_indices0->size(), 1);
    EXPECT_EQ((*child_indices0)[0], 0);

    const std::vector<unsigned int>* child_indices2 =
        (*child_children)[0]->getindices();
    EXPECT_EQ(child_indices2->size(), 1);
    EXPECT_EQ((*child_indices2)[0], 1);

    EXPECT_EQ(octree.getnumnodes(), 16);
  }
  // two points in the same cube below root, one more split until level two
  // positive z
  {
    array_t xpos(2), ypos(2), zpos(2);

    xpos << .75, .75;
    ypos << .75, .75;
    zpos << .625, .875;

    oct::Octree octree(extent, xpos, ypos, zpos, leafsize);
    octree.init();

    // verify data
    const std::array<oct::Octreenode*, 8>* children(
        octree.getroot()->getchildren());

    const std::vector<unsigned int>* indices((*children)[5]->getindices());
    EXPECT_EQ(indices->size(), 2);
    const std::array<oct::Octreenode*, 8>* child_children =
        (*children)[5]->getchildren();

    const std::vector<unsigned int>* child_indices0 =
        (*child_children)[2]->getindices();
    EXPECT_EQ(child_indices0->size(), 1);
    EXPECT_EQ((*child_indices0)[0], 0);

    const std::vector<unsigned int>* child_indices2 =
        (*child_children)[0]->getindices();
    EXPECT_EQ(child_indices2->size(), 1);
    EXPECT_EQ((*child_indices2)[0], 1);

    EXPECT_EQ(octree.getnumnodes(), 16);
  }
}

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

  EXPECT_EQ(octree.getnumnodes(), 8);
}
