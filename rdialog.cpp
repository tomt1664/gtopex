#include <QDial>
#include <QtWidgets>
#include <QDebug>

#include "rdialog.h"

//rotate dialog box constructor
RotateDialog::RotateDialog()
{

    angleDial = new QDial;
    angleDial->setFocusPolicy(Qt::StrongFocus);

    angleDial->setWrapping(true);

    angleDial->setMinimum(-180);
    angleDial->setMaximum(180);

    angleSpinBox = new QSpinBox;
    angleSpinBox->setRange(-180, 180);
    angleSpinBox->setSingleStep(1);

    okButton = new QPushButton(tr("OK"));

    connect(angleDial, SIGNAL(valueChanged(int)), angleSpinBox, SLOT(setValue(int)));
    connect(angleSpinBox, SIGNAL(valueChanged(int)),angleDial, SLOT(setValue(int)));
    connect(angleDial, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged(int)));
    connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonPress()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(angleDial);
    mainLayout->addWidget(angleSpinBox);
    mainLayout->addWidget(okButton);
    setLayout(mainLayout);

    angleSpinBox->setValue(0);

    setWindowTitle(tr("Rotate"));
}

void RotateDialog::okButtonPress()
{
    m_ang = angleSpinBox->text().toDouble();
    close();
}
