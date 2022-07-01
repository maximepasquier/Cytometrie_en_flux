#include "Application.hpp"

Application::Application()
{
    //*Create a file object
    main_window = new MainWindow(nullptr);
    //File csv_file;
    //main_window = new MainWindow(nullptr, csv_file.get_marqueurs(), csv_file.get_data_matrix());
}

Application::~Application()
{
}