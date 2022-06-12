#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include <eigen3/Eigen/Dense>

using Eigen::MatrixXd;
using Eigen::VectorXd;

std::pair<int, int> get_size(std::string file_path)
{
    std::pair<int, int> p;
    std::fstream file;
    file.open(file_path, std::ios::in);
    std::cout << "file open ? : " << file.is_open() << std::endl;
    std::string line, word, temp;
    int nombre_de_lignes = 0;
    int nombre_de_colonnes = 0;
    getline(file, line);
    std::stringstream s(line);
    while (getline(s, word, ';'))
    {
        nombre_de_colonnes++;
    }
    while (getline(file, line))
    {
        nombre_de_lignes++;
    }
    p.first = nombre_de_lignes;
    p.second = nombre_de_colonnes;
    return p;
}

void read(MatrixXd& m)
{
    std::fstream file;
    file.open(".././testCSV.csv", std::ios::in);
    std::string line, word, temp;

    //std::pair matrix_size = get_size();
    // std::cout << matrix_size.first << " " << matrix_size.second << std::endl;

    //m(matrix_size.first, matrix_size.second);
    int ligne = 0, colonne = 0;
    getline(file, line);

    while (getline(file, line))
    {
        std::stringstream s(line);
        while (getline(s, word, ';'))
        {
            // std::cout << word << ", ";
            m(ligne, colonne) = std::stod(word);
            colonne++;
        }
        // std::cout << std::endl;
        colonne = 0;
        ligne++;
    }
    // std::cout << ligne << ", " << colonne << std::endl;
    // std::cout << m << std::endl;
}
