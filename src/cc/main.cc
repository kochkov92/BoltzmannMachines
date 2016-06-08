#include <iostream>
#include "../headers/BoltzmannMachine.h"
#include "../headers/dataInput.h"
#include "../headers/BMtrainer.h"

using namespace std;
int main() {
  int total = 40;
  int visible = 30;
  BoltzmannMachine myMachine(total, visible);
  for (int i = 0; i < visible; ++i) {
    for (int j = i + 1; j < total; ++j) {
      myMachine.addEdge(i, j);
    }
  }
  // for (int i = visible; i < total - 1; ++i) {
  //   for (int j = i + 1; j < total; ++j) {
  //     myMachine.addEdge(i,j);
  //   }
  // }
  myMachine.initialize();
  dataInput<bool> dataSet;
  dataSet.readData("data/distribution.txt");
  BMtrainer myTrainer;
  myTrainer.train(myMachine, dataSet, 500, 1);
  myMachine.generateExamples(400, 4000);
  myMachine.printInfo();
}