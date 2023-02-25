#ifndef DATAREADER_HPP
#define DATAREADER_HPP

#include "File.hpp"
#include "DataStruct.hpp"

class DataReader
{
private:
    DataReader() {} // interdit d'instancier des objets de cette classe
public:
    static void read_data(DataStruct* data_obj, File* csv_file);
    static void read_marqueurs(File *csv_file, DataStruct* data_obj);
    static void read_data_matrix_size(File *csv_file, DataStruct* data_obj);
    static void read_data_matrix(File *csv_file, DataStruct* data_obj);
};

#endif