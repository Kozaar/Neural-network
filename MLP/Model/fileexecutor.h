#ifndef FILEEXECUTOR_H
#define FILEEXECUTOR_H

#include <fstream>
#include <string>
#include <vector>

namespace s21 {
class FileExecutor {
 public:
  FileExecutor();

  static auto getWeights(std::string filename, int firstLayoutSize)
      -> std::vector<std::vector<std::vector<double>>>;
  static auto saveWeights(
      std::string filename,
      const std::vector<std::vector<std::vector<double>>>& weights) -> void;
  static auto getSignal(std::vector<double>* signal, std::ifstream* fin) -> int;
  static auto countLineInFile(std::string filename) -> int;

 private:
  static auto getVector(std::string line) -> std::vector<double>;
  static auto getNumLayout(int sizeFirstLayout, std::string fileName) -> int;
};
}  //  namespace s21

#endif  // FILEEXECUTOR_H
