/****************************************************************************
** Graphene Topology Explorer
**
** Tom Trevethan 2016
** tptrevethan@googlemail.com
****************************************************************************/

#include "atom.h"
#include "bond.h"
#include "hlight.h"
#include "cdialog.h"
#include "sdialog.h"
#include "tdialog.h"
#include "rdialog.h"
#include "pdialog.h"
#include "mainwindow.h"

#include <QDebug>

#include <QProgressDialog>
#include <QColorDialog>
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

//main window
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

    // zoom control
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

    // rotate control
    QHBoxLayout *rotateslideLayout = new QHBoxLayout;
    rotateslideLayout->addWidget(rotateleftButton);
    rotateslideLayout->addWidget(rotateSlider);
    rotateslideLayout->addWidget(rotaterightButton);

    resetButton = new QToolButton;
    resetButton->setIcon(QIcon(QPixmap(":/icons/reset_32.png")));
    resetButton->setIconSize(iconSize);
    resetButton->setEnabled(false);

    // pointer settings
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

    colorButton = new QToolButton;
    colorButton->setIcon(QIcon(QPixmap(":/icons/color_32.png")));
    colorButton->setIconSize(iconSize);
    colorButton->setToolTip("Atom color");

    ringsButton = new QToolButton;
    ringsButton->setIcon(QIcon(QPixmap(":/icons/rings_32.png")));
    ringsButton->setIconSize(iconSize);
    ringsButton->setToolTip("Highlight rings");

    QButtonGroup *pointerMGroup = new QButtonGroup;
    pointerMGroup->setExclusive(true);
    pointerMGroup->addButton(selectMButton);
    pointerMGroup->addButton(dragMButton);

    labelLayout->addWidget(openButton);
    labelLayout->addWidget(saveButton);
    labelLayout->addWidget(printButton);
    labelLayout->addWidget(colorButton);
    labelLayout->addWidget(createButton);
    labelLayout->addStretch();
    labelLayout->addWidget(selectMButton);
    labelLayout->addWidget(dragMButton);
    labelLayout->addWidget(bondrotButton);
    labelLayout->addWidget(deleteButton);
    labelLayout->addStretch();
    labelLayout->addWidget(runButton);
    labelLayout->addWidget(strainButton);
    labelLayout->addWidget(ringsButton);
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
    connect(bondrotButton, SIGNAL(clicked()), this, SLOT(bondrotate()));
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
    connect(colorButton, SIGNAL(clicked()), this, SLOT(setcolor()));
    connect(ringsButton, SIGNAL(clicked()), this, SLOT(findrings()));

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

    //draw startup logo

    QGraphicsTextItem* text1  = new QGraphicsTextItem("GTopEx");
    QGraphicsTextItem* text2 = new QGraphicsTextItem("Graphene Topology Explorer");

    QFont bigfont;
    bigfont.setPointSize(82);
    QFont smallfont;
    smallfont.setPointSize(32);

    text1->setFont(bigfont);
    text2->setFont(smallfont);

    text1->setDefaultTextColor(Qt::darkGray);
    text2->setDefaultTextColor(Qt::darkGray);

    text1->setPos(40,-135);
    text2->setPos(-150,116);

    scene->addItem(text1);
    scene->addItem(text2);

    QColor color(215,215,215,255);
    Atom *atom1 = new Atom(color,this);
    atom1->setPos(QPointF(0.0, 0.0));
    scene->addItem(atom1);
    Atom *atom2 = new Atom(color,this);
    atom2->setPos(QPointF(0.0, -140.0));
    scene->addItem(atom2);
    Atom *atom3 = new Atom(color,this);
    atom3->setPos(QPointF(121.0, 70.0));
    scene->addItem(atom3);
    Atom *atom4 = new Atom(color,this);
    atom4->setPos(QPointF(-121.0, 70.0));
    scene->addItem(atom4);


    scene->addItem(new Bond(atom1, atom2));
    scene->addItem(new Bond(atom1, atom3));
    scene->addItem(new Bond(atom1, atom4));
//    scene->setSceneRect(-250,-250,770,500);
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
        QColor color(215,215,215,255);
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


//function to call the rotate group dialog and then perform the rotation
void MainWindow::bondrotate()
{
    RotateDialog *rotdialog = new RotateDialog;
    xcom = 0;
    ycom = 0;
    int nselect = 0;
    rcounter = 0;

    //get all selected atom positions and the C-O-M
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        Atom *atom = qgraphicsitem_cast<Atom *>(item);
        if (!atom) continue;
        QPointF apos = atom->pos();
        nselect++;
        xcom += apos.x();
        ycom += apos.y();
    }
    xcom = xcom/(nselect*1.0);
    ycom = ycom/(nselect*1.0);

    //connect the rotate dialog to the direct rotate function for real time response
    connect(rotdialog, SIGNAL(valueChanged(int)),this, SLOT(rotatevalue(int)));
    rotdialog->exec();

}

