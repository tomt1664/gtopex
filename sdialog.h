/****************************************************************************
** Graphene Topology Explorer
**
** Tom Trevethan 2016
** tptrevethan@googlemail.com
****************************************************************************/

#ifndef SDIALOG_H
#define SDIALOG_H

#include <QDialog>
#include <QtWidgets>
#include <QLineEdit>

class SettingsDialog : public QDialog   //relaxation parameters dialog box
{
    Q_OBJECT

public:
    SettingsDialog(QVector<float>& pot, QVector<float>& apot, float step, QVector<float>& bounds, int nstep);
    void getpots(QVector<float>& pot);
    void getapots(QVector<float>& apot);
    void getbounds(QVector<float>& bounds);
    float getstep() { return m_step; }
    int getnstep() { return m_nstep; }

private slots:
    void okButtonPress();
    void cancelButtonPress();

private:
    QLineEdit *morseDEdit;
    QLineEdit *morseaEdit;
    QLineEdit *morserEdit;
    QLineEdit *angleVEdit;
    QLineEdit *angletEdit;
    QLineEdit *xminEdit;
    QLineEdit *xmaxEdit;
    QLineEdit *yminEdit;
    QLineEdit *ymaxEdit;
    QLineEdit *stepEdit;
    QLineEdit *nstepEdit;
    QLineEdit *cutoffEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;

    QVector<float> m_pot;
    QVector<float> m_apot;
    QVector<float> m_bounds;
    float m_step;
    int m_nstep;
};

#endif // SDIALOG_H

