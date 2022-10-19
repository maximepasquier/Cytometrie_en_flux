#include "File.hpp"

File::File(std::string file_path, char CSV_delimiter_character)
{
    this->file_path = file_path;
    this->CSV_delimiter_character = CSV_delimiter_character;
}

File::~File()
{
}

std::string File::get_file_path()
{
    return file_path;
}

char File::get_CSV_delimiter_character()
{
    return CSV_delimiter_character;
}

CSV_File_Size File::read_CSV_file_size()
{
    std::fstream file;
    file.open(file_path, std::ios::in);
    std::string line, word, temp;
    int nb_lines = 0;
    int nb_columns = 0;
    getline(file, line);
    std::stringstream s(line);
    while (getline(s, word, CSV_delimiter_character))
    {
        nb_columns++;
    }
    while (getline(file, line))
    {
        nb_lines++;
    }
    file.close();
    CSV_File_Size csv_size = {nb_lines, nb_columns};
    return csv_size;
}

void File::read_markers()
{
}

void File::read_matrix()
{
}
