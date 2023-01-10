#ifndef GNN_H
#define GNN_H
#include <vector>

#include "neuralnet.h"

namespace s21 {
class GNN : public NeuralNet {
 private:
  class Neuron {
   public:
    Neuron();
    Neuron(const int sizeNextLayout);
    Neuron(const std::vector<double>& weights);

    auto setRandomWeights(const int sizeNextLayout) -> void;
    auto calcValue(const double inputSignal) -> void;
    auto calcSignal(const size_t indexNeuron) -> double;
    auto calcError(const double idealAnswer) -> void;
    auto calcError(const std::vector<Neuron*>& nextLayout) -> void;
    auto changeWeights(const std::vector<Neuron*>& nextLayout,
                       const double learningSpeed) -> void;

    inline auto value() -> double { return value_; }
    inline auto weights() -> const std::vector<double>& { return weights_; }

   private:
    std::vector<double> weights_;
    double error_;
    double value_;
    double inputValue_;
    double bios_;

    auto funcAct(const double value) -> double;
    auto difFuncAct(const double value) -> double;
  };

 public:
  GNN();
  ~GNN() override;

  auto recreate(const std::vector<int>& layouts) -> void override;
  auto recreate(const std::vector<std::vector<std::vector<double>>>& weights)
      -> void override;
  auto weights() -> std::vector<std::vector<std::vector<double>>> override;
  auto training(const std::vector<double>& inputSignal,
                const std::vector<double>& ans) -> void override;
  auto getAnswer(const std::vector<double>& inputSignal) -> int override;
  auto getConfig() -> std::vector<int> override;
  auto getError(const std::vector<double>& ans) -> double override;

  auto resultCalc() -> int;

  auto studingGNN(const std::vector<double>& inputSignal, int idealAnswer)
      -> void;

 private:
  std::vector<std::vector<Neuron*>> net_;

  auto calcValueToNextLayout(const int layoutIndex) -> void;
  auto clearNet() -> void;
};
}  //  namespace s21

#endif
