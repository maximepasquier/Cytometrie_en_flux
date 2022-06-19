#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QWidget>
#include "mainwindow.h"
#include "qcustomplot.h"
#include "File.hpp"
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
    std::string csv_file_path;

public:
    Application(/* args */);
    ~Application();
    MainWindow *main_window;
};

#endif
