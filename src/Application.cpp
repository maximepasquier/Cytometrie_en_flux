#include "Application.hpp"

Application::Application(/* args */)
{
    //*Create a file object
    File csv_file;
    main_window = new MainWindow(nullptr, csv_file.get_marqueurs(), csv_file.get_data_matrix());
}

Application::~Application()
{
}