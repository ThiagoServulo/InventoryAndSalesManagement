#include "salesmanagementwindow.h"
#include "ui_salesmanagementwindow.h"

SalesManagementWindow::SalesManagementWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SalesManagementWindow)
{
    ui->setupUi(this);
}

SalesManagementWindow::~SalesManagementWindow()
{
    delete ui;
}
