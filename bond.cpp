#include "bond.h"
#include "atom.h"

#include <math.h>

#include <QPainter>

Bond::Bond(Atom *sourceAtom, Atom *destAtom)
{
    setZValue(-10);
    setAcceptedMouseButtons(0);
    source = sourceAtom;
    dest = destAtom;
    source->addBond(this);
    dest->addBond(this);
    adjust();
}

Atom *Bond::sourceAtom() const
{
    return source;
}

Atom *Bond::destAtom() const
{
    return dest;
}

void Bond::adjust()
{
    if (!source || !dest)
        return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.) && length < qreal(190.0)) {
        sourcePoint = line.p1();
        destPoint = line.p2();
    } else {
        sourcePoint = destPoint = line.p1();
    }
}

QRectF Bond::boundingRect() const
{
    if (!source || !dest)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth) / 2.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void Bond::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    // Draw the line itself
    painter->setPen(QPen(Qt::gray, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

}
