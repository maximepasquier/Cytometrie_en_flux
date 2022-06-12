#include "mainwindow.h"
#include "csv.h"
#include "qcustomplot.h"
#include <iostream>
#include <iostream>
#include <eigen3/Eigen/Dense>

using Eigen::MatrixXd;
using Eigen::VectorXd;

#include <QApplication>

int main(int argc, char *argv[])
{
    //* File name
    std::string file_path = ".././testCSV.csv";
    //* Get file size (for eigen matrix)
    int number_lines_file, number_columns_file;
    std::pair<int, int> pair_file_size;
    pair_file_size = get_size(file_path);
    number_lines_file = pair_file_size.first;
    number_columns_file = pair_file_size.second;
    std::cout << number_columns_file << " " << number_lines_file << std::endl;
    //* Init MatrixXd
    MatrixXd m(number_lines_file, number_columns_file);
    QApplication app(argc, argv);
    read(m);
    std::cout << m << std::endl;
    //* Column selection for plotting in 2D
    int first_column = 3;
    int second_column = 5;
    MainWindow w(nullptr, m.col(first_column),m.col(second_column));
    w.show();
    return app.exec();
}
