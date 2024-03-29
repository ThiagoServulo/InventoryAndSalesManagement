#include "inventorymanagementwindow.h"
#include "ui_inventorymanagementwindow.h"
#include "utilities.h"
#include "mainwindow.h"
#include "databaseconnection.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QValidator>
#include <QtSql>

InventoryManagementWindow::InventoryManagementWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InventoryManagementWindow)
{
    Utilities utilities;

    ui->setupUi(this);

    // Window layout
    QIcon iconWindow;
    iconWindow.addFile(":/images/inventory.png");
    this->setWindowIcon(iconWindow);
    this->setWindowTitle("Inventory Management");
    this->setFixedSize(730, 502);

    // Set first tab
    ui->tabWidget->setCurrentIndex(0);

    // Configure table widget
    QStringList headerLabels = {"Id", "Description"};
    utilities.TableWidgetBasicConfigurations(ui->tableWidget_inventoryManagement, headerLabels);

    // Configure regex to float fields
    utilities.ConfigureRegexLineEdit(ui->lineEdit_newProduct_salePrice, 3);
    utilities.ConfigureRegexLineEdit(ui->lineEdit_newProduct_purchasePrice, 3);
    utilities.ConfigureRegexLineEdit(ui->lineEdit_inventoryManagement_salePrice, 3);
    utilities.ConfigureRegexLineEdit(ui->lineEdit_inventoryManagement_purchasePrice, 3);

    // Configure regex to int fields
    utilities.ConfigureRegexLineEdit(ui->lineEdit_newProduct_quantity, 2);
    utilities.ConfigureRegexLineEdit(ui->lineEdit_newProduct_id, 2);
    utilities.ConfigureRegexLineEdit(ui->lineEdit_inventoryManagement_quantity, 2);

    // Configure regex to string fields
    utilities.ConfigureRegexLineEdit(ui->lineEdit_newProduct_description, 1);
    utilities.ConfigureRegexLineEdit(ui->lineEdit_newProduct_supplier, 1);
    utilities.ConfigureRegexLineEdit(ui->lineEdit_inventoryManagement_description, 1);
    utilities.ConfigureRegexLineEdit(ui->lineEdit_inventoryManagement_supplier, 1);
}


InventoryManagementWindow::~InventoryManagementWindow()
{
    delete ui;
}


void InventoryManagementWindow::on_pushButton_newProduct_save_clicked()
{
    // Get fields
    int id = ui->lineEdit_newProduct_id->text().toInt();
    QString description = ui->lineEdit_newProduct_description->text();
    QString supplier = ui->lineEdit_newProduct_supplier->text();
    float purchasePrice = ui->lineEdit_newProduct_purchasePrice->text().toFloat();
    float salePrice = ui->lineEdit_newProduct_salePrice->text().toFloat();
    int quantity = ui->lineEdit_newProduct_quantity->text().toInt();

    // Check if product already exists
    if(ProductExists(id) != 0)
    {
        QMessageBox::warning(this, "Error", "This product already exists");
        return;
    }

    // Check valid product
    if(!CheckValidProduct(purchasePrice, salePrice, quantity, description, supplier))
    {
        return;
    }

    // Insert new product
    DatabaseConnection dbConnection;
    if(dbConnection.open())
    {
        // Build query
        QSqlQuery query;
        query.prepare("INSERT INTO tb_inventory (id, description, supplier, quantity, purchase_price, sale_price) "
                      "VALUES (" + QString::number(id) + ", '" + description + "', '" + supplier + "', "
                      + QString::number(quantity) + ", " + QString::number(purchasePrice) + ", "
                      + QString::number(salePrice) + ")");

        // Execute query
        if(!query.exec())
        {
            QMessageBox::warning(this, "Error", "Unable to save the product into database");
        }
        else
        {
            QMessageBox::information(this, "Success", "Product save into database");
            ClearNewProductTabFields();
        }

        dbConnection.close();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to save new product");
    }
}


bool InventoryManagementWindow::CheckValidProduct(float purchasePrice, float salePrice, int quantity,
                                                  QString description, QString supplier)
{
    // Check fields are not empty
    if(purchasePrice == 0 || salePrice == 0 || quantity == 0 || description == "" || supplier == "")
    {
        return false;
    }

    // Check if the sale price is greater than the purchase price
    if(salePrice <= purchasePrice)
    {
        QMessageBox::warning(this, "Error", "The sale price should be greater than the purchase price");
        return false;
    }

    // Is a valid product
    return true;
}


