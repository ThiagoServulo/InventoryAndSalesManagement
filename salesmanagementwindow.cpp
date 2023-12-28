#include "salesmanagementwindow.h"
#include "ui_salesmanagementwindow.h"
#include "utilities.h"
#include <QtSql>
#include <QMessageBox>

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

