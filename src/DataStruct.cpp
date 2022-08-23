#include "DataStruct.hpp"

DataStruct::DataStruct(File file)
{
    data_matrix = file.get_data_matrix();
    marqueurs = file.get_marqueurs();
    number_of_lines = file.get_number_of_lines();
    number_of_columns = file.get_number_of_columns();
    init_gated_data_array();
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

bool *DataStruct::get_gated_data_array()
{
    return gated_data_array;
}

void DataStruct::init_gated_data_array()
{
    gated_data_array = new bool[number_of_lines];
    for (size_t i = 0; i < number_of_lines; i++)
    {
        // All data not gated yet
        gated_data_array[i] = false;
    }
    /*
    for (size_t i = 100000; i < 600000; i++)
    {
        gated_data_array[i] = true;
    }
    */
}