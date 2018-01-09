#include "forcesolver.hh"

#include "global.hh"

void Naivesolver::solve(unsigned int numparticles, array_t& xpos, array_t& ypos,
                        array_t& zpos, array_t& masses, array_t& forcex,
                        array_t& forcey, array_t& forcez) {
  // TODO(dave): add softening
  // TODO(dave): remove self-force

  // NOT OPTIMIZED
  for (int i = 0; i < numparticles; ++i) {
    for (int j = 0; j < numparticles; ++j) {
      const precision_t x1(xpos(i));
      const precision_t y1(ypos(i));
      const precision_t z1(zpos(i));

      const precision_t x2(xpos(j));
      const precision_t y2(ypos(j));
      const precision_t z2(zpos(j));

      const precision_t x1x2(x1 - x2);
      const precision_t y1y2(y1 - y2);
      const precision_t z1z2(z1 - z2);

      const precision_t rmagnitude(
          std::sqrt(x1x2 * x1x2 + y1y2 * y1y2 + z1z2 * z1z2));

      const precision_t m1(masses(i));
      const precision_t m2(masses(j));

      const precision_t forcemagnitude =
          m1 * m2 / (rmagnitude * rmagnitude * rmagnitude);

      const precision_t fx1(rmagnitude * x1x2);
      const precision_t fy1(rmagnitude * y1y2);
      const precision_t fz1(rmagnitude * z1z2);

      forcex(i) = fx1;
      forcey(i) = fy1;
      forcez(i) = fz1;
      forcex(j) = -fx1;
      forcey(j) = -fy1;
      forcez(j) = -fz1;
    }
  }
};