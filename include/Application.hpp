#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QWidget>
#include "mainwindow.h"
#include "csv.h"
#include "qcustomplot.h"
#include <iostream>
#include <iostream>
#include <eigen3/Eigen/Dense>

using Eigen::MatrixXd;
using Eigen::VectorXd;

#include <QApplication>

class Application : public QWidget
{
    Q_OBJECT
private:
    /* data */
public:
    Application(/* args */);
    ~Application();
    MainWindow* main_window;
};

#endif
