#include "matrixneuralnet.h"

#include <math.h>

namespace s21 {
MatrixNeuralNet::MatrixNeuralNet() {
  countLayers_ = 4;
  std::vector<int> layers({784, 140, 140, 26});
  recreate(layers);
  std::random_device randDevice;
  std::mt19937 tempGenerator(randDevice());
  m_generator = tempGenerator;
}

MatrixNeuralNet::MatrixNeuralNet(int countLayers, int* neuronsInLayers) {
  countLayers_ = countLayers;
  neuronsInLayers_.resize(countLayers);
  for (int i = 0; i < countLayers_; i++) {
    neuronsInLayers_[i] = neuronsInLayers[i];
  }
  CreateNeurons();
  CreateWeights();
  std::random_device randDevice;
  std::mt19937 tempGenerator(randDevice());
  m_generator = tempGenerator;
}

double MatrixNeuralNet::randomWeight() {
  double rand = ((int)m_generator() % 10000) * 0.0001;
  return rand;
}

int MatrixNeuralNet::getAnswer(const std::vector<double>& data) {
  for (int n = 0; n < neuronsInLayers_[0]; n++) {
    neuron_[0][n] = Func(data[n]);
  }
  for (int i = 1; i < countLayers_; i++) {
    for (int j = 0; j < neuronsInLayers_[i]; j++) {
      double input = 0;
      for (int k = 0; k < neuronsInLayers_[i - 1]; k++) {
        input += neuron_[i - 1][k] * weights_[i - 1][k][j];
      }
      neuron_[i][j] = Func(input + bios_[i - 1][j]);
    }
  }
  return FindMax();
}

void MatrixNeuralNet::training(const std::vector<double>& inputSignal,
                               const std::vector<double>& ans) {
  getAnswer(inputSignal);
  double learningSpeed = 0.08;
  double sum, deltaWeight;
  for (int i = 0; i < neuronsInLayers_[countLayers_ - 1]; i++) {
    errors_[countLayers_ - 2][i] = (ans[i] - neuron_[countLayers_ - 1][i]) *
                                   FuncPr(neuron_[countLayers_ - 1][i]);
  }
  for (int i = countLayers_ - 2; i > 0; i--) {
    for (int j = 0; j < neuronsInLayers_[i]; j++) {
      sum = 0;
      for (int l = 0; l < neuronsInLayers_[i + 1]; l++) {
        sum += errors_[i][l] * weights_[i][j][l];
      }
      errors_[i - 1][j] = sum * FuncPr(neuron_[i][j]);
    }
  }
  for (int l = 0; l < countLayers_ - 1; l++) {
    for (int i = 0; i < neuronsInLayers_[l]; i++) {
      for (int j = 0; j < neuronsInLayers_[l + 1]; j++) {
        deltaWeight = learningSpeed * errors_[l][j] * neuron_[l][i];
        weights_[l][i][j] += deltaWeight;
      }
    }
  }
  for (int i = countLayers_ - 1; i > 0; i--) {
    for (int j = 0; j < neuronsInLayers_[i]; j++) {
      bios_[i - 1][j] += learningSpeed * errors_[i - 1][j];
    }
  }
}

double MatrixNeuralNet::getError(const std::vector<double>& ans) {
  double sum = 0;
  for (int i = 0; i < neuronsInLayers_[countLayers_ - 1]; i++) {
    sum += pow(ans[i] - neuron_[countLayers_ - 1][i], 2);
  }
  return sum / neuronsInLayers_[countLayers_ - 1];
}

void MatrixNeuralNet::CreateNeurons() {
  neuron_.clear();
  neuron_.resize(countLayers_);
  for (int i = 0; i < countLayers_; i++) {
    neuron_[i].resize(neuronsInLayers_[i]);
  }
}

void MatrixNeuralNet::CreateWeights() {
  weights_.clear();
  weights_.resize(countLayers_ - 1);
  for (int i = 0; i < countLayers_ - 1; i++) {
    weights_[i].resize(neuronsInLayers_[i]);
    for (int j = 0; j < neuronsInLayers_[i]; j++) {
      weights_[i][j].resize(neuronsInLayers_[i + 1]);
      for (int k = 0; k < neuronsInLayers_[i + 1]; k++) {
        weights_[i][j][k] = randomWeight();
      }
    }
  }
  errors_.resize(countLayers_ - 1);
  for (int i = 0; i < countLayers_ - 1; i++) {
    errors_[i].resize(neuronsInLayers_[i + 1]);
  }
  bios_.clear();
  bios_.resize(countLayers_ - 1);
  for (size_t i = 0; i < bios_.size(); i++) {
    bios_[i].resize(neuronsInLayers_[i + 1]);
    for (size_t j = 0; j < bios_[i].size(); j++) {
      bios_[i][j] = randomWeight();
    }
  }
}

double MatrixNeuralNet::Func(double in) { return 1 / (1 + exp(-in)); }

double MatrixNeuralNet::FuncPr(double& in) { return in * (1 - in); }

int MatrixNeuralNet::FindMax() {
  int max = 0;
  double maxValue = neuron_[countLayers_ - 1][0];
  for (int i = 0; i < neuronsInLayers_[countLayers_ - 1]; i++) {
    if (neuron_[countLayers_ - 1][i] > maxValue) {
      maxValue = neuron_[countLayers_ - 1][i];
      max = i;
    }
  }
  return max;
}

void MatrixNeuralNet::recreate(const std::vector<int>& layouts) {
  countLayers_ = layouts.size();
  neuronsInLayers_.resize(countLayers_);
  for (int i = 0; i < countLayers_; i++) {
    neuronsInLayers_[i] = layouts[i];
  }
  CreateNeurons();
  CreateWeights();
}

void MatrixNeuralNet::recreate(
    const std::vector<std::vector<std::vector<double> > >& weights) {
  countLayers_ = weights.size() + 1;
  neuronsInLayers_.resize(countLayers_);
  for (int i = 0; i < countLayers_ - 1; i++) {
    neuronsInLayers_[i] = weights[i].size();
  }
  neuronsInLayers_[countLayers_ - 1] = weights.back().back().size();
  CreateNeurons();
  weights_ = weights;
}

std::vector<std::vector<std::vector<double> > > MatrixNeuralNet::weights() {
  return weights_;
}

std::vector<int> MatrixNeuralNet::getConfig() { return neuronsInLayers_; }
}  //  namespace s21
