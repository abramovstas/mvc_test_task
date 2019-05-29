#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

#include "../qcustomplot/qcustomplot.h"

#include "graphdatagenerator.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void addNewGraphPoint(double x, double y);

private slots:
    void on_btn_play_clicked();

    void on_btn_pause_clicked();

    void on_btn_stop_clicked();

private:
    Ui::MainWindow *ui;
    void initGraph();
    void addRandomGraph();
    QVector<double> m_x_vector, m_y_vector;

    GraphDataGenerator m_generator;
    QThread m_generatorThread;
};

#endif // MAINWINDOW_H
