#include "../headers/BMtrainer.h"

BMtrainer::BMtrainer() {
  id_ = 1;
  samples_ = 1000;
  learningRate_ = 0.15;
}

BMtrainer::~BMtrainer() {
  for (int i = 0; i < dataParticles.size(); ++i) {
    delete dataParticles[i];
  }
  for (int i = 0; i < fantParticles.size(); ++i) {
    delete fantParticles[i];
  }
}

void BMtrainer::train(BoltzmannMachine& machine, dataInput<bool>& data, 
                      int iter, int numWorkers) {

  for (int i = 0; i < data.numInstances(); ++i) {
    dataParticles.push_back(machine.makeParticle(data.getInstance(i)));
    fantParticles.push_back(machine.makeFantasyParticle());
  }

  // std::vector<std::thread> workers;
  for (int id = 0; id < numWorkers; ++id) {
    runWorker(machine, iter, numWorkers, id);
    // workers.push_back(std::thread( [=] { runWorker(machine, data, iter, numWorkers, id); }));
  }

  // for (int id = 0; id < numWorkers; ++id) {
  //   workers[id].join();
  // }
}

void BMtrainer::constMult(dVec& vec, double val) {
  for (auto it = vec.begin(); it < vec.end(); ++it) {
    *it = val * *it;
  }
}

void BMtrainer::normalize(dVec& vec, int numSamples) {
  constMult(vec, learningRate_ / numSamples);
}

void BMtrainer::runWorker(BoltzmannMachine& machine,
                          int iter, int numWorkers, int workerId) {
  int samplesDone = 0;
  dVec weightsUpdate(machine.numBonds(), 0.);
  for (int it = 0; it < iter; ++it) {
    int instanceInd = workerId;
    while (instanceInd < dataParticles.size()) {
      machine.getStatistics(fantParticles[instanceInd], weightsUpdate, samples_, -1.);
      // constMult(weightsUpdate, -1.); // unlearning stage
      samplesDone += samples_; // keeping track of the number of MC runs
      machine.getStatistics(dataParticles[instanceInd], weightsUpdate, samples_, 1.);
      samplesDone += samples_;
      instanceInd += numWorkers; // in case several threads are at work
      // if (instanceInd % 2 == 0) {
      // normalize(weightsUpdate, samplesDone); // simply division
      // machine.updateWeights(weightsUpdate);
      // constMult(weightsUpdate, 0.); // cleaning up old correlations
      // samplesDone = 0;
      // }
    }
    normalize(weightsUpdate, samplesDone);
    machine.updateWeights(weightsUpdate);
    constMult(weightsUpdate, 0.); // cleaning up old correlations
    samplesDone = 0;
    std::cout << "done with iteration " << it << "\n";
  }
}