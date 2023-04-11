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
    user_is_drawing = false;
    draw_ellipse = false;
    draw_line = false;
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
    // customPlot = new QCustomPlot;
    customPlot = new QPlot;
    customPlot->setNoAntialiasingOnDrag(true);
    curGraph = customPlot->addGraph();

    //* Layer
    customPlot->addLayer("cursorLayer", 0, QCustomPlot::limAbove);
    cursorLayer = customPlot->layer("cursorLayer");
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

void GraphWindow::plotMouseClickEllipse(QMouseEvent *e)
{
    if (!draw_ellipse)
    {
        if (e->button() == Qt::LeftButton)
        {
            // qDebug() << "LeftClick" << e->pos();
            m_selectionCircle->setVisible(true);
            m_selectionCircle->setClipToAxisRect(false);
            int x = customPlot->xAxis->pixelToCoord(e->pos().x());
            int y = customPlot->yAxis->pixelToCoord(e->pos().y());

            m_selectionCircle->topLeft->setCoords(x, y);
            m_selectionCircle->bottomRight->setCoords(x + 1, y + 1);
        }
    }
    draw_ellipse = !draw_ellipse;
}

void GraphWindow::plotMouseClickLine(QMouseEvent *e)
{
    if (draw_line)
    {
        //* Vérifier si on ferme le polygone
        int x = customPlot->xAxis->pixelToCoord(e->pos().x());
        int y = customPlot->yAxis->pixelToCoord(e->pos().y());
        int epsilon = 20;
        // qDebug() << m_selectionLine.size();
        for (QCPItemLine *line : m_selectionLine)
        {
            //+ Calcul de distance euclidienne
            int x_line = line->start->key();
            int y_line = line->start->value();
            if (sqrt(pow(x - x_line, 2) + pow(y - y_line, 2)) < epsilon)
            {
                //+ Fermer le polygone
                m_selectionLine.back()->end->setCoords(x_line, y_line);
                disconnect(customPlot, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(plotMouseClickLine(QMouseEvent *)));
                disconnect(customPlot, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(plotMouseMoveLine(QMouseEvent *)));

                customPlot->setInteraction(QCP::iRangeDrag, true);
                customPlot->setInteraction(QCP::iRangeZoom, true);

                draw_line = false;
                return;
            }
        }

        QCPItemLine *line = new QCPItemLine(customPlot);
        line->setVisible(false);
        line->setPen(QPen(Qt::red));
        line->setLayer(cursorLayer);
        m_selectionLine.push_back(line);
        m_selectionLine.back()->setVisible(true);
        m_selectionLine.back()->setClipToAxisRect(false);

        m_selectionLine.back()->start->setCoords(x, y);
        m_selectionLine.back()->end->setCoords(x + 1, y + 1);
    }
    else
    {
        if (e->button() == Qt::LeftButton)
        {

            // qDebug() << "LeftClick" << e->pos();
            m_selectionLine.back()->setVisible(true);
            m_selectionLine.back()->setClipToAxisRect(false);
            int x = customPlot->xAxis->pixelToCoord(e->pos().x());
            int y = customPlot->yAxis->pixelToCoord(e->pos().y());

            m_selectionLine.back()->start->setCoords(x, y);
            m_selectionLine.back()->end->setCoords(x + 1, y + 1);
            draw_line = true;
        }
    }
    // draw_line = !draw_line;
}

void GraphWindow::plotMouseMoveEllipse(QMouseEvent *e)
{
    if (draw_ellipse)
    {
        // qDebug() << "Mouse moved";
        int x = customPlot->xAxis->pixelToCoord(e->pos().x());
        int y = customPlot->yAxis->pixelToCoord(e->pos().y());
        m_selectionCircle->bottomRight->setCoords(x, y);
        cursorLayer->replot();
    }
}

void GraphWindow::plotMouseMoveLine(QMouseEvent *e)
{
    if (draw_line)
    {
        // qDebug() << "Mouse moved";
        int x = customPlot->xAxis->pixelToCoord(e->pos().x());
        int y = customPlot->yAxis->pixelToCoord(e->pos().y());
        m_selectionLine.back()->end->setCoords(x, y);
        cursorLayer->replot();
    }
}

