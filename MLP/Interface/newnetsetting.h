#ifndef NEWNETSETTING_H
#define NEWNETSETTING_H

#include <QDialog>
#include <QSpinBox>

#include "../Controller/controller.h"

namespace Ui {
class NewNetSetting;
}

namespace s21 {
class NewNetSetting : public QDialog {
  Q_OBJECT

 public:
  explicit NewNetSetting(Controller *controller, QWidget *parent = nullptr);
  ~NewNetSetting();

 private slots:
  void on_spinBox_countLayers_valueChanged(int arg1);
  void on_buttonBox_accepted();

 signals:
  void buttonGraph();
  void buttonMatrix();

 private:
  Ui::NewNetSetting *ui;
  Controller *controller_;
  static constexpr int maxLayers = 7;
  QSpinBox spinboxes[maxLayers];
};
}  //  namespace s21

#endif  // NEWNETSETTING_H
