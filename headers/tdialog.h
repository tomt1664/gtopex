/****************************************************************************
** Graphene Topology Explorer
**
** Tom Trevethan 2016
** tptrevethan@googlemail.com
****************************************************************************/

#ifndef TDIALOG_H
#define TDIALOG_H

#include <QDialog>
#include <QtWidgets>
#include <QLineEdit>

class StrainColorDialog : public QDialog   //atomistic tensor color coding dialog box
{
    Q_OBJECT

public:
    StrainColorDialog();
    double maxval() {return m_mxv; }
    double minval() {return m_mnv; }
    int cscale() { return m_cscale; }

private slots:
    void scolorButtonPress();
    void cancelButtonPress();

private:
    QRadioButton *greyscaleRadioButton;
    QRadioButton *hotscaleRadioButton;
    QRadioButton *rainbowRadioButton;
    QLineEdit *minvalEdit;
    QLineEdit *maxvalEdit;
    QCheckBox *reverseCheckBox;
    QPushButton *scolorButton;
    QPushButton *cancelButton;

    double m_mnv, m_mxv; //minimum and maximum scale value
    int m_cscale; // color scale: 1 = greyscale, 2 = hot, 3 = rainbow, 4 = jet
};

#endif // TDIALOG_H

