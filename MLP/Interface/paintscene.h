#ifndef PAINTSCENE_H
#define PAINTSCENE_H
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

namespace s21 {
class PaintScene : public QGraphicsScene {
  Q_OBJECT
 public:
  PaintScene() : QGraphicsScene() {}

 private:
  QPointF point;

 private:
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};
}  //  namespace s21

#endif  // PAINTSCENE_H
