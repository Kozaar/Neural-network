#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "../Model/model.h"

namespace s21 {
class Controller {
 public:
  Controller();
  ~Controller();

  auto setRealisation(int type) -> void;
  auto createNewNet(const std::vector<int>& layouts) -> void;
  auto openWeights(std::string filename) -> void;
  auto saveWeights(std::string filename) -> void;
  auto training(std::string filename, int countEpoch) -> void;
  auto classificate(const std::vector<double>& inputSignal) -> int;
  auto startExperiment(std::string filename, double testPercent,
                       double* averageAccuracy, double* precision,
                       double* recall, double* f_measure, double* time) -> void;
  auto getNetConfig() -> std::vector<int>;
  auto getReport() -> std::vector<double>;
  auto crossValidation(std::string filename, int countGroup) -> void;

 private:
  Model* model;
};
}  //  namespace s21

#endif  // CONTROLLER_H
