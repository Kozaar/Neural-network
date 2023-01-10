#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMainWindow>

#include "../Controller/controller.h"
#include "paintscene.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Interface;
}
QT_END_NAMESPACE

namespace s21 {
class Interface : public QMainWindow {
  Q_OBJECT

 public:
  Interface(QWidget *parent = nullptr);
  ~Interface();

 private slots:
  void on_pushButtonClean_clicked();
  void on_radioButtonMatrix_clicked();
  void on_radioButtonGraph_clicked();
  void on_pushButtonOpenBMP_clicked();
  void OpenWeight_clicked();
  void SaveWeight_clicked();
  void on_pushButtonStartExperiment_clicked();
  void on_pushButtonClassificate_clicked();
  void on_pushButtonExperimentOpenFile_clicked();
  void on_pushButtonGoToExperiment_clicked();
  void on_pushButtonBack_clicked();
  void goToExperimentWindow();
  void goToMainWindow();
  void goToCrossValidationWindow();
  void createNet();
  void showNetConfig_clicked();
  void on_pushButton_aboutThisNet_clicked();
  void setMatrixType();
  void setGraphType();
  void startLearning();
  void askShowReport();
  void showReport();

 private:
  Ui::Interface *ui;
  PaintScene *scene;
  Controller controller;

  static constexpr int Graph = 0;
  static constexpr int Matrix = 1;

  auto test_func(double *time) -> void;
};
}  //  namespace s21
#endif  // INTERFACE_H
