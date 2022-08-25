#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Cytométrie en flux");
    theme();
    connect_signals_to_slots();
    setupSpacer();
    user_is_drawing = false;
    draw_ellipse = false;
    draw_line = false;
    adaptative_sampling_on_idle = false;
    mouse_wheel_is_turning = false;
    // Populate for adaptivesampling
    ui->setAdaptativeSampling->addItem("ON", 0);
    ui->setAdaptativeSampling->addItem("OFF", 1);
    ui->setAdaptativeSampling->addItem("IDLE", 2);
}

void MainWindow::setupSpacer()
{
    spacer = new QSpacerItem(0, 20, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding);
    ui->verticalLayout->insertSpacerItem(0, spacer);
}

void MainWindow::populate_marqueurs(int nombre_de_marqueurs)
{
    for (int i = 0; i < nombre_de_marqueurs; i++)
    {
        ui->comboBoxMarqueur1->addItem(marqueurs[i].c_str(), i);
        ui->comboBoxMarqueur2->addItem(marqueurs[i].c_str(), i);
    }
}

void MainWindow::connect_signals_to_slots()
{
    QObject::connect(ui->comboBoxMarqueur1, SIGNAL(activated(int)), this, SLOT(replot()));
    QObject::connect(ui->comboBoxMarqueur2, SIGNAL(activated(int)), this, SLOT(replot()));
    QObject::connect(ui->themeComboBox, SIGNAL(activated(int)), this, SLOT(updateTheme()));
}

void MainWindow::replot()
{
    int marqueur_number_1 = ui->comboBoxMarqueur1->itemData(ui->comboBoxMarqueur1->currentIndex()).toInt();
    int marqueur_number_2 = ui->comboBoxMarqueur2->itemData(ui->comboBoxMarqueur2->currentIndex()).toInt();
    makePlot(marqueur_number_1, marqueur_number_2);
}

void MainWindow::updateTheme()
{
    // Set palette colors based on selected theme
    QChart::ChartTheme theme = static_cast<QChart::ChartTheme>(
        ui->themeComboBox->itemData(ui->themeComboBox->currentIndex()).toInt());

    QPalette pal = window()->palette();
    if (theme == QChart::ChartThemeLight)
    {
        pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    }
    else if (theme == QChart::ChartThemeDark)
    {
        pal.setColor(QPalette::Window, QRgb(0x121218));
        pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
    }
    else if (theme == QChart::ChartThemeBlueCerulean)
    {
        pal.setColor(QPalette::Window, QRgb(0x40434a));
        pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
    }
    else if (theme == QChart::ChartThemeBrownSand)
    {
        pal.setColor(QPalette::Window, QRgb(0x9e8965));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    }
    else if (theme == QChart::ChartThemeBlueNcs)
    {
        pal.setColor(QPalette::Window, QRgb(0x018bba));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    }
    else if (theme == QChart::ChartThemeHighContrast)
    {
        pal.setColor(QPalette::Window, QRgb(0xffab03));
        pal.setColor(QPalette::WindowText, QRgb(0x181818));
    }
    else if (theme == QChart::ChartThemeBlueIcy)
    {
        pal.setColor(QPalette::Window, QRgb(0xcee7f0));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    }
    else
    {
        pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    }
    window()->setPalette(pal);
}