int InventoryManagementWindow::ProductExists(int id)
{
    // Init status
    int status = -1;

    DatabaseConnection dbConnection;
    if(dbConnection.open())
    {
        // Build query
        QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM tb_inventory WHERE id = " + QString::number(id));

        // Execute query
        if(!query.exec())
        {
            QMessageBox::warning(this, "Error", "Unable to read access type table");
        }
        else
        {
            // Get status
            query.first();
            status = query.value(0).toInt();
        }

        dbConnection.close();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to check if product exists");
    }

    return status;
}


void InventoryManagementWindow::on_pushButton_newProduct_cancel_clicked()
{
    ClearNewProductTabFields();
}


void InventoryManagementWindow::ClearNewProductTabFields()
{
    // Clear fields
    ui->lineEdit_newProduct_id->clear();
    ui->lineEdit_newProduct_description->clear();
    ui->lineEdit_newProduct_purchasePrice->clear();
    ui->lineEdit_newProduct_quantity->clear();
    ui->lineEdit_newProduct_salePrice->clear();
    ui->lineEdit_newProduct_supplier->clear();
    ui->lineEdit_newProduct_id->setFocus();
}


void InventoryManagementWindow::ClearInventoryManagementTabFields()
{
    // Clear fields
    ui->lineEdit_inventoryManagement_description->clear();
    ui->lineEdit_inventoryManagement_purchasePrice->clear();
    ui->lineEdit_inventoryManagement_quantity->clear();
    ui->lineEdit_inventoryManagement_salePrice->clear();
    ui->lineEdit_inventoryManagement_supplier->clear();
}


void InventoryManagementWindow::on_tabWidget_currentChanged(int index)
{
    // Check the user access
    if(index == 1 && MainWindow::access_collaborator != 1)
    {
        ui->tabWidget->setCurrentIndex(0);
        QMessageBox::information(this, "Information", "You don't have access");
        return;
    }

    // Update table
    ui->lineEdit_inventoryManagement_filter->clear();
    ClearInventoryManagementTabFields();
    UpdateInventoryManagementTableWidget();
}


void InventoryManagementWindow::UpdateInventoryManagementTableWidget()
{
    DatabaseConnection dbConnection;
    if(dbConnection.open())
    {
        // Build query
        QSqlQuery query;
        query.prepare("SELECT id, description FROM tb_inventory ORDER BY id");

        // Use query to update table
        Utilities utilities;
        if(!utilities.QueryToUpdateTableWidget(&query, ui->tableWidget_inventoryManagement))
        {
            QMessageBox::warning(this, "Error", "Unable to read inventory from database");
        }

        dbConnection.close();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to update the products table");
    }
}


