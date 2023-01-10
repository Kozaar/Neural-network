#include "dialogstarttraining.h"

#include "ui_dialogstarttraining.h"

namespace s21 {
DialogStartTraining::DialogStartTraining(Controller *controller,
                                         QString filename, QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogStartTraining) {
  ui->setupUi(this);
  ctrl_ = controller;
  filename_ = filename;
}

DialogStartTraining::~DialogStartTraining() { delete ui; }

void DialogStartTraining::on_buttonBox_accepted() {
  ctrl_->training(filename_.toStdString(), ui->spinBox->value());
  emit doing();
}

}  //  namespace s21
