#include "mainwindow.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    GraphWindow *graph = new GraphWindow();
    graph->show();
}

MainWindow::~MainWindow()
{
}