void InventoryManagementWindow::on_tableWidget_inventoryManagement_itemSelectionChanged()
{
    // Get current row
    int currentRow = ui->tableWidget_inventoryManagement->currentRow();

    // Check if the row is valid
    if(currentRow == -1)
    {
        return;
    }

    DatabaseConnection dbConnection;
    if(dbConnection.open())
    {
        // Get product id
        int id = ui->tableWidget_inventoryManagement->item(currentRow, 0)->text().toInt();

        // Build query
        QSqlQuery query;
        query.prepare("SELECT * FROM tb_inventory WHERE id = " + QString::number(id));

        // Execute query
        if(query.exec())
        {
            // Show product information
            query.first();
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

        dbConnection.close();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to read product information");
    }
}


void InventoryManagementWindow::on_pushButton_inventoryManagement_save_clicked()
{
    // Get description
    QString description = ui->lineEdit_inventoryManagement_description->text();

    // Check selection
    if(description == "")
    {
        QMessageBox::warning(this, "Error", "Select a product");
        return;
    }

    // Get other fields
    int id = ui->tableWidget_inventoryManagement->item(ui->tableWidget_inventoryManagement->currentRow(),
                                                       0)->text().toInt();
    QString supplier = ui->lineEdit_inventoryManagement_supplier->text();
    float purchasePrice = ui->lineEdit_inventoryManagement_purchasePrice->text().toFloat();
    float salePrice = ui->lineEdit_inventoryManagement_salePrice->text().toFloat();
    int quantity = ui->lineEdit_inventoryManagement_quantity->text().toInt();

    // Check valid product
    if(!CheckValidProduct(purchasePrice, salePrice, quantity, description, supplier))
    {
        return;
    }

    // Update a product
    DatabaseConnection dbConnection;
    if(dbConnection.open())
    {
        // Block signals to ignore items selections changed
        ui->tableWidget_inventoryManagement->blockSignals(true);

        // Build query
        QSqlQuery query;
        query.prepare("UPDATE tb_inventory SET description = '" + description + "', supplier = '" +
                      supplier + "', quantity = " + QString::number(quantity) +
                      ", purchase_price = " + QString::number(purchasePrice) + ", sale_price = " +
                      QString::number(salePrice) + " WHERE id = " + QString::number(id));

        // Execute query
        if(query.exec())
        {
            // Update table
            ui->tableWidget_inventoryManagement->clearSelection();
            ClearInventoryManagementTabFields();
            UpdateInventoryManagementTableWidget();
            QMessageBox::information(this, "Success", "Product information updated");
        }
        else
        {
            QMessageBox::warning(this, "Error", "Unable to update product information from database");
        }

        dbConnection.close();

        // Restore signals
        ui->tableWidget_inventoryManagement->blockSignals(false);
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to update a product");
    }
}


void InventoryManagementWindow::on_pushButton_inventoryManagement_remove_clicked()
{
    // Check selection
    if(ui->lineEdit_inventoryManagement_description->text() == "")
    {
        QMessageBox::warning(this, "Error", "Select a product");
        return;
    }

    // Remove product
    DatabaseConnection dbConnection;
    if(dbConnection.open())
    {       
        QMessageBox::StandardButton button = QMessageBox::question(this, "Remove",
                                                                   "Do you want to remove this product?",
                                                                   QMessageBox::Yes | QMessageBox::No);
        if(button == QMessageBox::Yes)
        {
            int id = ui->tableWidget_inventoryManagement->item(ui->tableWidget_inventoryManagement->currentRow(),
                                                               0)->text().toInt();
            QSqlQuery query;

            // Check if it's possible to remove
            int quant = 0;
            query.prepare("SELECT COUNT(*) FROM tb_products_sales WHERE id_product = " +
                          QString::number(id));
            if(query.exec())
            {
                query.first();
                quant = query.value(0).toInt();
            }

            // Check action
            QString message;
            if(quant > 0)
            {
                message = "Product quantity set to zero";
                query.prepare("UPDATE tb_inventory SET quantity = 0 WHERE id = " +
                              QString::number(id));
            }
            else
            {
                message = "Product removed with success";
                query.prepare("DELETE FROM tb_inventory WHERE id = " +
                              QString::number(id));
            }

            // Remove or update quantity
            if(query.exec())
            {
                // Update table
                ui->tableWidget_inventoryManagement->setCurrentCell(-1, -1);
                ClearInventoryManagementTabFields();
                UpdateInventoryManagementTableWidget();
                QMessageBox::information(this, "Success", message);
            }
            else
            {
                QMessageBox::warning(this, "Error", "Unable to remove product from database");
            }
        }

        dbConnection.close();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to remove product");
    }
}


void InventoryManagementWindow::on_pushButton_inventoryManagement_search_clicked()
{
    DatabaseConnection dbConnection;
    if(dbConnection.open())
    {
        // Block signals to ignore items selections changed
        ui->tableWidget_inventoryManagement->blockSignals(true);

        // Filter products at the table
        QSqlQuery query;
        if(ui->lineEdit_inventoryManagement_filter->text() == "")
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
                query.prepare("SELECT id, description FROM tb_inventory WHERE id = " +
                              ui->lineEdit_inventoryManagement_filter->text());
            }
            else
            {
                query.prepare("SELECT id, description FROM tb_inventory WHERE description LIKE '%" +
                              ui->lineEdit_inventoryManagement_filter->text() + "%'");
            }
        }

        // Clear fields
        ui->lineEdit_inventoryManagement_filter->clear();
        ClearInventoryManagementTabFields();

        // Update table
        if(query.exec())
        {
            Utilities utilities;
            utilities.QueryToInsertFieldsIntoTableWidget(&query, ui->tableWidget_inventoryManagement);
        }
        else
        {
            QMessageBox::warning(this, "Error", "Unable to filter products from database");
        }

        dbConnection.close();

        // Restore signals
        ui->tableWidget_inventoryManagement->blockSignals(false);
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to filter products");
    }
}
