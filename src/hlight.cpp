/****************************************************************************
** Graphene Topology Explorer
**
** Tom Trevethan 2016
** tptrevethan@googlemail.com
****************************************************************************/

#include "hlight.h"

#include <math.h>

#include <QPainter>

Highlight::Highlight(QPointF sPoint, QPointF dPoint, int col)
{
    setZValue(10); //draw on top
    setOpacity(0.3); //make semi-transparent
    setAcceptedMouseButtons(0);
    sourcePoint = sPoint;
    destPoint = dPoint;
    m_col = col;
}

QRectF Highlight::boundingRect() const
{
    qreal penWidth = 1;
    qreal extra = (penWidth) / 2.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void Highlight::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    // Draw the line itself
    if(m_col == 0)
    {
        painter->setPen(QPen(Qt::yellow, 16, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    } else if(m_col == 1)
    {
        painter->setPen(QPen(Qt::blue, 16, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    } else
    {
        painter->setPen(QPen(Qt::red, 16, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    painter->drawLine(line);

}

