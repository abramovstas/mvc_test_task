#include "coordinatestable.h"
#include "ui_coordinatestable.h"

#include "adddialog.h"

#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QClipboard>
#include <QDebug>

CoordinatesTable::CoordinatesTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CoordinatesTable)
{
    ui->setupUi(this);

    table = new TableModel(this);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setModel(table);

    //ui->tableView->hideColumn(2);

    connect(ui->tableView->selectionModel(),
        &QItemSelectionModel::selectionChanged,
        this, &CoordinatesTable::selectionChanged);
}

CoordinatesTable::~CoordinatesTable()
{
    delete ui;
}

void CoordinatesTable::readFromFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, tr("Unable to open file"),
            file.errorString());
        return;
    }

    QList<Coordinate> coordinates;
    QDataStream in(&file);
    in >> coordinates;

    if (coordinates.isEmpty()) {
        QMessageBox::information(this, tr("No coordinate in file"),
                                 tr("The file you are attempting to open contains no coordinate."));
    } else {
        table->removeRows(0, table->rowCount(QModelIndex()));
        for (const auto &coordinate: qAsConst(coordinates))
            addEntry(coordinate.x, coordinate.y, coordinate.z);
    }
}

void CoordinatesTable::writeToFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }

    QDataStream out(&file);
    out << table->getCoordinate();
}

void CoordinatesTable::slotPointChanged(int pos, const QPointF &point)
{
    int row_count = table->rowCount(QModelIndex());
    if (row_count >= pos) {
        QModelIndex index = table->index(pos, 0, QModelIndex());
        table->setData(index, point.x(), Qt::EditRole);

        index = table->index(pos, 1, QModelIndex());
        table->setData(index, point.y(), Qt::EditRole);
    }
}

void CoordinatesTable::showAddEntryDialog()
{
    AddDialog aDialog;
    if (aDialog.exec())
        addEntry(aDialog.x, aDialog.y, aDialog.z);
}

void CoordinatesTable::addEntry(double x, double y, double z)
{
    int row_count = table->rowCount(QModelIndex());
    QPointF p(x, y);
    if  (row_count >= 1) {
        QPointF p1(table->getCoordinate().at(row_count-1).x, table->getCoordinate().at(row_count-1).y);
        QPointF p2(x, y);
        QPointF p3(getPointNext(p1, p2));
        p.setX(p3.x());
        p.setY(p3.y());
    }
    table->insertRows(row_count, 1, QModelIndex());

    QModelIndex index = table->index(row_count, 0, QModelIndex());
    table->setData(index, p.x(), Qt::EditRole);

    index = table->index(row_count, 1, QModelIndex());
    table->setData(index, p.y(), Qt::EditRole);

    index = table->index(row_count, 2, QModelIndex());
    table->setData(index, z, Qt::EditRole);

    emit signalAddEntry(p.x(), p.y(), z);
}

void CoordinatesTable::editEntry()
{
    QTableView *temp = ui->tableView;
    QItemSelectionModel *selectionModel = temp->selectionModel();

    QModelIndexList indexes = selectionModel->selectedRows();
    double x = 0, y = 0, z = 0;
    int row = -1;

    foreach (QModelIndex index, indexes) {
        row = index.row();
        QModelIndex xIndex = table->index(row, 0, QModelIndex());
        QVariant varX = table->data(xIndex, Qt::DisplayRole);
        x = varX.toDouble();

        QModelIndex yIndex = table->index(row, 1, QModelIndex());
        QVariant varY = table->data(yIndex, Qt::DisplayRole);
        y = varY.toDouble();

        QModelIndex zIndex = table->index(row, 2, QModelIndex());
        QVariant varZ = table->data(zIndex, Qt::DisplayRole);
        z = varZ.toDouble();

        AddDialog aDialog;
        aDialog.setWindowTitle(tr("Edit a Coordinate"));
        aDialog.setX(x);
        aDialog.setY(y);
        aDialog.setZ(z);

        if (aDialog.exec()) {
            if (aDialog.x != x) {
                QModelIndex index = table->index(row, 0, QModelIndex());
                table->setData(index, aDialog.x, Qt::EditRole);
            }
            if (aDialog.y != y) {
                QModelIndex index = table->index(row, 1, QModelIndex());
                table->setData(index, aDialog.y, Qt::EditRole);
            }
            if (aDialog.z != z) {
                QModelIndex index = table->index(row, 2, QModelIndex());
                table->setData(index, aDialog.z, Qt::EditRole);
            }
            emit signalPointChanged(row, QPointF(aDialog.x, aDialog.y));
        }
    }
}

