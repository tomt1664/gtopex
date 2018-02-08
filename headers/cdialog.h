/****************************************************************************
** Graphene Topology Explorer
**
** Tom Trevethan 2016
** tptrevethan@googlemail.com
****************************************************************************/

#ifndef CDIALOG_H
#define CDIALOG_H

#include <QDialog>
#include <QtWidgets>
#include <QLineEdit>

class LatticeDialog : public QDialog   //create lattice dialog box
{
    Q_OBJECT

public:
    LatticeDialog();
    double bondl() {return bl; }
    int xlatr() { return xlrp; }
    int ylatr() { return ylrp; }
    int center() { return ctr; }

private slots:
    void latticeButtonPress();

private:
    QLineEdit *bondlEdit;
    QLineEdit *xunitsEdit;
    QLineEdit *yunitsEdit;
    QCheckBox *centerCheckBox;
    QPushButton *createlatticeButton;

    double bl;
    int xlrp, ylrp; // unit cell repetition
    int ctr;
};

#endif // CDIALOG_H

