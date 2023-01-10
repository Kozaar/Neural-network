#include "newnetsetting.h"

#include "ui_newnetsetting.h"

namespace s21 {
NewNetSetting::NewNetSetting(Controller *controller, QWidget *parent)
    : QDialog(parent), ui(new Ui::NewNetSetting) {
  ui->setupUi(this);
  controller_ = controller;
  for (int i = 0; i < maxLayers; i++) {
    spinboxes[i].setMinimum(1);
    spinboxes[i].setMaximum(10000);
    spinboxes[i].setValue(140 + i);
    spinboxes[i].hide();
  }
  for (int i = 0; i < ui->spinBox_countLayers->value() - 1; i++) {
    spinboxes[i].show();
  }
  spinboxes[0].setMinimum(784);
  spinboxes[0].setMaximum(784);
  spinboxes[0].setValue(784);
  spinboxes[maxLayers - 1].setMinimum(26);
  spinboxes[maxLayers - 1].setMaximum(26);
  spinboxes[maxLayers - 1].setValue(26);
  spinboxes[maxLayers - 1].show();
  for (int i = 0; i < maxLayers; i++) {
    ui->horizontalLayout->addWidget(&spinboxes[i]);
  }
}

NewNetSetting::~NewNetSetting() { delete ui; }

void NewNetSetting::on_spinBox_countLayers_valueChanged(int arg1) {
  spinboxes[arg1 - 2].show();
  if (arg1 < 7) spinboxes[arg1 - 1].hide();
}

void NewNetSetting::on_buttonBox_accepted() {
  std::vector<int> layers;
  for (int i = 0; i < maxLayers; i++) {
    if (!spinboxes[i].isHidden()) {
      layers.push_back(spinboxes[i].value());
    }
  }
  if (ui->radioButton_graphType->isChecked()) {
    emit buttonGraph();
  } else {
    emit buttonMatrix();
  }
  controller_->createNewNet(layers);
}
}  //  namespace s21
