#include "GraphWindow.hpp"

GraphWindow::GraphWindow()
{
    graph_window = new Ui::GraphWindow;
    graph_window->setupUi(this);
    this->setWindowTitle("Cytométrie en flux");
    setup_graph_spacer();
    setup_GUI();
    setup_variables();
}

GraphWindow::~GraphWindow()
{
    delete graph_window;
}

void GraphWindow::setup_graph_spacer()
{
    spacer = new QSpacerItem(0, 20, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding);
    graph_window->verticalLayout->insertSpacerItem(0, spacer);
}

void GraphWindow::setup_variables()
{
    user_is_drawing = false;
    draw_ellipse = false;
    draw_line = false;
    adaptative_sampling_on_idle = false;
    mouse_wheel_is_turning = false;
}

void GraphWindow::setup_GUI()
{
    // Populate for adaptivesampling
    graph_window->setAdaptativeSampling->addItem("ON", 0);
    graph_window->setAdaptativeSampling->addItem("OFF", 1);
    graph_window->setAdaptativeSampling->addItem("IDLE", 2);
}

void GraphWindow::on_actionOpen_triggered()
{
    qDebug() << "actionOpen_triggered !";
    QString fileName = QFileDialog::getOpenFileName(nullptr,
                                                    tr("Open File"),
                                                    "/home",
                                                    tr("CSV files (*.csv)"));

    create_data(fileName);
    create_plot();
}

void GraphWindow::create_data(QString fileName)
{
    char delimiter = ',';
    File csv_file(fileName.toStdString(),delimiter);
    data_set = new DataStruct(csv_file);
}

void GraphWindow::create_plot()
{

}