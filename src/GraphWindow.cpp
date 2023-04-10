#include "GraphWindow.hpp"

GraphWindow::GraphWindow()
{
    //* Setup new graph window using GraphWindow.ui
    graph_window = new Ui::GraphWindow;
    graph_window->setupUi(this);
    this->setWindowTitle("Cytométrie en flux");

    //* Setup GUI
    setup_graph_spacer();
    setup_GUI();

    //* Setup variables
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
    //user_is_drawing = false;
    //draw_ellipse = false;
    //draw_line = false;
}

void GraphWindow::setup_GUI()
{
    //* Populate for adaptivesampling button
    graph_window->setAdaptativeSampling->addItem("ON", 0);
    graph_window->setAdaptativeSampling->addItem("OFF", 1);
}

void GraphWindow::populate_marqueurs(int nombre_de_marqueurs)
{
    //* Populate Marqueurs buttons with all marqueurs
    for (int i = 0; i < nombre_de_marqueurs; i++)
    {
        graph_window->comboBoxMarqueur1->addItem(visual_data_set->get_marqueurs()[i].c_str(), i);
        graph_window->comboBoxMarqueur2->addItem(visual_data_set->get_marqueurs()[i].c_str(), i);
    }
}

void GraphWindow::setup_buttons_connections()
{
    //* Connect all buttons to slots
    QObject::connect(graph_window->comboBoxMarqueur1, SIGNAL(activated(int)), this, SLOT(replot_graph()));
    QObject::connect(graph_window->comboBoxMarqueur2, SIGNAL(activated(int)), this, SLOT(replot_graph()));
    QObject::connect(graph_window->setAdaptativeSampling, SIGNAL(activated(int)), this, SLOT(setAdaptativeSampling()));
}

void GraphWindow::replot_graph()
{
    //* Reselect marqueurs to replot
    int marqueur_number_1 = graph_window->comboBoxMarqueur1->itemData(graph_window->comboBoxMarqueur1->currentIndex()).toInt();
    int marqueur_number_2 = graph_window->comboBoxMarqueur2->itemData(graph_window->comboBoxMarqueur2->currentIndex()).toInt();

    //* Replot with new marqueurs
    refresh_plot(marqueur_number_1, marqueur_number_2);
}

void GraphWindow::refresh_plot(int marqueur_number_1, int marqueur_number_2)
{
    //* Select columns
    VectorXd first_column = visual_data_set->get_matrix()->col(marqueur_number_1);
    VectorXd second_column = visual_data_set->get_matrix()->col(marqueur_number_2);

    //* Convert VectorXd to std::vector to Qvector...
    std::vector<double> first_column_std_vector(first_column.data(), first_column.data() + first_column.rows() * first_column.cols());
    std::vector<double> second_column_std_vector(second_column.data(), second_column.data() + second_column.rows() * second_column.cols());
    // QVector<double> first_column_QVector = QVector<double>::fromStdVector(first_column_std_vector);
    // QVector<double> second_column_QVector = QVector<double>::fromStdVector(second_column_std_vector);
    QVector<double> first_column_QVector = QVector<double>(first_column_std_vector.begin(), first_column_std_vector.end());
    QVector<double> second_column_QVector = QVector<double>(second_column_std_vector.begin(), second_column_std_vector.end());

    //* Rename axis
    customPlot->xAxis->setLabel(visual_data_set->get_marqueurs()[marqueur_number_1].c_str());
    customPlot->yAxis->setLabel(visual_data_set->get_marqueurs()[marqueur_number_2].c_str());

    /*
    //* Delete all shapes (ellipse or polygon)
    if (customPlot->hasItem(m_selectionCircle))
        customPlot->removeItem(m_selectionCircle);
    if (!m_selectionLine.empty())
    {
        for (QCPItemLine *line : m_selectionLine)
        {
            customPlot->removeItem(line);
        }
        m_selectionLine.clear();
    }
    user_is_drawing = false;
    */

    //* Set new data and replot
    customPlot->graph(0)->setData(first_column_QVector, second_column_QVector);
    customPlot->replot();
}

