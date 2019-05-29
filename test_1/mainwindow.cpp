#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    srand(QDateTime::currentDateTime().toTime_t());
    ui->setupUi(this);

    m_generator.moveToThread(&m_generatorThread);
    connect(&m_generator, &GraphDataGenerator::addNewGraphPoint, this, &MainWindow::addNewGraphPoint);
    m_generatorThread.start(QThread::LowestPriority);

    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->customPlot->xAxis->setRange(-15, 15);
    ui->customPlot->yAxis->setRange(-5, 5);
    ui->customPlot->axisRect()->setupFullAxesBox();

    ui->customPlot->xAxis->setLabel("x Axis");
    ui->customPlot->yAxis->setLabel("y Axis");
    ui->customPlot->legend->setVisible(false);

    initGraph();
}

MainWindow::~MainWindow()
{
    if (m_generatorThread.isRunning()) {
        m_generatorThread.quit();
        m_generatorThread.wait();
    }

    delete ui;
}

void MainWindow::addNewGraphPoint(double x, double y)
{
    m_x_vector.push_back(x);
    m_y_vector.push_back(y);
    addRandomGraph();
}

void MainWindow::addRandomGraph()
{
    ui->customPlot->graph()->setData(m_x_vector, m_y_vector);

    if (m_x_vector.last() > ui->customPlot->xAxis->range().upper)
    ui->customPlot->xAxis->setRange(-15, m_x_vector.last() + 1.0);

    ui->customPlot->replot();
}

void MainWindow::on_btn_play_clicked()
{
    m_generator.startGenerate();
}

void MainWindow::on_btn_pause_clicked()
{
    m_generator.pauseGenerate();
}

void MainWindow::on_btn_stop_clicked()
{
    m_generator.stopGenerate();
    m_x_vector.clear();
    m_y_vector.clear();
    ui->customPlot->clearGraphs();
    initGraph();
    ui->customPlot->xAxis->setRange(-15, 15);
    ui->customPlot->yAxis->setRange(-5, 5);
    ui->customPlot->replot();

}

void MainWindow::initGraph()
{
    ui->customPlot->addGraph();
    QCPGraph::LineStyle line_style = (QCPGraph::LineStyle)(rand()%5+1);
    ui->customPlot->graph()->setLineStyle(line_style);

    if (rand()%100 > 50) {    //  Points style
        QCPScatterStyle scatter_style = QCPScatterStyle((QCPScatterStyle::ScatterShape)(rand()%14+1));
        ui->customPlot->graph()->setScatterStyle(scatter_style);
    }

    QPen graphPen;
    QColor graph_pen_color = QColor(rand()%245+10, rand()%245+10, rand()%245+10);
    graphPen.setColor(graph_pen_color);
    graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->rescaleAxes();
}
