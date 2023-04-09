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
    VisualData(/* args */);
    ~VisualData();
    std::string* get_marqueurs();
    int get_marqueurs_number();
    void two_columns_selection(DataStruct* data_set);
};

#endif