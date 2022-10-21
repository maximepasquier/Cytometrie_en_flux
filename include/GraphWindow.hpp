#ifndef GRAPHWINDOW_HPP
#define GRAPHWINDOW_HPP

#include <QWindow>
#include <QFileDialog>
#include <QMainWindow>
//#include <QtCharts>
#include <QDebug>

#include "../src/ui_GraphWindow.h"
#include "File.hpp"
#include "DataStruct.hpp"

class GraphWindow : public QMainWindow
{
    Q_OBJECT

public:
    GraphWindow();
    void setup_graph_spacer();
    void setup_variables();
    void setup_GUI();
    ~GraphWindow();

private:
    Ui::GraphWindow *graph_window;
    bool user_is_drawing;
    bool draw_ellipse;
    bool draw_line;
    bool adaptative_sampling_on_idle;
    bool mouse_wheel_is_turning;
    QSpacerItem *spacer;

public slots:
    void on_actionOpen_triggered();
};

#endif
