#ifndef GRAPHWINDOW_HPP
#define GRAPHWINDOW_HPP

#include <QWindow>
//#include <QFileDialog>
#include <QtCharts>

#include "../src/ui_GraphWindow.h"
#include "File.hpp"
#include "DataStruct.hpp"

class GraphWindow : QWindow
{
private slots:
    void on_actionOpen_triggered();

private:
    Ui::MainWindow *graph_ui;
    bool user_is_drawing;
    bool draw_ellipse;
    bool draw_line;
    bool adaptative_sampling_on_idle;
    bool mouse_wheel_is_turning;
    QSpacerItem *spacer;

public:
    GraphWindow(Ui::MainWindow *graph_ui);
    void setup_graph_spacer();
    void setup_variables();
    void setup_GUI();
    ~GraphWindow();
};

#endif
