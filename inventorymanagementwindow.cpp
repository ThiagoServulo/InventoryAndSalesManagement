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

void InventoryManagementWindow::on_pushButton_newProduct_clicked()
{
    ui->lineEdit_id->clear();
    ui->lineEdit_description->clear();
    ui->lineEdit_purchasePrice->clear();
    ui->lineEdit_quantity->clear();
    ui->lineEdit_salePrice->clear();
    ui->lineEdit_supplier->clear();
    ui->lineEdit_id->setFocus();
}


void InventoryManagementWindow::on_pushButton_saveNewProduct_clicked()
{

}

