#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include <QMap>

namespace Ui {
class GraphWidget;
}

class GraphWidget : public QWidget
{
    Q_OBJECT

public:

    enum ControlType { FirstControl, HorizontalControl, VerticalControl, LastControl };

    explicit GraphWidget(QWidget *parent = nullptr);
    ~GraphWidget();

    virtual void paint(QPainter *painter);

public slots:
    void addNewGraphPoint(double x, double y, double);
    void slotPointChanged(int pos, const QPointF &point);
    void slotRemovePoint(int pos);
    void clear();

signals:
    void signalPointChanged(int pos, const QPointF &point);

protected:
    void paintEvent(QPaintEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *) override;

private:
    Ui::GraphWidget *ui;
    int m_pointSize;
    int m_activePoint;
    QVector<QPointF> m_points;
    QMap<int, QPointF> m_changed_points;
    QPoint m_mousePress;
    bool m_mouseDrag;
    QHash<int, int> m_fingerPointMapping;

    int getQuad(qreal dx, qreal dy);

    QPointF getPointNext(QPointF p1, QPointF p2);

    ControlType m_controltype;
};

#endif // GRAPHWIDGET_H
