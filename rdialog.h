/****************************************************************************
** Graphene Topology Explorer
**
** Tom Trevethan 2016
** tptrevethan@googlemail.com
****************************************************************************/

#ifndef RDIALOG_H
#define RDIALOG_H

#include <QDialog>
#include <QtWidgets>
#include <QDial>

class RotateDialog : public QDialog   //create lattice dialog box
{
    Q_OBJECT

public:
    RotateDialog();
    double angle() {return m_ang; }

signals:
    void valueChanged(int value);

private slots:
    void okButtonPress();

private:
    QDial *angleDial;
    QSpinBox *angleSpinBox;
    QPushButton *okButton;

    double m_ang;
};

#endif // RDIALOG_H

