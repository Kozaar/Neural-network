#ifndef MODEL_H
#define MODEL_H

#include <map>
#include <vector>

#include "GNN.h"
#include "fileexecutor.h"
#include "matrixneuralnet.h"

namespace s21 {
class Model {
 public:
  Model();
  ~Model();

  auto setRealisation(int type) -> void;
  auto createNewNet(const std::vector<int>& layouts) -> void;
  auto openWeights(std::string filename) -> std::vector<int>;
  auto saveWeights(std::string filename) -> void;
  auto training(std::string filename, int countEpoch) -> void;
  auto classificate(const std::vector<double>& inputSignal) -> int;
  auto experiment(std::string filename, double testPercent,
                  double* averageAccuracy, double* precision, double* recall,
                  double* f_measure, double* time) -> void;
  auto getNetConfig() -> std::vector<int>;
  auto getReport() -> std::vector<double>;
  auto crossValidation(std::string filename, int countGroup) -> void;

 private:
  std::map<int, NeuralNet*> net_;
  int current_;
  std::vector<double> report_;

  static constexpr int countInputNeurons = 784;
  static constexpr int countOutputNeurons = 26;
};
}  //  namespace s21

#endif  // MODEL_H
