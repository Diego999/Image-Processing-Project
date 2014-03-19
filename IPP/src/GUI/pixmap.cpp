#include "include/GUI/pixmap.h"

Pixmap::Pixmap(const QPixmap &pix) : QGraphicsObject(), p(pix)
{
}

void Pixmap::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->drawPixmap(QPointF(), p);
}
