#include "../headers/BoltzmannMachine.h"

using namespace bMachine;

bool bondOrder(bond i, bond j) { 
  return i.first < j.first;
}

BoltzmannMachine::BoltzmannMachine(ind numNeurons, ind numVisibleNeurons) {
  initialized = false;
  initAmplitude = 0.0;
  numNeurons_ = numNeurons;
  numVisibleNeurons_ = numVisibleNeurons;
  locInBonds_.resize(numNeurons_, 0);
  locInAuxBonds_.resize(numNeurons_, 0);
  equiSteps_ = 100000;
  burnSteps_ = 300;
  // equiSteps_ = 4 * numNeurons_ * numNeurons_; // is it reasonable?
  equilibratedParticle.initialize(numNeurons_);
  std::cout << "Boltzman machine was initialized \n";
}

void BoltzmannMachine::printInfo() {
  for (int i = 0; i < weights_.size(); ++i) {
    std::cout << bonds_[i].first << " " << bonds_[i].second << " " << weights_[i] << "\n";
  }
}

void BoltzmannMachine::addEdge(ind i, ind j) {
  if (i < j) {
    bonds_.push_back(std::make_pair(i,j));
    auxBonds_.push_back(std::make_pair(j,i));
    weights_.push_back((0.1 - distribution_(generator_)) * initAmplitude);
  }
  else {
    bonds_.push_back(std::make_pair(j,i));
    auxBonds_.push_back(std::make_pair(i,j));
    weights_.push_back((0.1 - distribution_(generator_)) * initAmplitude);
  }
}

void BoltzmannMachine::updateWeights(dVec& update) {
  for (int i = 0; i < weights_.size(); ++i) {
    weights_[i] += update[i];
  }
}

void BoltzmannMachine::makeLookupTable(edges& bondsVec, indVec& table) {
  ind prevNeuron = 0;
  table[0] = 0;
  for (ind i = 0; i < bondsVec.size(); ++i) {
    if (bondsVec[i].first != prevNeuron) {
      prevNeuron = bondsVec[i].first;
      table[prevNeuron] = i;
    }
  }
}

void BoltzmannMachine::initialize() {
  std::sort(bonds_.begin(), bonds_.end(), bondOrder);
  std::sort(auxBonds_.begin(), auxBonds_.end(), bondOrder);
  makeLookupTable(bonds_, locInBonds_);
  makeLookupTable(auxBonds_, locInAuxBonds_);
  initialized = true;
}

ind BoltzmannMachine::numNeurons() {
  return numNeurons_;
}

ind BoltzmannMachine::numBonds() {
  return bonds_.size();
}

double BoltzmannMachine::computeEnergy(bVec& state) {
  double energy = 0.;
  for (int i = 0; i < bonds_.size(); ++i) {
    if (state[bonds_[i].first] == true && state[bonds_[i].second] == true) {
      energy -= weights_[i];
    }
  }
  return energy;
}


bVec BoltzmannMachine::generateExample(int numUpdates) {
  Particle* configuration = makeFantasyParticle();
  for (int i = 0; i < numUpdates; ++i) {
    // updateState(&equilibratedParticle);
    updateState(configuration);
  }
  // bVec visNeurons = equilibratedParticle.getState();
  bVec visNeurons = configuration->getState();
  // delete configuration;
  visNeurons.resize(numVisibleNeurons_);
  return visNeurons;
}

void BoltzmannMachine::generateExamples(int numExamples, int numUpdates) {
  std::ofstream fOut;
  fOut.open("generatedExamples.txt");
  for (int i = 0; i < numExamples; ++i) {
    bVec example = generateExample(numUpdates);
    for (auto it = example.begin(); it < example.end(); ++it) {
      fOut << *it << " ";
    }
    fOut << "\n";
  }
  fOut.close();
}


double BoltzmannMachine::moveAcceptence(ind position, bVec& state) {
  double energyChange = 0.;
  ind bond = locInBonds_[position];
  while (bonds_[bond].first == position) { // accumulating contributions from
    if (state[bonds_[bond].second] == true) { // bonds connecting "position"
      if (state[position] == false) { // with neurons with higher ind
        energyChange -= weights_[bond];
      }
      else {
        energyChange += weights_[bond];
      }
    }
    bond++; // move to the next bond
  }

  bond = locInAuxBonds_[position]; // accumulating contributions from bonds
  while (bonds_[bond].first == position) { // that connect "position" with
    if (state[bonds_[bond].second] == true) { // neurons of lower ind
      if (state[position] == false) { // if we change false to true, we add
        energyChange -= weights_[bond];
      }
      else { // if we change true to false, then we subtract energy
        energyChange += weights_[bond];
      }
    }
    bond++; // move to the next bond
  }
  return exp(-energyChange);
}

void BoltzmannMachine::updateState(Particle* particle) {
  ind position = particle->proposeMove();
  bVec& state = particle->getStateRef();
  double acceptence = moveAcceptence(position, state);
  if (acceptence > distribution_(generator_)) {
    state[position] = !state[position];
  }
}

void BoltzmannMachine::updateCorrelations(Particle* particle, dVec& correlations,
                                          double sign) {
  bVec& state = particle->getStateRef();
  for (int i = 0; i < bonds_.size(); ++i) {
    if (state[bonds_[i].first] == true &&
        state[bonds_[i].second] == true) { 
      correlations[i] += sign;
    }
  }
}


void BoltzmannMachine::getStatistics(Particle* particle, dVec& correlations, 
                             ind numSamples, double sign) {
  for (int i = 0; i < burnSteps_; ++i) {
    updateState(particle);
  }
  for (int i = 0; i < numSamples; ++i) {
    updateState(particle);
    updateCorrelations(particle, correlations, sign);
  }
}


Particle* BoltzmannMachine::makeFantasyParticle() {
  Particle* particle = new Particle(numNeurons_);
  for (int i = 0; i < equiSteps_; ++i) {
    updateState(particle);
  }
  return particle; 
}

Particle* BoltzmannMachine::makeParticle(bVec& data) {
  Particle* particle = new Particle(numNeurons_, data);
  for (int i = 0; i < equiSteps_; ++i) {
    updateState(particle);
  }
  return particle;
}

