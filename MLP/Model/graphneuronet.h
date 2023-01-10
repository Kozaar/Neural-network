#ifndef GRAPHNEURONET_H
#define GRAPHNEURONET_H
#include <vector>

class Neuron {
 public:
  Neuron() = default;
  Neuron(const std::vector<Neuron*>& nextLayout);
  Neuron(const std::vector<Neuron*>& nextLayout,
         const std::vector<double>& weights);

  inline auto error() -> double& { return error_; }
  inline auto value() -> double { return value_; }
  inline auto inputValue() -> double& { return inputValue_; }
  inline auto nextLayout() -> const std::vector<Neuron*>& {
    return nextLayout_;
  }
  inline auto weights() -> std::vector<double>& { return weights_; }
  auto calcValue() -> void;

  auto initialWeight() -> double;
  auto weightedSum(int index) -> double;

 private:
  std::vector<double> weights_;
  std::vector<Neuron*> nextLayout_;
  double error_;
  double value_;
  double inputValue_;
};

class NeuroLayout {
 public:
  NeuroLayout();
  NeuroLayout(int countNeurons, const std::vector<Neuron*>& nextLayout);
  NeuroLayout(int countNeurons, const std::vector<Neuron*>& nextLayout,
              const std::vector<double>& weights);
  NeuroLayout(const NeuroLayout& other);
  NeuroLayout(NeuroLayout&& other);
  ~NeuroLayout();

  NeuroLayout& operator=(const NeuroLayout&);
  NeuroLayout& operator=(NeuroLayout&&);

  inline auto getNeurons() -> const std::vector<Neuron*>& { return neurons_; }

  auto changeNextLayout() -> void;
  auto weightedSum(int index) -> double;

 private:
  std::vector<Neuron*> neurons_;
};

class GraphNeuroNet {
 public:
  GraphNeuroNet() = default;
  explicit GraphNeuroNet(std::vector<std::vector<double>> weights);
  explicit GraphNeuroNet(int countLayout);
  ~GraphNeuroNet();

  auto calcError(int indexIdealResult) -> double;
  auto calcNeurons(std::vector<double> signals) -> void;
  auto identifyPicture() -> int;
  auto getSignals(std::vector<double> signals) -> void;
  auto training(int indexIdealResult, std::vector<double> signals) -> void;

  static auto sigmoid(double x) -> double;
  static auto sigmoidDif(double x) -> double;
  static auto actualFunc(double x) -> double;
  static auto actualFuncDif(double x) -> double;

  constexpr static int hiddenLayoutSize = 193;
  constexpr static int outputSize = 26;
  constexpr static int inputSize = 784;
  constexpr static double learningRate = 0.4;

 private:
  std::vector<NeuroLayout*> net_;
};

#endif  // GRAPHNEURONET_H
