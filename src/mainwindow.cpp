#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtCharts/QBarCategoryAxis>
#include <QtWidgets/QApplication>
#include <QtCharts/QValueAxis>

MainWindow::MainWindow(QWidget *parent, std::string *marqueurs, MatrixXd *data_matrix)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->marqueurs = marqueurs;
    //! Matrice complète == matrice de visualisation
    m_visualisation = data_matrix;

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

    // Test : add items to comboBox Test
    for (int i = 0; i < 18; i++)
    {
        ui->comboBox1->addItem(marqueurs[i].c_str(), i);
        ui->comboBox2->addItem(marqueurs[i].c_str(), i);
    }
    QObject::connect(ui->comboBox1, SIGNAL(activated(int)), this, SLOT(updateUI()));
    QObject::connect(ui->comboBox2, SIGNAL(activated(int)), this, SLOT(updateUI()));
    updateUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateUI()
{
    int marqueur_number_1 = ui->comboBox1->itemData(ui->comboBox1->currentIndex()).toInt();
    int marqueur_number_2 = ui->comboBox2->itemData(ui->comboBox2->currentIndex()).toInt();
    MainWindow::makePlot(marqueur_number_1, marqueur_number_2);

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

void MainWindow::makePlot(int marqueur_number_1, int marqueur_number_2)
{
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
    drawPen.setWidth(4);

    QVector<double> latVector, lonVector;
    latVector << -75 << -50 << -50 << 0 << 50 << 100 << 75;
    lonVector << -75 << -50 << -25 << 0 << 25 << 50 << 75;

    QCPGraph *curGraph = ui->customPlot->addGraph();
    curGraph->setPen(drawPen);
    curGraph->setLineStyle(QCPGraph::lsNone);
    curGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
    curGraph->setData(latVector, lonVector);

    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(first_column_QVector, second_column_QVector);
    ui->customPlot->setInteraction(QCP::iRangeDrag, true);
    ui->customPlot->setInteraction(QCP::iRangeZoom, true);

    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("marqueur 1");
    ui->customPlot->yAxis->setLabel("marqueur 2");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(first_column_minValues, first_column_maxValues);
    ui->customPlot->yAxis->setRange(second_column_minValues, second_column_maxValues);

    ui->customPlot->replot();
}

void MainWindow::on_pushButton_clicked()
{
    qDebug() << "pushButton_clicked !";
}

void MainWindow::on_pushButton_2_clicked()
{
    qDebug() << "pushButton_2_clicked !";
}

void MainWindow::on_actionOpen_triggered()
{
    qDebug() << "actionOpen_triggered !";
}
