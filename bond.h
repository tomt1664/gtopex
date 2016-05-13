#ifndef BOND_H
#define BOND_H

#include <QGraphicsItem>

class Atom;

//! [0]
class Bond : public QGraphicsItem
{
public:
    Bond(Atom *sourceAtom, Atom *destAtom);

    Atom *sourceAtom() const;
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

#endif // BOND_H

