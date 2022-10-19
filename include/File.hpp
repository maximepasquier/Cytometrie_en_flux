#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include <fstream>
#include <sstream>

struct CSV_File_Size
{
    int number_of_lines, number_of_columns;
};

class File
{
private:
    std::string file_path;
    char CSV_delimiter_character;

public:
    File(std::string file_path, char CSV_delimiter_character);
    ~File();
    std::string get_file_path();
    char get_CSV_delimiter_character();
    CSV_File_Size read_CSV_file_size();
    void read_markers();
    void read_matrix();
};

#endif