#include "saleswindow.h"
#include "ui_saleswindow.h"
#include <QMessageBox>

SalesWindow::SalesWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SalesWindow)
{
    ui->setupUi(this);

    if(!dbConnection.open())
    {
        QMessageBox::warning(this, "Error", "Unable to connect database");
    }

    ui->lineEdit_quantity->setText("1");
    ui->lineEdit_quantity->setFocus();
    ui->tableWidget_listProducts->setColumnCount(5);
    ui->tableWidget_listProducts->setColumnWidth(0, 100);
    ui->tableWidget_listProducts->setColumnWidth(1, 200);
    ui->tableWidget_listProducts->setColumnWidth(2, 100);
    ui->tableWidget_listProducts->setColumnWidth(3, 100);
    ui->tableWidget_listProducts->setColumnWidth(4, 100);
    ui->tableWidget_listProducts->setHorizontalHeaderLabels({"Id", "Description", "Unit value", "Quantity", "Total"});
    ui->tableWidget_listProducts->setStyleSheet("QTableView {selection-background-color: red}");
    ui->tableWidget_listProducts->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_listProducts->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_listProducts->verticalHeader()->setVisible(false);
}

SalesWindow::~SalesWindow()
{
    delete ui;
}

void SalesWindow::on_lineEdit_idProduct_returnPressed()
{
    float total;
    QSqlQuery query;
    query.prepare("SELECT id, description, sale_price FROM tb_inventory WHERE id = " + ui->lineEdit_idProduct->text());
    if(query.exec())
    {
        query.first();
        if(query.value(0).toString() != "")
        {
            int line = 0;
            ui->tableWidget_listProducts->insertRow(line);
            ui->tableWidget_listProducts->setItem(line, 0, new QTableWidgetItem(query.value(0).toString()));
            ui->tableWidget_listProducts->setItem(line, 1, new QTableWidgetItem(query.value(1).toString()));
            ui->tableWidget_listProducts->setItem(line, 2, new QTableWidgetItem(query.value(2).toString()));
            ui->tableWidget_listProducts->setItem(line, 3, new QTableWidgetItem(ui->lineEdit_quantity->text()));
            total = ui->lineEdit_quantity->text().toInt() * query.value(2).toString().toFloat();
            ui->tableWidget_listProducts->setItem(line, 4, new QTableWidgetItem(QString::number(total)));
            ui->tableWidget_listProducts->setRowHeight(line, 20);
        }
        else
        {
            QMessageBox::warning(this, "Error", "Product not found");
        }
    }
    else
    {
        QMessageBox::warning(this, "Error", "Error to search this product");
    }

    ui->lineEdit_quantity->setText("1");
    ui->lineEdit_idProduct->clear();
}

