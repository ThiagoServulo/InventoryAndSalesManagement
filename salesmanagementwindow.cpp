#include "salesmanagementwindow.h"
#include "ui_salesmanagementwindow.h"
#include "databaseconnection.h"
#include "utilities.h"
#include <iostream>
#include <fstream>
#include <QDateEdit>
#include <QtSql>
#include <QMessageBox>
#include <QDir>

SalesManagementWindow::SalesManagementWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SalesManagementWindow)
{
    ui->setupUi(this);

    // Window layout
    QIcon iconWindow;
    iconWindow.addFile(":/images/sale_management.png");
    this->setWindowIcon(iconWindow);
    this->setWindowTitle("Sales Management");
    this->setFixedSize(789, 522);

    // Configure data format
    ui->dateEdit_initial->setDisplayFormat("yyyy-MM-dd");
    ui->dateEdit_final->setDisplayFormat("yyyy-MM-dd");

    // Init with a date
    QDate today = QDate::currentDate();
    ui->dateEdit_initial->setDate(today);
    ui->dateEdit_final->setDate(today);

    // Configure products sales table
    Utilities utilities;
    QStringList headerLabels = {"Product Id", "Quantity", "Unitary Price", "Total Price"};
    utilities.TableWidgetBasicConfigurations(ui->tableWidget_productsSales, headerLabels);
    utilities.CleanTableWidget(ui->tableWidget_productsSales);

    // Configure sales table
    headerLabels = {"Id", "Date", "Collaborator", "Total"};
    utilities.TableWidgetBasicConfigurations(ui->tableWidget_sales, headerLabels);
    ShowAllSalesIntoTableWidget();
}


void SalesManagementWindow::ShowAllSalesIntoTableWidget()
{
    // Prepare tables
    Utilities utilities;
    ui->tableWidget_sales->clearSelection();
    utilities.CleanTableWidget(ui->tableWidget_productsSales);

    // Show all sales
    DatabaseConnection dbConnection;
    if(dbConnection.open())
    {
        // Build query
        QSqlQuery query;
        query.prepare("SELECT s.id, s.date, COALESCE(c.name, 'not found'), "
                      "printf('%.2f', s.total_value) "
                      "FROM tb_sales s LEFT JOIN tb_collaborators c "
                      "ON s.id_collaborator = c.id ORDER BY s.id");

        // Update table widget
        if(!utilities.QueryToUpdateTableWidget(&query, ui->tableWidget_sales))
        {
            QMessageBox::warning(this, "Error", "Unable to read sales from database");
        }

        dbConnection.close();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to read all sales");
    }
}


SalesManagementWindow::~SalesManagementWindow()
{
    delete ui;
}


void SalesManagementWindow::on_tableWidget_sales_itemSelectionChanged()
{
    // Get current row
    int currentRow = ui->tableWidget_sales->currentRow();

    // Check whether is a valid row
    if(currentRow < 0)
    {
        return;
    }

    // Show sales products
    DatabaseConnection dbConnection;
    if(dbConnection.open())
    {
        Utilities utilities;
        QSqlQuery query;

        // Get sale id
        int idSale = ui->tableWidget_sales->item(currentRow, 0)->text().toInt();

        // Build query
        query.prepare("SELECT i.description, ps.quantity, printf('%.2f', ps.sale_price), "
                      "printf('%.2f', ps.quantity * ps.sale_price) "
                      "FROM tb_products_sales ps JOIN tb_inventory i ON ps.id_product = i.id "
                      "WHERE ps.id_sale = " + QString::number(idSale) + " ORDER BY ps.id");

        // Update table widget
        if(!utilities.QueryToUpdateTableWidget(&query, ui->tableWidget_productsSales))
        {
            QMessageBox::warning(this, "Error", "Unable to read prducts of sales from database");
        }

        dbConnection.close();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to read products of sales");
    }
}


void SalesManagementWindow::on_pushButton_filter_clicked()
{
    // Check dates
    if(ui->dateEdit_final->date() < ui->dateEdit_initial->date())
    {
        QMessageBox::information(this, "Information", "The final date should be later than the initial");
        return;
    }

    // Filter sales
    DatabaseConnection dbConnection;
    if(dbConnection.open())
    {
        // Block signals to ignore items selections changed
        ui->tableWidget_sales->blockSignals(true);

        Utilities utilities;
        QSqlQuery query;

        // Build query
        query.prepare("SELECT s.id, s.date, c.name, printf('%.2f', s.total_value)"
                      " FROM tb_sales s JOIN tb_collaborators c "
                      "ON s.id_collaborator = c.id WHERE s.date BETWEEN '" +
                      ui->dateEdit_initial->text() + ":00:00:00' AND '" +
                      ui->dateEdit_final->text() + ":23:59:59' ORDER BY s.id");

        // Update table widget
        if(!utilities.QueryToUpdateTableWidget(&query, ui->tableWidget_sales))
        {
            QMessageBox::warning(this, "Error", "Unable to read sales from database");
        }

        // Clean product sales table
        utilities.CleanTableWidget(ui->tableWidget_productsSales);
        ui->tableWidget_sales->clearSelection();
        ui->tableWidget_sales->setCurrentCell(-1, -1);

        dbConnection.close();

        // Restore signals
        ui->tableWidget_sales->blockSignals(false);
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to filter sales");
    }
}


