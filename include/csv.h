#ifndef CSV_H
#define CSV_H

#include <eigen3/Eigen/Dense>

using Eigen::MatrixXd;
using Eigen::VectorXd;

std::pair<int,int> get_size(std::string file_path);
void read(MatrixXd& m);

#endif // CSV_H
