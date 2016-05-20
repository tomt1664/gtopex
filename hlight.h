/****************************************************************************
** Graphene Topology Explorer
**
** Tom Trevethan 2016
** tptrevethan@googlemail.com
****************************************************************************/

#ifndef HLIGHT_H
#define HLIGHT_H

#include <QGraphicsItem>

class Atom;

class Highlight : public QGraphicsItem
{
public:
    Highlight(Atom *sourceAtom, Atom *destAtom); //highlight constructor

    Atom *sourceAtom() const; //return bond atoms
    Atom *destAtom() const;

    void adjust();

    enum { Type = UserType + 2 };
    int type() const Q_DECL_OVERRIDE { return Type; }

protected:
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

private:
    Atom *source, *dest;

    QPointF sourcePoint;
    QPointF destPoint;
};

#endif // HLIGHT_H

