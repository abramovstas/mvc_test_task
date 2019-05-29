#ifndef COORDINATESTABLE_H
#define COORDINATESTABLE_H

#include "tablemodel.h"

#include <QItemSelection>

#include <QWidget>

namespace Ui {
class CoordinatesTable;
}

class CoordinatesTable : public QWidget
{
    Q_OBJECT

public:
    explicit CoordinatesTable(QWidget *parent = nullptr);
    ~CoordinatesTable();

    void readFromFile(const QString &fileName);

    void writeToFile(const QString &fileName);

    static int getQuad(int dx, int dy);
    static QPoint getPointNext(QPoint p1, QPoint p2);

signals:
    void signalAddEntry(double x, double y, double z);

    void signalPointChanged(int pos, const QPointF &point);
    
    void signalRemovePoint(int pos);

public slots:
    void slotPointChanged(int index, const QPointF &point);

    void showAddEntryDialog();

    void addEntry(double x, double y, double z);

    void editEntry();

    void removeEntry();

    void copyEntry();

    void pasteEntry();

signals:
    void selectionChanged(const QItemSelection &selected);

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::CoordinatesTable *ui;

    TableModel *table;

    int getQuad(qreal dx, qreal dy);

    QPointF getPointNext(QPointF p1, QPointF p2);
};

#endif // COORDINATESTABLE_H
