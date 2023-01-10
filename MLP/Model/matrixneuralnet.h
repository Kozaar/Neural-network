#ifndef MATRIXNEURALNET_H
#define MATRIXNEURALNET_H

#include <random>
#include <vector>

#include "neuralnet.h"

namespace s21 {
class MatrixNeuralNet : public NeuralNet {
 public:
  MatrixNeuralNet();
  MatrixNeuralNet(int countLayers, int* neuronsInLayers);
  ~MatrixNeuralNet() = default;

  auto getAnswer(const std::vector<double>& data) -> int override;
  auto training(const std::vector<double>& inputSignal,
                const std::vector<double>& ans) -> void override;
  auto recreate(const std::vector<int>& layouts) -> void override;
  auto recreate(const std::vector<std::vector<std::vector<double>>>& weights)
      -> void override;
  auto weights() -> std::vector<std::vector<std::vector<double>>> override;
  auto getConfig() -> std::vector<int> override;
  auto getError(const std::vector<double>& ans) -> double override;
  auto FindMax() -> int;

 private:
  std::vector<std::vector<std::vector<double>>> weights_;
  std::vector<std::vector<double>> neuron_;
  std::vector<std::vector<double>> bios_;
  std::vector<std::vector<double>> errors_;
  int countLayers_;
  std::vector<int> neuronsInLayers_;

  auto CreateNeurons() -> void;
  auto CreateWeights() -> void;
  auto Func(double in) -> double;
  auto FuncPr(double& in) -> double;

  std::mt19937 m_generator;
  auto randomWeight() -> double;
};
}  //  namespace s21

#endif  // MATRIXNEURALNET_H
