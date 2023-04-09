#include "VisualData.hpp"

VisualData::VisualData()
{
}

VisualData::~VisualData()
{
}

std::string* VisualData::get_marqueurs()
{
    return visual_marqueurs;
}

int VisualData::get_marqueurs_number()
{
    return marqueurs_number;
}

void VisualData::two_columns_selection(DataStruct* data_set)
{
    marqueurs_number = data_set->get_matrix_columns_number();
    visual_marqueurs = new std::string[marqueurs_number];
    for (int i = 0; i < marqueurs_number; i++)
    {
        visual_marqueurs[i] = data_set->get_marqueurs()[i];
    }
    
}