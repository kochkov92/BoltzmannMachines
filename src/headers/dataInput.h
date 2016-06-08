#ifndef __BMACHINE_DATA_INPUT_H
#define __BMACHINE_DATA_INPUT_H

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

template<class Type>
class dataInput {
private:
  std::vector<std::vector<Type> > data_;

  int openFile(std::string file, std::ifstream& fin) {
    fin.open(file.c_str());
    if(fin.is_open() == false) {
      std::cerr << "File" + file + " can't be opened."  << std::endl;
      std::abort();
    }
    return 1;
  }

public:

  void readData(std::string fileName) {
    std::ifstream fin; // fstream of input data
    openFile(fileName, fin);
    std::string line; // data lines will go through it
    while(std::getline(fin, line)) {
    // std::getline(fin, line ); // first line sets init values for bounds
      std::stringstream ssline(line);
      data_.push_back(std::vector<Type>()); // new linew instance 
      while(ssline.eof() == false) {
        Type val;
        ssline >> val;
        data_.back().push_back(val);
      }
    }
  }

  void printData() {
    for (auto it = data_.begin(); it < data_.end(); ++it) {
      for (auto it2 = it->begin(); it2 < it->end(); ++it2) {
        std::cout << *it2 << " ";
      }
      std::cout << "\n";
    }
  }

  std::vector<Type>& getInstance(int instanceInd) {
    return data_[instanceInd];
  }

  unsigned int numInstances() {
    return data_.size();
  }
};

#endif