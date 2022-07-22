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
    VectorXd first_column = m_visualisation->col(first_column_number);
    VectorXd second_column = m_visualisation->col(second_column_number);

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
    drawPen.setColor(Qt::red);
    drawPen.setWidth(1);

    QVector<double> latVector, lonVector;
    latVector << -75 << -50 << -50 << 0 << 50 << 100 << 75;
    lonVector << -75 << -50 << -25 << 0 << 25 << 50 << 75;

    ui->verticalLayout->insertWidget(0, customPlot);

    QCPGraph *curGraph = customPlot->addGraph();
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
    replot();

    //* Layer
    // cursorLayer = new QCPLayer(customPlot, "cursorLayer");
    customPlot->addLayer("cursorLayer", 0, QCustomPlot::limAbove);
    cursorLayer = customPlot->layer("cursorLayer");
    cursorLayer->setMode(QCPLayer::lmBuffered);
}

void MainWindow::plotMouseClick(QMouseEvent *e)
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

void MainWindow::plotMouseMove(QMouseEvent *e)
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
        ui->DrawEllipse->setChecked(false);
        delete m_selectionCircle;

        disconnect(customPlot, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(plotMouseClick(QMouseEvent *)));
        disconnect(customPlot, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(plotMouseMove(QMouseEvent *)));

        customPlot->setInteraction(QCP::iRangeDrag, true);
        customPlot->setInteraction(QCP::iRangeZoom, true);
    }
    else
    {
        ui->DrawEllipse->setChecked(true);
        m_selectionCircle = new QCPItemEllipse(customPlot);
        m_selectionCircle->setVisible(false);
        m_selectionCircle->setPen(QPen(Qt::black)); // show black border around text
        m_selectionCircle->setBrush(Qt::NoBrush);
        m_selectionCircle->setLayer(cursorLayer);

        connect(customPlot, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(plotMouseClick(QMouseEvent *)));
        connect(customPlot, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(plotMouseMove(QMouseEvent *)));

        customPlot->setInteraction(QCP::iRangeDrag, false);
        customPlot->setInteraction(QCP::iRangeZoom, false);
    }
    // Switch status of user_is_drawing
    user_is_drawing = !user_is_drawing;
    customPlot->replot();
}
void MainWindow::on_validateDrawing_clicked()
{
    if(m_selectionCircle == nullptr)
    {
        qDebug() << "No shape drawn !";
    }
    else
    {
        //std::cout << m_selectionCircle->topLeft << std::endl;
        /*
        QCPItemPosition *x1,*x2,*y1,*y2;
        x1 = m_selectionCircle->topLeft;
        x2 = m_selectionCircle->top;
        y1 = m_selectionCircle->top;
        y2 = m_selectionCircle->bottom;
        double a = x1->value();
        std::cout << a << std::endl;
        */
    }
}
