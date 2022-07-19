#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

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

#include <eigen3/Eigen/Dense>

using Eigen::MatrixXd;
using Eigen::VectorXd;

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent);
    ~MainWindow();
    void theme();
    void populate_marqueurs(int nombre_de_marqueurs);
    void connect_signals_to_slots();
    void setupSpacer();

public slots:
    void makePlot(int marqueur_number_1, int marqueur_number_2);

private slots:
    void updateTheme();
    void replot();

    void on_actionOpen_triggered();

    void on_setAdaptativeSampling_stateChanged(int arg1);

    void on_setOpenGL_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    std::string *marqueurs;
    MatrixXd *m_visualisation;
    QSpacerItem *spacer;
    DataStruct *dataSet;
    QCustomPlot *customPlot;
};
#endif // MAINWINDOW_HPP
