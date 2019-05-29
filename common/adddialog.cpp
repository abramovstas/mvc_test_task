#include "adddialog.h"
#include "ui_adddialog.h"

AddDialog::AddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);
    x = ui->dsp_x->value();
    y = ui->dsp_y->value();
    z = ui->dsp_z->value();
}

AddDialog::~AddDialog()
{
    delete ui;
}

void AddDialog::setX(double x)
{
    this->x = x;
    ui->dsp_x->setValue(x);
}

void AddDialog::setY(double y)
{
    this->y = y;
    ui->dsp_y->setValue(y);
}

void AddDialog::setZ(double z)
{
    this->z = z;
    ui->dsp_z->setValue(z);
}

void AddDialog::on_btn_add_clicked()
{
    this->accept();
}

void AddDialog::on_btn_cancel_clicked()
{
    this->reject();
}

void AddDialog::on_dsp_x_valueChanged(double arg1)
{
    x = arg1;
}

void AddDialog::on_dsp_y_valueChanged(double arg1)
{
    y = arg1;
}

void AddDialog::on_dsp_z_valueChanged(double arg1)
{
    z = arg1;
}
