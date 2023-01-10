#include "graphneuronet.h"

#include <cmath>
#include <iostream>

using namespace std;
// ------------------------------------------------------------
// Neuron
// ------------------------------------------------------------

Neuron::Neuron(const std::vector<Neuron*>& nextLayout) {
  nextLayout_ = nextLayout;
  weights_.resize(nextLayout_.size());
  for (size_t i = 0; i < weights_.size(); i++) {
    weights_[i] = initialWeight();
  }
}

Neuron::Neuron(const std::vector<Neuron*>& nextLayout,
               const std::vector<double>& weights) {
  nextLayout_ = nextLayout;
  weights_ = weights;
}

double Neuron::initialWeight() { return arc4random_uniform(100) / 100.0; }

double Neuron::weightedSum(int index) { return value_ * weights_[index]; }

void Neuron::calcValue() { value_ = GraphNeuroNet::actualFunc(inputValue_); }

// ------------------------------------------------------------
// NeuroLayout
// ------------------------------------------------------------

NeuroLayout::NeuroLayout() {
  neurons_.resize(GraphNeuroNet::outputSize);
  for (size_t i = 0; i < neurons_.size(); i++) {
    neurons_[i] = new Neuron;
  }
}

NeuroLayout::NeuroLayout(int countNeurons,
                         const std::vector<Neuron*>& nextLayout) {
  neurons_.resize(countNeurons);
  for (size_t i = 0; i < neurons_.size(); i++) {
    neurons_[i] = new Neuron(nextLayout);
  }
}

NeuroLayout::NeuroLayout(int countNeurons,
                         const std::vector<Neuron*>& nextLayout,
                         const std::vector<double>& weights) {
  neurons_.resize(countNeurons);
  for (size_t i = 0; i < neurons_.size(); i++) {
    neurons_[i] = new Neuron(nextLayout, weights);
  }
}

NeuroLayout::NeuroLayout(const NeuroLayout& other) { *this = other; }

NeuroLayout::NeuroLayout(NeuroLayout&& other) { neurons_.swap(other.neurons_); }

NeuroLayout& NeuroLayout::operator=(const NeuroLayout& other) {
  if (neurons_.size()) {
    for (size_t i = 0; i < neurons_.size(); i++) {
      delete neurons_[i];
    }
  }
  neurons_ = other.neurons_;
  return *this;
}

NeuroLayout& NeuroLayout::operator=(NeuroLayout&& other) {
  neurons_.swap(other.neurons_);
  return *this;
}

NeuroLayout::~NeuroLayout() {
  for (size_t i = 0; i < neurons_.size(); i++) {
    delete neurons_[i];
  }
}

double NeuroLayout::weightedSum(int index) {
  double res = 0;
  for (size_t i = 0; i < neurons_.size(); i++) {
    res += neurons_[i]->weightedSum(index);
  }
  return res;
}

void NeuroLayout::changeNextLayout() {
  std::vector<Neuron*> nextLayout = neurons_[0]->nextLayout();
  for (size_t i = 0; i < nextLayout.size(); i++) {
    nextLayout[i]->inputValue() = weightedSum(i);
    nextLayout[i]->calcValue();
  }
}

// ------------------------------------------------------------
// GraphNeuroNet
// ------------------------------------------------------------

GraphNeuroNet::GraphNeuroNet(int countLayout) {
  net_.resize(countLayout);
  if (countLayout < 3) throw std::invalid_argument("must be > 2 layouts");
  net_.back() = new NeuroLayout;
  for (int i = countLayout - 2; i > 0; i--) {
    net_[i] = new NeuroLayout(hiddenLayoutSize, net_[i + 1]->getNeurons());
  }
  net_[0] = new NeuroLayout(inputSize, net_[1]->getNeurons());
}

GraphNeuroNet::GraphNeuroNet(std::vector<std::vector<double>> weights) {
  net_.resize(weights.size());
  for (size_t i = net_.size() - 2; i > 0; i--) {
    net_[i] = new NeuroLayout(hiddenLayoutSize, net_[i + 1]->getNeurons(),
                              weights[1]);
  }
  net_[0] = new NeuroLayout(inputSize, net_[1]->getNeurons(), weights[0]);
}

GraphNeuroNet::~GraphNeuroNet() {
  for (size_t i = 0; i < net_.size(); i++) {
    delete net_[i];
  }
}

double GraphNeuroNet::calcError(int indexIdealResult) {
  double error = 0;
  double rightValue = 0;
  std::vector<Neuron*> lastLayout = net_.back()->getNeurons();
  for (int i = 0; i < outputSize; i++) {
    rightValue = (i == indexIdealResult - 1) ? 1 : 0;
    error += pow(lastLayout.at(i)->value() - rightValue, 2);
  }
  return error;
}

void GraphNeuroNet::calcNeurons(std::vector<double> signals) {
  getSignals(signals);
  for (size_t i = 0; i < net_.size() - 1; i++) {
    net_[i]->changeNextLayout();
  }
}

int GraphNeuroNet::identifyPicture() {
  std::vector<Neuron*> lastLayout = net_.back()->getNeurons();
  int answer = 0;
  for (size_t i = 1; i < lastLayout.size(); i++) {
    if (lastLayout[answer] < lastLayout[i]) answer = i;
  }
  return answer;
}

void GraphNeuroNet::getSignals(std::vector<double> signals) {
  std::vector<Neuron*> firstLayout = net_[0]->getNeurons();
  for (size_t i = 0; i < firstLayout.size(); i++) {
    firstLayout[i]->inputValue() = signals[i];
    firstLayout[i]->calcValue();
  }
}

void GraphNeuroNet::training(int indexIdealResult,
                             std::vector<double> signals) {
  calcNeurons(signals);
  int idealValue;
  std::vector<Neuron*> nextLayout = net_.back()->getNeurons();
  for (int i = 0; i < outputSize; i++) {
    idealValue = indexIdealResult == i ? 1 : 0;
    nextLayout[i]->error() = (idealValue - nextLayout[i]->value()) *
                             (1 - nextLayout[i]->value()) *
                             nextLayout[i]->value();
  }
  double grad = 0, sum;
  std::vector<Neuron*> currentLayout;
  for (int countLayout = net_.size() - 2; countLayout >= 0; countLayout--) {
    currentLayout = net_[countLayout]->getNeurons();
    for (size_t i = 0; i < currentLayout.size(); i++) {
      sum = 0;
      for (size_t j = 0; j < nextLayout.size(); j++) {
        grad = nextLayout[j]->error() * currentLayout[i]->value();
        sum += currentLayout[i]->weights()[j] * nextLayout[j]->error();
        currentLayout[i]->weights()[j] = grad * learningRate;
      }
      currentLayout[i]->error() =
          (1 - currentLayout[i]->value()) * currentLayout[i]->value() * sum;
    }
    nextLayout.swap(currentLayout);
  }
}

double GraphNeuroNet::sigmoid(double x) { return 1. / (1. + pow(M_E, -x)); }

double GraphNeuroNet::sigmoidDif(double x) {
  double sigma = sigmoid(x);
  return sigma * (1. - sigma);
}

double GraphNeuroNet::actualFunc(double x) { return sigmoid(x); }

double GraphNeuroNet::actualFuncDif(double x) { return sigmoidDif(x); }