void MainWindow::rotatevalue(int ang)
{
    float angle = (ang-rcounter)*3.14159/180.0;
    float cosang = qCos(-angle);
    float sinang = qSin(-angle);

    //rotate the selected positions about the COM
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        Atom *atom = qgraphicsitem_cast<Atom *>(item);
        if (!atom) continue;
        QPointF apos = atom->pos();
        float xrot = cosang*(apos.x()-xcom) + sinang*(apos.y()-ycom);
        float yrot = cosang*(apos.y()-ycom) - sinang*(apos.x()-xcom);
        atom->setPos(xrot+xcom,yrot+ycom);
    }
    rcounter = ang;
}

//function to delete selected atoms from the scene
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
            tr("<p><b>GTopEx version 0.68</b></p>"
               "<br> "
               "<p>Build date: %1"
               "<br> "
               "<br>This program is built using Qt 5.6"
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

//function to set the atom color using qcolordialog
void MainWindow::setcolor()
{
    QColor color = QColorDialog::getColor(Qt::green, this, "Select Color");

    if( !color.isValid() ){
       return;
    }

    //set the chosen color for all atoms
    foreach (QGraphicsItem *item, scene->items()) {
        Atom *atom = qgraphicsitem_cast<Atom *>(item);
        if (!atom) continue;

        atom->changeColor(color);
        scene->update();
    }
}

//relax the system based on simplified force-field
void MainWindow::runSystem()
{
    //set-up progress dialog
    QProgressDialog progress("Optimising...", "Cancel", 0, nstep, this);
    progress.setWindowModality(Qt::WindowModal);

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

        progress.setValue(icount); //updtate progress bar
        if(progress.wasCanceled()) break;

        //exit iteration loop if movement stops
        int itemmoved = 0;
        foreach (Atom *atom, atoms)
        {
            if(atom->advance()) itemmoved = 1;
        }

        if (!itemmoved) {
            progress.setValue(nstep);
            return;
        }

    }
    progress.setValue(nstep);
}

//get the potentials and optimisation settings
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
    StrainColorDialog scolordialog;
    scolordialog.exec();

    int cscl;
    float mxv,mnv;
    cscl = scolordialog.cscale();
    mnv = scolordialog.minval();
    mxv = scolordialog.maxval();

    if(cscl == 0) return;

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

    //set nearest neighbour list for each atom object
    natopt = 0;
    foreach (Atom *atom, atoms)
    {
        atom->getnlist(pot,apot);
        natopt++;
    }

    //calculate the atomistic strain for each atom
    foreach (Atom *atom, atoms)
    {
        atom->calcStrain(pot[2]);
    }

    //do coloring
    foreach (QGraphicsItem *item, scene->items()) {

        Atom *atom = qgraphicsitem_cast<Atom *>(item);
        if (!atom) continue;
        double strn = atom->getStrain();
        int red,green,blue;

        if(strn < mnv) strn = mnv;
        if(strn > mxv) strn = mxv;

        strn = (strn - mnv)/(mxv - mnv);

        if(cscl == 1) //grayscale
        {
            red = strn*255;
            green = red;
            blue = red;
        } else if(cscl == 2) //hot RGB scale
        {
            if(strn <= 0.3333)
            {
                red = strn*3.0*255;
                green = 0.0;
                blue = 0.0;
            } else if(strn <= 0.6666)
            {
                red = 255;
                green = (strn-0.3333)*3*255;
                blue = 0.0;
            } else
            {
                red = 255;
                green = 255;
                blue = (strn-0.6666)*3*255;
            }

        } else if(cscl == 3) //rainbow RGB scale
        {
            if(strn <= 0.50)
            {
                red = 255;
                green = strn*2*255;
                blue = 0.0;
            } else if(strn <= 0.75)
            {
                red = (1-(strn-0.5)*4)*255;
                green = 255;
                blue = 0.0;
            } else
            {
                red = 0;
                green = (1-(strn-0.75)*4)*255;
                blue = (strn-0.75)*4*255;
            }
        }

        QColor color(red,green,blue,255);

        atom->changeColor(color);
        scene->update();

    }
}

