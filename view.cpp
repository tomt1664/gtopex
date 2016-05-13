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

#include "view.h"

#ifndef QT_NO_PRINTER
#include <QPrinter>
#include <QPrintDialog>
#endif
#include <QMessageBox>
#include <QFileDialog>
#include <QtWidgets>
#include <qmath.h>

#ifndef QT_NO_WHEELEVENT
/*
void GraphicsView::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() & Qt::ControlModifier) {
        if (e->delta() > 0)
            view->zoomIn(6);
        else
            view->zoomOut(6);
        e->accept();
    } else {
        QGraphicsView::wheelEvent(e);
    }
}
*/
#endif

View::View(QWidget *parent)
    : QFrame(parent)
{
    setFrameStyle(Sunken | StyledPanel);
    graphicsView = new GraphicsView(this);
    graphicsView->setRenderHint(QPainter::Antialiasing, false);
    graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    graphicsView->setRenderHint(QPainter::Antialiasing);

    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);

    QToolButton *zoomInIcon = new QToolButton;
    zoomInIcon->setAutoRepeat(true);
    zoomInIcon->setAutoRepeatInterval(33);
    zoomInIcon->setAutoRepeatDelay(0);
    zoomInIcon->setIcon(QPixmap(":/plus_32.png"));
    zoomInIcon->setIconSize(iconSize);
    QToolButton *zoomOutIcon = new QToolButton;
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(33);
    zoomOutIcon->setAutoRepeatDelay(0);
    zoomOutIcon->setIcon(QPixmap(":/minus_32.png"));
    zoomOutIcon->setIconSize(iconSize);
    zoomSlider = new QSlider;
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(500);
    zoomSlider->setValue(250);
//    zoomSlider->setTickPosition(QSlider::TicksRight);

    // Zoom slider layout
    QVBoxLayout *zoomSliderLayout = new QVBoxLayout;
    zoomSliderLayout->addWidget(zoomInIcon);
    zoomSliderLayout->addWidget(zoomSlider);
    zoomSliderLayout->addWidget(zoomOutIcon);

    QToolButton *rotateLeftIcon = new QToolButton;
    rotateLeftIcon->setIcon(QPixmap(":/rotl_32.png"));
    rotateLeftIcon->setIconSize(iconSize);
    QToolButton *rotateRightIcon = new QToolButton;
    rotateRightIcon->setIcon(QPixmap(":/rotr_32.png"));
    rotateRightIcon->setIconSize(iconSize);
    rotateSlider = new QSlider;
    rotateSlider->setOrientation(Qt::Horizontal);
    rotateSlider->setMinimum(-360);
    rotateSlider->setMaximum(360);
    rotateSlider->setValue(0);
