#include "DataReader.hpp"

void DataReader::read_data(DataStruct *data_obj, File *csv_file)
{
    read_data_matrix_size(csv_file, data_obj);
    read_marqueurs(csv_file, data_obj);
    read_data_matrix(csv_file, data_obj);
}

void DataReader::read_data_matrix_size(File *csv_file, DataStruct *data_obj)
{
    std::fstream file;
    file.open((*csv_file).get_file_path(), std::ios::in);
    std::string line, word, temp;
    int nb_lines = 0;
    int nb_columns = 0;
    getline(file, line);
    std::stringstream s(line);
    while (getline(s, word, (*csv_file).get_CSV_delimiter_character()))
    {
        nb_columns++;
    }
    while (getline(file, line))
    {
        nb_lines++;
    }
    file.close();
    data_obj->set_matrix_lines_number(nb_lines);
    data_obj->set_matrix_columns_number(nb_columns);
}

void DataReader::read_data_matrix(File *csv_file, DataStruct *data_obj)
{
    int number_of_lines = data_obj->get_matrix_lines_number();
    int number_of_columns = data_obj->get_matrix_columns_number();
    //* Init la matrice de données;
    MatrixXd* data_matrix = new MatrixXd(number_of_lines, number_of_columns);

    std::fstream file;
    file.open((*csv_file).get_file_path(), std::ios::in);
    std::string line, word, temp;
    int ligne = 0, colonne = 0;
    getline(file, line);

    while (getline(file, line))
    {
        std::stringstream s(line);
        while (getline(s, word, (*csv_file).get_CSV_delimiter_character()))
        {
            (*data_matrix)(ligne, colonne) = std::stod(word);
            colonne++;
        }
        colonne = 0;
        ligne++;
    }
    // std::cout << *data_matrix << std::endl;
    file.close();
}

void DataReader::read_marqueurs(File *csv_file, DataStruct *data_obj)
{
    //* Création de la structure de données contenant les marqueurs
    int matrix_columns = data_obj->get_matrix_columns_number();
    std::string *data_marqueurs = new std::string[matrix_columns];
    std::fstream file;
    file.open((*csv_file).get_file_path(), std::ios::in);
    std::string line, word;
    getline(file, line);
    std::stringstream s(line);
    int iterator = 0;
    while (getline(s, word, (*csv_file).get_CSV_delimiter_character()))
    {
        data_marqueurs[iterator++] = word;
    }
    /*
    for (int i = 0; i < number_of_columns; i++)
    {
        std::cout << marqueurs[i] << ", i = " << i << std::endl;
    }
    */
    file.close();
}