#include "inventorymanagementwindow.h"
#include "ui_inventorymanagementwindow.h"

InventoryManagementWindow::InventoryManagementWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InventoryManagementWindow)
{
    ui->setupUi(this);
}

InventoryManagementWindow::~InventoryManagementWindow()
{
    delete ui;
}