//    rotateSlider->setTickPosition(QSlider::TicksBelow);

    // Rotate slider layout
    QHBoxLayout *rotateSliderLayout = new QHBoxLayout;
    rotateSliderLayout->addWidget(rotateLeftIcon);
    rotateSliderLayout->addWidget(rotateSlider);
    rotateSliderLayout->addWidget(rotateRightIcon);

    resetButton = new QToolButton;
    resetButton->setIcon(QIcon(QPixmap(":/reset_32.png")));
    resetButton->setIconSize(iconSize);
    resetButton->setEnabled(false);

    // Label layout
    QHBoxLayout *labelLayout = new QHBoxLayout;
    selectModeButton = new QToolButton;
    selectModeButton->setIcon(QIcon(QPixmap(":/point_32.png")));
    selectModeButton->setIconSize(iconSize);
    selectModeButton->setCheckable(true);
    selectModeButton->setChecked(true);

    dragModeButton = new QToolButton;
    dragModeButton->setIcon(QIcon(QPixmap(":/move_32.png")));
    dragModeButton->setIconSize(iconSize);
    dragModeButton->setCheckable(true);
    dragModeButton->setChecked(false);

    bondrotButton = new QToolButton;
    bondrotButton->setIcon(QIcon(QPixmap(":/bondrot_32.png")));
    bondrotButton->setIconSize(iconSize);
    bondrotButton->setToolTip("Rotate Selection");

    deleteButton = new QToolButton;
    deleteButton->setIcon(QIcon(QPixmap(":/delete_32.png")));
    deleteButton->setIconSize(iconSize);
    deleteButton->setToolTip("Rotate Selection");

    printButton = new QToolButton;
    printButton->setIcon(QIcon(QPixmap(":/picture_32.png")));
    printButton->setIconSize(iconSize);
    printButton->setToolTip("Export Image");

    openButton = new QToolButton;
    openButton->setIcon(QIcon(QPixmap(":/open_32.png")));
    openButton->setIconSize(iconSize);
    openButton->setToolTip("Open file");

    saveButton = new QToolButton;
    saveButton->setIcon(QIcon(QPixmap(":/save_32.png")));
    saveButton->setIconSize(iconSize);
    saveButton->setToolTip("Save file");

    infoButton = new QToolButton;
    infoButton->setIcon(QIcon(QPixmap(":/info_32.png")));
    infoButton->setIconSize(iconSize);
    infoButton->setToolTip("Info");

    QButtonGroup *pointerModeGroup = new QButtonGroup;
    pointerModeGroup->setExclusive(true);
    pointerModeGroup->addButton(selectModeButton);
    pointerModeGroup->addButton(dragModeButton);

    labelLayout->addWidget(openButton);
    labelLayout->addWidget(saveButton);
    labelLayout->addStretch();
    labelLayout->addWidget(selectModeButton);
    labelLayout->addWidget(dragModeButton);
    labelLayout->addWidget(bondrotButton);
    labelLayout->addWidget(deleteButton);
    labelLayout->addStretch();
    labelLayout->addWidget(infoButton);
    labelLayout->addWidget(printButton);

    QGridLayout *topLayout = new QGridLayout;
    topLayout->addLayout(labelLayout, 0, 0);
    topLayout->addWidget(graphicsView, 1, 0);
    topLayout->addLayout(zoomSliderLayout, 1, 1);
    topLayout->addLayout(rotateSliderLayout, 2, 0);
    topLayout->addWidget(resetButton, 2, 1);
    setLayout(topLayout);

    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetView()));
    connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
    connect(rotateSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
    connect(graphicsView->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(setResetButtonEnabled()));
    connect(graphicsView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(setResetButtonEnabled()));
    connect(selectModeButton, SIGNAL(toggled(bool)), this, SLOT(togglePointerMode()));
    connect(dragModeButton, SIGNAL(toggled(bool)), this, SLOT(togglePointerMode()));
    connect(bondrotButton, SIGNAL(toggled(bool)), this, SLOT(bondrotate()));
    connect(rotateLeftIcon, SIGNAL(clicked()), this, SLOT(rotateLeft()));
    connect(rotateRightIcon, SIGNAL(clicked()), this, SLOT(rotateRight()));
    connect(zoomInIcon, SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect(zoomOutIcon, SIGNAL(clicked()), this, SLOT(zoomOut()));
    connect(printButton, SIGNAL(clicked()), this, SLOT(print()));
    connect(infoButton, SIGNAL(clicked()), this, SLOT(slotAbout()));
    connect(openButton, SIGNAL(clicked()), this, SLOT(openfile()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(savefile()));


    setupMatrix();
}

QGraphicsView *View::view() const
{
    return static_cast<QGraphicsView *>(graphicsView);
}

void View::resetView()
{
    zoomSlider->setValue(250);
    rotateSlider->setValue(0);
    setupMatrix();
    graphicsView->ensureVisible(QRectF(0, 0, 0, 0));

    resetButton->setEnabled(false);
}

void View::setResetButtonEnabled()
{
    resetButton->setEnabled(true);
}

void View::setupMatrix()
{
    qreal scale = qPow(qreal(2), (zoomSlider->value() - 250) / qreal(50));

    QMatrix matrix;
    matrix.scale(scale, scale);
    matrix.rotate(rotateSlider->value());

    graphicsView->setMatrix(matrix);
    setResetButtonEnabled();
}

void View::togglePointerMode()
{
    graphicsView->setDragMode(selectModeButton->isChecked()
                              ? QGraphicsView::RubberBandDrag
                              : QGraphicsView::ScrollHandDrag);
    graphicsView->setInteractive(selectModeButton->isChecked());
}

void View::bondrotate()
{

}

void View::deleteatom()
{
    graphicsView->
}

void View::slotAbout()
{
    QMessageBox::about(this, tr("About "),
            tr("<p><b>VizMol version 0.4</b></p>"
               "<p>Build date: %1"
               "<br>This program uses Qt 4.7"
               "<br>Abraham Max Santos Ramos"
               "<br>Email: <a href=\"mailto:amsantosr@gmail.com>\">amsantosr@gmail.com</a>"
               "<br>2011"
               "</p>").arg(__TIMESTAMP__));
}

void View::openfile()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open XYZ File",
                                                    QString(),
                                                    "XYZ Files (*.xyz)");


/*    if (!filename.isNull()) {
        std::string fn = filename.toStdString();
        try {
            molecule.load(fn.c_str());
        } catch (std::exception &ex) {
            QMessageBox::information(this, "Error",
                                     QString::fromStdString(ex.what()),
                                     QMessageBox::Ok);
        }
        glMolecule->setMolecule(&molecule);
        setWindowTitle(tr("%1 - VizMol").arg(QFileInfo(filename).fileName()));
    }
*/
}

void View::savefile()
{
}

void View::print()
{
#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        graphicsView->render(&painter);
    }
#endif
}

void View::zoomIn(int level)
{
    zoomSlider->setValue(zoomSlider->value() + level);
}

void View::zoomOut(int level)
{
    zoomSlider->setValue(zoomSlider->value() - level);
}

void View::rotateLeft()
{
    rotateSlider->setValue(rotateSlider->value() - 10);
}

void View::rotateRight()
{
    rotateSlider->setValue(rotateSlider->value() + 10);
}