void SalesManagementWindow::on_pushButton_allSales_clicked()
{
    ShowAllSalesIntoTableWidget();
}


void SalesManagementWindow::on_pushButton_export_clicked()
{
    DatabaseConnection dbConnection;
    if(dbConnection.open())
    {
        // Directory to save reports
        QString directoryPath = "reports/";

        // Create directory if it doesn't exist
        if (!QDir(directoryPath).exists())
        {
            QDir().mkdir(directoryPath);
        }

        // Get current date
        QDateTime currentDateTime = QDateTime::currentDateTime();

        // Convert to string
        QString currentDateTimeString = currentDateTime.toString("yyyy-MM-dd_hh_mm_ss");

        // Build report file
        std::string filePath = directoryPath.toStdString() + "report_" +
                               currentDateTimeString.toStdString() + ".txt";
        std::ofstream reportFile(filePath, std::ios::out);

        // Init sales quantity
        int quantSales = 0;

        // Open file
        if (reportFile.is_open())
        {
            // Build query
            QSqlQuery query1;
            query1.prepare("SELECT s.id, s.date, c.name, printf('%.2f', s.total_value)"
                          " FROM tb_sales s JOIN tb_collaborators c"
                          " ON s.id_collaborator = c.id ORDER BY s.id");

            // Execute query
            if(query1.exec())
            {
                // Write elements
                while(query1.next())
                {
                    // Check sales quantity
                    if(!quantSales)
                    {
                        // Write header
                        reportFile << "Report sales\n\n";
                    }

                    // Increment sales quantity
                    ++quantSales;

                    // Get sale information
                    int idSale = query1.value(0).toInt();
                    QString date = query1.value(1).toString();
                    QString collaborator = query1.value(2).toString();
                    double totalValue = query1.value(3).toDouble();

                    // Write elements
                    reportFile << ("Sale id: " + std::to_string(idSale) + "\n");
                    reportFile << ("Date: " + date.toStdString() + "\n");
                    reportFile << ("Collaborator: " + collaborator.toStdString() + "\n");
                    reportFile << ("Total sale: " + std::to_string(totalValue) + "\n");

                    // Build query
                    QSqlQuery query2;
                    query2.prepare("SELECT i.description, ps.quantity, ps.sale_price "
                                  "FROM tb_products_sales ps JOIN tb_inventory i ON ps.id_product = i.id "
                                  "WHERE ps.id_sale = " + QString::number(idSale) + " ORDER BY ps.id");

                    // Execute query
                    if(query2.exec())
                    {
                        // Write header
                        reportFile << "Products: \n";
                        while(query2.next())
                        {
                            // Get products information
                            QString product = query2.value(0).toString();
                            int quantity = query2.value(1).toInt();
                            double salePrice = query2.value(2).toDouble();

                            // Write products
                            reportFile << ("\tProduct: " + product.toStdString() + "\n");
                            reportFile << ("\tQuantity: " + std::to_string(quantity) + "\n");
                            reportFile << ("\tSale price: " + std::to_string(salePrice) + "\n\n");
                        }
                    }

                    reportFile << "\n";
                }
            }

            dbConnection.close();

            // Check sales quantity
            if(!quantSales)
            {
                reportFile << "There are no sales";
            }

            // Close file
            reportFile.close();

            // Show message box
            QMessageBox::information(this, "Information", "The report was created");
        }
        else
        {
            QMessageBox::warning(this, "Error", "Error to create file");
        }
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to export information");
    }
}


void SalesManagementWindow::on_pushButton_removeSale_clicked()
{
    // Get current line
    int currentRow = ui->tableWidget_sales->currentRow();

    // Check whether the sale is invalid
    if(currentRow < 0)
    {
        QMessageBox::warning(this, "Error", "Select a sale first");
        return;
    }

    // Remove sale
    DatabaseConnection dbConnection;
    if(dbConnection.open())
    {
        QMessageBox::StandardButton button = QMessageBox::question(this, "Remove",
                                                                   "Do you want to remove this sale?",
                                                                   QMessageBox::Yes | QMessageBox::No);

        if(button == QMessageBox::Yes)
        {
            // Block signals to ignore items selections changed
            ui->tableWidget_sales->blockSignals(true);

            // Get sale id
            int idSale = ui->tableWidget_sales->item(currentRow, 0)->text().toInt();

            // Build query
            QSqlQuery query;
            query.prepare("DELETE FROM tb_products_sales WHERE id_sale = " + QString::number(idSale));

            // Execute query
            if(query.exec())
            {
                // Build query
                query.prepare("DELETE FROM tb_sales WHERE id = " + QString::number(idSale));

                // Execute query
                if(query.exec())
                {
                    // Update table widget
                    ShowAllSalesIntoTableWidget();
                    QMessageBox::information(this, "Success", "Sale '" +
                                                                  QString::number(idSale) +
                                                                  "' removed with success");
                }
                else
                {
                    QMessageBox::warning(this, "Error", "Unable to remove sales from database");
                }
            }
            else
            {
                QMessageBox::warning(this, "Error", "Unable to remove product sales from database");
            }
        }

        dbConnection.close();

        // Restore signals
        ui->tableWidget_sales->blockSignals(false);
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to remove sale");
    }
}
