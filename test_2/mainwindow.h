#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "coordinatestable.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateActions(const QItemSelection &selection);

    void openFile();

    void saveFile();

    void on_actionOpen_triggered();

    void on_actionSave_As_triggered();

    void on_actionExit_triggered();

    void on_actionAdd_Entry_triggered();

    void on_actionEdit_Entry_triggered();

    void on_actionRemove_Entry_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