void MainWindow::theme()
{
    //+ Populate themecombobox
    ui->themeComboBox->addItem("Light", QChart::ChartThemeLight);
    ui->themeComboBox->addItem("Blue Cerulean", QChart::ChartThemeBlueCerulean);
    ui->themeComboBox->addItem("Dark", QChart::ChartThemeDark);
    ui->themeComboBox->addItem("Brown Sand", QChart::ChartThemeBrownSand);
    ui->themeComboBox->addItem("Blue NCS", QChart::ChartThemeBlueNcs);
    ui->themeComboBox->addItem("High Contrast", QChart::ChartThemeHighContrast);
    ui->themeComboBox->addItem("Blue Icy", QChart::ChartThemeBlueIcy);
    ui->themeComboBox->addItem("Qt", QChart::ChartThemeQt);

    //+ Set the colors from the light theme as default ones
    QPalette pal = qApp->palette();
    pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
    pal.setColor(QPalette::WindowText, QRgb(0x404044));
    qApp->setPalette(pal);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makePlot(int marqueur_number_1, int marqueur_number_2)
{
    ui->verticalLayout->removeItem(spacer);
    std::cout << "makePlot !" << std::endl;
    std::cout << "marqueur_number1 number is : " << marqueur_number_1 << ", marqueur_number2 number is : " << marqueur_number_2 << std::endl;

    //* Select columns
    int first_column_number = marqueur_number_1;
    int second_column_number = marqueur_number_2;
    first_column = m_visualisation->col(first_column_number);
    second_column = m_visualisation->col(second_column_number);

    //* Convert VectorXd to std::vector to Qvector...
    std::vector<double> first_column_std_vector(first_column.data(), first_column.data() + first_column.rows() * first_column.cols());
    std::vector<double> second_column_std_vector(second_column.data(), second_column.data() + second_column.rows() * second_column.cols());
    // QVector<double> first_column_QVector = QVector<double>(first_column_std_vector.begin(),first_column_std_vector.end());
    // QVector<double> second_column_QVector = QVector<double>(second_column_std_vector.begin(),second_column_std_vector.end());
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

    ui->verticalLayout->insertWidget(0, customPlot);

    curGraph->setPen(drawPen);
    /*
    QLinearGradient lGrad(QPointF(0, 0), QPointF(0, 500));
    lGrad.setColorAt(0, QColor(Qt::red));
    lGrad.setColorAt(1, QColor(Qt::green));
    QBrush lBrush(lGrad);
    QPen myPen;
    myPen.setBrush(lBrush);
    curGraph->setPen(myPen);
    */
    curGraph->setPen(drawPen);
    curGraph->setLineStyle(QCPGraph::lsNone);
    //! Performances du scatterStyle ?
    curGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 1));
    curGraph->setData(latVector, lonVector);

    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(first_column_QVector, second_column_QVector, dataSet->get_gated_data_array());
    customPlot->setInteraction(QCP::iRangeDrag, true);
    customPlot->setInteraction(QCP::iRangeZoom, true);

    // give the axes some labels:
    customPlot->xAxis->setLabel(marqueurs[marqueur_number_1].c_str());
    customPlot->yAxis->setLabel(marqueurs[marqueur_number_2].c_str());
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(first_column_minValues, first_column_maxValues);
    customPlot->yAxis->setRange(second_column_minValues, second_column_maxValues);

    std::cout << "Replot time is : " << customPlot->replotTime() << std::endl;
    customPlot->replot();
}

void MainWindow::on_actionOpen_triggered()
{
    qDebug() << "actionOpen_triggered !";
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    "/home",
                                                    tr("CSV files (*.csv)"));
    std::cout << "Path to csv file is : " << fileName.toStdString() << std::endl;

    File csv_file(fileName.toStdString());
    dataSet = new DataStruct(csv_file);

    marqueurs = dataSet->get_marqueurs();
    m_visualisation = dataSet->get_data_matrix();
    populate_marqueurs(dataSet->get_number_of_columns());
    customPlot = new QCustomPlot;
    customPlot->setNoAntialiasingOnDrag(true);
    curGraph = customPlot->addGraph();

    //* Layer
    // cursorLayer = new QCPLayer(customPlot, "cursorLayer");
    customPlot->addLayer("cursorLayer", 0, QCustomPlot::limAbove);
    cursorLayer = customPlot->layer("cursorLayer");
    cursorLayer->setMode(QCPLayer::lmBuffered);

    replot();

    // connect_adaptive_sampling_on_idle();
}

void MainWindow::connect_adaptive_sampling_on_idle()
{
    //* connect AdaptiveSampling on Idle
    connect(customPlot, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(mousePressed(QMouseEvent *)));
    connect(customPlot, SIGNAL(mouseRelease(QMouseEvent *)), this, SLOT(mouseReleased(QMouseEvent *)));
    connect(customPlot, SIGNAL(mouseWheel(QWheelEvent *)), this, SLOT(wheelMoved(QWheelEvent *)));
}

void MainWindow::disconnect_adaptive_sampling_on_idle()
{
    //* disconnect AdaptiveSampling on Idle
    disconnect(customPlot, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(mousePressed(QMouseEvent *)));
    disconnect(customPlot, SIGNAL(mouseRelease(QMouseEvent *)), this, SLOT(mouseReleased(QMouseEvent *)));
    disconnect(customPlot, SIGNAL(mouseWheel(QWheelEvent *)), this, SLOT(wheelMoved(QWheelEvent *)));
}

void MainWindow::wheelMoved(QWheelEvent *)
{
    qDebug() << "Wheel moved !";
    if (!mouse_wheel_is_turning)
    {
        QTimer::singleShot(2000, this, SLOT(wheelStopped()));
        mouse_wheel_is_turning = true;
        customPlot->graph(0)->setAdaptiveSampling(true);
    }
}

void MainWindow::wheelStopped()
{
    qDebug() << "Wheel stopped !";
    mouse_wheel_is_turning = false;
    customPlot->graph(0)->setAdaptiveSampling(false);
    customPlot->replot();
}