//function to open find rings dialog box and find 3,4 or 5-fold rings and highlight them
void MainWindow::findrings()
{
    PolyDialog *ringdialog = new PolyDialog;

/*    xcom = 0;
    ycom = 0;
    int nselect = 0;
    rcounter = 0;

    //get all selected atom positions and the C-O-M
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        Atom *atom = qgraphicsitem_cast<Atom *>(item);
        if (!atom) continue;
        QPointF apos = atom->pos();
        nselect++;
        xcom += apos.x();
        ycom += apos.y();
    }
    xcom = xcom/(nselect*1.0);
    ycom = ycom/(nselect*1.0);
*/
    //connect the rotate dialog to the direct rotate function for real time response
    connect(ringdialog, SIGNAL(valueChanged(int)),this, SLOT(plotrings(int)));

    ringdialog->exec();

    //clear current highlighting
    foreach (QGraphicsItem *item, scene->items())
    {
        if (item->type() == Highlight::Type) {
            scene->removeItem(item);
            delete item;
        }
    }

}

//scan system connectivity and highlight selected nfold rings
void MainWindow::plotrings(int nfold)
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

    //get the nearest neighbour list for each atom object
    natopt = 0;
    int num = 0;
    numbonds = 0;
    QVector<int> bondl1; //list of all bonds
    QVector<int> bondl2;
    QVector<int> nlist;
    foreach (Atom *atom, atoms)
    {
        atom->nnlist(nlist, num);
        for(int ib = 0; ib < num; ib++)
        {
            if(nlist[ib] > natopt-1)
            {
                numbonds++;
                bondl1.push_back(natopt);
                bondl2.push_back(nlist[ib]);
            }
        }
        natopt++;
    }

    //get the 3 bond (angle) list
    QVector<int> n31;
    QVector<int> n32;
    QVector<int> n33;
    int nn3 = 0;
    for(int nb = 0; nb < numbonds; nb++)
    {
        for(int nb2 = nb + 1; nb2 < numbonds; nb2++)
        {
            if(bondl1[nb] == bondl1[nb2])
            {
                nn3++;
                n31.push_back(bondl2[nb]);
                n32.push_back(bondl1[nb]);
                n33.push_back(bondl2[nb2]);
            } else if(bondl2[nb] == bondl1[nb2])
            {
                nn3++;
                n31.push_back(bondl1[nb]);
                n32.push_back(bondl2[nb]);
                n33.push_back(bondl2[nb2]);
            } else if(bondl2[nb] == bondl2[nb2])
            {
                nn3++;
                n31.push_back(bondl1[nb]);
                n32.push_back(bondl2[nb]);
                n33.push_back(bondl1[nb2]);
            } else if(bondl1[nb] == bondl2[nb2])
            {
                nn3++;
                n31.push_back(bondl2[nb]);
                n32.push_back(bondl1[nb]);
                n33.push_back(bondl1[nb2]);
            }
        }
    }

    //get the 4 bond (torsion list)
    QVector<int> n41;
    QVector<int> n42;
    QVector<int> n43;
    QVector<int> n44;
    int nn4 = 0;
    for(int na = 0; na < nn3; na++)
    {
        for(int na2 = na + 1; na2 < nn3; na2++)
        {
            if(n32[na] == n31[na2] && n33[na] == n32[na2])
            {
                nn4++;
                n41.push_back(n31[na]);
                n42.push_back(n32[na]);
                n43.push_back(n33[na]);
                n44.push_back(n33[na2]);
            } else if(n31[na] == n32[na2] && n32[na] == n33[na2])
            {
                nn4++;
                n41.push_back(n31[na2]);
                n42.push_back(n31[na]);
                n43.push_back(n32[na]);
                n44.push_back(n33[na]);
            } else if(n32[na] == n33[na2] && n33[na] == n32[na2])
            {
                nn4++;
                n41.push_back(n31[na]);
                n42.push_back(n32[na]);
                n43.push_back(n33[na]);
                n44.push_back(n31[na2]);
            } else if(n31[na] == n32[na2] && n32[na] == n31[na2])
            {
                nn4++;
                n41.push_back(n33[na2]);
                n42.push_back(n32[na2]);
                n43.push_back(n31[na2]);
                n44.push_back(n33[na]);
            } else if(n32[na] == n31[na2] && n31[na] == n32[na2])
            {
                nn4++;
                n41.push_back(n33[na2]);
                n42.push_back(n31[na]);
                n43.push_back(n32[na]);
                n44.push_back(n33[na]);
            } else if(n33[na] == n32[na2] && n32[na] == n33[na2])
            {
                nn4++;
                n41.push_back(n31[na2]);
                n42.push_back(n33[na]);
                n43.push_back(n32[na]);
                n44.push_back(n31[na]);
            } else if(n32[na] == n33[na2] && n31[na] == n32[na2])
            {
                nn4++;
                n41.push_back(n31[na2]);
                n42.push_back(n33[na]);
                n43.push_back(n32[na]);
                n44.push_back(n31[na]);
            } else if(n33[na] == n32[na2] && n32[na] == n31[na2])
            {
                nn4++;
                n41.push_back(n33[na]);
                n42.push_back(n32[na]);
                n43.push_back(n31[na]);
                n44.push_back(n33[na2]);
            }
        }
    }

    //highlight triangles
    if(nfold == 3)
    {
        int ntri = 0;
        QVector<int> tri1;
        QVector<int> tri2;
        QVector<int> tri3;
        for(int nt = 0; nt < nn4; nt++)
        {
            if(n41[nt] == n44[nt])
            {
                ntri++;
                tri1.push_back(n41[nt]);
                tri2.push_back(n42[nt]);
                tri3.push_back(n43[nt]);
            }
        }
        qDebug() << "n tri " << ntri;

        for(int nt = 0; nt < ntri; nt++)
        {
            scene->addItem(new Highlight(atoms[tri1[nt]]->pos(), atoms[tri2[nt]]->pos(),0));
            scene->addItem(new Highlight(atoms[tri2[nt]]->pos(), atoms[tri3[nt]]->pos(),0));
            scene->addItem(new Highlight(atoms[tri3[nt]]->pos(), atoms[tri1[nt]]->pos(),0));
        }
    }

    //highlight squares
    if(nfold == 4)
    {
        int nsqr = 0;
        QVector<int> sqr1;
        QVector<int> sqr2;
        QVector<int> sqr3;
        QVector<int> sqr4;
        for(int ns = 0; ns < nn3; ns++)
        {
            for(int ns2 = ns + 1; ns2 < nn3; ns2++)
            {
                if(n31[ns] == n31[ns2] && n33[ns] == n33[ns2])
                {
                    nsqr++;
                    sqr1.push_back(n31[ns]);
                    sqr2.push_back(n32[ns]);
                    sqr3.push_back(n33[ns]);
                    sqr4.push_back(n32[ns2]);
                } else if(n31[ns] == n33[ns2] && n33[ns] == n31[ns2])
                {
                    nsqr++;
                    sqr1.push_back(n31[ns]);
                    sqr2.push_back(n32[ns]);
                    sqr3.push_back(n33[ns]);
                    sqr4.push_back(n32[ns2]);
                }
            }
        }
        qDebug() << "n sqr " << nsqr;

        for(int ns = 0; ns < nsqr; ns++)
        {
            scene->addItem(new Highlight(atoms[sqr1[ns]]->pos(), atoms[sqr2[ns]]->pos(),1));
            scene->addItem(new Highlight(atoms[sqr2[ns]]->pos(), atoms[sqr3[ns]]->pos(),1));
            scene->addItem(new Highlight(atoms[sqr3[ns]]->pos(), atoms[sqr4[ns]]->pos(),1));
            scene->addItem(new Highlight(atoms[sqr4[ns]]->pos(), atoms[sqr1[ns]]->pos(),1));
        }
    }

    //highlight pentagons
    if(nfold == 5)
    {
        int npen = 0;
        QVector<int> pen1;
        QVector<int> pen2;
        QVector<int> pen3;
        QVector<int> pen4;
        QVector<int> pen5;
        for(int nt = 0; nt < nn4; nt++)
        {
            for(int na = 0; na < nn3; na++)
            {
                if(n41[nt] == n31[na] && n44[nt] == n33[na] && n42[nt] != n32[na] && n43[nt] != n32[na])
                {
                    npen++;
                    pen1.push_back(n41[nt]);
                    pen2.push_back(n42[nt]);
                    pen3.push_back(n43[nt]);
                    pen4.push_back(n44[nt]);
                    pen5.push_back(n32[na]);
                } else if(n44[nt] == n31[na] && n41[nt] == n33[na] && n42[nt] != n32[na] && n43[nt] != n32[na])
                {
                    npen++;
                    pen1.push_back(n41[nt]);
                    pen2.push_back(n42[nt]);
                    pen3.push_back(n43[nt]);
                    pen4.push_back(n44[nt]);
                    pen5.push_back(n32[na]);
                }
            }
        }

        for(int np = 0; np < npen; np++)
        {
            scene->addItem(new Highlight(atoms[pen1[np]]->pos(), atoms[pen2[np]]->pos(),2));
            scene->addItem(new Highlight(atoms[pen2[np]]->pos(), atoms[pen3[np]]->pos(),2));
            scene->addItem(new Highlight(atoms[pen3[np]]->pos(), atoms[pen4[np]]->pos(),2));
            scene->addItem(new Highlight(atoms[pen4[np]]->pos(), atoms[pen5[np]]->pos(),2));
            scene->addItem(new Highlight(atoms[pen5[np]]->pos(), atoms[pen1[np]]->pos(),2));
        }
    }

    scene->update();
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
