#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"
#include "utilities.h"
#include "salesmanagementwindow.h"
#include "editproductfromsalewindow.h"
#include "inventorymanagementwindow.h"
#include "collaboratosmanagementwindow.h"
#include <QMessageBox>

// Globals variables
int MainWindow::id_collaborator;
QString MainWindow::name_collaborator;
int MainWindow::access_collaborator;
QString MainWindow::username_collaborator;
bool MainWindow::userLogged;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    userLogged = false;
    ui->label_name->setText("No user logged in");
    lockedPadlock.addFile(":/images/lock.png");
    unlockedPadlock.addFile(":/images/unlock.png");

    ui->pushButton_Block->setText("");
    ui->pushButton_Block->setIcon(lockedPadlock);
    ui->statusbar->addWidget(ui->pushButton_Block);
    ui->statusbar->addWidget(ui->label_name);

    // Init window configurations
    InitFieldsWindow();
    ui->pushButton_editProduct_4->setAutoDefault(false);
    ui->pushButton_finalizeSale_4->setAutoDefault(false);
    ui->pushButton_removeProduct_4->setAutoDefault(false);
    ui->pushButton_searchProduct_4->setAutoDefault(false);
    ui->lineEdit_total_4->setEnabled(false);

    // Configure sales table
    Utilities utilities;
    QStringList headerLabels = {"Id", "Description", "Unit value", "Quantity", "Total"};
    utilities.TableWidgetBasicConfigurations(ui->tableWidget_listProducts_4, headerLabels);

    // Configure regex to int fields
    utilities.ConfigureRegexLineEdit(ui->lineEdit_idProduct_4, 2);
    utilities.ConfigureRegexLineEdit(ui->lineEdit_quantity_4, 2);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_Block_clicked()
{
    if(!userLogged)
    {
        LoginWindow loginWindow;
        loginWindow.exec();

        if(userLogged)
        {
            ui->pushButton_Block->setIcon(unlockedPadlock);
            ui->label_name->setText(name_collaborator);
        }
    }
    else
    {
        userLogged = false;
        ui->label_name->setText("No user logged in");
        ui->pushButton_Block->setIcon(lockedPadlock);
    }
}

void MainWindow::on_actionInventory_triggered()
{
    if(userLogged && access_collaborator == 1)
    {
        InventoryManagementWindow inventoryManagementWindow;
        inventoryManagementWindow.exec();
    }
    else
    {
        QMessageBox::information(this, "Information", "Unauthorized access");
    }
}


void MainWindow::on_actionCollaborators_triggered()
{
    if(userLogged && access_collaborator == 1)
    {
        CollaboratosManagementWindow collaboratorsManagementWindow;
        collaboratorsManagementWindow.exec();
    }
    else
    {
        QMessageBox::information(this, "Information", "Unauthorized access");
    }
}


void MainWindow::on_actionSales_triggered()
{
    if(userLogged && access_collaborator == 1)
    {
        SalesManagementWindow salesManagementWindow;
        salesManagementWindow.exec();
    }
    else
    {
        QMessageBox::information(this, "Information", "Unauthorized access");
    }
}


void MainWindow::InsertProductIntoTableWidget()
{
    DatabaseConnection dbConnection;

    // Get product informations
    if(dbConnection.open())
    {
        QSqlQuery query;
        query.prepare("SELECT id, quantity, description, sale_price FROM tb_inventory WHERE id = " + ui->lineEdit_idProduct_4->text());

        if(query.exec())
        {
            query.first();

            if(query.value(0).toString() != "")
            {
                // Check if the quantity is valid
                int quantity_storage = query.value(1).toInt();
                if (quantity_storage >= ui->lineEdit_quantity_4->text().toInt())
                {
                    float total;
                    int line = 0;

                    // Insert into table widget
                    ui->tableWidget_listProducts_4->insertRow(line);
                    ui->tableWidget_listProducts_4->setItem(line, 0, new QTableWidgetItem(query.value(0).toString()));
                    ui->tableWidget_listProducts_4->setItem(line, 1, new QTableWidgetItem(query.value(2).toString()));
                    ui->tableWidget_listProducts_4->setItem(line, 2, new QTableWidgetItem(query.value(3).toString()));
                    ui->tableWidget_listProducts_4->setItem(line, 3, new QTableWidgetItem(ui->lineEdit_quantity_4->text()));
                    total = ui->lineEdit_quantity_4->text().toInt() * query.value(3).toString().toFloat();
                    ui->tableWidget_listProducts_4->setItem(line, 4, new QTableWidgetItem(QString::number(total)));
                    ui->tableWidget_listProducts_4->setRowHeight(line, 20);

                    // Update total sale
                    total = CalculateTotalSale(ui->tableWidget_listProducts_4, 4);
                    ui->lineEdit_total_4->setText(QString::number(total));

                    // Update product quantity at database
                    UpdateProductQuantiy(query.value(0).toInt(), -(ui->lineEdit_quantity_4->text().toInt()));
                }
                else
                {
                    QString message = (quantity_storage == 0) ? "Product unavailable. Quantity in stock is zero." :
                                          "Invalid quantity. Max quantity available: " + query.value(1).toString();
                    QMessageBox::warning(this, "Error", message);
                }
            }
            else
            {
                QMessageBox::warning(this, "Error", "Product not found");
            }
        }
        else
        {
            QMessageBox::warning(this, "Error", "Erro to add product to sale");
        }

        dbConnection.close();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to read product informations");
    }

    // Restore window configurations
    InitFieldsWindow();
}


