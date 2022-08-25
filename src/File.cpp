#include "File.hpp"

File::File(std::string file_path)
{
    // file_path = ".././testCSV.csv";
    this->file_path = file_path;
    //! define file delimiter
    delimiter = ',';
    read_file_size();
    read_marqueurs();
    read_matrix();
}

File::~File()
{
}

void File::read_file_size()
{
    std::fstream file;
    file.open(file_path, std::ios::in);
    // std::cout << "file open ? : " << file.is_open() << std::endl;
    std::string line, word, temp;
    int nombre_de_lignes = 0;
    int nombre_de_colonnes = 0;
    getline(file, line);
    std::stringstream s(line);
    while (getline(s, word, delimiter))
    {
        nombre_de_colonnes++;
    }
    while (getline(file, line))
    {
        nombre_de_lignes++;
    }
    number_of_lines = nombre_de_lignes;
    number_of_columns = nombre_de_colonnes;
    file.close();
}

void File::read_marqueurs()
{
    //* Création de la structure de données contenant les marqueurs
    marqueurs = new std::string[number_of_columns];
    std::fstream file;
    file.open(file_path, std::ios::in);
    std::string line, word;
    getline(file, line);
    std::stringstream s(line);
    int iterator = 0;
    while (getline(s, word, delimiter))
    {
        marqueurs[iterator++] = word;
    }
    /*
    for (int i = 0; i < number_of_columns; i++)
    {
        std::cout << marqueurs[i] << ", i = " << i << std::endl;
    }
    */
    file.close();
}

void File::read_matrix()
{
    //* Init la matrice de données;
    data_matrix = new MatrixXd(number_of_lines, number_of_columns);

    std::fstream file;
    file.open(file_path, std::ios::in);
    std::string line, word, temp;
    int ligne = 0, colonne = 0;
    getline(file, line);

    while (getline(file, line))
    {
        std::stringstream s(line);
        while (getline(s, word, delimiter))
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

std::string *File::get_marqueurs()
{
    return marqueurs;
}
MatrixXd *File::get_data_matrix()
{
    return data_matrix;
}

int File::get_number_of_lines()
{
    return number_of_lines;
}
int File::get_number_of_columns()
{
    return number_of_columns;
}