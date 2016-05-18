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
    int scomp() { return m_component; }

private slots:
    void scolorButtonPress();
    void cancelButtonPress();

private:
    QComboBox *sselectComboBox;
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
    int m_component; //the selected component of the strain tensor
};

#endif // TDIALOG_H

