#include "global.hh"
#include "simenv.hh"
#include "solver/forcesolver.hh"

#include <gtest/gtest.h>
#include <eigen3/Eigen/Dense>

TEST(MultipoleTest, Monopole) {
  SimEnv simenv;
  simenv._octreeleafnodesize = 1;
  Multipolesolver solver(simenv);

  const unsigned int numparticles(3);

  array_t xpos(numparticles), ypos(numparticles), zpos(numparticles);
  array_t masses(numparticles);
  array_t forcex(numparticles), forcey(numparticles), forcez(numparticles);

  xpos << 0.0, 0.0, 0.0;
  ypos << 0.0, 0.0, 0.0;
  zpos << 0.0, 0.125, 0.25;
  masses << 1.0, 2.0, 3.0;

  Extent extent;
  extent.x = extent.y = extent.z = std::make_pair(0, 1);

  solver.solve(numparticles, xpos, ypos, zpos, masses, forcex, forcey, forcez,
               0.1, extent);
  array_t* monopole(solver.getmonopole());
  EXPECT_EQ((*monopole)(5), 6);
  EXPECT_EQ((*monopole)(11), 3);
  EXPECT_EQ((*monopole)(17), 1);
  EXPECT_EQ((*monopole)(19), 2);
  EXPECT_EQ((*monopole)(21), 3);
}

TEST(MultipoleTest, Quadrapole) {}
