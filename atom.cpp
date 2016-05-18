/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "atom.h"
#include "bond.h"
#include "mainwindow.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QtWidgets>
#include <QDebug>

Atom::Atom(const QColor &color, MainWindow *mainWindow) : graph(mainWindow)
{
    this->color = color;
    setZValue(-1);

    setFlags(ItemIsSelectable | ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
}

void Atom::addBond(Bond *bond)
{
    bondList << bond;
    bond->adjust();
}

QList<Bond *> Atom::bonds() const
{
    return bondList;
}

//remove bond from bondlist
void Atom::removeBond(Bond *bond)
{
    int index = bondList.indexOf(bond);

    if (index != -1)
        bondList.removeAt(index);
}

//remove bond from scene
void Atom::removeBonds()
{
    foreach (Bond *bond, bondList) {
        bond->sourceAtom()->removeBond(bond);
        bond->destAtom()->removeBond(bond);
        scene()->removeItem(bond);
        delete bond;
    }
}

void Atom::changeColor(QColor& color)
{
    this->color = color;
}

//function to find the three nearest neighbour atoms and stores the index in nlist
//currently uses a full loop - consider using a link cell list to improve speed
//
//or implement OMP
void Atom::getnlist(QVector<float>& potin,QVector<float>& apotin)
{
    pot = potin;
    apot = apotin;

    qreal rmin1 = 10000;
    qreal rmin2 = 11000;
    qreal rmin3 = 12000;
    int nn1 = -1;
    int nn2 = -1;
    int nn3 = -1;
    int atcnt = 0;
    nlist.clear();

    foreach (QGraphicsItem *item, scene()->items()) {
        Atom *atom = qgraphicsitem_cast<Atom *>(item);
        if (!atom)
            continue;

        atcnt++;
        QPointF vec = mapToItem(atom, 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();
        qreal r = qSqrt(dx * dx + dy * dy);

        if(r > 0.01)
        {
            if(r < rmin1)
            {
                rmin3 = rmin2;
                nn3 = nn2;

                rmin2 = rmin1;
                nn2 = nn1;

                rmin1 = r;
                nn1 = atcnt;
            } else if(r < rmin2)
            {
              rmin3 = rmin2;
              nn3 = nn2;

              rmin2 = r;
              nn2 = atcnt;
            } else if(r < rmin3)
            {
              rmin3 = r;
              nn3 = atcnt;
            }
        }
    }
    nlist.append(nn1);
    nlist.append(nn2);
    nlist.append(nn3);
}

//function to calculate the total force on the atom (from the three nearest neighbours)
//employs a morse potential and sp2 harmonic bond angle potential
void Atom::atomForces(float step)
{
    int atcnt = 0;
    QVector<float> xbnd; //bond list
    QVector<float> ybnd;

    qreal xfrc = 0; //total force on atom
    qreal yfrc = 0;

    float dotp, mag1, mag2, ang, agrad, xag1, xag2, yag1, yag2;
    int nbond = 0;

    //loop over all atoms
    foreach (QGraphicsItem *item, scene()->items()) {
        Atom *atom = qgraphicsitem_cast<Atom *>(item);
        atcnt++;
        if (!atom)
            continue;

        //calculate forces only if it is a nearest neighbour
        if(atcnt == nlist[0] || atcnt == nlist[1] || atcnt == nlist[2])
        {
            //get interatomic separation
            QPointF vec = mapToItem(atom, 0, 0);
            qreal dx = vec.x();
            qreal dy = vec.y();

            qreal d = 0.01*qSqrt(dx*dx + dy*dy);

//            qDebug() << pos() << " dx " << dx << " dy " << dy << " r " << d;

            if(d < pot[3])
            {
                //calculate magnitude of the Morse potential
                float morse = 2*pot[1]*pot[0]*(qExp(-2*pot[1]*(d-pot[2]))-qExp(-pot[1]*(d-pot[2])));

                xfrc += dx * morse * step; //add to atom force vector
                yfrc += dy * morse * step;
//                qDebug() << atcnt << " r " << d << " morse " << morse;

                xbnd.append(dx);
                ybnd.append(dy);
                nbond++;
            }
        }
    }

    //add harmonic bond angle forces
    if(nbond == 2) //1 angle term if only 2 bonds
    {
        //calculate angle
        dotp = xbnd[0]*xbnd[1] + ybnd[0]*ybnd[1];
        mag1 = qSqrt(xbnd[0]*xbnd[0] + ybnd[0]*ybnd[0]);
        mag2 = qSqrt(xbnd[1]*xbnd[1] + ybnd[1]*ybnd[1]);
        ang = qAcos(dotp/(mag1*mag2));

        agrad = -2*apot[1]*(ang - apot[0])/qSin(ang);

        xag1 = agrad*((xbnd[1]/(mag1*mag2)) - (qCos(ang)*xbnd[0]/(mag1*mag2)));
        xag2 = agrad*((xbnd[0]/(mag1*mag2)) - (qCos(ang)*xbnd[1]/(mag1*mag2)));

        yag1 = agrad*((ybnd[1]/(mag1*mag2)) - (qCos(ang)*ybnd[0]/(mag1*mag2)));
        yag2 = agrad*((ybnd[0]/(mag1*mag2)) - (qCos(ang)*ybnd[1]/(mag1*mag2)));

        xfrc -= xag1 + xag2; //add to angle force to vector
        yfrc -= yag1 + yag2;

//        qDebug() << " angle " << ang << " frc " << xag1 + xag2 << " " << yag1 + yag2;

    } else if(nbond == 3)
    {
        //calculate angle 1
        dotp = xbnd[0]*xbnd[1] + ybnd[0]*ybnd[1];
        mag1 = qSqrt(xbnd[0]*xbnd[0] + ybnd[0]*ybnd[0]);
        mag2 = qSqrt(xbnd[1]*xbnd[1] + ybnd[1]*ybnd[1]);
        ang = qAcos(dotp/(mag1*mag2));

        agrad = -2*apot[1]*(ang - apot[0])/qSin(ang);

        xag1 = agrad*((xbnd[1]/(mag1*mag2)) - (qCos(ang)*xbnd[0]/(mag1*mag2)));
        xag2 = agrad*((xbnd[0]/(mag1*mag2)) - (qCos(ang)*xbnd[1]/(mag1*mag2)));

        yag1 = agrad*((ybnd[1]/(mag1*mag2)) - (qCos(ang)*ybnd[0]/(mag1*mag2)));
        yag2 = agrad*((ybnd[0]/(mag1*mag2)) - (qCos(ang)*ybnd[1]/(mag1*mag2)));

        xfrc -= xag1 + xag2; //add to angle force to vector
        yfrc -= yag1 + yag2;

//        qDebug() << " angle1 " << ang << " frc " << xag1 + xag2 << " " << yag1 + yag2;

        //angle 2
        dotp = xbnd[0]*xbnd[2] + ybnd[0]*ybnd[2];
        mag1 = qSqrt(xbnd[0]*xbnd[0] + ybnd[0]*ybnd[0]);
        mag2 = qSqrt(xbnd[2]*xbnd[2] + ybnd[2]*ybnd[2]);
        ang = qAcos(dotp/(mag1*mag2));

        agrad = -2*apot[1]*(ang - apot[0])/qSin(ang);

        xag1 = agrad*((xbnd[2]/(mag1*mag2)) - (qCos(ang)*xbnd[0]/(mag1*mag2)));
        xag2 = agrad*((xbnd[0]/(mag1*mag2)) - (qCos(ang)*xbnd[2]/(mag1*mag2)));

        yag1 = agrad*((ybnd[2]/(mag1*mag2)) - (qCos(ang)*ybnd[0]/(mag1*mag2)));
        yag2 = agrad*((ybnd[0]/(mag1*mag2)) - (qCos(ang)*ybnd[2]/(mag1*mag2)));

        xfrc -= xag1 + xag2; //add to angle force to vector
        yfrc -= yag1 + yag2;

//        qDebug() << " angle2 " << ang << " frc " << xag1 + xag2 << " " << yag1 + yag2;

        //angle 3
        dotp = xbnd[1]*xbnd[2] + ybnd[1]*ybnd[2];
        mag1 = qSqrt(xbnd[1]*xbnd[1] + ybnd[1]*ybnd[1]);
        mag2 = qSqrt(xbnd[2]*xbnd[2] + ybnd[2]*ybnd[2]);
        ang = qAcos(dotp/(mag1*mag2));

        agrad = -2*apot[1]*(ang - apot[0])/qSin(ang);

        xag1 = agrad*((xbnd[2]/(mag1*mag2)) - (qCos(ang)*xbnd[1]/(mag1*mag2)));
        xag2 = agrad*((xbnd[1]/(mag1*mag2)) - (qCos(ang)*xbnd[2]/(mag1*mag2)));

        yag1 = agrad*((ybnd[2]/(mag1*mag2)) - (qCos(ang)*ybnd[1]/(mag1*mag2)));
        yag2 = agrad*((ybnd[1]/(mag1*mag2)) - (qCos(ang)*ybnd[2]/(mag1*mag2)));

        xfrc -= xag1 + xag2; //add to angle force to vector
        yfrc -= yag1 + yag2;

    }


    //stop movement if forces very small
if (qAbs(xfrc) < 0.07 && qAbs(yfrc) < 0.07)
    xfrc = yfrc = 0;

    newPos = pos() + QPointF(xfrc, yfrc);
    xfrc = yfrc = 0;

}

//advance atom position
bool Atom::advance()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}



//function to calculate the x,y and volumetric components of the 2D atomistic strain tensor based on the 3 nearest neighbours
void Atom::tensors(QVector<float>& otensor, float bondl, int tcomp)
{

}






QRectF Atom::boundingRect() const
{
    return QRectF(-50, -50, 100, 100);
}

QPainterPath Atom::shape() const
{
    QPainterPath path;
    path.addEllipse(-25, -25, 50, 50);
    return path;
}

void Atom::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor fillColor = (option->state & QStyle::State_Selected) ? color.dark(150) : color;
    if (option->state & QStyle::State_MouseOver)
        fillColor = fillColor.light(125);

    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    int width = 0;
    if (option->state & QStyle::State_Selected)
        width += 2;

    pen.setWidth(width);
    QBrush b = painter->brush();
    painter->setBrush(QBrush(fillColor.dark(option->state & QStyle::State_Sunken ? 120 : 100)));

    painter->drawEllipse(-25, -25, 50, 50);
    painter->setBrush(b);

}

QVariant Atom::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Bond *bond, bondList)
            bond->adjust();
        graph->itemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}



void Atom::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void Atom::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

