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
#include "VisualData.hpp"

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
    void populate_marqueurs(int nombre_de_marqueurs);
    void setup_buttons_connections();
    ~GraphWindow();

public slots:
    void replot_graph();
    void on_actionOpen_triggered();
    void on_setOpenGL_stateChanged(int arg1);
    void refresh_plot(int marqueur_number_1, int marqueur_number_2);
    void setAdaptativeSampling();

private:
    Ui::GraphWindow *graph_window;
    QSpacerItem *spacer;
    DataStruct* data_set;
    VisualData* visual_data_set;
    QCustomPlot *customPlot;   
};

#endif
