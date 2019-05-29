#ifndef GRAPHDATAGENERATOR_H
#define GRAPHDATAGENERATOR_H

#include <QObject>
#include <QTimer>

typedef QVector<double> PointArray;

class GraphDataGenerator : public QObject
{
    Q_OBJECT

public:
    GraphDataGenerator();

public slots:
    void startGenerate();
    void pauseGenerate();
    void stopGenerate();
    void generateRandomGraphPoint();

signals:
    void addNewGraphPoint(double x, double y);

private:
    bool is_run;
    QTimer m_timer;
    int i = 0;
    double xScale, yScale, xOffset, yOffset ,r1,r2,r3,r4;
    void initVariables();
};

#endif // GRAPHDATAGENERATOR_H
