/****************************************************************************
** Graphene Topology Explorer
**
** Tom Trevethan 2016
** tptrevethan@googlemail.com
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QVector>
#include <QFrame>
#include <QGraphicsView>

class Atom;

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QLabel;
class QSlider;
class QToolButton;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

    void itemMoved();

public slots:
    void zoomIn(int level = 1);
    void zoomOut(int level = 1);

private slots:
    void resetView();
    void setResetButtonEnabled();
    void setupMatrix();
    void togglePMode();
    void bondrotate();
    void rotatevalue(int ang);
    void deleteatom();
    void openfile();
    void savefile();
    void slotAbout();
    void print();
    void rotateLeft();
    void rotateRight();
    void createConfig();
    void runSystem();
    void settings();
    void dostrain();
    void drawcharts();
    void setcolor();
    void findrings();
    void plotrings(int nfold);

private:
    void populateScene();

    QVector<float> xatmpos; //atom positions
    QVector<float> yatmpos;
    QList<Atom *> atoms; //list of all current atom objects
    QGraphicsScene *scene;

    QVector<float> bounds;
    int timerId;
    int nitems; //number of atoms in scene
    QVector<float> pot; //morse potential parameters
    QVector<float> apot; //angle potential parameters
    float step; //steepest descent step size
    int nstep; //max number of optimisation steps
    float xcom,ycom; //center of mass of group
    int rcounter;

    QGraphicsView *gView;
    QLabel *label;
    QLabel *label2;
    QToolButton *selectMButton;
    QToolButton *dragMButton;
    QToolButton *bondrotButton;
    QToolButton *deleteButton;
    QToolButton *printButton;
    QToolButton *resetButton;
    QToolButton *infoButton;
    QToolButton *openButton;
    QToolButton *saveButton;
    QToolButton *createButton;
    QToolButton *strainButton;
    QToolButton *runButton;
    QToolButton *paramButton;
    QToolButton *chartButton;
    QToolButton *colorButton;
    QToolButton *ringsButton;
    QSlider *zoomSlider;
    QSlider *rotateSlider;
};

#endif // MAINWINDOW_H
