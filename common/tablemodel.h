#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>

struct Coordinate
{
    double x, y, z;

    bool operator==(const Coordinate &other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }
};

inline QDataStream &operator<<(QDataStream &stream, const Coordinate &coordinate)
{
    return stream << coordinate.x << coordinate.y << coordinate.z;
}

inline QDataStream &operator>>(QDataStream &stream, Coordinate &coordinate)
{
    return stream >> coordinate.x >> coordinate.y >> coordinate.z;
}

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TableModel(QObject *parent = 0);
    TableModel(QList<Coordinate> coordinate, QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    QList<Coordinate> getCoordinate() const;

private:
    QList<Coordinate> coordinates;
};


#endif // TABLEMODEL_H
