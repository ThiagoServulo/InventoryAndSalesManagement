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

    if(!dbConnection.open())
    {
        QMessageBox::warning(this, "Error", "Unable to connect database");
    }

    ui->tabWidget->setCurrentIndex(0);
    ui->tableWidget_im_inventory->setColumnCount(2);
    ui->tableWidget_im_inventory->setColumnWidth(0, 100);
    ui->tableWidget_im_inventory->setColumnWidth(1, 200);
    ui->tableWidget_im_inventory->setHorizontalHeaderLabels({"Id", "Description"});
    ui->tableWidget_im_inventory->setStyleSheet("QTableView {selection-background-color: red}");
    ui->tableWidget_im_inventory->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_im_inventory->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_im_inventory->verticalHeader()->setVisible(false);

    QValidator *validatorFloat = new QRegularExpressionValidator(QRegularExpression("[0-9]*\\.?[0-9]*"));
    ui->lineEdit_salePrice->setValidator(validatorFloat);
    ui->lineEdit_purchasePrice->setValidator(validatorFloat);
    ui->lineEdit_im_salePrice->setValidator(validatorFloat);
    ui->lineEdit_im_purchasePrice->setValidator(validatorFloat);

    QValidator *validatorInt = new QRegularExpressionValidator(QRegularExpression("[0-9]*"));
    ui->lineEdit_quantity->setValidator(validatorInt);
    ui->lineEdit_supplier->setValidator(validatorInt);
    ui->lineEdit_id->setValidator(validatorInt);
    ui->lineEdit_im_quantity->setValidator(validatorInt);
    ui->lineEdit_im_supplier->setValidator(validatorInt);
    ui->lineEdit_im_id->setValidator(validatorInt);
}

InventoryManagementWindow::~InventoryManagementWindow()
{
    dbConnection.close();
    delete ui;
}

void InventoryManagementWindow::on_pushButton_saveNewProduct_clicked()
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

void InventoryManagementWindow::on_tabWidget_currentChanged(int index)
{
    if(index == 1)
    {
        UpdateIMTableWidget();
    }
}

void InventoryManagementWindow::UpdateIMTableWidget()
{
    CleanTableWidget(ui->tableWidget_im_inventory);
    int line = 0;
    QSqlQuery query;
    query.prepare("SELECT id, description FROM tb_inventory ORDER BY id");
    if(query.exec())
    {
        while(query.next())
        {
            ui->tableWidget_im_inventory->insertRow(line);
            ui->tableWidget_im_inventory->setItem(line, 0, new QTableWidgetItem(query.value(0).toString()));
            ui->tableWidget_im_inventory->setItem(line, 1, new QTableWidgetItem(query.value(1).toString()));
            ui->tableWidget_im_inventory->setRowHeight(line, 20);
            line++;
        }
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to read inventory from database");
    }
}

void InventoryManagementWindow::CleanTableWidget(QTableWidget *tableWidget)
{
    while(tableWidget->rowCount())
    {
        tableWidget->removeRow(0);
    }
}

void InventoryManagementWindow::on_tableWidget_im_inventory_itemSelectionChanged()
{
    int id = ui->tableWidget_im_inventory->item(ui->tableWidget_im_inventory->currentRow(), 0)->text().toInt();
    QSqlQuery query;
    query.prepare("SELECT * FROM tb_inventory WHERE id = " + QString::number(id));
    if(query.exec())
    {
        query.first();
        ui->lineEdit_im_id->setText(query.value(0).toString());
        ui->lineEdit_im_description->setText(query.value(1).toString());
        ui->lineEdit_im_supplier->setText(query.value(2).toString());
        ui->lineEdit_im_quantity->setText(query.value(3).toString());
        ui->lineEdit_im_purchasePrice->setText(query.value(4).toString());
        ui->lineEdit_im_salePrice->setText(query.value(5).toString());
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to read product information from database");
    }
}

void InventoryManagementWindow::on_pushButton_im_save_clicked()
{
    if(ui->lineEdit_im_id->text() == "")
    {
        QMessageBox::warning(this, "Error", "Select a product");
    }
    else
    {
        int idNew = ui->lineEdit_im_id->text().toInt();
        int idOld = ui->tableWidget_im_inventory->item(ui->tableWidget_im_inventory->currentRow(), 0)->text().toInt();
        QString description = ui->lineEdit_im_description->text();
        int id_supplier = ui->lineEdit_im_supplier->text().toInt();
        float purchasePrice = ui->lineEdit_im_purchasePrice->text().toFloat();
        float salePrice = ui->lineEdit_im_salePrice->text().toFloat();
        int quantity = ui->lineEdit_im_quantity->text().toInt();

        QSqlQuery query;
        query.prepare("UPDATE tb_inventory SET id = " + QString::number(idNew) + ", description = '" + description +
                      "', supplier = " + QString::number(id_supplier) + ", quantity = " + QString::number(quantity) +
                      ", purchase_price = " + QString::number(purchasePrice) + ", sale_price = " + QString::number(salePrice) +
                      " WHERE id = " + QString::number(idOld));

        if(query.exec())
        {
            UpdateIMTableWidget();
            QMessageBox::information(this, "Success", "Product information updated");
        }
        else
        {
            QMessageBox::warning(this, "Error", "Unable to update product information from database");
        }
    }
}

