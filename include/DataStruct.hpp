#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <eigen3/Eigen/Dense>
#include "File.hpp"

using Eigen::MatrixXd;
using Eigen::VectorXd;

class DataStruct : MatrixXd
{
private:
    int matrix_columns_number, matrix_lines_number;
    std::string* marqueurs;
    MatrixXd* data_matrix;
public:
    DataStruct(File csv_file);
    ~DataStruct();
    int get_matrix_columns_number();
    int get_matrix_lines_number();  
    MatrixXd* get_matrix();
    std::string* get_marqueurs();
    void set_matrix_columns_number(int nb_columns);
    void set_matrix_lines_number(int nb_lines);
    void set_marqueurs(std::string* data_marqueurs);
    void set_matrix(MatrixXd* data_m);
};

#endif