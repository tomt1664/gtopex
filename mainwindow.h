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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QVector>
#include <QFrame>
#include <QGraphicsView>

class Atom;

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QLabel;
class QSlider;
class QToolButton;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

    void itemMoved();

public slots:
    void zoomIn(int level = 1);
    void zoomOut(int level = 1);

private slots:
    void resetView();
    void setResetButtonEnabled();
    void setupMatrix();
    void togglePMode();
    void bondrotate();
    void rotatevalue(int ang);
    void deleteatom();
    void openfile();
    void savefile();
    void slotAbout();
    void print();
    void rotateLeft();
    void rotateRight();
    void createConfig();
    void runSystem();
    void settings();
    void dostrain();
    void drawcharts();

private:
    void populateScene();

    QVector<float> xatmpos; //atom positions
    QVector<float> yatmpos;
    QList<Atom *> atoms; //list of all current atom objects
    QGraphicsScene *scene;

    QVector<float> bounds;
    int timerId;
    int nitems; //number of atoms in scene
    QVector<float> pot; //morse potential parameters
    QVector<float> apot; //angle potential parameters
    float step; //steepest descent step size
    int nstep; //max number of optimisation steps
    float xcom,ycom; //center of mass of group
    int rcounter;

    QGraphicsView *gView;
    QLabel *label;
    QLabel *label2;
    QToolButton *selectMButton;
    QToolButton *dragMButton;
    QToolButton *bondrotButton;
    QToolButton *deleteButton;
    QToolButton *printButton;
    QToolButton *resetButton;
    QToolButton *infoButton;
    QToolButton *openButton;
    QToolButton *saveButton;
    QToolButton *createButton;
    QToolButton *strainButton;
    QToolButton *runButton;
    QToolButton *paramButton;
    QToolButton *chartButton;
    QSlider *zoomSlider;
    QSlider *rotateSlider;
};

#endif // MAINWINDOW_H
