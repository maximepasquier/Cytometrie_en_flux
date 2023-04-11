#include "VisualData.hpp"

VisualData::VisualData(DataStruct *data_set)
{
    marqueurs_number = data_set->get_matrix_columns_number();
    visual_marqueurs = new std::string[marqueurs_number];
    for (int i = 0; i < marqueurs_number; i++)
    {
        visual_marqueurs[i] = data_set->get_marqueurs()[i];
    }
    visual_data_matrix = new MatrixXd(data_set->get_matrix_lines_number(), data_set->get_matrix_columns_number());
}

VisualData::~VisualData()
{
}

std::string *VisualData::get_marqueurs()
{
    return visual_marqueurs;
}

int VisualData::get_marqueurs_number()
{
    return marqueurs_number;
}

void VisualData::copy_data(DataStruct *data_set)
{
    //+ Copies the matrix
    *visual_data_matrix = *(data_set->get_matrix());
}

MatrixXd *VisualData::get_matrix()
{
    return visual_data_matrix;
}

void VisualData::set_matrix(MatrixXd matrix)
{
    *visual_data_matrix = matrix;
}

void VisualData::truncate_matrix(std::vector<int> *indicesToKeep)
{
    *visual_data_matrix = (*visual_data_matrix)(*indicesToKeep, Eigen::all);
}