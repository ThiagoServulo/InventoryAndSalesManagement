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

    ui->tableWidget_sales->horizontalHeader()->setVisible(true);
    ui->tableWidget_sales->setColumnCount(5);
    ui->tableWidget_sales->setHorizontalHeaderLabels({"Id", "Date", "Collaborator", "Total", "Payment Type"});
    ui->tableWidget_sales->setStyleSheet("QTableView {selection-background-color: red}");
    ui->tableWidget_sales->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_sales->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_sales->verticalHeader()->setVisible(false);
    ui->tableWidget_sales->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget_sales->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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
