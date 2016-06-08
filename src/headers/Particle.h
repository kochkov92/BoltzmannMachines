#ifndef __BMACHINE_PARTICLE_H
#define __BMACHINE_PARTICLE_H

#include <iostream>
#include <vector>
#include <random>
#include "BmachineNamespace.h"
// #include "BoltzmannMachine.h"

using namespace bMachine;

class Particle {
private:
  // double energy_;
  bVec state_;
  ind offset_;

public:
  Particle();
  Particle(ind numNeurons);
  Particle(ind numNeurons, bVec& data);
  void initialize(ind numNeurons);
  ind proposeMove();
  bVec getState(); 
  bVec& getStateRef();
  void randInit();
};

#endif