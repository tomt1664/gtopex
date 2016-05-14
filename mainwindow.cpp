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
#include "cdialog.h"
#include "sdialog.h"
#include "mainwindow.h"

#include <QDebug>

#include <QHBoxLayout>
#include <QKeyEvent>
#include <QTime>
#include <QtMath>
#include <QtAlgorithms>
#include <QList>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#include <QPrintDialog>
#endif
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QGraphicsItem>
#include <QtWidgets>
#include <qmath.h>


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), timerId(0)
{
    scene = new QGraphicsScene;

    setWindowFilePath(QString());

    setWindowTitle(tr("GTopEx"));

    gView = new QGraphicsView(scene);
    gView->setRenderHint(QPainter::Antialiasing, false);
    gView->setDragMode(QGraphicsView::RubberBandDrag);
    gView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    gView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    gView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    gView->setRenderHint(QPainter::Antialiasing);

    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);

    QToolButton *zoominButton = new QToolButton;
    zoominButton->setAutoRepeat(true);
    zoominButton->setAutoRepeatInterval(33);
    zoominButton->setAutoRepeatDelay(0);
    zoominButton->setIcon(QPixmap(":/icons/plus_32.png"));
    zoominButton->setIconSize(iconSize);
    QToolButton *zoomoutButton = new QToolButton;
    zoomoutButton->setAutoRepeat(true);
    zoomoutButton->setAutoRepeatInterval(33);
    zoomoutButton->setAutoRepeatDelay(0);
    zoomoutButton->setIcon(QPixmap(":/icons/minus_32.png"));
    zoomoutButton->setIconSize(iconSize);
    zoomSlider = new QSlider;
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(500);
    zoomSlider->setValue(250);

    // Zoom slide bar
    QVBoxLayout *zoomslideLayout = new QVBoxLayout;
    zoomslideLayout->addWidget(zoominButton);
    zoomslideLayout->addWidget(zoomSlider);
    zoomslideLayout->addWidget(zoomoutButton);

    QToolButton *rotateleftButton = new QToolButton;
    rotateleftButton->setIcon(QPixmap(":/icons/rotl_32.png"));
    rotateleftButton->setIconSize(iconSize);
    QToolButton *rotaterightButton = new QToolButton;
    rotaterightButton->setIcon(QPixmap(":/icons/rotr_32.png"));
    rotaterightButton->setIconSize(iconSize);
    rotateSlider = new QSlider;
    rotateSlider->setOrientation(Qt::Horizontal);
    rotateSlider->setMinimum(-360);
    rotateSlider->setMaximum(360);
    rotateSlider->setValue(0);

    // Rotate slide bar
    QHBoxLayout *rotateslideLayout = new QHBoxLayout;
    rotateslideLayout->addWidget(rotateleftButton);
    rotateslideLayout->addWidget(rotateSlider);
    rotateslideLayout->addWidget(rotaterightButton);

    resetButton = new QToolButton;
    resetButton->setIcon(QIcon(QPixmap(":/icons/reset_32.png")));
    resetButton->setIconSize(iconSize);
    resetButton->setEnabled(false);

    // Label layout
    QHBoxLayout *labelLayout = new QHBoxLayout;
    selectMButton = new QToolButton;
    selectMButton->setIcon(QIcon(QPixmap(":/icons/point_32.png")));
    selectMButton->setIconSize(iconSize);
    selectMButton->setCheckable(true);
    selectMButton->setChecked(true);

    dragMButton = new QToolButton;
    dragMButton->setIcon(QIcon(QPixmap(":/icons/move_32.png")));
    dragMButton->setIconSize(iconSize);
    dragMButton->setCheckable(true);
    dragMButton->setChecked(false);

    bondrotButton = new QToolButton;
    bondrotButton->setIcon(QIcon(QPixmap(":/icons/bondrot_32.png")));
    bondrotButton->setIconSize(iconSize);
    bondrotButton->setToolTip("Rotate selection");

    deleteButton = new QToolButton;
    deleteButton->setIcon(QIcon(QPixmap(":/icons/delete_32.png")));
    deleteButton->setIconSize(iconSize);
    deleteButton->setToolTip("Delete selection");

    printButton = new QToolButton;
    printButton->setIcon(QIcon(QPixmap(":/icons/picture_32.png")));
    printButton->setIconSize(iconSize);
    printButton->setToolTip("Export image");

    openButton = new QToolButton;
    openButton->setIcon(QIcon(QPixmap(":/icons/open_32.png")));
    openButton->setIconSize(iconSize);
    openButton->setToolTip("Open file");

    saveButton = new QToolButton;
    saveButton->setIcon(QIcon(QPixmap(":/icons/save_32.png")));
    saveButton->setIconSize(iconSize);
    saveButton->setToolTip("Save file");

    infoButton = new QToolButton;
    infoButton->setIcon(QIcon(QPixmap(":/icons/info_32.png")));
    infoButton->setIconSize(iconSize);
    infoButton->setToolTip("Info");

    createButton = new QToolButton;
    createButton->setIcon(QIcon(QPixmap(":/icons/create_32.png")));
    createButton->setIconSize(iconSize);
    createButton->setToolTip("Create lattice");

    strainButton = new QToolButton;
    strainButton->setIcon(QIcon(QPixmap(":/icons/compr_32.png")));
    strainButton->setIconSize(iconSize);
    strainButton->setToolTip("Calculate strain tensors");

    runButton = new QToolButton;
    runButton->setIcon(QIcon(QPixmap(":/icons/run_32.png")));
    runButton->setIconSize(iconSize);
    runButton->setToolTip("Run optimisation");

    paramButton = new QToolButton;
    paramButton->setIcon(QIcon(QPixmap(":/icons/list_32.png")));
    paramButton->setIconSize(iconSize);
    paramButton->setToolTip("Optimisation settings");

    chartButton = new QToolButton;
    chartButton->setIcon(QIcon(QPixmap(":/icons/graph_32.png")));
    chartButton->setIconSize(iconSize);
    chartButton->setToolTip("System analysis");

    QButtonGroup *pointerMGroup = new QButtonGroup;
    pointerMGroup->setExclusive(true);
    pointerMGroup->addButton(selectMButton);
    pointerMGroup->addButton(dragMButton);

    labelLayout->addWidget(openButton);
    labelLayout->addWidget(saveButton);
    labelLayout->addWidget(createButton);
    labelLayout->addWidget(printButton);
    labelLayout->addStretch();
    labelLayout->addWidget(selectMButton);
    labelLayout->addWidget(dragMButton);
    labelLayout->addWidget(bondrotButton);
    labelLayout->addWidget(deleteButton);
    labelLayout->addStretch();
    labelLayout->addWidget(runButton);
    labelLayout->addWidget(strainButton);
    labelLayout->addWidget(chartButton);
    labelLayout->addWidget(paramButton);
    labelLayout->addWidget(infoButton);

    QGridLayout *topLayout = new QGridLayout;
    topLayout->addLayout(labelLayout, 0, 0);
    topLayout->addWidget(gView, 1, 0);
    topLayout->addLayout(zoomslideLayout, 1, 1);
    topLayout->addLayout(rotateslideLayout, 2, 0);
    topLayout->addWidget(resetButton, 2, 1);
    setLayout(topLayout);

    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetView()));
    connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
    connect(rotateSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
    connect(gView->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(setResetButtonEnabled()));
    connect(gView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(setResetButtonEnabled()));
    connect(selectMButton, SIGNAL(toggled(bool)), this, SLOT(togglePMode()));
    connect(dragMButton, SIGNAL(toggled(bool)), this, SLOT(togglePMode()));
    connect(bondrotButton, SIGNAL(toggled(bool)), this, SLOT(bondrotate()));
    connect(rotateleftButton, SIGNAL(clicked()), this, SLOT(rotateLeft()));
    connect(rotaterightButton, SIGNAL(clicked()), this, SLOT(rotateRight()));
    connect(zoominButton, SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect(zoomoutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));
    connect(printButton, SIGNAL(clicked()), this, SLOT(print()));
    connect(infoButton, SIGNAL(clicked()), this, SLOT(slotAbout()));
    connect(openButton, SIGNAL(clicked()), this, SLOT(openfile()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(savefile()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteatom()));
    connect(createButton, SIGNAL(clicked()), this, SLOT(createConfig()));
    connect(runButton, SIGNAL(clicked()), this, SLOT(runSystem()));
    connect(paramButton, SIGNAL(clicked()), this, SLOT(settings()));
    connect(strainButton, SIGNAL(clicked()), this, SLOT(dostrain()));

    setupMatrix();

    //default periodic supercell for open system
    bounds.append(-10000.0);
    bounds.append(10000.0);
    bounds.append(-10000.0);
    bounds.append(10000.0);

    //default potentials
    pot.append(5.7);
    pot.append(1.96);
    pot.append(1.42);
    pot.append(1.90);
    apot.append(2.0944);
    apot.append(500.00);
    step = 0.005;
    nstep = 20;
}

void MainWindow::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}


