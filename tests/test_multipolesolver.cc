#include "global.hh"
#include "simenv.hh"
#include "solver/forcesolver.hh"

#include <gtest/gtest.h>
#include <eigen3/Eigen/Dense>

TEST(MultipoleTest, Monopole) {
  SimEnv emptysimenv;
  Multipolesolver solver(emptysimenv);

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
}

TEST(MultipoleTest, Quadrapole) {}
