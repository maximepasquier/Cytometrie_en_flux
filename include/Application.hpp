#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QWidget>
#include <iostream>
#include <iostream>
#include "mainwindow.hpp"
#include "File.hpp"

#include <QApplication>

class Application : public QWidget
{
    Q_OBJECT
private:
    std::string csv_file_path;

public:
    Application();
    ~Application();
    MainWindow *main_window;
};

#endif
