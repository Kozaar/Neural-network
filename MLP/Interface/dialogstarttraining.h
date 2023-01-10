#ifndef DIALOGSTARTTRAINING_H
#define DIALOGSTARTTRAINING_H

#include <QDialog>

#include "../Controller/controller.h"

namespace Ui {
class DialogStartTraining;
}

namespace s21 {
class DialogStartTraining : public QDialog {
  Q_OBJECT

 public:
  explicit DialogStartTraining(Controller *controller, QString filename,
                               QWidget *parent = nullptr);
  ~DialogStartTraining();

 private slots:
  void on_buttonBox_accepted();

 signals:
  void doing();

 private:
  Ui::DialogStartTraining *ui;
  Controller *ctrl_;
  QString filename_;
};

}  //  namespace s21

#endif  // DIALOGSTARTTRAINING_H
