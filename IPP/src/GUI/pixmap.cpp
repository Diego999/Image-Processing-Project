#include "include/GUI/pixmap.h"

Pixmap::Pixmap(const QPixmap &pix) : QGraphicsObject(), m_pixmap(pix)
{
}

void Pixmap::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->drawPixmap(QPointF(), m_pixmap);
}