void GraphWindow::on_DrawEllipse_clicked()
{
    if (user_is_drawing)
    {
        // connect_adaptive_sampling_on_idle();
        graph_window->DrawEllipse->setChecked(false);
        if (customPlot->hasItem(m_selectionCircle))
            customPlot->removeItem(m_selectionCircle);

        disconnect(customPlot, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(plotMouseClickEllipse(QMouseEvent *)));
        disconnect(customPlot, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(plotMouseMoveEllipse(QMouseEvent *)));

        customPlot->setInteraction(QCP::iRangeDrag, true);
        customPlot->setInteraction(QCP::iRangeZoom, true);
    }
    else
    {
        // disconnect_adaptive_sampling_on_idle();
        graph_window->DrawEllipse->setChecked(true);
        m_selectionCircle = new QCPItemEllipse(customPlot);
        m_selectionCircle->setVisible(false);
        m_selectionCircle->setPen(QPen(Qt::red));
        m_selectionCircle->setBrush(Qt::NoBrush);
        m_selectionCircle->setLayer(cursorLayer);

        connect(customPlot, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(plotMouseClickEllipse(QMouseEvent *)));
        connect(customPlot, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(plotMouseMoveEllipse(QMouseEvent *)));

        customPlot->setInteraction(QCP::iRangeDrag, false);
        customPlot->setInteraction(QCP::iRangeZoom, false);
    }
    // Switch status of user_is_drawing
    user_is_drawing = !user_is_drawing;
    customPlot->replot();
}

void GraphWindow::on_validateDrawing_clicked()
{
    if (!customPlot->hasItem(m_selectionCircle) && m_selectionLine.size() == 0)
    {
        qDebug() << "No shape drawn !";
    }
    else
    {
        if (customPlot->hasItem(m_selectionCircle))
        {
            graph_window->DrawEllipse->setChecked(false);

            disconnect(customPlot, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(plotMouseClickEllipse(QMouseEvent *)));
            disconnect(customPlot, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(plotMouseMoveEllipse(QMouseEvent *)));

            // connect_adaptive_sampling_on_idle();
            // qDebug() << "Ellipse gating !";
            gating_ellipse();
        }
        if (!m_selectionLine.empty())
        {
            graph_window->DrawPolygon->setChecked(false);
            disconnect(customPlot, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(plotMouseClickLine(QMouseEvent *)));
            disconnect(customPlot, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(plotMouseMoveLine(QMouseEvent *)));

            // qDebug() << "Polygon gating !";
            gating_polygon();
        }
        customPlot->setInteraction(QCP::iRangeDrag, true);
        customPlot->setInteraction(QCP::iRangeZoom, true);
        user_is_drawing = false;
    }
}

void GraphWindow::gating_polygon()
{
    /*
    bool *gated_data_array = dataSet->get_gated_data_array();
    int x1 = m_selectionLine.back()->start->key();
    int x2 = m_selectionLine.back()->end->key();
    int polygon_vertices_number = m_selectionLine.size();
    Point polygon[polygon_vertices_number];
    int n = sizeof(polygon) / sizeof(polygon[0]);

    int iter = 0;
    for (QCPItemLine *line : m_selectionLine)
    {
        polygon[iter].x = line->start->key();
        polygon[iter].y = line->start->value();
        // qDebug() << polygon[i].x << " " << polygon[i].y;
        iter++;
        // qDebug() << line->start->key();
    }

    for (int i = 0; i < first_column.size(); i++)
    {
        Point p;
        p.x = first_column[i];
        p.y = second_column[i];
        // std::cout << p.x << " " << p.y << std::endl;
        if (InsidePolygon(polygon, polygon_vertices_number, p))
        {
            // std::cout << p.x << " " << p.y << std::endl;
            gated_data_array[i] = true;
        }
    }
    replot_graph();
    */
}