void MainWindow::UpdateProductQuantiy(int id_product, int quantity)
{
    DatabaseConnection dbConnection;
    if(dbConnection.open())
    {
        // Get current quantity
        QSqlQuery query;
        query.prepare("SELECT quantity FROM tb_inventory WHERE id = " + QString::number(id_product));

        if(query.exec())
        {
            query.first();

            // Calculate new quantity
            int new_quantity = query.value(0).toInt() + quantity;

            // Update quantity at database
            query.prepare("UPDATE tb_inventory SET quantity = " + QString::number(new_quantity) + " WHERE id = " + QString::number(id_product));

            // Check query status
            if(!query.exec())
            {
                QMessageBox::warning(this, "Error", "Erro to update product quantity");
            }
        }
        else
        {
            QMessageBox::warning(this, "Error", "Product not found");
        }

        dbConnection.close();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to read product quantity");
    }
}


void MainWindow::InitFieldsWindow()
{
    // Init window configurations
    ui->lineEdit_quantity_4->setText("1");
    ui->lineEdit_idProduct_4->clear();
    ui->lineEdit_idProduct_4->setFocus();
}

float MainWindow::CalculateTotalSale(QTableWidget *tableWidget, int column)
{
    float total = 0;

    for(int i = 0; i < tableWidget->rowCount(); i++)
    {
        total += tableWidget->item(i, column)->text().toFloat();
    }

    return total;
}


void MainWindow::EraseTableWidget(QTableWidget *tableWidget)
{
    while(tableWidget->rowCount() > 0)
    {
        tableWidget->removeRow(0);
    }
}


void MainWindow::on_lineEdit_idProduct_4_returnPressed()
{
    // Check if user is logged
    if(!userLogged)
    {
        QMessageBox::information(this, "Error", "Log in first");
        return;
    }

    InsertProductIntoTableWidget();
}


void MainWindow::on_pushButton_finalizeSale_4_clicked()
{
    // Check if user is logged
    if(!userLogged)
    {
        QMessageBox::information(this, "Error", "Log in first");
        return;
    }

    if(ui->tableWidget_listProducts_4->rowCount() != 0)
    {
        int id_collaborator = MainWindow::id_collaborator;
        float total = CalculateTotalSale(ui->tableWidget_listProducts_4, 4);
        QString date = QDate::currentDate().toString("yyyy-MM-dd") +  QTime::currentTime().toString(":hh:mm:ss");
        QSqlQuery query;
        query.prepare("INSERT INTO tb_sales (date, id_collaborator, total_value) VALUES ('" + date
                      + "', " + QString::number(id_collaborator) + ", " +  QString::number(total) + ")");

        if(query.exec())
        {
            query.prepare("SELECT id FROM tb_sales ORDER BY id DESC LIMIT 1");
            if(query.exec())
            {
                query.first();
                int id_sale = query.value(0).toInt();

                for(int i = 0; i < ui->tableWidget_listProducts_4->rowCount(); i++)
                {
                    int id_product = ui->tableWidget_listProducts_4->item(i, 0)->text().toInt();
                    float sale_price = ui->tableWidget_listProducts_4->item(i, 2)->text().toFloat();
                    int quantity = ui->tableWidget_listProducts_4->item(i, 3)->text().toInt();
                    query.prepare("INSERT INTO tb_products_sales (id_sale, id_product, quantity, sale_price) VALUES "
                                  "(" + QString::number(id_sale) + ", " + QString::number(id_product) + ", " +
                                  QString::number(quantity) + ", " + QString::number(sale_price) + ")");
                    if(!query.exec())
                    {
                        QMessageBox::warning(this, "Error", "Error to insert products sale information into database");
                        return;
                    }
                }
                QMessageBox::information(this, "Success", "Sale: '" + QString::number(id_sale) + "' inserted with success");
            }
            else
            {
                QMessageBox::warning(this, "Error", "Error to get sale id from database");
            }

            InitFieldsWindow();
            EraseTableWidget(ui->tableWidget_listProducts_4);
            ui->lineEdit_total_4->setText("0");
        }
        else
        {
            QMessageBox::warning(this, "Error", "Error to insert new sale into database");
        }
    }
    else
    {
        QMessageBox::information(this, "Error", "Add a product first");
    }
}


