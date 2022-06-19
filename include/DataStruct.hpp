#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <eigen3/Eigen/Dense>

using Eigen::MatrixXd;
using Eigen::VectorXd;

class DataStruct
{
private:
    MatrixXd data_matrix;
public:
    DataStruct(/* args */);
    ~DataStruct();
};

#endif