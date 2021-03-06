/****************************************************************************
** Graphene Topology Explorer
**
** Tom Trevethan 2016
** tptrevethan@googlemail.com
****************************************************************************/

#include <QtWidgets>

#include <QDebug>

#include "tdialog.h"

//atomistic strain color coding dialog box constructor
StrainColorDialog::StrainColorDialog()
{
    m_cscale = 0;

    QGroupBox *cscaleGroup = new QGroupBox(tr("Color Gradients"));
    QPixmap gsimage(":/icons/grayscale.png");
    QLabel *gsimageLabel = new QLabel(tr("grayscale"));
    gsimageLabel->setPixmap(gsimage);
    QPixmap hsimage(":/icons/hotscale.png");
    QLabel *hsimageLabel = new QLabel(tr("hotscale"));
    hsimageLabel->setPixmap(hsimage);
    QPixmap rsimage(":/icons/rainbowscale.png");
    QLabel *rsimageLabel = new QLabel(tr("rainbow"));
    rsimageLabel->setPixmap(rsimage);

    greyscaleRadioButton = new QRadioButton;
    hotscaleRadioButton = new QRadioButton;
    rainbowRadioButton = new QRadioButton;

    greyscaleRadioButton->setChecked(true);

    QGroupBox *valuesGroup = new QGroupBox;
    QLabel *minvalLabel = new QLabel(tr("Min %"));
    minvalEdit = new QLineEdit;
    minvalEdit->setText(QString::number(-1));
    minvalEdit->setValidator(new QDoubleValidator(-99.0,99.0, 2, minvalEdit));
    QLabel *maxvalLabel = new QLabel(tr("Max %"));
    maxvalEdit = new QLineEdit;
    maxvalEdit->setText(QString::number(1));
    maxvalEdit->setValidator(new QDoubleValidator(-99.0,99.0, 2, maxvalEdit));

    QGroupBox *buttonGroup = new QGroupBox;
    scolorButton = new QPushButton(tr("Apply"));
    cancelButton = new QPushButton(tr("Cancel"));

    QGridLayout *scaleLayout = new QGridLayout;
    scaleLayout->addWidget(greyscaleRadioButton, 0, 0, 1, 1);
    scaleLayout->addWidget(gsimageLabel, 0, 1, 1, 3);
    scaleLayout->addWidget(hotscaleRadioButton, 1, 0, 1, 1);
    scaleLayout->addWidget(hsimageLabel, 1, 1, 1, 3);
    scaleLayout->addWidget(rainbowRadioButton, 2, 0, 1, 1);
    scaleLayout->addWidget(rsimageLabel, 2, 1, 1, 3);
    cscaleGroup->setLayout(scaleLayout);

    QGridLayout *valuesLayout = new QGridLayout;
    valuesLayout->addWidget(minvalLabel, 0, 0);
    valuesLayout->addWidget(minvalEdit, 1, 0);
    valuesLayout->addWidget(maxvalLabel, 0, 1);
    valuesLayout->addWidget(maxvalEdit, 1, 1);
    valuesGroup->setLayout(valuesLayout);

    QGridLayout *buttonLayout = new QGridLayout;
    buttonLayout->addWidget(scolorButton,0,0);
    buttonLayout->addWidget(cancelButton,0,1);
    buttonGroup->setLayout(buttonLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(cscaleGroup);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(valuesGroup);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(buttonGroup);
    setLayout(mainLayout);

    connect(scolorButton, SIGNAL(clicked()), this, SLOT(scolorButtonPress()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonPress()));

    setWindowTitle(tr("Atomistic strain mapping"));
}

void StrainColorDialog::scolorButtonPress()
{
    if(greyscaleRadioButton->isChecked())
    {
        m_cscale = 1;
    } else if(hotscaleRadioButton->isChecked())
    {
        m_cscale = 2;
    } else if(rainbowRadioButton->isChecked())
    {
        m_cscale = 3;
    }

    m_mnv = 0.01*minvalEdit->text().toDouble();
    m_mxv = 0.01*maxvalEdit->text().toDouble();

    close();
}

void StrainColorDialog::cancelButtonPress()
{
    m_cscale = 0;
    close();
}
