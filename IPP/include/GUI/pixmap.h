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
        return QRectF( QPointF(0, 0), m_pixmap.size());
    }

    void pixmap(const QPixmap& pixmap) { m_pixmap = pixmap; }

    int width() const { return m_pixmap.width(); }
    int height() const { return m_pixmap.height(); }

private:
    QPixmap m_pixmap;
};

#endif // PIXMAP_H
