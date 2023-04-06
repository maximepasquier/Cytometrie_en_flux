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
    File csv_file(fileName.toStdString(), delimiter);
    data_set = new DataStruct(csv_file);
}

void GraphWindow::create_plot()
{
    QCustomPlot *customPlot = new QCustomPlot;
    customPlot->setNoAntialiasingOnDrag(true);
    QCPGraph *curGraph = customPlot->addGraph();

    //* Layer
    customPlot->addLayer("cursorLayer", 0, QCustomPlot::limAbove);
    QCPLayer *cursorLayer = customPlot->layer("cursorLayer");
    cursorLayer->setMode(QCPLayer::lmBuffered);

    //* Remove spacer
    graph_window->verticalLayout->removeItem(spacer);

    //* Get marqueurs
    int marqueur_number_1 = graph_window->comboBoxMarqueur1->itemData(graph_window->comboBoxMarqueur1->currentIndex()).toInt();
    int marqueur_number_2 = graph_window->comboBoxMarqueur2->itemData(graph_window->comboBoxMarqueur2->currentIndex()).toInt();

    //* Select columns
    int first_column_number = marqueur_number_1;
    int second_column_number = marqueur_number_2;
    VectorXd first_column = data_set->get_matrix()->col(first_column_number);
    VectorXd second_column = data_set->get_matrix()->col(second_column_number);

    //* Convert VectorXd to std::vector to Qvector...
    std::vector<double> first_column_std_vector(first_column.data(), first_column.data() + first_column.rows() * first_column.cols());
    std::vector<double> second_column_std_vector(second_column.data(), second_column.data() + second_column.rows() * second_column.cols());
    QVector<double> first_column_QVector = QVector<double>::fromStdVector(first_column_std_vector);
    QVector<double> second_column_QVector = QVector<double>::fromStdVector(second_column_std_vector);

    //* Set axis
    double first_column_maxValues = first_column.maxCoeff();
    double first_column_minValues = first_column.minCoeff();

    double second_column_maxValues = second_column.maxCoeff();
    double second_column_minValues = second_column.minCoeff();

    // Set the pen style
    QPen drawPen;
    drawPen.setColor(Qt::black);
    drawPen.setWidth(1);

    QVector<double> latVector, lonVector;
    latVector << -75 << -50 << -50 << 0 << 50 << 100 << 75;
    lonVector << -75 << -50 << -25 << 0 << 25 << 50 << 75;

    graph_window->verticalLayout->insertWidget(0, customPlot);

    curGraph->setPen(drawPen);
    curGraph->setLineStyle(QCPGraph::lsNone);
    //! Performances du scatterStyle ?
    curGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 1));
    curGraph->setData(latVector, lonVector);

    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(first_column_QVector, second_column_QVector);
    customPlot->setInteraction(QCP::iRangeDrag, true);
    customPlot->setInteraction(QCP::iRangeZoom, true);

    // give the axes some labels:
    customPlot->xAxis->setLabel(data_set->get_marqueurs()[marqueur_number_1].c_str());
    customPlot->yAxis->setLabel(data_set->get_marqueurs()[marqueur_number_2].c_str());
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(first_column_minValues, first_column_maxValues);
    customPlot->yAxis->setRange(second_column_minValues, second_column_maxValues);

    // std::cout << "Replot time is : " << customPlot->replotTime() << std::endl;
    customPlot->replot();
}