void MainWindow::mouseReleased(QMouseEvent *e)
{
    qDebug() << "mouseRelease !";
    if (adaptative_sampling_on_idle)
    {
        customPlot->graph(0)->setAdaptiveSampling(false);
        customPlot->replot();
    }
}

void MainWindow::mousePressed(QMouseEvent *e)
{
    qDebug() << "mousePressed !";
    if (adaptative_sampling_on_idle)
    {
        customPlot->graph(0)->setAdaptiveSampling(true);
        customPlot->replot();
    }
}

void MainWindow::plotMouseClickEllipse(QMouseEvent *e)
{
    if (draw_ellipse)
    {
        if (e->button() == Qt::LeftButton)
        {
            qDebug() << "fix LeftClick" << e->pos();
        }
    }
    else
    {
        if (e->button() == Qt::LeftButton)
        {
            qDebug() << "LeftClick" << e->pos();
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

void MainWindow::plotMouseClickLine(QMouseEvent *e)
{
    if (draw_line)
    {
        //* Vérifier si on ferme le polygone
        int x = customPlot->xAxis->pixelToCoord(e->pos().x());
        int y = customPlot->yAxis->pixelToCoord(e->pos().y());
        int epsilon = 10;
        for (QCPItemLine *line : m_selectionLine)
        {
            //+ Calcul de distance euclidienne
            int x_line = line->start->key();
            int y_line = line->start->value();
            if (sqrt(pow(x - x_line, 2) + pow(y - y_line, 2)) < epsilon)
            {
                //+ Fermer le polygone
                m_selectionLine.back()->end->setCoords(x_line, y_line);
                // draw_line = false;
                disconnect(customPlot, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(plotMouseClickLine(QMouseEvent *)));
                disconnect(customPlot, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(plotMouseMoveLine(QMouseEvent *)));

                customPlot->setInteraction(QCP::iRangeDrag, true);
                customPlot->setInteraction(QCP::iRangeZoom, true);
                return;
            }
        }

        QCPItemLine *line = new QCPItemLine(customPlot);
        line->setVisible(false);
        line->setPen(QPen(Qt::black));
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

            qDebug() << "LeftClick" << e->pos();
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

void MainWindow::plotMouseMoveEllipse(QMouseEvent *e)
{
    if (draw_ellipse)
    {
        qDebug() << "Mouse moved";
        int x = customPlot->xAxis->pixelToCoord(e->pos().x());
        int y = customPlot->yAxis->pixelToCoord(e->pos().y());
        m_selectionCircle->bottomRight->setCoords(x, y);
        cursorLayer->replot();
    }
}

void MainWindow::plotMouseMoveLine(QMouseEvent *e)
{
    if (draw_line)
    {
        qDebug() << "Mouse moved";
        int x = customPlot->xAxis->pixelToCoord(e->pos().x());
        int y = customPlot->yAxis->pixelToCoord(e->pos().y());
        m_selectionLine.back()->end->setCoords(x, y);
        cursorLayer->replot();
    }
}

void MainWindow::on_setAdaptativeSampling_stateChanged(int arg1)
{
    //! AdaptiveSampling
    if (arg1 == Qt::Checked)
    {
        customPlot->graph(0)->setAdaptiveSampling(true);
        qDebug() << "Adaptive Sampling on !";
        replot();
    }
    else if (arg1 == Qt::Unchecked)
    {
        customPlot->graph(0)->setAdaptiveSampling(false);
        qDebug() << "Adaptive Sampling off !";
        replot();
    }
}

void MainWindow::on_setAdaptativeSampling_activated()
{
    int index = ui->setAdaptativeSampling->itemData(ui->setAdaptativeSampling->currentIndex()).toInt();
    // qDebug() << index;
    adaptative_sampling_on_idle = false;
    disconnect_adaptive_sampling_on_idle();
    if (index == 0) // ON
    {
        customPlot->graph(0)->setAdaptiveSampling(true);
        qDebug() << "Adaptive Sampling on !";
        replot();
    }
    else if (index == 1) // OFF
    {
        customPlot->graph(0)->setAdaptiveSampling(false);
        qDebug() << "Adaptive Sampling off !";
        replot();
    }
    else if (index == 2) // IDLE
    {
        adaptative_sampling_on_idle = true;
        qDebug() << "Adaptive Sampling on idle !";
        connect_adaptive_sampling_on_idle();
        replot();
    }
}

void MainWindow::on_setOpenGL_stateChanged(int arg1)
{
    //! OpenGL
    if (arg1 == Qt::Checked)
    {
        customPlot->setOpenGl(true);
        qDebug() << "OpenGL on !";
        replot();
    }
    else if (arg1 == Qt::Unchecked)
    {
        customPlot->setOpenGl(false);
        qDebug() << "OpenGL off !";
        replot();
    }
}

void MainWindow::on_DrawEllipse_clicked()
{
    if (user_is_drawing)
    {
        connect_adaptive_sampling_on_idle();
        ui->DrawEllipse->setChecked(false);
        delete m_selectionCircle;

        disconnect(customPlot, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(plotMouseClickEllipse(QMouseEvent *)));
        disconnect(customPlot, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(plotMouseMoveEllipse(QMouseEvent *)));

        customPlot->setInteraction(QCP::iRangeDrag, true);
        customPlot->setInteraction(QCP::iRangeZoom, true);
    }
    else
    {
        disconnect_adaptive_sampling_on_idle();
        ui->DrawEllipse->setChecked(true);
        m_selectionCircle = new QCPItemEllipse(customPlot);
        m_selectionCircle->setVisible(false);
        m_selectionCircle->setPen(QPen(Qt::black));
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
void MainWindow::on_validateDrawing_clicked()
{
    if (m_selectionCircle == nullptr && m_selectionLine.size() == 0)
    {
        qDebug() << "No shape drawn !";
    }
    else
    {
        if (m_selectionCircle != nullptr)
        {
            ui->DrawEllipse->setChecked(false);

            disconnect(customPlot, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(plotMouseClickEllipse(QMouseEvent *)));
            disconnect(customPlot, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(plotMouseMoveEllipse(QMouseEvent *)));

            // connect_adaptive_sampling_on_idle();
            qDebug() << "Ellipse gating !";
            gating_ellipse();
        }
        if (m_selectionLine.size() != 0)
        {
            ui->DrawPolygon->setChecked(false);
            disconnect(customPlot, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(plotMouseClickLine(QMouseEvent *)));
            disconnect(customPlot, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(plotMouseMoveLine(QMouseEvent *)));

            qDebug() << "Polygon gating !";
            gating_polygon();
        }
        customPlot->setInteraction(QCP::iRangeDrag, true);
        customPlot->setInteraction(QCP::iRangeZoom, true);
    }
}

void MainWindow::gating_polygon()
{
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
        if (InsidePolygon(polygon,polygon_vertices_number,p))
        {
            // std::cout << p.x << " " << p.y << std::endl;
            gated_data_array[i] = true;
        }
    }
    for (size_t i = 0; i < polygon_vertices_number; i++)
    {
        std::cout << polygon[i].x << " " << polygon[i].y << std::endl;
    }

    replot();
}

//* https://www.eecs.umich.edu/courses/eecs380/HANDOUTS/PROJ2/InsidePoly.html
bool MainWindow::InsidePolygon(Point polygon[],int N,Point p)
{
  int counter = 0;
  int i;
  double xinters;
  Point p1,p2;

  p1 = polygon[0];
  for (i=1;i<=N;i++) {
    p2 = polygon[i % N];
    if (p.y > MIN(p1.y,p2.y)) {
      if (p.y <= MAX(p1.y,p2.y)) {
        if (p.x <= MAX(p1.x,p2.x)) {
          if (p1.y != p2.y) {
            xinters = (p.y-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;
            if (p1.x == p2.x || p.x <= xinters)
              counter++;
          }
        }
      }
    }
    p1 = p2;
  }

  if (counter % 2 == 0)
    return(false);
  else
    return(true);
}


void MainWindow::gating_ellipse()
{
    bool *gated_data_array = dataSet->get_gated_data_array();
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
    qDebug() << x1 << " " << y1;
    for (size_t i = 0; i < first_column.size(); i++)
    {
        if (pow((first_column[i] - center.x), 2) / pow(rayon_horizontal, 2) + pow((second_column[i] - center.y), 2) / pow(rayon_vertical, 2) < 1)
        {
            gated_data_array[i] = true;
        }
    }
    replot();
}

void MainWindow::on_DrawPolygon_clicked()
{
    if (user_is_drawing)
    {
        ui->DrawPolygon->setChecked(false);
        for (QCPItemLine *line : m_selectionLine)
        {
            delete line;
        }
        m_selectionLine.clear();

        disconnect(customPlot, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(plotMouseClickLine(QMouseEvent *)));
        disconnect(customPlot, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(plotMouseClickLine(QMouseEvent *)));

        customPlot->setInteraction(QCP::iRangeDrag, true);
        customPlot->setInteraction(QCP::iRangeZoom, true);
    }
    else
    {
        ui->DrawPolygon->setChecked(true);
        QCPItemLine *line = new QCPItemLine(customPlot);
        line->setVisible(false);
        line->setPen(QPen(Qt::black));
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
