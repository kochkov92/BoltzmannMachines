#ifndef __BMACHINE_BMACHINE_H
#define __BMACHINE_BMACHINE_H

#include <iostream> // printing
#include <fstream>
#include <cmath> // exp function, move to particle
#include <memory> // shared_pointer
#include "BmachineNamespace.h"
#include "Particle.h"


using namespace bMachine;

class BoltzmannMachine {
private:
  bool initialized;
  double initAmplitude;
  ind numNeurons_; // how many Neurons we have
  ind numVisibleNeurons_;
  ind equiSteps_;
  ind burnSteps_;
  edges bonds_; // primary list of links in the machine
  edges auxBonds_; // auxil. improve dE computation
  dVec weights_; // parameters of the machine
  indVec locInBonds_; // lookup table for links in bonds
  indVec locInAuxBonds_; // lookup table for links in auxBonds
  Particle equilibratedParticle;

  std::default_random_engine generator_;
  std::uniform_real_distribution<double> distribution_;

  void updateState(Particle* particle);
  void updateCorrelations(Particle* particle, dVec& correlations, double sign);
  double moveAcceptence(ind position, bVec& state);
  void makeLookupTable(edges& bondsVec, indVec& table);

public:
  BoltzmannMachine(ind numNeurons, ind numVisibleNeurons);
  ind numNeurons(); // do I even need that?
  ind numBonds(); // and this?
  void initialize(); 
  void printInfo();
  void addEdge(ind i, ind j);
  void updateWeights(dVec& update); // implement that!
  double computeEnergy(bVec& state); // maybe usefull
  void getStatistics(Particle* particle, dVec& correlations,
                     ind numSamples, double sign);
  bVec generateExample(int numUpdates);
  void generateExamples(int numExamples, int numUpdates);
  Particle* makeFantasyParticle();
  Particle* makeParticle(bVec& data);
  // Particle* makeParticle(bVec);
};

#endif