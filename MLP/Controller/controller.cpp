#include "controller.h"

namespace s21 {
Controller::Controller() { model = new Model; }

Controller::~Controller() { delete model; }

void Controller::setRealisation(int type) { model->setRealisation(type); }

void Controller::createNewNet(const std::vector<int>& layouts) {
  model->createNewNet(layouts);
}

void Controller::openWeights(std::string filename) {
  model->openWeights(filename);
}

void Controller::saveWeights(std::string filename) {
  model->saveWeights(filename);
}

void Controller::training(std::string filename, int countEpoch) {
  model->training(filename, countEpoch);
}

int Controller::classificate(const std::vector<double>& inputSignal) {
  return model->classificate(inputSignal);
}

void Controller::startExperiment(std::string filename, double testPercent,
                                 double* averageAccuracy, double* precision,
                                 double* recall, double* f_measure,
                                 double* time) {
  model->experiment(filename, testPercent, averageAccuracy, precision, recall,
                    f_measure, time);
}

std::vector<int> Controller::getNetConfig() { return model->getNetConfig(); }

std::vector<double> Controller::getReport() { return model->getReport(); }

void Controller::crossValidation(std::string filename, int countGroup) {
  model->crossValidation(filename, countGroup);
}
}  //  namespace s21