void MainWindow::createConfig()  //function to create a graphene lattice
{
    atoms.clear(); //clear configuration arrays
    xatmpos.clear();
    yatmpos.clear();

    LatticeDialog  latdialog;
    latdialog.exec();

    double bndl = latdialog.bondl(); //get bond length from dialog
    int nlx = latdialog.xlatr(); //get lattice repetitions
    int nly = latdialog.ylatr();
    int ctr = latdialog.center();

    QList<double> xc;
    QList<double> yc;

    //define orthorhombic the unit cell
    xc.append(0.0);
    yc.append(0.0);
    xc.append(bndl*86.603);
    yc.append(bndl*50.0);
    xc.append(bndl*86.603);
    yc.append(bndl*150.0);
    xc.append(0.0);
    yc.append(bndl*200.0);

    //define lattice vectors
    double xlv = bndl*173.205;
    double ylv = bndl*300.0;

    double xcntr = (ctr*nlx)/2.0; //do centering
    double ycntr = (ctr*nly)/2.0;

    nitems = 0;

    //set supercell size
    bounds[0] = xlv*(-xcntr);
    bounds[1] = (xlv)*(nlx-xcntr);
    bounds[2] = ylv*(-ycntr);
    bounds[3] = (ylv)*(nly-ycntr);

    //create lattice
    for (int j = 0; j < nlx; j++)
    {
        for(int i = 0; i < nly; i++)
        {
            for(int n = 0; n < 4; n++)
            {
                xatmpos.append(xc[n]+(xlv*(j-xcntr)));
                yatmpos.append(yc[n]+(ylv*(i-ycntr)));
                nitems++;
            }
        }
    }

    populateScene(); // re-draw scene with new coordinates
    gView->setScene(scene);
}

