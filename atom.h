/****************************************************************************
** Graphene Topology Explorer
**
** Tom Trevethan 2016
** tptrevethan@googlemail.com
****************************************************************************/

#ifndef ATOM_H
#define ATOM_H

#include <QColor>
#include <QGraphicsItem>
#include <QList>
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class Bond;
class MainWindow;

class Atom : public QGraphicsItem
{
public:
    Atom(const QColor &color, MainWindow *mainWindow);

    void addBond(Bond *bond); //add bond
    QList<Bond *> bonds() const;

    void removeBond(Bond *bond);
    void removeBonds(); //remove bonds after atom deletion

    void changeColor(QColor &color); //change the atom color

    void getnlist(QVector<float>& potin,QVector<float>& apotin); //determine the neighbour list for the atom and initialise potentials
    void atomForces(float step); //calculate total force on atom due to neighbours and integrate translation

    void nnlist(QVector<int>& nlst,int& num); //return the number of nearest neighbours and their indexes

    //atomistic strain calculation
    void calcStrain(float bondl);
    double getStrain() { return m_strain; }

    enum { Type = UserType + 1 };
    int type() const Q_DECL_OVERRIDE { return Type; }

    //advance atom positions
    bool advance();

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) Q_DECL_OVERRIDE;

protected:

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QColor color; //atom color
    QVector<QPointF> stuff;
    QList<Bond *> bondList; //list of bonds for atom
    QPointF newPos; //new position after optimisation step
    MainWindow *graph;
    QVector<int> nlist; //list of nearest neighbours
    QVector<float> pot; //morse potential parameters
    QVector<float> apot; //angle potential parameters
    double m_strain; //the volumetric atomistic strain
};

#endif // ATOM_H
