/****************************************************************************
** Graphene Topology Explorer
**
** Tom Trevethan 2016
** tptrevethan@googlemail.com
****************************************************************************/

#include <QtWidgets>
#include <QDebug>

#include "pdialog.h"

//rotate dialog box constructor
PolyDialog::PolyDialog()
{
    threefoldButton = new QPushButton;
    threefoldButton->setIcon(QIcon(QPixmap(":/icons/p3_48.png")));
    threefoldButton->setIconSize(QSize(48,48));

    fourfoldButton = new QPushButton;
    fourfoldButton->setIcon(QIcon(QPixmap(":/icons/p4_48.png")));
    fourfoldButton->setIconSize(QSize(48,48));

    fivefoldButton = new QPushButton;
    fivefoldButton->setIcon(QIcon(QPixmap(":/icons/p5_48.png")));
    fivefoldButton->setIconSize(QSize(48,48));

    okButton = new QPushButton(tr("OK"));

    connect(threefoldButton, SIGNAL(clicked()), this, SLOT(threefold()));
    connect(fourfoldButton, SIGNAL(clicked()), this, SLOT(fourfold()));
    connect(fivefoldButton, SIGNAL(clicked()), this, SLOT(fivefold()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonPress()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(threefoldButton);
    mainLayout->addWidget(fourfoldButton);
    mainLayout->addWidget(fivefoldButton);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(okButton);
    setLayout(mainLayout);

    setWindowTitle(tr("Rings"));
}

void PolyDialog::okButtonPress()
{
    close();
}

