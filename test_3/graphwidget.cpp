#include "graphwidget.h"
#include "ui_graphwidget.h"

#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

#include "coordinatestable.h"

GraphWidget::GraphWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphWidget),
    m_pointSize(10),
    m_activePoint(-1)
{
    ui->setupUi(this);
}

GraphWidget::~GraphWidget()
{
    delete ui;
}

void GraphWidget::paint(QPainter *painter)
{
    if (m_points.isEmpty())
        return ;//initializePoints();

    painter->setRenderHint(QPainter::Antialiasing);

    QPalette pal = palette();
    painter->setPen(Qt::NoPen);

    // Construct the path
    QPainterPath path;
    path.moveTo(m_points.at(0));


    for (int i=1; i<m_points.size(); ++i)
        path.lineTo(m_points.at(i));

    qreal m_penWidth = 2;
    Qt::PenCapStyle m_capStyle = Qt::SquareCap;
    Qt::PenJoinStyle m_joinStyle = Qt::BevelJoin;
    Qt::PenStyle m_penStyle = Qt::SolidLine;

    QColor lg = Qt::red;
    QPen pen(lg, m_penWidth, m_penStyle, m_capStyle, m_joinStyle);
    painter->strokePath(path, pen);

    // Draw the control points
    painter->setPen(QColor(50, 100, 120, 200));
    painter->setBrush(QColor(200, 200, 210, 120));
    for (int i=0; i<m_points.size(); ++i) {
        QPointF pos = m_points.at(i);
        painter->drawEllipse(QRectF(pos.x() - m_pointSize,
                                    pos.y() - m_pointSize,
                                    m_pointSize*2, m_pointSize*2));
    }
    painter->setPen(QPen(Qt::lightGray, 0, Qt::SolidLine));
    painter->setBrush(Qt::NoBrush);
    painter->drawPolyline(m_points);
}

void GraphWidget::addNewGraphPoint(double x, double y, double)
{
    m_points << QPointF(x, y);
    update();
}

void GraphWidget::slotPointChanged(int pos, const QPointF &point)
{
    if (m_points.count() >= pos) {
        m_points[pos].setX(point.x());
        m_points[pos].setY(point.y());
        update();
    }
}

void GraphWidget::slotRemovePoint(int pos)
{
    m_points.remove(pos);
    update();
}

void GraphWidget::clear()
{
    m_points.clear();
    m_activePoint = -1;
    m_mousePress = QPoint(0,0);
    m_mouseDrag = false;
}

void GraphWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter;
    painter.begin(this);
    painter.setClipRect(e->rect());
    painter.setRenderHint(QPainter::Antialiasing);

    QRect r = rect();
    qreal left = r.x() + 1;
    qreal top = r.y() + 1;
    qreal right = r.right();
    qreal bottom = r.bottom();
    qreal radius2 = 1;

    QPainterPath clipPath;
    clipPath.moveTo(right - radius2, top);
    clipPath.arcTo(right - radius2, top, radius2, radius2, 90, -90);
    clipPath.arcTo(right - radius2, bottom - radius2, radius2, radius2, 0, -90);
    clipPath.arcTo(left, bottom - radius2, radius2, radius2, 270, -90);
    clipPath.arcTo(left, top, radius2, radius2, 180, -90);
    clipPath.closeSubpath();

    painter.save();
    painter.setClipPath(clipPath, Qt::IntersectClip);

    // client painting
    paint(&painter);
    painter.restore();

    int level = 180;
    painter.setPen(QPen(QColor(level, level, level), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawPath(clipPath);
}

void GraphWidget::mousePressEvent(QMouseEvent *e)
{
    if (!m_fingerPointMapping.isEmpty())
        return;

    m_activePoint = -1;
    qreal distance = -1;
    for (int i = 0; i < m_points.size(); ++i) {
        qreal d = QLineF(e->pos(), m_points.at(i)).length();
        if ( (distance < 0 && d < 8 * m_pointSize) || d < distance ) {
            distance = d;
            m_activePoint = i;
        }
    }

    if (m_activePoint == 0) {
        m_controltype = FirstControl;
    } else if (m_activePoint == m_points.size()-1) {
        m_controltype = LastControl;
    } else if (m_activePoint >= 1) {
        if (m_points[m_activePoint-1].y() == m_points[m_activePoint].y()) {
            m_controltype = HorizontalControl;
        } else if (m_points[m_activePoint-1].x() == m_points[m_activePoint].x()) {
            m_controltype = VerticalControl;
        }
    }

    m_mouseDrag = true;
    m_mousePress = e->pos();
}

void GraphWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (!m_fingerPointMapping.isEmpty())
        return;

    if (!m_mouseDrag && QPoint(m_mousePress - e->pos()).manhattanLength() > 25)
        m_mouseDrag = true;

    if (m_mouseDrag && m_activePoint >= 0 && m_activePoint < m_points.size()) {
        if (m_points.size() >= 2) {
            QPointF p3(getPointNext(m_points[m_activePoint], e->pos()));
            m_points[m_activePoint] = p3;

            switch (m_controltype) {
            case FirstControl:
                m_points[m_activePoint+1].setX(e->pos().x());
                m_changed_points.insert(m_activePoint+1, m_points[m_activePoint+1]);
                break;
            case HorizontalControl:
                m_points[m_activePoint-1].setY(e->pos().y());
                m_points[m_activePoint+1].setX(e->pos().x());
                m_changed_points.insert(m_activePoint-1, m_points[m_activePoint-1]);
                m_changed_points.insert(m_activePoint+1, m_points[m_activePoint+1]);
                break;
            case VerticalControl:
                m_points[m_activePoint-1].setX(e->pos().x());
                m_points[m_activePoint+1].setY(e->pos().y());
                m_changed_points.insert(m_activePoint-1, m_points[m_activePoint-1]);
                m_changed_points.insert(m_activePoint+1, m_points[m_activePoint+1]);
                break;
            case LastControl:
                m_points[m_activePoint-1].setX(e->pos().x());
                m_changed_points.insert(m_activePoint-1, m_points[m_activePoint-1]);
                break;
            }
        } else m_points[m_activePoint] = e->pos();  //  The only point
        m_changed_points.insert(m_activePoint, m_points[m_activePoint]);
        update();
    }
}

void GraphWidget::mouseReleaseEvent(QMouseEvent *)
{
    if (!m_fingerPointMapping.isEmpty())
        return;

    if (m_activePoint != -1) {
        QMapIterator<int, QPointF> i(m_changed_points);
        while (i.hasNext()) {
            i.next();
            emit signalPointChanged(i.key(), i.value());
        }
    }

    m_activePoint = -1;
}

#include "math.h"

int GraphWidget::getQuad(qreal dx, qreal dy)
{
    if (dx > 0 && dy == 0)
        return 1;
    else if (dx > 0 && dy > 0)
        return 2;
    else if (dx == 0 && dy > 0)
        return 3;
    else if (dx < 0 && dy > 0)
        return 4;
    else if (dx < 0 && dy == 0)
        return 5;
    else if (dx < 0 && dy < 0)
        return 6;
    else if (dx == 0 && dy < 0)
        return 7;
    else if (dx > 0 && dy < 0)
        return 8;

    return 0;
}

QPointF GraphWidget::getPointNext(QPointF p1, QPointF p2)
{
    qreal dx = p2.x() - p1.x();
    qreal dy = p2.y() - p1.y();
    if ( dx == dy && dx == 0)
        return p1;

    QPointF new_point(0, 0);

    switch (getQuad(dx, dy)) {
    case 1: case 3: case 5: case 7:
        new_point.setX(p2.x());
        new_point.setY(p2.y());
        break;
    case 2: case 4: case 6: case 8:
        if (fabs(dx) >= fabs(dy)) {
            new_point.setX(p2.x());
            new_point.setY(p1.y());
        } else if (fabs(dx) < fabs(dy)) {
            new_point.setX(p1.x());
            new_point.setY(p2.y());
        }
        break;
    default:
        break;
    }
    return new_point;
}