void MainWindow::on_pushButton_searchProduct_4_clicked()
{
    // Check if user is logged
    if(!userLogged)
    {
        QMessageBox::information(this, "Error", "Log in first");
        return;
    }

    InsertProductIntoTableWidget();
}


void MainWindow::on_pushButton_editProduct_4_clicked()
{
    // Check if user is logged
    if(!userLogged)
    {
        QMessageBox::information(this, "Error", "Log in first");
        return;
    }

    if(ui->tableWidget_listProducts_4->currentRow() != -1)
    {
        int idProduct = ui->tableWidget_listProducts_4->item(ui->tableWidget_listProducts_4->currentRow(), 0)->text().toInt();
        QString description = ui->tableWidget_listProducts_4->item(ui->tableWidget_listProducts_4->currentRow(), 1)->text();
        float salePrice = ui->tableWidget_listProducts_4->item(ui->tableWidget_listProducts_4->currentRow(), 2)->text().toFloat();
        int quantity = ui->tableWidget_listProducts_4->item(ui->tableWidget_listProducts_4->currentRow(), 3)->text().toInt();

        EditProductFromSaleWindow *editProductFromSaleWindow;
        editProductFromSaleWindow = new EditProductFromSaleWindow(idProduct, description, salePrice, quantity);
        editProductFromSaleWindow->exec();

        ui->tableWidget_listProducts_4->setItem(ui->tableWidget_listProducts_4->currentRow(), 2, new QTableWidgetItem(QString::number(editProductFromSaleWindow->salePrice)));
        ui->tableWidget_listProducts_4->setItem(ui->tableWidget_listProducts_4->currentRow(), 3, new QTableWidgetItem(QString::number(editProductFromSaleWindow->quantity)));

        float total = editProductFromSaleWindow->quantity * editProductFromSaleWindow->salePrice;
        ui->tableWidget_listProducts_4->setItem(ui->tableWidget_listProducts_4->currentRow(), 4, new QTableWidgetItem(QString::number(total)));

        total = CalculateTotalSale(ui->tableWidget_listProducts_4, 4);
        ui->lineEdit_total_4->setText(QString::number(total));
        ui->tableWidget_listProducts_4->setCurrentCell(-1, -1);
    }
    else
    {
        QMessageBox::information(this, "Error", "Select a product first");
    }
}


void MainWindow::on_pushButton_removeProduct_4_clicked()
{
    // Check if user is logged
    if(!userLogged)
    {
        QMessageBox::information(this, "Error", "Log in first");
        return;
    }

    if(ui->tableWidget_listProducts_4->currentColumn() != -1)
    {
        QMessageBox::StandardButton option;
        option = QMessageBox::question(this, "Remove", "Do you want to remove this product?", QMessageBox::Yes | QMessageBox::No);
        if(option == QMessageBox::Yes)
        {
            ui->tableWidget_listProducts_4->removeRow(ui->tableWidget_listProducts_4->currentRow());
            ui->lineEdit_total_4->setText(QString::number(CalculateTotalSale(ui->tableWidget_listProducts_4, 4)));
            ui->tableWidget_listProducts_4->setCurrentCell(-1, -1);

            // ATUALIZAR QUANTIDADE
        }
    }
    else
    {
        QMessageBox::information(this, "Error", "Select a product first");
    }
}

