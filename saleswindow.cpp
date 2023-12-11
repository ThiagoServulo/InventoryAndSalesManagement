#include "saleswindow.h"
#include "ui_saleswindow.h"

SalesWindow::SalesWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SalesWindow)
{
    ui->setupUi(this);
}

SalesWindow::~SalesWindow()
{
    delete ui;
}
