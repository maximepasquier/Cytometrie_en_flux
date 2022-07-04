#include "DataStruct.hpp"

DataStruct::DataStruct(File file)
{
    data_matrix = file.get_data_matrix();
    marqueurs = file.get_marqueurs();
    number_of_lines = file.get_number_of_lines();
    number_of_columns = file.get_number_of_columns();
}

DataStruct::~DataStruct()
{
}

std::string *DataStruct::get_marqueurs()
{
    return marqueurs;
}
MatrixXd *DataStruct::get_data_matrix()
{
    return data_matrix;
}

int DataStruct::get_number_of_lines()
{
    return number_of_lines;
}
int DataStruct::get_number_of_columns()
{
    return number_of_columns;
}