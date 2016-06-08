#include "../headers/Particle.h"

using namespace bMachine;

Particle::Particle() {}

Particle::Particle(ind numNeurons) {
  offset_ = 0;
  state_.resize(numNeurons);
  randInit();
  // std::cout << "A new particle was created \n";
}

void Particle::initialize(ind numNeurons) {
  offset_ = 0;
  state_.resize(numNeurons);
  randInit();
}

Particle::Particle(ind numNeurons, bVec& data) {
  state_.resize(numNeurons);
  offset_ = data.size();
  randInit();
  for (ind i = 0; i < offset_; ++i) {
    state_[i] = data[i];
  }
}

ind Particle::proposeMove() {
  ind position = rand() % (state_.size() - offset_);
  return position + offset_;
}

void Particle::randInit() {
  for (int i = 0; i < state_.size(); ++i) {
    state_[i] = rand() % 2;
  }
}

bVec Particle::getState() { 
  return state_;
}

bVec& Particle::getStateRef() {
  return state_;
}