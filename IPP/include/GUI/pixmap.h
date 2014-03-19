#ifndef PIXMAP_H
#define PIXMAP_H

#include <QGraphicsObject>
#include <QPixmap>
#include <QPainter>

class Pixmap : public QGraphicsObject
{
    Q_OBJECT
public:
    Pixmap(const QPixmap &pix);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    QRectF boundingRect() const
    {
        return QRectF( QPointF(0, 0), p.size());
    }

private:
    QPixmap p;
};

#endif // PIXMAP_H
