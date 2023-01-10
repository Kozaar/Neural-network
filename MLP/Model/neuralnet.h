#ifndef NEURALNET_H
#define NEURALNET_H
#include <string>
#include <vector>

namespace s21 {
class NeuralNet {
 public:
  NeuralNet(){};
  virtual ~NeuralNet(){};

  virtual auto recreate(const std::vector<int>& layouts) -> void = 0;
  virtual auto recreate(
      const std::vector<std::vector<std::vector<double>>>& weights) -> void = 0;
  virtual auto training(const std::vector<double>& inputSignal,
                        const std::vector<double>& ans) -> void = 0;
  virtual auto weights() -> std::vector<std::vector<std::vector<double>>> = 0;
  virtual auto getAnswer(const std::vector<double>& inputSignal) -> int = 0;
  virtual auto getConfig() -> std::vector<int> = 0;
  virtual auto getError(const std::vector<double>& ans) -> double = 0;
};
}  //  namespace s21

#endif  // NEURALNET_H
