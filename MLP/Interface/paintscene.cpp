#include "paintscene.h"

namespace s21 {
void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
  addEllipse(event->scenePos().x() - 15, event->scenePos().y() - 15, 30, 30,
             QPen(Qt::red), QBrush(Qt::red));
  point = event->scenePos();
}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
  addLine(point.x(), point.y(), event->scenePos().x(), event->scenePos().y(),
          QPen(Qt::red, 30, Qt::SolidLine, Qt::RoundCap));
  point = event->scenePos();
}
}  //  namespace s21
