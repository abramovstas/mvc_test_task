#include "graphdatagenerator.h"

#include <QDebug>
#include <QThread>
#include <QVector>
#include <QtMath>

GraphDataGenerator::GraphDataGenerator()
{
    initVariables();
    m_timer.setInterval(500);
    connect(&m_timer, &QTimer::timeout, this, &GraphDataGenerator::generateRandomGraphPoint);
}

void GraphDataGenerator::startGenerate()
{
    m_timer.start();
}

void GraphDataGenerator::pauseGenerate()
{
    m_timer.stop();
}

void GraphDataGenerator::stopGenerate()
{
    m_timer.stop();
    initVariables();
}

void GraphDataGenerator::generateRandomGraphPoint()
{
    int n = 10; // number of points in graph
    double x = (i/(double)n-0.5)*10.0*xScale + xOffset;
    double y = (qSin(x*r1*5)*qSin(qCos(x*r2)*r4*3)+r3*qCos(qSin(x)*r4*2))*yScale + yOffset;
    i++;
    emit addNewGraphPoint(x, y);
}

void GraphDataGenerator::initVariables()
{
    i = 0;
    xScale = (rand()/(double)RAND_MAX + 0.5)*2;
    yScale = (rand()/(double)RAND_MAX + 0.5)*2;
    xOffset = (rand()/(double)RAND_MAX - 0.5)*4;
    yOffset = (rand()/(double)RAND_MAX - 0.5)*10;
    r1 = (rand()/(double)RAND_MAX - 0.5)*2;
    r2 = (rand()/(double)RAND_MAX - 0.5)*2;
    r3 = (rand()/(double)RAND_MAX - 0.5)*2;
    r4 = (rand()/(double)RAND_MAX - 0.5)*2;
}
