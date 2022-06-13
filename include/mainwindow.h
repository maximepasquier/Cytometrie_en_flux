#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <iostream>

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
    MainWindow(QWidget *parent, std::string* marqueurs, MatrixXd m);
    ~MainWindow();

private slots:
    void makePlot();

private:
    Ui::MainWindow *ui;
    std::string* marqueurs;
    MatrixXd m_visualisation;
};
#endif // MAINWINDOW_H