void MainWindow::populateScene()  //function to draw the structure
{
    scene->clear();
    gView->items().clear();
//    scene = new QGraphicsScene;

    //draw atoms
    for(int i = 0; i < nitems; i++)
    {
        QColor color(100,100,255,255);
        Atom *item = new Atom(color,this);
        item->setPos(QPointF(xatmpos[i], yatmpos[i]));
        scene->addItem(item);

        atoms << item;
    }

    //draw bonds
    for(int i = 0; i < nitems; i++)
    {
        for(int j = (i+1); j < nitems; j++)
        {
            qreal tsq = qPow(xatmpos[i]-xatmpos[j],2) + qPow(yatmpos[i]-yatmpos[j],2);
            qreal bdist = qSqrt(tsq);
            if(bdist < 500.0)
            {
                scene->addItem(new Bond(atoms[i], atoms[j]));
            }
        }
    }
}

void MainWindow::resetView()
{
    zoomSlider->setValue(250);
    rotateSlider->setValue(0);
    setupMatrix();
    gView->ensureVisible(QRectF(0, 0, 0, 0));

    resetButton->setEnabled(false);
}

void MainWindow::setResetButtonEnabled()
{
    resetButton->setEnabled(true);
}

void MainWindow::setupMatrix()
{
    qreal scale = qPow(qreal(2), (zoomSlider->value() - 250) / qreal(50));

    QMatrix matrix;
    matrix.scale(scale, scale);
    matrix.rotate(rotateSlider->value());

    gView->setMatrix(matrix);
    setResetButtonEnabled();
}

