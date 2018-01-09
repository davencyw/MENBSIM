#ifndef __FORCESOLVER_HH__
#define __FORCESOLVER_HH__

#include <global.hh>

class Forcesolver {
 public:
  Forcesolver() = default;
  virtual void solve(unsigned int numparticles, array_t& xpos, array_t& ypos,
                     array_t& zpos, array_t& masses, array_t& forcex,
                     array_t& forcey, array_t& forcez) = 0;
};

class Naivesolver : public Forcesolver {
 public:
  Naivesolver() = default;
  void solve(unsigned int numparticles, array_t& xpos, array_t& ypos,
             array_t& zpos, array_t& masses, array_t& forcex, array_t& forcey,
             array_t& forcez) override;
};

class Multipolesolver : public Forcesolver {
 public:
  Multipolesolver() = default;
  void solve(unsigned int numparticles, array_t& xpos, array_t& ypos,
             array_t& zpos, array_t& masses, array_t& forcex, array_t& forcey,
             array_t& forcez) override;
};

#endif  //__FORCESOLVER_HH__