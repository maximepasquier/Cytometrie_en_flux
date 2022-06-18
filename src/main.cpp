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
    std::cout << "File size : " << number_columns_file << " " << number_lines_file << std::endl;
    //* Get tous les marqueurs
    std::string marqueurs[number_columns_file];
    get_marqueurs(marqueurs, file_path);
    for(int i = 0; i < number_columns_file; i++)
    {
        std::cout << marqueurs[i] << std::endl;
    }
    //* Init MatrixXd
    MatrixXd m(number_lines_file, number_columns_file);
    QApplication app(argc, argv);
    read(m);
    std::cout << m << std::endl;
    //* Column selection for plotting in 2D
    MainWindow w(nullptr, marqueurs, m);
    w.show();
    return app.exec();
}
