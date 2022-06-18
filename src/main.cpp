#include <QApplication>
#include "Application.hpp"

int main(int argc, char *argv[])
{
    QApplication q_app(argc, argv);
    Application app;
    app.main_window->show();
    return q_app.exec();
}
