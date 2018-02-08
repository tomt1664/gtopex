/****************************************************************************
** Graphene Topology Explorer
**
** Tom Trevethan 2016
** tptrevethan@googlemail.com
****************************************************************************/

#ifndef HLIGHT_H
#define HLIGHT_H

#include <QGraphicsItem>

// class to draw bond highlights and show polygons
class Highlight : public QGraphicsItem
{
public:
    Highlight(QPointF sPoint, QPointF dPoint, int col); //highlight constructor

    void adjust();

    enum { Type = UserType + 3 };
    int type() const Q_DECL_OVERRIDE { return Type; }

protected:
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

private:

    QPointF sourcePoint;
    QPointF destPoint;
    int m_col; //the highlight color (0 = yellow, 1 = blue, 2 = red)
};

#endif // HLIGHT_H

