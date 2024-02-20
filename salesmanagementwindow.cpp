#include "salesmanagementwindow.h"
#include "ui_salesmanagementwindow.h"
#include "utilities.h"
#include <QDateEdit>
#include <QtSql>
#include <QMessageBox>
#include <QPrinter>
#include <QPainter>
#include <QDir>
#include <QDesktopServices>

SalesManagementWindow::SalesManagementWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SalesManagementWindow)
{
    ui->setupUi(this);

    Utilities utilities;

    // Window layout
    iconWindow.addFile(":/images/sale_management.png");
    this->setWindowIcon(iconWindow);
    this->setWindowTitle("Sales Management");
    this->setFixedSize(789, 522);

    // Configure data format
    ui->dateEdit_initial->setDisplayFormat("yyyy-MM-dd");
    ui->dateEdit_final->setDisplayFormat("yyyy-MM-dd");

    // Init with a date
    QDate today = QDate::currentDate();
    QDate yesterday = today.addDays(-1);
    ui->dateEdit_initial->setDate(yesterday);
    ui->dateEdit_final->setDate(today);

    // Configure products sales table
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
    if(dbConnection.open())
    {
        QSqlQuery query;
        query.prepare("SELECT s.id, s.date, COALESCE(c.name, 'not found'), "
                      "printf('%.2f', s.total_value) "
                      "FROM tb_sales s LEFT JOIN tb_collaborators c "
                      "ON s.id_collaborator = c.id ORDER BY s.id");

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
    int currentRow = ui->tableWidget_sales->currentRow();

    // Check if is a valid row
    if(currentRow < 0)
    {
        return;
    }

    // Show sales products
    if(dbConnection.open())
    {
        Utilities utilities;
        QSqlQuery query;

        // Get sale id
        int idSale = ui->tableWidget_sales->item(currentRow, 0)->text().toInt();

        query.prepare("SELECT i.description, ps.quantity, printf('%.2f', ps.sale_price), "
                      "printf('%.2f', ps.quantity * ps.sale_price) "
                      "FROM tb_products_sales ps JOIN tb_inventory i ON ps.id_product = i.id "
                      "WHERE ps.id_sale = " + QString::number(idSale) + " ORDER BY ps.id");

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
    if(dbConnection.open())
    {
        // Block signals to ignore items selections changed
        ui->tableWidget_sales->blockSignals(true);

        Utilities utilities;
        QSqlQuery query;
        query.prepare("SELECT s.id, s.date, c.name, printf('%.2f', s.total_value)"
                      " FROM tb_sales s JOIN tb_collaborators c "
                      "ON s.id_collaborator = c.id WHERE s.date BETWEEN '" +
                      ui->dateEdit_initial->text() + "' AND '" +
                      ui->dateEdit_final->text() + "' ORDER BY s.id");

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

void SalesManagementWindow::on_pushButton_exportPdf_clicked()
{
    QString nameFile = ui->tableWidget_sales->item(ui->tableWidget_sales->currentRow(), 0)->text() + "_sale.pdf";
    QString path = QDir::currentPath();

    QString file = path + "/" + nameFile;

    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(file);

    QPainter painter;
    if(!painter.begin(&printer))
    {
        QMessageBox::warning(this, "Error", "Unable to open pdf");
    }
    else
    {
        painter.drawText(25, 25, "Sale: " + ui->tableWidget_sales->item(ui->tableWidget_sales->currentRow(), 0)->text());
        painter.drawText(100, 25, "Date: " + ui->tableWidget_sales->item(ui->tableWidget_sales->currentRow(), 1)->text());

        for(int i = 0, line = 100 ; i < ui->tableWidget_productsSales->rowCount(); i++, line += 20)
        {
            painter.drawText(25, line, "Id: " + ui->tableWidget_productsSales->item(i, 0)->text());
            painter.drawText(75, line, "Product Id: " + ui->tableWidget_productsSales->item(i, 1)->text());
            painter.drawText(200, line, "Quantity: " + ui->tableWidget_productsSales->item(i, 2)->text());
            painter.drawText(300, line, "Unitary Price: " + ui->tableWidget_productsSales->item(i, 3)->text());
            painter.drawText(400, line, "Total Price: " + ui->tableWidget_productsSales->item(i, 4)->text());
        }

        painter.end();

        QDesktopServices::openUrl(QUrl("file:///" + file));
    }
}

void SalesManagementWindow::on_pushButton_removeSale_clicked()
{
    int currentRow = ui->tableWidget_sales->currentRow();

    // Sale invalid
    if(currentRow < 0)
    {
        QMessageBox::warning(this, "Error", "Select a sale first");
        return;
    }

    // Remove sale
    if(dbConnection.open())
    {
        QMessageBox::StandardButton button = QMessageBox::question(this, "Remove", "Do you want to remove this sale?", QMessageBox::Yes | QMessageBox::No);

        if(button == QMessageBox::Yes)
        {
            // Block signals to ignore items selections changed
            ui->tableWidget_sales->blockSignals(true);

            int idSale = ui->tableWidget_sales->item(currentRow, 0)->text().toInt();
            QSqlQuery query;
            query.prepare("DELETE FROM tb_products_sales WHERE id_sale = " + QString::number(idSale));

            if(query.exec())
            {
                query.prepare("DELETE FROM tb_sales WHERE id = " + QString::number(idSale));

                if(query.exec())
                {
                    ShowAllSalesIntoTableWidget();
                    QMessageBox::information(this, "Success", "Sale '" + QString::number(idSale) + "' removed with success");
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
