#ifndef FILE_HPP
#define FILE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <eigen3/Eigen/Dense>

using Eigen::MatrixXd;

class File
{
private:
    std::string file_path;
    int number_of_lines, number_of_columns;
    std::string* marqueurs;
    MatrixXd *data_matrix;
public:
    File(/* args */);
    ~File();
    void read_file_size();
    void read_marqueurs();
    void read_matrix();
    std::string* get_marqueurs();
    MatrixXd* get_data_matrix();
};

#endif
