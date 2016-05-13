#include <QtWidgets>

#include <QDebug>

#include "sdialog.h"

SettingsDialog::SettingsDialog(QVector<float>& pot, QVector<float>& apot, float step, QVector<float>& bounds, int nstep)
{
    m_pot = pot;
    m_apot = apot;
    m_bounds = bounds;

    QGroupBox *morseGroup = new QGroupBox(tr("Morse potential parameters"));

    QLabel *morseDLabel = new QLabel(tr("D:"));
    morseDEdit = new QLineEdit;
    morseDEdit->setText(QString::number(pot[0]));
    morseDEdit->setValidator(new QDoubleValidator(0.0,999.0, 2, morseDEdit));

    QLabel *morseaLabel = new QLabel(tr("a:"));
    morseaEdit = new QLineEdit;
    morseaEdit->setText(QString::number(pot[1]));
    morseaEdit->setValidator(new QDoubleValidator(0.0,999.0, 2, morseaEdit));

    QLabel *morserLabel = new QLabel(tr("r:"));
    morserEdit = new QLineEdit;
    morserEdit->setText(QString::number(pot[2]));
    morserEdit->setValidator(new QDoubleValidator(0.0,999.0, 2, morserEdit));

    QGroupBox *angleGroup = new QGroupBox(tr("Angle potential parameters"));

    QLabel *angleVLabel = new QLabel(tr("V:"));
    angleVEdit = new QLineEdit;
    angleVEdit->setText(QString::number(apot[1]));
    angleVEdit->setValidator(new QDoubleValidator(0.0,999.0, 2, angleVEdit));

    QLabel *angletLabel = new QLabel(tr("theta:"));
    angletEdit = new QLineEdit;
    angletEdit->setText(QString::number(apot[0]));
    angletEdit->setValidator(new QDoubleValidator(0.0,999.0, 2, angletEdit));

    QGroupBox *optGroup = new QGroupBox(tr("Optimisation parameters"));

    QLabel *stepLabel = new QLabel(tr("Step:"));
    stepEdit = new QLineEdit;
    stepEdit->setText(QString::number(step));
    stepEdit->setValidator(new QDoubleValidator(0.0,999.0, 2, stepEdit));

    QLabel *nstepLabel = new QLabel(tr("Max steps:"));
    nstepEdit = new QLineEdit;
    nstepEdit->setText(QString::number(nstep));
    nstepEdit->setValidator(new QIntValidator(nstepEdit));

    QLabel *cutoffLabel = new QLabel(tr("Bond cutoff:"));
    cutoffEdit = new QLineEdit;
    cutoffEdit->setText(QString::number(pot[3]));
    cutoffEdit->setValidator(new QDoubleValidator(0.0,999.0, 2, cutoffEdit));

    QGroupBox *boundsGroup = new QGroupBox(tr("Periodic cell dimensions"));

    QLabel *xminLabel = new QLabel(tr("x min:"));
    xminEdit = new QLineEdit;
    xminEdit->setText(QString::number(bounds[0]*0.01));
    xminEdit->setValidator(new QDoubleValidator(0.0,999.0, 2, xminEdit));

    QLabel *xmaxLabel = new QLabel(tr("x max:"));
    xmaxEdit = new QLineEdit;
    xmaxEdit->setText(QString::number(bounds[1]*0.01));
    xmaxEdit->setValidator(new QDoubleValidator(0.0,999.0, 2, xmaxEdit));

    QLabel *yminLabel = new QLabel(tr("y min:"));
    yminEdit = new QLineEdit;
    yminEdit->setText(QString::number(bounds[2]*0.01));
    yminEdit->setValidator(new QDoubleValidator(0.0,999.0, 2, yminEdit));

    QLabel *ymaxLabel = new QLabel(tr("y max:"));
    ymaxEdit = new QLineEdit;
    ymaxEdit->setText(QString::number(bounds[3]*0.01));
    ymaxEdit->setValidator(new QDoubleValidator(0.0,999.0, 2, ymaxEdit));

    okButton = new QPushButton(tr("OK"));
    cancelButton = new QPushButton(tr("Cancel"));

    QGridLayout *morseLayout = new QGridLayout;
    morseLayout->addWidget(morseDLabel, 0, 0);
    morseLayout->addWidget(morseDEdit, 0, 1);
    morseLayout->addWidget(morseaLabel, 1, 0);
    morseLayout->addWidget(morseaEdit, 1, 1);
    morseLayout->addWidget(morserLabel, 2, 0);
    morseLayout->addWidget(morserEdit, 2, 1);
    morseGroup->setLayout(morseLayout);

    QGridLayout *angleLayout = new QGridLayout;
    angleLayout->addWidget(angleVLabel, 0, 0);
    angleLayout->addWidget(angleVEdit, 0, 1);
    angleLayout->addWidget(angletLabel, 1, 0);
    angleLayout->addWidget(angletEdit, 1, 1);
    angleGroup->setLayout(angleLayout);

    QGridLayout *optLayout = new QGridLayout;
    optLayout->addWidget(stepLabel, 0, 0);
    optLayout->addWidget(stepEdit, 0, 1);
    optLayout->addWidget(nstepLabel, 1, 0);
    optLayout->addWidget(nstepEdit, 1, 1);
    optLayout->addWidget(cutoffLabel, 2, 0);
    optLayout->addWidget(cutoffEdit, 2, 1);
    optGroup->setLayout(optLayout);

    QGridLayout *boundsLayout = new QGridLayout;
    boundsLayout->addWidget(xminLabel, 0, 0);
    boundsLayout->addWidget(xminEdit, 0, 1);
    boundsLayout->addWidget(xmaxLabel, 1, 0);
    boundsLayout->addWidget(xmaxEdit, 1, 1);
    boundsLayout->addWidget(yminLabel, 2, 0);
    boundsLayout->addWidget(yminEdit, 2, 1);
    boundsLayout->addWidget(ymaxLabel, 3, 0);
    boundsLayout->addWidget(ymaxEdit, 3, 1);
    boundsGroup->setLayout(boundsLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(morseGroup);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(angleGroup);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(optGroup);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(boundsGroup);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(okButton);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(cancelButton);
    //    mainLayout->addStretch(1);
    setLayout(mainLayout);

    connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonPress()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonPress()));

    setWindowTitle(tr("System settings"));
}

void SettingsDialog::okButtonPress()
{
    m_pot[0] = morseDEdit->text().toDouble();
    m_pot[1] = morseaEdit->text().toDouble();
    m_pot[2] = morserEdit->text().toDouble();
    m_pot[3] = cutoffEdit->text().toDouble();

    m_apot[0] = angletEdit->text().toDouble();
    m_apot[1] = angleVEdit->text().toDouble();

    m_step = stepEdit->text().toDouble();
    m_nstep = nstepEdit->text().toInt();

    m_bounds[0] = xminEdit->text().toDouble();
    m_bounds[1] = xmaxEdit->text().toDouble();
    m_bounds[2] = yminEdit->text().toDouble();
    m_bounds[3] = ymaxEdit->text().toDouble();

    close();
}

void SettingsDialog::cancelButtonPress()
{
    close();
}

void SettingsDialog::getpots(QVector<float> &pot)
{
    pot = m_pot;
}

void SettingsDialog::getapots(QVector<float> &apot)
{
    apot = m_apot;
}

void SettingsDialog::getbounds(QVector<float> &bounds)
{
    bounds = m_bounds;
}
