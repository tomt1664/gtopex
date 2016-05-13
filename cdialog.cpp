#include <QtWidgets>

#include <QDebug>

#include "cdialog.h"

//create lattice dialog box constructor
LatticeDialog::LatticeDialog()
{
    QGroupBox *latticeGroup = new QGroupBox(tr("Lattice parameters"));

    QLabel *bondlLabel = new QLabel(tr("C-C bond length:"));
    bondlEdit = new QLineEdit;
    bondlEdit->setValidator(new QDoubleValidator(-999.0,999.0, 2, bondlEdit));

    QLabel *xunitsLabel = new QLabel(tr("Unit cell repeat (x):"));
    xunitsEdit = new QLineEdit;
    xunitsEdit->setValidator(new QIntValidator(xunitsEdit));

    QLabel *yunitsLabel = new QLabel(tr("Unit cell repeat (y):"));
    yunitsEdit = new QLineEdit;
    yunitsEdit->setValidator(new QIntValidator(yunitsEdit));

    centerCheckBox = new QCheckBox(tr("Center coodinates"));

    createlatticeButton = new QPushButton(tr("Create lattice"));

    QGridLayout *latticeLayout = new QGridLayout;
    latticeLayout->addWidget(bondlLabel, 0, 0);
    latticeLayout->addWidget(bondlEdit, 0, 1);
    latticeLayout->addWidget(xunitsLabel, 1, 0);
    latticeLayout->addWidget(xunitsEdit, 1, 1);
    latticeLayout->addWidget(yunitsLabel, 2, 0);
    latticeLayout->addWidget(yunitsEdit, 2, 1);
    latticeLayout->addWidget(centerCheckBox, 3, 0);
    latticeGroup->setLayout(latticeLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(latticeGroup);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(createlatticeButton);
    mainLayout->addStretch(1);
    setLayout(mainLayout);

    connect(createlatticeButton, SIGNAL(clicked()), this, SLOT(latticeButtonPress()));

    setWindowTitle(tr("Create Lattice"));
}

void LatticeDialog::latticeButtonPress()
{
    bl = bondlEdit->text().toDouble();
    xlrp = xunitsEdit->text().toInt();
    ylrp = yunitsEdit->text().toInt();
    ctr = centerCheckBox->isChecked();

    close();
}

