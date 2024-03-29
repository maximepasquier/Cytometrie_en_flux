#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)

#include <QMainWindow>

//#define QCUSTOMPLOT_USE_OPENGL

#include "qcustomplot.h"
#include <iostream>
#include <QtCharts>
#include <QtCharts/QChartGlobal>
#include "File.hpp"
#include "DataStruct.hpp"

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtCharts/QBarCategoryAxis>
#include <QtWidgets/QApplication>
#include <QtCharts/QValueAxis>

#include <QTimer>

#include <eigen3/Eigen/Dense>
#include <chrono>
#include <thread>
#include <cmath>

using Eigen::MatrixXd;
using Eigen::VectorXd;

using namespace std::this_thread;
using namespace std::chrono;

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

struct Coords
{
    int x, y;
};

struct Point
{
    int x;
    int y;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent);
    ~MainWindow();
    void setup_mainwindow(std::string window_name, MainWindow *window);
    void setup_variables();
    void setup_GUI();
    void setup_graph_spacer();
    void setup_buttons_connections();
    void theme();
    void populate_marqueurs(int nombre_de_marqueurs);
    void gating_ellipse();
    void gating_polygon();
    bool InsidePolygon(Point polygon[], int N, Point p);

public slots:
    void makePlot();
    void refreshPlot(int marqueur_number_1, int marqueur_number_2);
    void plotMouseClickEllipse(QMouseEvent *e);
    void plotMouseClickLine(QMouseEvent *e);
    void mousePressed(QMouseEvent *e);
    void mouseReleased(QMouseEvent *e);
    void plotMouseMoveEllipse(QMouseEvent *e);
    void plotMouseMoveLine(QMouseEvent *e);
    void wheelMoved(QWheelEvent *);
    void wheelStopped();

private slots:
    void updateTheme();
    void replot_graph();
    void on_actionOpen_triggered();
    void on_setOpenGL_stateChanged(int arg1);
    void on_DrawEllipse_clicked();
    void on_DrawPolygon_clicked();
    void on_validateDrawing_clicked();
    void setAdaptativeSampling();
    void connect_adaptive_sampling_on_idle();
    void disconnect_adaptive_sampling_on_idle();

private:
    bool user_is_drawing;
    bool draw_ellipse, draw_line;
    bool adaptative_sampling_on_idle;
    bool mouse_wheel_is_turning;
    VectorXd first_column;
    VectorXd second_column;
    Ui::MainWindow *ui;
    std::string *marqueurs;
    MatrixXd *m_visualisation;
    QSpacerItem *spacer;
    DataStruct *dataSet;
    QCPGraph *curGraph;
    QCustomPlot *customPlot;
    QCPItemEllipse *m_selectionCircle;
    std::vector<QCPItemLine *> m_selectionLine;
    QCPLayer *cursorLayer;
};
#endif // MAINWINDOW_HPP
