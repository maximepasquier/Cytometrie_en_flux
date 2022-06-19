#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <iostream>
#include "customtoolbutton.h"
#include <QtCharts>
#include <QtCharts/QChartGlobal>

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
    MainWindow(QWidget *parent, std::string *marqueurs, MatrixXd *data_matrix);
    ~MainWindow();

public slots:
    void makePlot(int marqueur_number_1, int marqueur_number_2);

private slots:
    void updateUI();

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_actionOpen_triggered();

private:
    Ui::MainWindow *ui;
    std::string *marqueurs;
    MatrixXd *m_visualisation;
};
#endif // MAINWINDOW_H
