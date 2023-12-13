#include "inventorymanagementwindow.h"
#include "ui_inventorymanagementwindow.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QValidator>
#include <QtSql>

InventoryManagementWindow::InventoryManagementWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InventoryManagementWindow)
{
    ui->setupUi(this);

    QValidator *validatorSalePrice = new QRegularExpressionValidator(
        QRegularExpression("[0-9]*\\.?[0-9]*"), ui->lineEdit_salePrice);
    ui->lineEdit_salePrice->setValidator(validatorSalePrice);

    QValidator *validatorPurchasePrice = new QRegularExpressionValidator(
        QRegularExpression("[0-9]*\\.?[0-9]*"), ui->lineEdit_purchasePrice);
    ui->lineEdit_purchasePrice->setValidator(validatorPurchasePrice);

    QValidator *validatorQuantity = new QRegularExpressionValidator(
        QRegularExpression("[0-9]*"), ui->lineEdit_quantity);
    ui->lineEdit_quantity->setValidator(validatorQuantity);

    QValidator *validatorSupplier = new QRegularExpressionValidator(
        QRegularExpression("[0-9]*"), ui->lineEdit_supplier);
    ui->lineEdit_supplier->setValidator(validatorSupplier);

    QValidator *validatorIdProduct = new QRegularExpressionValidator(
        QRegularExpression("[0-9]*"), ui->lineEdit_id);
    ui->lineEdit_id->setValidator(validatorIdProduct);
}

InventoryManagementWindow::~InventoryManagementWindow()
{
    delete ui;
}

void InventoryManagementWindow::on_pushButton_saveNewProduct_clicked()
{
    if(!dbConnection.open())
    {
        QMessageBox::warning(this, "Error", "Unable to connect database");
    }
    else
    {
        int id = ui->lineEdit_id->text().toInt();
        QString description = ui->lineEdit_description->text();
        int id_supplier = ui->lineEdit_supplier->text().toInt();
        float purchasePrice = ui->lineEdit_purchasePrice->text().toFloat();
        float salePrice = ui->lineEdit_salePrice->text().toFloat();
        int quantity = ui->lineEdit_quantity->text().toInt();

        QSqlQuery query;
        query.prepare("INSERT INTO tb_inventory (id, description, supplier, quantity, purchase_price, sale_price) "
                      "VALUES (" + QString::number(id) + ", '" + description + "', " + QString::number(id_supplier) + ", "
                      + QString::number(quantity) + ", " + QString::number(purchasePrice) + ", " + QString::number(salePrice) + ")");

        qDebug() << "INSERT INTO tb_inventory (id, description, supplier, quantity, purchase_price, sale_price) "
                      "VALUES (" + QString::number(id) + ", '" + description + "', " + QString::number(id_supplier) + ", "
                      + QString::number(quantity) + ", " + QString::number(purchasePrice) + ", " + QString::number(salePrice) + ")";
        if(!query.exec())
        {
            QMessageBox::warning(this, "Error", "Unable to save the product into database");
        }
        else
        {
            QMessageBox::information(this, "Success", "Product save into database");
            ui->lineEdit_id->clear();
            ui->lineEdit_description->clear();
            ui->lineEdit_purchasePrice->clear();
            ui->lineEdit_quantity->clear();
            ui->lineEdit_salePrice->clear();
            ui->lineEdit_supplier->clear();
            ui->lineEdit_id->setFocus();
        }
    }
}


void InventoryManagementWindow::on_pushButton_cancel_clicked()
{
    ui->lineEdit_id->clear();
    ui->lineEdit_description->clear();
    ui->lineEdit_purchasePrice->clear();
    ui->lineEdit_quantity->clear();
    ui->lineEdit_salePrice->clear();
    ui->lineEdit_supplier->clear();
    ui->lineEdit_id->setFocus();
}

