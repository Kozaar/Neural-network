#ifndef CROSSVALIDATIONCONF_H
#define CROSSVALIDATIONCONF_H

#include <QDialog>

#include "../Controller/controller.h"

namespace Ui {
class CrossValidationConf;
}

namespace s21 {
class CrossValidationConf : public QDialog {
  Q_OBJECT

 public:
  CrossValidationConf(Controller *controller, const QString netType,
                      const std::vector<int> &netConfig,
                      QWidget *parent = nullptr);
  ~CrossValidationConf();

 private slots:
  void on_pushButton_openFile_clicked();

  void on_buttonBox_accepted();
  void showReport();

 private:
  Ui::CrossValidationConf *ui;
  Controller *controller_;
};
}  //  namespace s21

#endif  // CROSSVALIDATIONCONF_H
