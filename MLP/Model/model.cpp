#include "model.h"

#include <time.h>

namespace s21 {
Model::Model() {
  current_ = 1;
  net_[0] = new GNN;
  net_[1] = new MatrixNeuralNet;
}

Model::~Model() {
  for (auto i = net_.begin(); i != net_.end(); i++) {
    delete i->second;
  }
}

void Model::setRealisation(int type) { current_ = type; }

void Model::createNewNet(const std::vector<int>& layouts) {
  net_[current_]->recreate(layouts);
}

std::vector<int> Model::openWeights(std::string filename) {
  std::vector<std::vector<std::vector<double>>> weights =
      FileExecutor::getWeights(filename, countInputNeurons);
  std::vector<int> layouts;
  net_[current_]->recreate(weights);
  return layouts;
}

void Model::saveWeights(std::string filename) {
  FileExecutor::saveWeights(filename, net_[current_]->weights());
}

void Model::training(std::string filename, int countEpoch) {
  report_.clear();
  std::vector<double> currentErrors;
  std::ifstream fin(filename);
  if (fin.is_open()) {
    std::vector<double> signal(countInputNeurons);
    std::vector<double> answer(countOutputNeurons, 0.1);
    int realAnswer = 1;
    while (!(fin.eof())) {
      realAnswer = FileExecutor::getSignal(&signal, &fin);
      if (realAnswer == -1) {
        break;
      }
      answer[realAnswer - 1] = 0.9;
      net_[current_]->getAnswer(signal);
      currentErrors.push_back(net_[current_]->getError(answer));
      answer[realAnswer - 1] = 0.1;
    }
  }
  double error = 0;
  for (size_t i = 0; i < currentErrors.size(); i++) {
    error += currentErrors[i];
  }
  report_.push_back(error / currentErrors.size());
  fin.close();
  for (int i = 0; i < countEpoch; i++) {
    currentErrors.clear();
    fin.open(filename);
    if (fin.is_open()) {
      std::vector<double> signal(countInputNeurons);
      std::vector<double> answer(countOutputNeurons, 0.1);
      int realAnswer = 1;
      while (!(fin.eof())) {
        realAnswer = FileExecutor::getSignal(&signal, &fin);
        if (realAnswer == -1) {
          break;
        }
        answer[realAnswer - 1] = 0.9;
        net_[current_]->training(signal, answer);
        currentErrors.push_back(net_[current_]->getError(answer));
        answer[realAnswer - 1] = 0.1;
      }
    }
    error = 0;
    for (size_t i = 0; i < currentErrors.size(); i++) {
      error += currentErrors[i];
    }
    report_.push_back(error / currentErrors.size());
    fin.close();
  }
}

int Model::classificate(const std::vector<double>& inputSignal) {
  return net_[current_]->getAnswer(inputSignal) + 1;
}

void Model::experiment(std::string filename, double testPercent,
                       double* averageAccuracy, double* precision,
                       double* recall, double* f_measure, double* time) {
  int countStrings = FileExecutor::countLineInFile(filename) * testPercent;
  std::ifstream fin(filename);
  if (fin.is_open()) {
    int realAnswer;
    int netAnswer;
    std::vector<int> truePositive(countOutputNeurons, 0);
    std::vector<int> falseNegative(countOutputNeurons, 0);
    std::vector<int> falsePositive(countOutputNeurons, 0);
    int countGoodAnswers = 0;

    clock_t start = clock();

    std::vector<double> signal(countInputNeurons);
    for (int i = 0; i < countStrings; i++) {
      realAnswer = FileExecutor::getSignal(&signal, &fin);
      netAnswer = net_[current_]->getAnswer(signal) + 1;
      if (realAnswer == netAnswer) {
        countGoodAnswers++;
        truePositive[realAnswer - 1]++;
      } else {
        falseNegative[realAnswer - 1]++;
        falsePositive[netAnswer - 1]++;
      }
    }
    clock_t end = clock();
    *averageAccuracy = (double)countGoodAnswers / countStrings;
    *time = (double)(end - start) / CLOCKS_PER_SEC;
    *precision = 0;
    *recall = 0;
    for (int i = 0; i < countOutputNeurons; i++) {
      if (truePositive[i] || (falsePositive[i] && falseNegative[i])) {
        *precision +=
            (double)truePositive[i] / (truePositive[i] + falsePositive[i]);
        *recall +=
            (double)truePositive[i] / (truePositive[i] + falseNegative[i]);
      }
    }
    *precision /= countOutputNeurons;
    *recall /= countOutputNeurons;
    *f_measure = 2 * (*precision) * (*recall) / (*precision + *recall);
  }
}

std::vector<int> Model::getNetConfig() { return net_[current_]->getConfig(); }

std::vector<double> Model::getReport() { return report_; }

void Model::crossValidation(std::string filename, int countGroup) {
  unsigned int countLine = FileExecutor::countLineInFile(filename);
  report_.clear();
  unsigned int numLineInGroup = countLine / countGroup;
  std::ifstream fin(filename);
  if (fin.is_open()) {
    std::vector<double> signal(countInputNeurons);
    std::vector<double> answer(countOutputNeurons, 0.1);
    std::vector<double> currentErrors;
    int realAnswer = 1;
    double error = 0;
    for (int i = 0; i < countGroup; i++) {
      currentErrors.clear();
      for (unsigned int j = 0; j < (countGroup - 1) * numLineInGroup; j++) {
        if (fin.eof()) {
          fin.close();
          fin.open(filename);
          j -= countLine % countGroup;
        }
        realAnswer = FileExecutor::getSignal(&signal, &fin);
        if (realAnswer == -1) {
          break;
        }
        answer[realAnswer - 1] = 0.9;
        net_[current_]->training(signal, answer);
        answer[realAnswer - 1] = 0.1;
      }
      for (unsigned int j = 0; j < numLineInGroup; j++) {
        if (fin.eof()) {
          fin.close();
          fin.open(filename);
        }
        realAnswer = FileExecutor::getSignal(&signal, &fin);
        net_[current_]->getAnswer(signal);
        answer[realAnswer - 1] = 0.9;
        currentErrors.push_back(net_[current_]->getError(answer));
        answer[realAnswer - 1] = 0.1;
      }
      error = 0;
      for (size_t i = 0; i < currentErrors.size(); i++) {
        error += currentErrors[i];
      }
      report_.push_back(error / currentErrors.size());
      for (unsigned int j = 0; j < numLineInGroup; j++) {
        if (fin.eof()) {
          fin.close();
          fin.open(filename);
        }
        FileExecutor::getSignal(&signal, &fin);
      }
    }
  }
}
}  //  namespace s21
