#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <eigen3/Eigen/Dense>
#include "File.hpp"

using Eigen::MatrixXd;
using Eigen::VectorXd;

class DataStruct : MatrixXd
{
private:
    int number_of_lines, number_of_columns;
    std::string *marqueurs;
    MatrixXd *data_matrix;

public:
    DataStruct(File file);
    ~DataStruct();
    std::string *get_marqueurs();
    MatrixXd *get_data_matrix();
    int get_number_of_lines();
    int get_number_of_columns();
};

#endif