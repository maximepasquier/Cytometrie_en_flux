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
#include "QPlot.hpp"
#include "QGraph.hpp"

struct Point
{
    int x;
    int y;
};

struct Coords
{
    int x, y;
};

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
    void gating_ellipse();
    void gating_polygon();
    bool InsidePolygon(Point polygon[], int N, Point p);
    ~GraphWindow();

public slots:
    void replot_graph();
    void on_actionOpen_triggered();
    void on_setOpenGL_stateChanged(int arg1);
    void refresh_plot(int marqueur_number_1, int marqueur_number_2);
    void setAdaptativeSampling();
    void plotMouseClickEllipse(QMouseEvent *e);
    void plotMouseClickLine(QMouseEvent *e);
    void plotMouseMoveEllipse(QMouseEvent *e);
    void plotMouseMoveLine(QMouseEvent *e);
    void on_DrawPolygon_clicked();
    void on_DrawEllipse_clicked();
    void on_validateDrawing_clicked();

private:
    bool user_is_drawing;
    bool draw_ellipse, draw_line;
    QCPItemEllipse *m_selectionCircle;
    std::vector<QCPItemLine *> m_selectionLine;
    QCPLayer *cursorLayer;
    Ui::GraphWindow *graph_window;
    QSpacerItem *spacer;
    DataStruct* data_set;
    VisualData* visual_data_set;
    QPlot *customPlot;  
    QCPGraph *curGraph; 
};

#endif
