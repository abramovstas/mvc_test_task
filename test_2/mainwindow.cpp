#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->ct_widget, &CoordinatesTable::selectionChanged,
        this, &MainWindow::updateActions);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateActions(const QItemSelection &selection)
{
    QModelIndexList indexes = selection.indexes();

    if (!indexes.isEmpty()) {
        ui->actionRemove_Entry->setEnabled(true);
        ui->actionEdit_Entry->setEnabled(true);
    } else {
        ui->actionRemove_Entry->setEnabled(false);
        ui->actionEdit_Entry->setEnabled(false);
    }
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        ui->ct_widget->readFromFile(fileName);
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (!fileName.isEmpty())
        ui->ct_widget->writeToFile(fileName);
}

void MainWindow::on_actionOpen_triggered()
{
    openFile();
}

void MainWindow::on_actionSave_As_triggered()
{
    saveFile();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionAdd_Entry_triggered()
{
    ui->ct_widget->showAddEntryDialog();
}

void MainWindow::on_actionEdit_Entry_triggered()
{
    ui->ct_widget->editEntry();
}

void MainWindow::on_actionRemove_Entry_triggered()
{
    ui->ct_widget->removeEntry();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->ct_widget->copyEntry();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->ct_widget->pasteEntry();
}
