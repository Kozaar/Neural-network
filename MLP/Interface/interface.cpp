#include "interface.h"

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGraphicsItem>

#include "crossvalidationconf.h"
#include "dialogstarttraining.h"
#include "newnetsetting.h"
#include "qcustomplot.h"
#include "reportgraph.h"
#include "ui_interface.h"

namespace s21 {
Interface::Interface(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Interface) {
  ui->setupUi(this);
  scene = new PaintScene;
  ui->paintWidget->setSceneRect(0, 0, ui->paintWidget->width() - 2,
                                ui->paintWidget->height() - 2);
  ui->paintWidget->setScene(scene);
  scene->setBackgroundBrush(Qt::white);
  goToMainWindow();
}

Interface::~Interface() {
  delete scene;
  delete ui;
}

void Interface::on_pushButtonClean_clicked() { scene->clear(); }

void Interface::on_pushButtonOpenBMP_clicked() {
  QString filename;
  filename = QFileDialog::getOpenFileName(this, "Open File", "BMP_pictures",
                                          "BMP pictures (*.bmp)");
  if (filename != nullptr) {
    std::vector<double> img;
    QImage image(filename);
    image = image.scaled(512, 512);
    QGraphicsItem *immage = scene->addPixmap(QPixmap::fromImage(image));
    immage->setPos(QPoint(0, 0));
    image = image.scaled(28, 28);
    image.invertPixels(QImage::InvertRgb);
    for (int i = 0; i < image.width(); i++) {
      for (int j = 0; j < image.height(); j++) {
        img.push_back(image.pixelColor(i, j).green() +
                      image.pixelColor(i, j).blue());
      }
    }
    char ans = 64 + controller.classificate(img);
    ui->labelTextAnswer->setText(QString(ans));
  }
}

void Interface::on_radioButtonGraph_clicked() {
  controller.setRealisation(Graph);
}

void Interface::on_radioButtonMatrix_clicked() {
  controller.setRealisation(Matrix);
}

void Interface::OpenWeight_clicked() {
  QString filename;
  filename = QFileDialog::getOpenFileName(this, "Open File", "weights",
                                          "Text files (*.txt)");
  if (filename != nullptr) {
    controller.openWeights(filename.toStdString());
  }
}

void Interface::SaveWeight_clicked() {
  QString format = "txt";
  QString default_path = "weights";
  QFileDialog save_dialog(this, tr("Сохранить как"), default_path);
  save_dialog.setAcceptMode(QFileDialog::AcceptSave);
  save_dialog.setDirectory(default_path);
  save_dialog.setDefaultSuffix(format);
  const QStringList filter("Text files (*.txt)");
  save_dialog.setMimeTypeFilters(filter);
  if (save_dialog.exec() == QDialog::Accepted) {
    const QString fname = save_dialog.selectedFiles().constFirst();
    controller.saveWeights(fname.toStdString());
  }
}

void Interface::on_pushButtonStartExperiment_clicked() {
  QString filename;
  if (ui->labelExperimentFilename->text() == "") {
    filename = QFileDialog::getOpenFileName(
        this, "Open File", "../../../../MLP/Model", "Text files (*.csv)");
    ui->labelExperimentFilename->setText(filename);
  } else {
    filename = ui->labelExperimentFilename->text();
  }
  if (filename != nullptr) {
    double averageAccuracy = 0, precision = 0, recall = 0, f_measure = 0,
           time = 0;
    controller.startExperiment(filename.toStdString(),
                               ui->spinBoxTestSet->value(), &averageAccuracy,
                               &precision, &recall, &f_measure, &time);
    ui->labelResultAccuracy->setText(QString::number(averageAccuracy));
    ui->labelResultPrecision->setText(QString::number(precision));
    ui->labelResultRecall->setText(QString::number(recall));
    ui->labelResultFMeasure->setText(QString::number(f_measure));
    ui->labelResultSpentTime->setText(QString::number(time));
  }
}

void Interface::on_pushButtonClassificate_clicked() {
  std::vector<double> img;
  QImage image = ui->paintWidget->grab().toImage();
  image = image.scaled(28, 28);
  image.invertPixels(QImage::InvertRgb);
  for (int i = 0; i < image.width(); i++) {
    for (int j = 0; j < image.height(); j++) {
      img.push_back(image.pixelColor(i, j).green() +
                    image.pixelColor(i, j).blue());
    }
  }
  char ans = 64 + controller.classificate(img);
  ui->labelTextAnswer->setText(QString(ans));
}

void Interface::goToExperimentWindow() {
  this->window()->setFixedSize(600, 600);
  ui->menuExperiment->clear();
  QAction *newAct = new QAction("Open file", this);
  ui->menuExperiment->addAction(newAct);
  connect(newAct, SIGNAL(triggered()), this,
          SLOT(on_pushButtonExperimentOpenFile_clicked()));
  newAct = new QAction("Back to main window", this);
  ui->menuExperiment->addAction(newAct);
  connect(newAct, SIGNAL(triggered()), this, SLOT(goToMainWindow()));
  ui->stackedWidget->setCurrentIndex(1);
}

void Interface::goToMainWindow() {
  this->window()->setFixedSize(1050, 650);
  ui->menuExperiment->clear();
  QAction *newAct = new QAction("Start experiment", this);
  ui->menuExperiment->addAction(newAct);
  connect(newAct, SIGNAL(triggered()), this, SLOT(goToExperimentWindow()));
  ui->stackedWidget->setCurrentIndex(0);
}

void Interface::goToCrossValidationWindow() {
  CrossValidationConf w(&controller,
                        ui->radioButtonGraph->isChecked() ? "Graph" : "Matrix",
                        controller.getNetConfig(), this);
  w.exec();
}

void Interface::createNet() {
  NewNetSetting dialog(&controller);
  connect(&dialog, SIGNAL(buttonGraph()), this, SLOT(setGraphType()));
  connect(&dialog, SIGNAL(buttonMatrix()), this, SLOT(setMatrixType()));
  dialog.exec();
}

void Interface::on_pushButtonExperimentOpenFile_clicked() {
  QString filename;
  filename = QFileDialog::getOpenFileName(
      this, "Open File", "../../../../MLP/Model", "Text files (*.csv)");
  if (filename != nullptr) {
    ui->labelExperimentFilename->setText(filename);
  }
}

void Interface::on_pushButtonGoToExperiment_clicked() {
  goToExperimentWindow();
}

void Interface::on_pushButtonBack_clicked() { goToMainWindow(); }

void Interface::showNetConfig_clicked() {
  QDialog d(this);
  d.setFixedSize(300, 150);
  QGridLayout grid;
  QLabel type("Your net type: ", this);
  type.setText(type.text() +
               (ui->radioButtonGraph->isChecked() ? "Graph" : "Matrix"));
  std::vector<int> netConfig = controller.getNetConfig();
  QLabel countLayer(
      QString("Count layers: ") + QString::number(netConfig.size()), this);
  QLabel neuronInLayersLabel("Neurons in layers:", this);
  for (size_t i = 0; i < netConfig.size(); i++) {
    neuronInLayersLabel.setText(neuronInLayersLabel.text() + "  " +
                                QString::number(netConfig[i]));
  }
  grid.addWidget(&type);
  grid.addWidget(&countLayer);
  grid.addWidget(&neuronInLayersLabel);
  d.setLayout(&grid);
  d.exec();
}

void Interface::on_pushButton_aboutThisNet_clicked() {
  showNetConfig_clicked();
}

void Interface::setMatrixType() {
  ui->radioButtonMatrix->setChecked(true);
  ui->radioButtonGraph->setChecked(false);
  on_radioButtonMatrix_clicked();
}

void Interface::setGraphType() {
  ui->radioButtonMatrix->setChecked(false);
  ui->radioButtonGraph->setChecked(true);
  on_radioButtonGraph_clicked();
}

void Interface::startLearning() {
  QString filename;
  filename = QFileDialog::getOpenFileName(
      this, "Open File", "../../../../MLP/Model", "Text files (*.csv)");
  if (filename != nullptr) {
    DialogStartTraining dStartTrain(&controller, filename);
    connect(&dStartTrain, SIGNAL(doing()), this, SLOT(askShowReport()));
    dStartTrain.exec();
  }
}

void Interface::askShowReport() {
  QDialog d(this);
  d.setFixedSize(300, 150);
  QGridLayout grid;
  QLabel type("Do you want to see report?", this);
  QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  grid.addWidget(&type);
  grid.addWidget(&buttons);
  d.setLayout(&grid);
  connect(&buttons, SIGNAL(accepted()), this, SLOT(showReport()));
  connect(&buttons, SIGNAL(accepted()), &d, SLOT(accept()));
  connect(&buttons, SIGNAL(rejected()), &d, SLOT(reject()));
  d.exec();
}

void Interface::showReport() {
  ReportGraph *rg = new ReportGraph(controller.getReport());
  rg->setAttribute(Qt::WA_DeleteOnClose);
  rg->show();
}
}  //  namespace s21