void MainWindow::togglePMode()
{
    gView->setDragMode(selectMButton->isChecked()
                              ? QGraphicsView::RubberBandDrag
                              : QGraphicsView::ScrollHandDrag);
    gView->setInteractive(selectMButton->isChecked());
}





void MainWindow::bondrotate()
{

    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() == Bond::Type) {
            scene->removeItem(item);
            Bond *bond = qgraphicsitem_cast<Bond *>(item);
            bond->sourceAtom()->removeBond(bond);
            bond->destAtom()->removeBond(bond);
            delete item;
        }
    }

    foreach (QGraphicsItem *item, scene->selectedItems()) {
         if (item->type() == Atom::Type)
             qgraphicsitem_cast<Atom *>(item)->removeBonds();
         scene->removeItem(item);
         delete item;
         nitems--;
    }


}




void MainWindow::deleteatom()
{
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() == Bond::Type) {
            scene->removeItem(item);
            Bond *bond = qgraphicsitem_cast<Bond *>(item);
            bond->sourceAtom()->removeBond(bond);
            bond->destAtom()->removeBond(bond);
            delete item;
        }
    }

    foreach (QGraphicsItem *item, scene->selectedItems()) {
         if (item->type() == Atom::Type)
             qgraphicsitem_cast<Atom *>(item)->removeBonds();
         scene->removeItem(item);
         delete item;
         nitems--;
    }
}

void MainWindow::slotAbout()
{
    QMessageBox::about(this, tr("About "),
            tr("<p><b>GTopEx version 0.1</b></p>"
               "<p>Build date: %1"
               "<br> "
               "<br>This program is built using Qt 5.5"
               "<br> "
               "<br>Tom Trevethan"
               "<br>Email: <a href=\"mailto:tptrevethan@googlemail.com>\">tptrevethan@googlemail.com</a>"
               "<br>2016"
               "</p>").arg(__TIMESTAMP__));
}

void MainWindow::openfile() //function to open file dialog and read xyz configuration file
{
    QString inputfile = QFileDialog::getOpenFileName(this, "Open XYZ File",
                                                    QString(),
                                                    "XYZ Files (*.xyz)");

    if (!inputfile.isNull()) {
        QString line;

        atoms.clear(); //clear configuration arrays
        xatmpos.clear();
        yatmpos.clear();

        nitems = 0;

        QFile file(inputfile);
        if (!file.open(QFile::ReadOnly)) {
            throw std::bad_exception();
        }
        QTextStream input(&file);

        line = input.readLine();
        bool ok;
        nitems = line.toInt(&ok,10);

        if(!ok)
        {
            QMessageBox msgbox;
            msgbox.setText("Error reading XYZ file");
            msgbox.exec();
            return;
        }

        line = input.readLine();
        QStringList cell = line.split(" ",QString::SkipEmptyParts);
        if(cell.size() == 3)
        {
            bounds[0] = 0.0;
            bounds[1] = 100*cell[0].toFloat(&ok);
            bounds[2] = 0.0;
            bounds[3] = 100*cell[1].toFloat(&ok);
        }

        for(int i = 0; i < nitems; i++)
        {
            line = input.readLine();
            QStringList coords = line.split(" ",QString::SkipEmptyParts);
            if(coords.size() != 4)
            {
                QMessageBox msgbox;
                msgbox.setText("Error reading XYZ file");
                msgbox.exec();
                return;
            }
            float xap =  100*coords[1].toFloat(&ok);
            float yap =  100*coords[2].toFloat(&ok);

            xatmpos.append(xap);
            yatmpos.append(yap);
        }

        populateScene(); // re-draw scene with new coordinates
        gView->setScene(scene);
    }
}

