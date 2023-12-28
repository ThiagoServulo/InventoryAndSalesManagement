#include "salesmanagementwindow.h"
#include "ui_salesmanagementwindow.h"
#include "utilities.h"
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

    if(!dbConnection.open())
    {
        QMessageBox::warning(this, "Error", "Unable to connect database");
    }

    ui->dateEdit_initial->setDisplayFormat("yyyy-MM-dd");
    ui->dateEdit_final->setDisplayFormat("yyyy-MM-dd");

    ui->tableWidget_productsSales->horizontalHeader()->setVisible(true);
    ui->tableWidget_productsSales->setColumnCount(5);
    ui->tableWidget_productsSales->setHorizontalHeaderLabels({"Id", "Product Id", "Quantity", "Unitary Price", "Total Price"});
    ui->tableWidget_productsSales->setStyleSheet("QTableView {selection-background-color: red}");
    ui->tableWidget_productsSales->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_productsSales->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_productsSales->verticalHeader()->setVisible(false);
    ui->tableWidget_productsSales->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget_productsSales->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget_sales->horizontalHeader()->setVisible(true);
    ui->tableWidget_sales->setColumnCount(5);
    ui->tableWidget_sales->setHorizontalHeaderLabels({"Id", "Date", "Collaborator", "Total", "Payment Type"});
    ui->tableWidget_sales->setStyleSheet("QTableView {selection-background-color: red}");
    ui->tableWidget_sales->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_sales->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_sales->verticalHeader()->setVisible(false);
    ui->tableWidget_sales->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget_sales->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    Utilities utilities;
    utilities.CleanTableWidget(ui->tableWidget_productsSales);

    ShowAllSales();
}

void SalesManagementWindow::ShowAllSales()
{
    Utilities utilities;
    QSqlQuery query;
    query.prepare("SELECT * FROM tb_sales ORDER BY id");
    if(!utilities.QueryToUpdateTableWidget(&query, ui->tableWidget_sales))
    {
        QMessageBox::warning(this, "Error", "Unable to read sales from database");
    }
}

SalesManagementWindow::~SalesManagementWindow()
{
    delete ui;
}

void SalesManagementWindow::on_tableWidget_sales_itemSelectionChanged()
{
    Utilities utilities;
    QSqlQuery query;
    int idSale = ui->tableWidget_sales->item(ui->tableWidget_sales->currentRow(), 0)->text().toInt();
    query.prepare("SELECT id, id_product, quantity, sale_price, quantity * sale_price FROM tb_products_sales WHERE id_sale = " +
                  QString::number(idSale) + " ORDER BY id");
    if(!utilities.QueryToUpdateTableWidget(&query, ui->tableWidget_productsSales))
    {
        QMessageBox::warning(this, "Error", "Unable to read prducts sales from database");
    }
}


void SalesManagementWindow::on_pushButton_filter_clicked()
{
    Utilities utilities;
    QSqlQuery query;
    query.prepare("SELECT * FROM tb_sales WHERE date BETWEEN '" + ui->dateEdit_initial->text() + "' AND '" +
                  ui->dateEdit_final->text() + "' ORDER BY id");
    if(!utilities.QueryToUpdateTableWidget(&query, ui->tableWidget_sales))
    {
        QMessageBox::warning(this, "Error", "Unable to read sales from database");
    }
}


void SalesManagementWindow::on_pushButton_allSales_clicked()
{
    ShowAllSales();
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

