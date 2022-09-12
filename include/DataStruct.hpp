#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <eigen3/Eigen/Dense>
#include "File.hpp"
#include <vector>

using Eigen::MatrixXd;
using Eigen::VectorXd;

class DataStruct : MatrixXd
{
private:
    int number_of_lines, number_of_columns;
    std::string *marqueurs;
    MatrixXd *data_matrix;
    bool *gated_data_array;

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    DataStruct(File file);
    ~DataStruct();
    std::string *get_marqueurs();
    MatrixXd *get_data_matrix();
    bool *get_gated_data_array();
    int get_number_of_lines();
    int get_number_of_columns();
    void init_gated_data_array();
};

#endif