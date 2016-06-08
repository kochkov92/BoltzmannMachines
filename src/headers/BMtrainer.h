#ifndef __BMACHINE_TRAINER_H
#define __BMACHINE_TRAINER_H

#include <iostream>
#include <thread>
#include <cstdlib>
#include "BmachineNamespace.h"
#include "BoltzmannMachine.h"
#include "Particle.h"
#include "dataInput.h"

using namespace bMachine;

class BMtrainer {
private:
  int id_;
  int samples_;
  double learningRate_;
  std::vector<Particle*> dataParticles;
  std::vector<Particle*> fantParticles;

  void constMult(dVec& vec, double val);
  void normalize(dVec& vec, int numSamples);
  void runWorker(BoltzmannMachine& machine,
                 int iter, int numWorkers, int workerId);

public:
  BMtrainer();
  ~BMtrainer();
  void train(BoltzmannMachine& machine, dataInput<bool>& data,
             int iter, int numWorkers);
};


#endif