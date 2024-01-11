#include "inventorymanagementwindow.h"
#include "ui_inventorymanagementwindow.h"
#include "utilities.h"
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
    ui->tableWidget_inventoryManagement->setColumnCount(2);
    ui->tableWidget_inventoryManagement->setColumnWidth(0, 100);
    ui->tableWidget_inventoryManagement->setColumnWidth(1, 200);
    ui->tableWidget_inventoryManagement->setHorizontalHeaderLabels({"Id", "Description"});
    ui->tableWidget_inventoryManagement->setStyleSheet("QTableView {selection-background-color: red}");
    ui->tableWidget_inventoryManagement->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_inventoryManagement->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_inventoryManagement->verticalHeader()->setVisible(false);

    QValidator *validatorFloat = new QRegularExpressionValidator(QRegularExpression("[0-9]*\\.?[0-9]*"));
    ui->lineEdit_newProduct_salePrice->setValidator(validatorFloat);
    ui->lineEdit_newProduct_purchasePrice->setValidator(validatorFloat);
    ui->lineEdit_inventoryManagement_salePrice->setValidator(validatorFloat);
    ui->lineEdit_inventoryManagement_purchasePrice->setValidator(validatorFloat);

    QValidator *validatorInt = new QRegularExpressionValidator(QRegularExpression("[0-9]*"));
    ui->lineEdit_newProduct_quantity->setValidator(validatorInt);
    ui->lineEdit_newProduct_supplier->setValidator(validatorInt);
    ui->lineEdit_newProduct_id->setValidator(validatorInt);
    ui->lineEdit_inventoryManagement_quantity->setValidator(validatorInt);
    ui->lineEdit_inventoryManagement_supplier->setValidator(validatorInt);
    ui->lineEdit_inventoryManagement_id->setValidator(validatorInt);
}

InventoryManagementWindow::~InventoryManagementWindow()
{
    dbConnection.close();
    delete ui;
}

void InventoryManagementWindow::on_pushButton_newProduct_save_clicked()
{
    int id = ui->lineEdit_newProduct_id->text().toInt();
    QString description = ui->lineEdit_newProduct_description->text();
    int id_supplier = ui->lineEdit_newProduct_supplier->text().toInt();
    float purchasePrice = ui->lineEdit_newProduct_purchasePrice->text().toFloat();
    float salePrice = ui->lineEdit_newProduct_salePrice->text().toFloat();
    int quantity = ui->lineEdit_newProduct_quantity->text().toInt();

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
        ClearNewProductTabFields();
    }
}

void InventoryManagementWindow::on_pushButton_newProduct_cancel_clicked()
{
    ClearNewProductTabFields();
}

void InventoryManagementWindow::ClearNewProductTabFields()
{
    ui->lineEdit_newProduct_id->clear();
    ui->lineEdit_newProduct_description->clear();
    ui->lineEdit_newProduct_purchasePrice->clear();
    ui->lineEdit_newProduct_quantity->clear();
    ui->lineEdit_newProduct_salePrice->clear();
    ui->lineEdit_newProduct_supplier->clear();
    ui->lineEdit_newProduct_id->setFocus();
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
    QSqlQuery query;
    query.prepare("SELECT id, description FROM tb_inventory ORDER BY id");
    Utilities utilities;
    if(!utilities.QueryToUpdateTableWidget(&query, ui->tableWidget_inventoryManagement))
    {
        QMessageBox::warning(this, "Error", "Unable to read inventory from database");
    }
}

