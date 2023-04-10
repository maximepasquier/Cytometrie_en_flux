#ifndef VISUALDATA_HPP
#define VISUALDATA_HPP

#include "DataStruct.hpp"

class VisualData
{
private:
    int matrix_columns_number, matrix_lines_number;
    std::string* visual_marqueurs;
    MatrixXd* visual_data_matrix;
    int marqueurs_number;
public:
    VisualData(DataStruct* data_set);
    ~VisualData();
    std::string* get_marqueurs();
    int get_marqueurs_number();
    void copy_data(DataStruct* data_set);
    MatrixXd* get_matrix();
};

#endif