#include "DataStruct.hpp"
#include "DataReader.hpp"

DataStruct::DataStruct(File csv_file)
{
    DataReader::read_data(this, &csv_file);
}

DataStruct::~DataStruct()
{
}

int DataStruct::get_matrix_lines_number()
{
    return matrix_lines_number;
}

int DataStruct::get_matrix_columns_number()
{
    return matrix_columns_number;
}

MatrixXd *DataStruct::get_matrix()
{
    return data_matrix;
}

std::string* DataStruct::get_marqueurs()
{
    return marqueurs;
}

void DataStruct::set_matrix_lines_number(int nb_lines)
{
    matrix_lines_number = nb_lines;
}

void DataStruct::set_matrix_columns_number(int nb_columns)
{
    matrix_columns_number = nb_columns;
}

void DataStruct::set_marqueurs(std::string *data_marqueurs)
{
    marqueurs = data_marqueurs;
}

void DataStruct::set_matrix(MatrixXd *data_m)
{
    data_matrix = data_m;
}
