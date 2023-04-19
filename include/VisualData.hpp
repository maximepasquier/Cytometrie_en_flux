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
    bool *gated_data_array;
public:
    VisualData(DataStruct* data_set);
    ~VisualData();
    std::string* get_marqueurs();
    int get_marqueurs_number();
    void copy_data(DataStruct* data_set);
    MatrixXd* get_matrix();
    void set_matrix(MatrixXd matrix);
    void truncate_matrix(std::vector<int> *indicesToKeep);
    bool* get_gated_data_array();
};

#endif