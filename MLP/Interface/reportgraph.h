#ifndef REPORTGRAPH_H
#define REPORTGRAPH_H

#include <QDialog>

namespace Ui {
class ReportGraph;
}

class ReportGraph : public QDialog {
  Q_OBJECT

 public:
  explicit ReportGraph(std::vector<double> errors, QWidget *parent = nullptr);
  ~ReportGraph();

 private:
  Ui::ReportGraph *ui;

  auto showReport(std::vector<double> errors) -> void;
};

#endif  // REPORTGRAPH_H