void GraphWindow::on_setOpenGL_stateChanged(int arg1)
{
    //* OpenGL
    if (arg1 == Qt::Checked)
    {
        customPlot->setOpenGl(true);
        qDebug() << "OpenGL on !";
        replot_graph();
    }
    else if (arg1 == Qt::Unchecked)
    {
        customPlot->setOpenGl(false);
        qDebug() << "OpenGL off !";
        replot_graph();
    }
}

void GraphWindow::on_actionOpen_triggered()
{
    //* Open file
    QString fileName = QFileDialog::getOpenFileName(nullptr,
                                                    tr("Open File"),
                                                    "/home",
                                                    tr("CSV files (*.csv)"));

    //* Process the file
    create_data(fileName);
    create_plot();
    setup_buttons_connections();
}

void GraphWindow::create_data(QString fileName)
{
    //* Create data_set
    char delimiter = ',';
    File csv_file(fileName.toStdString(), delimiter);
    data_set = new DataStruct(csv_file);

    //* Create visual_data_set
    visual_data_set = new VisualData(data_set);

    //* Populate marqueurs
    populate_marqueurs(visual_data_set->get_marqueurs_number());

    //* Init and format visual_data_set
    visual_data_set->copy_data(data_set);
}

void GraphWindow::create_plot()
{
    //* Create plot
    customPlot = new QCustomPlot;
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
    VectorXd first_column = visual_data_set->get_matrix()->col(marqueur_number_1);
    VectorXd second_column = visual_data_set->get_matrix()->col(marqueur_number_2);

    //* Convert VectorXd to std::vector to Qvector...
    std::vector<double> first_column_std_vector(first_column.data(), first_column.data() + first_column.rows() * first_column.cols());
    std::vector<double> second_column_std_vector(second_column.data(), second_column.data() + second_column.rows() * second_column.cols());
    // QVector<double> first_column_QVector = QVector<double>::fromStdVector(first_column_std_vector);
    // QVector<double> second_column_QVector = QVector<double>::fromStdVector(second_column_std_vector);
    QVector<double> first_column_QVector = QVector<double>(first_column_std_vector.begin(), first_column_std_vector.end());
    QVector<double> second_column_QVector = QVector<double>(second_column_std_vector.begin(), second_column_std_vector.end());

    //* Set axis
    double first_column_maxValues = first_column.maxCoeff();
    double first_column_minValues = first_column.minCoeff();

    double second_column_maxValues = second_column.maxCoeff();
    double second_column_minValues = second_column.minCoeff();

    //* Set the pen style
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

    //* Create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(first_column_QVector, second_column_QVector);
    customPlot->setInteraction(QCP::iRangeDrag, true);
    customPlot->setInteraction(QCP::iRangeZoom, true);

    // give the axes some labels:
    customPlot->xAxis->setLabel(visual_data_set->get_marqueurs()[marqueur_number_1].c_str());
    customPlot->yAxis->setLabel(visual_data_set->get_marqueurs()[marqueur_number_2].c_str());

    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(first_column_minValues, first_column_maxValues);
    customPlot->yAxis->setRange(second_column_minValues, second_column_maxValues);

    //* Plot
    // std::cout << "Replot time is : " << customPlot->replotTime() << std::endl;
    customPlot->replot();
}

void GraphWindow::setAdaptativeSampling()
{
    int index = graph_window->setAdaptativeSampling->itemData(graph_window->setAdaptativeSampling->currentIndex()).toInt();
    if (index == 0) // ON
    {
        customPlot->graph(0)->setAdaptiveSampling(true);
        replot_graph();
    }
    else if (index == 1) // OFF
    {
        customPlot->graph(0)->setAdaptiveSampling(false);
        replot_graph();
    }
}
