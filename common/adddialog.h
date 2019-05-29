#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDialog(QWidget *parent = nullptr);
    ~AddDialog();

    double x = 0, y = 0, z = 0;
    void setX(double x);
    void setY(double y);
    void setZ(double z);

private slots:
    void on_btn_add_clicked();

    void on_btn_cancel_clicked();

    void on_dsp_x_valueChanged(double arg1);

    void on_dsp_y_valueChanged(double arg1);

    void on_dsp_z_valueChanged(double arg1);

private:
    Ui::AddDialog *ui;
};

#endif // ADDDIALOG_H