void CoordinatesTable::removeEntry()
{
    QTableView *temp = ui->tableView;
    QItemSelectionModel *selectionModel = temp->selectionModel();

    QModelIndexList indexes = selectionModel->selectedRows();

    foreach (QModelIndex index, indexes) {
        int row = index.row();
        table->removeRows(row, 1, QModelIndex());
        emit signalRemovePoint(row);
    }
}

void CoordinatesTable::copyEntry()
{
    QTableView *temp = ui->tableView;
    QItemSelectionModel *selectionModel = temp->selectionModel();

    QModelIndexList indexes = selectionModel->selectedRows();

    if (!indexes.isEmpty()) {
        QString text;
        QItemSelectionRange range = selectionModel->selection().first();
        for (auto i = range.top(); i <= range.bottom(); ++i) {
            QStringList rowContents;
            for (auto j = range.left(); j <= range.right(); ++j)
                rowContents << table->index(i,j).data().toString();
            text += rowContents.join("\t");
            text += "\n";
        }
        QApplication::clipboard()->setText(text);
    }
}

void CoordinatesTable::pasteEntry()
{
    QTableView *temp = ui->tableView;
    QItemSelectionModel *selectionModel = temp->selectionModel();

    QModelIndexList indexes = selectionModel->selectedRows();

    if (!indexes.isEmpty()) {
        QString text = QApplication::clipboard()->text();
        QStringList rowContents = text.split("\n", QString::SkipEmptyParts);

        for (auto i = 0; i < rowContents.size(); ++i) {
            QStringList columnContents = rowContents.at(i).split("\t");
            addEntry(columnContents.at(0).toDouble(), columnContents.at(1).toDouble(), columnContents.at(2).toDouble());
        }
    }
}

void CoordinatesTable::keyPressEvent(QKeyEvent *event)
{
    QTableView *temp = ui->tableView;
    QItemSelectionModel *selectionModel = temp->selectionModel();

    QModelIndexList indexes = selectionModel->selectedRows();

    if (!indexes.isEmpty()) {
        if (event->matches(QKeySequence::Copy)) {
            QString text;
            QItemSelectionRange range = selectionModel->selection().first();
            for (auto i = range.top(); i <= range.bottom(); ++i) {
                QStringList rowContents;
                for (auto j = range.left(); j <= range.right(); ++j)
                    rowContents << table->index(i,j).data().toString();
                text += rowContents.join("\t");
                text += "\n";
            }
            QApplication::clipboard()->setText(text);
        } else if (event->matches(QKeySequence::Paste)) {
            QString text = QApplication::clipboard()->text();
            QStringList rowContents = text.split("\n", QString::SkipEmptyParts);

            for (auto i = 0; i < rowContents.size(); ++i) {
                QStringList columnContents = rowContents.at(i).split("\t");
                addEntry(columnContents.at(0).toDouble(), columnContents.at(1).toDouble(), columnContents.at(2).toDouble());
            }
        } else QWidget::keyPressEvent(event);
    }
}

#include "math.h"

int CoordinatesTable::getQuad(qreal dx, qreal dy)
{
    if (dx > 0 && dy == 0)
        return 1;
    else if (dx > 0 && dy > 0)
        return 2;
    else if (dx == 0.0 && dy > 0)
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

QPointF CoordinatesTable::getPointNext(QPointF p1, QPointF p2)
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
