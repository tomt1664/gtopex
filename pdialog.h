/****************************************************************************
** Graphene Topology Explorer
**
** Tom Trevethan 2016
** tptrevethan@googlemail.com
****************************************************************************/

#ifndef PDIALOG_H
#define PDIALOG_H

#include <QDialog>
#include <QtWidgets>

class PolyDialog : public QDialog   //find and highlight n-fold rings dialog
{
    Q_OBJECT

public:
    PolyDialog();

signals:
    void valueChanged(int);

public slots:
    void threefold() { emit valueChanged(3); }
    void fourfold() { emit valueChanged(4); }
    void fivefold() { emit valueChanged(5); }

private slots:
    void okButtonPress();

private:
    QPushButton *threefoldButton;
    QPushButton *fourfoldButton;
    QPushButton *fivefoldButton;
    QPushButton *okButton;
};

#endif // PDIALOG_H