void InventoryManagementWindow::on_tableWidget_inventoryManagement_itemSelectionChanged()
{
    int id = ui->tableWidget_inventoryManagement->item(ui->tableWidget_inventoryManagement->currentRow(), 0)->text().toInt();
    QSqlQuery query;
    query.prepare("SELECT * FROM tb_inventory WHERE id = " + QString::number(id));
    if(query.exec())
    {
        query.first();
        ui->lineEdit_inventoryManagement_id->setText(query.value(0).toString());
        ui->lineEdit_inventoryManagement_description->setText(query.value(1).toString());
        ui->lineEdit_inventoryManagement_supplier->setText(query.value(2).toString());
        ui->lineEdit_inventoryManagement_quantity->setText(query.value(3).toString());
        ui->lineEdit_inventoryManagement_purchasePrice->setText(query.value(4).toString());
        ui->lineEdit_inventoryManagement_salePrice->setText(query.value(5).toString());
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to read product information from database");
    }
}

void InventoryManagementWindow::on_pushButton_inventoryManagement_save_clicked()
{
    if(ui->lineEdit_inventoryManagement_id->text() == "")
    {
        QMessageBox::warning(this, "Error", "Select a product");
    }
    else
    {
        int idNew = ui->lineEdit_inventoryManagement_id->text().toInt();
        int idOld = ui->tableWidget_inventoryManagement->item(ui->tableWidget_inventoryManagement->currentRow(), 0)->text().toInt();
        QString description = ui->lineEdit_inventoryManagement_description->text();
        int id_supplier = ui->lineEdit_inventoryManagement_supplier->text().toInt();
        float purchasePrice = ui->lineEdit_inventoryManagement_purchasePrice->text().toFloat();
        float salePrice = ui->lineEdit_inventoryManagement_salePrice->text().toFloat();
        int quantity = ui->lineEdit_inventoryManagement_quantity->text().toInt();

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

void InventoryManagementWindow::on_pushButton_inventoryManagement_remove_clicked()
{
    QMessageBox::StandardButton button = QMessageBox::question(this, "Remove", "Do you want to remove this product?", QMessageBox::Yes | QMessageBox::No);
    if(button == QMessageBox::Yes)
    {
        int id = ui->tableWidget_inventoryManagement->item(ui->tableWidget_inventoryManagement->currentRow(), 0)->text().toInt();
        QSqlQuery query;
        query.prepare("DELETE FROM tb_inventory WHERE id = " + QString::number(id));
        if(query.exec())
        {
            ui->tableWidget_inventoryManagement->removeRow(ui->tableWidget_inventoryManagement->currentRow());
            ui->lineEdit_inventoryManagement_id->clear();
            ui->lineEdit_inventoryManagement_description->clear();
            ui->lineEdit_inventoryManagement_supplier->clear();
            ui->lineEdit_inventoryManagement_quantity->clear();
            ui->lineEdit_inventoryManagement_purchasePrice->clear();
            ui->lineEdit_inventoryManagement_salePrice->clear();
            QMessageBox::information(this, "Success", "Product removed with success");
        }
        else
        {
            QMessageBox::warning(this, "Error", "Unable to remove product from database");
        }
    }
}

void InventoryManagementWindow::on_pushButton_inventoryManagement_search_clicked()
{
    QSqlQuery query;
    if(ui->lineEdit_im_filter->text() == "")
    {
        if(ui->radioButton_inventoryManagement_id->isChecked())
        {
            query.prepare("SELECT id, description FROM tb_inventory ORDER BY id");
        }
        else
        {
            query.prepare("SELECT id, description FROM tb_inventory ORDER BY description");
        }
    }
    else
    {
        if(ui->radioButton_inventoryManagement_id->isChecked())
        {
            query.prepare("SELECT id, description FROM tb_inventory WHERE id = " + ui->lineEdit_im_filter->text());
        }
        else
        {
            query.prepare("SELECT id, description FROM tb_inventory WHERE description LIKE '%" + ui->lineEdit_im_filter->text() + "%'");
        }
    }

    ui->lineEdit_im_filter->clear();

    if(query.exec())
    {
        Utilities utilities;
        utilities.QueryToInsertFieldsIntoTableWidget(&query, ui->tableWidget_inventoryManagement);
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to filter products from database");
    }

}