void MainWindow::savefile() //save scene configuration to an xyz file
{
    QString savefile = QFileDialog::getSaveFileName(this, "Save coordinates",
                                                    QString(),
                                                    "XYZ Files (*.xyz)");

    if (!savefile.isNull()) {

        QFile sfile(savefile);
        if (sfile.open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream out(&sfile);
            out << nitems << "\n";
            out << (bounds[1]-bounds[0])*0.01 << " " << (bounds[3]-bounds[2])*0.01 << " 10.0\n"; //write cell dimensions

            foreach (QGraphicsItem *item, scene->items()) {
                Atom *atom = qgraphicsitem_cast<Atom *>(item);
                if (!atom) continue;
                QPointF vec = atom->pos();
                float xout = vec.x()/100.0;
                float yout = vec.y()/100.0;
                out << "C  " << xout << " " << yout << " 0.0\n";
            }
            sfile.close();
        } else
        {
            QMessageBox msgbox;
            msgbox.setText("Error writing XYZ file");
            msgbox.exec();
            return;
        }
    } else
    {
        QMessageBox msgbox;
        msgbox.setText("Error writing XYZ file");
        msgbox.exec();
        return;
    }
}

void MainWindow::print()
{
#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        gView->render(&painter);
    }
#endif
}

//relax the system based on simplified force-field
void MainWindow::runSystem()
{

    //get atom list
    int natopt = 0;
    QList<Atom *> atoms;
    foreach (QGraphicsItem *item, scene->items()) {
        if (Atom *atom = qgraphicsitem_cast<Atom *>(item))
        {
            atoms << atom;
            natopt++;
        }
    }

    //get nearest neighbour list for each atom and initialise potentials
    natopt = 0;
    foreach (Atom *atom, atoms)
    {
        atom->getnlist(pot,apot);
        natopt++;
    }

    for(int icount = 0; icount < nstep; icount++)
    {
        //do forces and steepest descent step
        natopt = 0;
        foreach (Atom *atom, atoms)
        {
            natopt++;
            atom->atomForces(step);

        }

        //exit iteration loop if movement stops
        int itemmoved = 0;
        foreach (Atom *atom, atoms)
        {
            if(atom->advance()) itemmoved = 1;
        }

        if (!itemmoved) {
            return;
        }
        qDebug() << " iter " << icount;
    }
}

void MainWindow::settings()
{
    SettingsDialog  setdialog(pot,apot,step,bounds,nstep);
    setdialog.exec();

    //get the settings dialog values
    setdialog.getpots(pot);
    setdialog.getapots(apot);
    setdialog.getbounds(bounds);
    step = setdialog.getstep();
    nstep = setdialog.getnstep();
}

void MainWindow::dostrain()
{
    QVector<float> xtensor;
    QVector<float> ytensor;
    QVector<float> voltensor;


    //open dialog box to select options and color pallete









    //get atom list
    int natopt = 0;
    QList<Atom *> atoms;
    foreach (QGraphicsItem *item, scene->items()) {
        if (Atom *atom = qgraphicsitem_cast<Atom *>(item))
        {
            atoms << atom;
            natopt++;
        }
    }

    //get nearest neighbour list for each atom
    natopt = 0;
    foreach (Atom *atom, atoms)
    {
        atom->getnlist(pot,apot);
        natopt++;
    }







    //calculate strain tensors
    foreach (Atom *atom, atoms)
    {
        atom->tensors(xtensor,ytensor,voltensor);
        natopt++;
    }



    //do coloring




    int icnt = 0;
    foreach (QGraphicsItem *item, scene->items()) {

        QColor color(icnt*8,0,255-icnt*8,255);

        Atom *atom = qgraphicsitem_cast<Atom *>(item);
        if (!atom) continue;
        icnt++;

        qDebug() << icnt << " " << atom;
        atom->changeColor(color);
        scene->update();

    }

}



void MainWindow::drawcharts()
{

}




void MainWindow::zoomIn(int level)
{
    zoomSlider->setValue(zoomSlider->value() + level);
}

void MainWindow::zoomOut(int level)
{
    zoomSlider->setValue(zoomSlider->value() - level);
}

void MainWindow::rotateLeft()
{
    rotateSlider->setValue(rotateSlider->value() - 10);
}

void MainWindow::rotateRight()
{
    rotateSlider->setValue(rotateSlider->value() + 10);
}