//* https://www.eecs.umich.edu/courses/eecs380/HANDOUTS/PROJ2/InsidePoly.html
bool GraphWindow::InsidePolygon(Point polygon[], int N, Point p)
{
    /*
    int counter = 0;
    int i;
    double xinters;
    Point p1, p2;

    p1 = polygon[0];
    for (i = 1; i <= N; i++)
    {
        p2 = polygon[i % N];
        if (p.y > MIN(p1.y, p2.y))
        {
            if (p.y <= MAX(p1.y, p2.y))
            {
                if (p.x <= MAX(p1.x, p2.x))
                {
                    if (p1.y != p2.y)
                    {
                        xinters = (p.y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y) + p1.x;
                        if (p1.x == p2.x || p.x <= xinters)
                            counter++;
                    }
                }
            }
        }
        p1 = p2;
    }

    if (counter % 2 == 0)
        return (false);
    else
        return (true);
    */
    return false;
}

void GraphWindow::gating_ellipse()
{
    // bool *gated_data_array = dataSet->get_gated_data_array();
    QCPItemPosition *x1y1, *x2y2;
    x1y1 = m_selectionCircle->topLeft;
    x2y2 = m_selectionCircle->bottomRight;
    int x1 = x1y1->key();
    int y1 = x1y1->value();
    int x2 = x2y2->key();
    int y2 = x2y2->value();
    Coords center;
    int rayon_horizontal;
    int rayon_vertical;
    center.x = int((x1 + x2) / 2);
    center.y = int((y1 + y2) / 2);
    rayon_horizontal = int(int(abs(x1 - x2)) / 2);
    rayon_vertical = int(int(abs(y1 - y2)) / 2);
    // qDebug() << x1 << " " << y1;
    //* Get the 2 columns to plot
    VectorXd first_column = visual_data_set->get_matrix()->col(graph_window->comboBoxMarqueur1->itemData(graph_window->comboBoxMarqueur1->currentIndex()).toInt());
    VectorXd second_column = visual_data_set->get_matrix()->col(graph_window->comboBoxMarqueur2->itemData(graph_window->comboBoxMarqueur2->currentIndex()).toInt());

    std::vector<int> *indicesToKeep = new std::vector<int>;
    (*indicesToKeep).reserve(1000000);

    for (size_t i = 0; i < first_column.size(); i++)
    {
        if (pow((first_column[i] - center.x), 2) / pow(rayon_horizontal, 2) + pow((second_column[i] - center.y), 2) / pow(rayon_vertical, 2) < 1)
        {
            //+ Remove line from visal_data_set matrix
        }
        else
        {
            (*indicesToKeep).push_back(i);
        }
    }
    qDebug() << (*indicesToKeep).size();
    // visual_data_set->set_matrix();
    visual_data_set->truncate_matrix(indicesToKeep);
    // VectorXd indicesToKeepVector = VectorXd::Map(&indicesToKeep, 12, 13);

    replot_graph();
}

void GraphWindow::on_DrawPolygon_clicked()
{
    if (user_is_drawing)
    {
        graph_window->DrawPolygon->setChecked(false);
        if (!m_selectionLine.empty())
        {
            for (QCPItemLine *line : m_selectionLine)
            {
                customPlot->removeItem(line);
            }
            m_selectionLine.clear();
        }

        disconnect(customPlot, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(plotMouseClickLine(QMouseEvent *)));
        disconnect(customPlot, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(plotMouseClickLine(QMouseEvent *)));

        customPlot->setInteraction(QCP::iRangeDrag, true);
        customPlot->setInteraction(QCP::iRangeZoom, true);
    }
    else
    {
        graph_window->DrawPolygon->setChecked(true);
        QCPItemLine *line = new QCPItemLine(customPlot);
        line->setVisible(false);
        line->setPen(QPen(Qt::red));
        line->setLayer(cursorLayer);
        m_selectionLine.push_back(line);

        connect(customPlot, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(plotMouseClickLine(QMouseEvent *)));
        connect(customPlot, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(plotMouseMoveLine(QMouseEvent *)));

        customPlot->setInteraction(QCP::iRangeDrag, false);
        customPlot->setInteraction(QCP::iRangeZoom, false);
    }
    // Switch status of user_is_drawing
    user_is_drawing = !user_is_drawing;
    customPlot->replot();
}
