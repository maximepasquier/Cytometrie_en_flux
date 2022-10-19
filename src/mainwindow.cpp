#include "mainwindow.hpp"
#include "./ui_GraphWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    graph_ui = new Ui::MainWindow;
    setup_mainwindow("Cytométrie en flux", this);
    Graph *graph = new Graph(graph_ui);
}

MainWindow::~MainWindow()
{
    delete graph_ui;
}

void MainWindow::setup_mainwindow(std::string window_name, MainWindow *window)
{
    graph_ui->setupUi(window);
    window->setWindowTitle("Cytométrie en flux");
}
