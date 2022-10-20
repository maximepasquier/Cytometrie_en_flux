#include "mainwindow.hpp"
#include "./ui_GraphWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    graph_ui = new Ui::MainWindow;
    graph_ui->setupUi(this);
    this->setWindowTitle("Cytométrie en flux");
    GraphWindow *graph = new GraphWindow(graph_ui);
}

MainWindow::~MainWindow()
{
    delete graph_ui;
}
