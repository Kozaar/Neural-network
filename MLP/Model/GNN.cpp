#include "GNN.h"

#include <cmath>

// --------------------------------------------
// Neuron
// --------------------------------------------

namespace s21 {
GNN::Neuron::Neuron() {
  error_ = 0;
  value_ = 0;
  inputValue_ = 0;
  bios_ = 0;
  weights_.clear();
}

GNN::Neuron::Neuron(const int sizeNextLayout) : Neuron() {
  setRandomWeights(sizeNextLayout);
}

GNN::Neuron::Neuron(const std::vector<double>& weights) : Neuron() {
  weights_ = weights;
}

void GNN::Neuron::setRandomWeights(const int sizeNextLayout) {
  weights_.resize(sizeNextLayout);
  for (int i = 0; i < sizeNextLayout; i++) {
    weights_[i] = 0.0001 * (std::rand() % 20001 - 10000);
  }
}

void GNN::Neuron::calcValue(const double inputSignal) {
  inputValue_ = inputSignal + bios_;
  value_ = funcAct(inputValue_);
}

double GNN::Neuron::calcSignal(const size_t indexNeuron) {
  return weights_[indexNeuron] * value_;
}

void GNN::Neuron::calcError(const double idealAnswer) {
  error_ = (idealAnswer - value_) * difFuncAct(value_);
}

void GNN::Neuron::calcError(const std::vector<Neuron*>& nextLayout) {
  double sum = 0;
  for (size_t i = 0; i < nextLayout.size(); i++) {
    sum += nextLayout[i]->error_ * weights_[i];
  }
  error_ = sum * difFuncAct(value_);
}

void GNN::Neuron::changeWeights(const std::vector<Neuron*>& nextLayout,
                                const double learningSpeed) {
  for (size_t i = 0; i < weights_.size(); i++) {
    weights_[i] += learningSpeed * nextLayout[i]->error_ * value_;
  }
}

double GNN::Neuron::funcAct(const double value) {
  return 1. / (1. + exp(-value));
}

double GNN::Neuron::difFuncAct(const double value) {
  //    return funcAct(value) * (1 - funcAct(value));
  return value * (1 - value);
}

// --------------------------------------------
// GNN
// --------------------------------------------

GNN::GNN() {
  net_.clear();
  std::vector<int> neuronsInLayers({784, 140, 140, 26});
  recreate(neuronsInLayers);
}

GNN::~GNN() { clearNet(); }

void GNN::recreate(const std::vector<int>& layouts) {
  clearNet();
  net_.resize(layouts.size());
  for (size_t i = 0; i < net_.size(); i++) {
    net_[i].resize(layouts[i]);
  }
  for (size_t i = 0; i < net_.back().size(); i++) {
    net_.back()[i] = new Neuron(0);
  }
  for (size_t i = 0; i < net_.size() - 1; i++) {
    for (size_t j = 0; j < net_[i].size(); j++) {
      net_[i][j] = new Neuron(net_[i + 1].size());
    }
  }
}

void GNN::recreate(
    const std::vector<std::vector<std::vector<double>>>& weights) {
  clearNet();
  net_.resize(weights.size() + 1);
  for (size_t i = 0; i < net_.size() - 1; i++) {
    net_[i].resize(weights[i].size());
  }
  net_.back().resize(weights.back()[0].size());
  for (size_t i = 0; i < net_.back().size(); i++) {
    net_.back()[i] = new Neuron(0);
  }
  for (size_t i = 0; i < net_.size() - 1; i++) {
    for (size_t j = 0; j < net_[i].size(); j++) {
      net_[i][j] = new Neuron(weights[i][j]);
    }
  }
}

void GNN::clearNet() {
  for (size_t i = 0; i < net_.size(); i++) {
    for (size_t j = 0; j < net_[i].size(); j++) {
      delete net_[i][j];
    }
  }
}

int GNN::resultCalc() {
  int result = 0;
  for (size_t i = 1; i < net_.back().size(); i++) {
    if (net_.back()[i]->value() > net_.back()[result]->value()) {
      result = i;
    }
  }
  return result;
}

std::vector<std::vector<std::vector<double>>> GNN::weights() {
  std::vector<std::vector<std::vector<double>>> weights;
  weights.resize(net_.size() - 1);
  for (size_t i = 0; i < weights.size(); i++) {
    weights[i].resize(net_[i].size());
    for (size_t j = 0; j < net_[i].size(); j++) {
      weights[i][j] = net_[i][j]->weights();
    }
  }
  return weights;
}

void GNN::calcValueToNextLayout(const int layoutIndex) {
  for (size_t i = 0; i < net_[layoutIndex + 1].size(); i++) {
    double signal = 0;
    for (size_t j = 0; j < net_[layoutIndex].size(); j++) {
      signal += net_[layoutIndex].at(j)->calcSignal(i);
    }
    net_[layoutIndex + 1].at(i)->calcValue(signal);
  }
}

int GNN::getAnswer(const std::vector<double>& inputSignal) {
  for (size_t i = 0; i < net_[0].size(); i++) {
    net_[0][i]->calcValue(inputSignal[i]);
  }
  for (size_t i = 0; i < net_.size() - 1; i++) {
    calcValueToNextLayout(i);
  }
  return resultCalc();
}

std::vector<int> GNN::getConfig() {
  std::vector<int> config;
  for (size_t i = 0; i < net_.size(); i++) {
    config.push_back(net_[i].size());
  }
  return config;
}

double GNN::getError(const std::vector<double>& ans) {
  double sum = 0;
  for (size_t i = 0; i < net_.back().size(); i++) {
    sum += pow(ans[i] - net_.back()[i]->value(), 2);
  }
  return sum / net_.back().size();
}

void GNN::studingGNN(const std::vector<double>& inputSignal, int idealAnswer) {
  getAnswer(inputSignal);
  std::vector<double> idealResult(net_.back().size());
  idealResult[idealAnswer - 1] = 1.;
  for (size_t i = 0; i < net_.back().size(); i++) {
    net_.back()[i]->calcError(idealResult[i]);
  }
  for (size_t i = net_.size() - 2; i > 0; i--) {
    for (size_t j = 0; j < net_[i].size(); j++) {
      net_[i][j]->calcError(net_[i + 1]);
      net_[i][j]->changeWeights(net_[i + 1], 0.08);
    }
  }
  for (size_t j = 0; j < net_[0].size(); j++) {
    net_[0][j]->changeWeights(net_[1], 0.08);
  }
}

void GNN::training(const std::vector<double>& inputSignal,
                   const std::vector<double>& ans) {
  getAnswer(inputSignal);
  for (size_t i = 0; i < net_.back().size(); i++) {
    net_.back()[i]->calcError(ans[i]);
  }
  for (int i = net_.size() - 2; i > 0; i--) {
    for (size_t j = 0; j < net_[i].size(); j++) {
      net_[i][j]->calcError(net_[i + 1]);
      net_[i][j]->changeWeights(net_[i + 1], 0.08);
    }
  }
  for (size_t j = 0; j < net_[0].size(); j++) {
    net_[0][j]->changeWeights(net_[1], 0.08);
  }
}
}  //  namespace s21
