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

    void getnlist(QVector<float>& potin,QVector<float>& apotin); //determine the neighbour list for the atom and initialise potentials
    void atomForces(float step); //calculate total force on atom due to neighbours and integrate translation

    enum { Type = UserType + 1 };
    int type() const Q_DECL_OVERRIDE { return Type; }

    bool advance();

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) Q_DECL_OVERRIDE;

protected:

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QColor color;
    QVector<QPointF> stuff;
    QList<Bond *> bondList;
    QPointF newPos;
    MainWindow *graph;
    QVector<int> nlist; //list of nearest neighbours
    QVector<float> pot; //morse potential parameters
    QVector<float> apot; //angle potential parameters
};

#endif // ATOM_H
