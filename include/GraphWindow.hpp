#ifndef GRAPHWINDOW_HPP
#define GRAPHWINDOW_HPP

#include <QWindow>
#include <QFileDialog>
#include <QMainWindow>
#include <QDebug>

#include "../src/ui_GraphWindow.h"
#include "File.hpp"
#include "DataStruct.hpp"
#include "qcustomplot.h"

class GraphWindow : public QMainWindow
{
    Q_OBJECT

public:
    GraphWindow();
    void setup_graph_spacer();
    void setup_variables();
    void setup_GUI();
    void create_data(QString fileName);
    void create_plot();
    ~GraphWindow();

private:
    Ui::GraphWindow *graph_window;
    bool user_is_drawing;
    bool draw_ellipse;
    bool draw_line;
    bool adaptative_sampling_on_idle;
    bool mouse_wheel_is_turning;
    QSpacerItem *spacer;
    DataStruct* data_set;

public slots:
    void on_actionOpen_triggered();
};

#endif
