#include "fileexecutor.h"

namespace s21 {
std::vector<std::vector<std::vector<double>>> FileExecutor::getWeights(
    std::string fileName, int firstLayoutSize) {
  int allgetNumLayout = getNumLayout(firstLayoutSize, fileName);
  std::vector<std::vector<std::vector<double>>> result;
  result.resize(allgetNumLayout);
  std::ifstream fin(fileName);
  std::string line;
  int sizeLayout = firstLayoutSize;
  if (fin.is_open()) {
    for (int i = 0; i < allgetNumLayout; i++) {
      if (i != 0) {
        sizeLayout = result[i - 1][0].size();
      }
      result[i].resize(sizeLayout);
      for (int j = 0; j < sizeLayout && getline(fin, line); j++) {
        if (line.size() == 0) {
          j--;
          continue;
        }
        std::vector<double> tempVec = getVector(line);
        result[i][j] = tempVec;
      }
    }
  }
  return result;
}

void FileExecutor::saveWeights(
    std::string filename,
    const std::vector<std::vector<std::vector<double>>>& weights) {
  std::ofstream fout(filename);
  if (fout.is_open()) {
    for (size_t i = 0; i < weights.size(); i++) {
      for (size_t j = 0; j < weights[i].size(); j++) {
        for (size_t l = 0; l < weights[i][j].size(); l++) {
          fout << weights[i][j][l];
          fout << " ";
        }
        fout << "\n";
      }
    }
  }
  fout.close();
}

int FileExecutor::getSignal(std::vector<double>* signal, std::ifstream* fin) {
  std::string line;
  int res = -1;
  try {
    std::getline(*fin, line);
    size_t buf;
    res = std::stoi(line, &buf);
    line.erase(0, buf + 1);
    for (int i = 0; i < 784; i++) {
      signal->at(i) = std::stoi(line, &buf);
      line.erase(0, buf + 1);
    }
  } catch (std::exception& e) {
    res = -1;
  }
  return res;
}

int FileExecutor::countLineInFile(std::string filename) {
  std::ifstream fin(filename);
  int countStrings = 0;
  if (fin.is_open()) {
    std::string line;
    while (!(fin.eof())) {
      std::getline(fin, line);
      countStrings++;
    }
  }
  fin.close();
  return countStrings;
}

std::vector<double> FileExecutor::getVector(std::string line) {
  std::vector<double> result;
  std::string temp_str = "";
  const std::string str = "-e0123456789.";
  for (size_t i = 0; i < line.size(); i++) {
    if (str.find(line[i]) != std::string::npos) {
      temp_str.push_back(line[i]);
    } else {
      if (line[i] == ' ') {
        result.push_back(std::stod(temp_str));
        temp_str.clear();
      }
    }
  }
  if (line[line.size() - 1] != ' ') {
    result.push_back(std::stod(temp_str));
  }
  return result;
}

int FileExecutor::getNumLayout(int sizeFirstLayout, std::string fileName) {
  std::ifstream fin(fileName);
  std::string line;
  int result = 0;
  while (getline(fin, line)) {
    if (line.size() == 0) continue;
    if (sizeFirstLayout == 1) {
      result++;
      break;
    }
    sizeFirstLayout--;
  }

  std::vector<double> tempVec = getVector(line);
  int sizeNextLayout = tempVec.size();
  while (getline(fin, line)) {
    if (line.size() == 0) {
      continue;
    }
    tempVec = getVector(line);
    if (sizeNextLayout == 2) {
      sizeNextLayout = tempVec.size();
      result++;
    } else {
      sizeNextLayout--;
    }
  }
  fin.close();
  return result;
}
}  //  namespace s21
