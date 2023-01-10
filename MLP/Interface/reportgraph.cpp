#include "reportgraph.h"

#include "ui_reportgraph.h"

ReportGraph::ReportGraph(std::vector<double> errors, QWidget *parent)
    : QDialog(parent), ui(new Ui::ReportGraph) {
  ui->setupUi(this);
  Qt::WindowFlags flags = windowFlags();
  setWindowFlags(flags | Qt::WindowStaysOnTopHint);
  showReport(errors);
}

ReportGraph::~ReportGraph() { delete ui; }

void ReportGraph::showReport(std::vector<double> errors) {
  if (errors.size()) {
    QVector<double> ybuf(errors.size());
    QVector<double> xbuf(errors.size());
    double yMin = errors[0], yMax = errors[0];
    for (size_t i = 0; i < errors.size(); i++) {
      xbuf[i] = i;
      ybuf[i] = errors[i];
      if (errors[i] < yMin) {
        yMin = errors[i];
      }
      if (errors[i] > yMax) {
        yMax = errors[i];
      }
    }
    ui->graphik->addGraph();
    ui->graphik->graph(0)->setData(xbuf, ybuf);
    ui->graphik->yAxis->setRange(0, yMax);
    ui->graphik->xAxis->setRange(0, errors.size());
    ui->graphik->replot();
  }
}
