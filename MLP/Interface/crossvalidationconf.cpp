#include "crossvalidationconf.h"

#include <QFileDialog>

#include "reportgraph.h"
#include "ui_crossvalidationconf.h"

namespace s21 {
CrossValidationConf::CrossValidationConf(Controller *controller,
                                         const QString netType,
                                         const std::vector<int> &netConfig,
                                         QWidget *parent)
    : QDialog(parent), ui(new Ui::CrossValidationConf) {
  ui->setupUi(this);
  ui->label_netTypeAnswer->setText(netType);
  ui->label_countLayersAnswer->setText(QString::number(netConfig.size()));
  for (size_t i = 0; i < netConfig.size(); i++) {
    ui->label_neuronsInLayersAnswer->setText(
        ui->label_neuronsInLayersAnswer->text() + " " +
        QString::number(netConfig[i]));
  }
  controller_ = controller;
}

CrossValidationConf::~CrossValidationConf() { delete ui; }

void CrossValidationConf::on_pushButton_openFile_clicked() {
  QString filename;
  filename = QFileDialog::getOpenFileName(
      this, "Open File", "../../../../MLP/Model", "Text files (*.csv)");
  if (filename != nullptr) {
    ui->label_filename->setText(filename);
  }
}

void CrossValidationConf::on_buttonBox_accepted() {
  if (ui->label_filename->text() == QString("File name")) {
    on_pushButton_openFile_clicked();
  }
  controller_->crossValidation(ui->label_filename->text().toStdString(),
                               ui->spinBox_countGroups->value());
  QDialog d(this);
  d.setFixedSize(300, 150);
  QGridLayout grid;
//  QLabel type("Do you want to see report?", this);
  QLabel type("Cross validation complete", &d);
  QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  grid.addWidget(&type);
  grid.addWidget(&buttons);
  d.setLayout(&grid);
//  connect(&buttons, SIGNAL(accepted()), this, SLOT(showReport()));
  connect(&buttons, SIGNAL(accepted()), &d, SLOT(accept()));
  connect(&buttons, SIGNAL(rejected()), &d, SLOT(reject()));
  d.exec();
}

void CrossValidationConf::showReport() {
  ReportGraph *rg = new ReportGraph(controller_->getReport());
  rg->setAttribute(Qt::WA_DeleteOnClose);
  rg->show();
}

}  //  namespace s21
