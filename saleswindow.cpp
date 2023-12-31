#include "saleswindow.h"
#include "ui_saleswindow.h"
#include "editproductfromsalewindow.h"
#include "mainwindow.h"
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

    InitFieldsWindow();
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
    ui->pushButton_editProduct->setAutoDefault(false);
    ui->pushButton_finalizeSale->setAutoDefault(false);
    ui->pushButton_removeProduct->setAutoDefault(false);
    ui->pushButton_searchProduct->setAutoDefault(false);
    ui->lineEdit_total->setEnabled(false);
}

SalesWindow::~SalesWindow()
{
    delete ui;
}

void SalesWindow::on_lineEdit_idProduct_returnPressed()
{
    InsertProductIntoTableWidget();
}

void SalesWindow::InsertProductIntoTableWidget()
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

            total = CalculateTotalSale(ui->tableWidget_listProducts, 4);
            ui->lineEdit_total->setText(QString::number(total));
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

    InitFieldsWindow();
}

void SalesWindow::InitFieldsWindow()
{
    ui->lineEdit_quantity->setText("1");
    ui->lineEdit_idProduct->clear();
    ui->lineEdit_idProduct->setFocus();
}

float SalesWindow::CalculateTotalSale(QTableWidget *tableWidget, int column)
{
    float total = 0;

    for(int i = 0; i < tableWidget->rowCount(); i++)
    {
        total += tableWidget->item(i, column)->text().toFloat();
    }

    return total;
}

void SalesWindow::on_pushButton_removeProduct_clicked()
{
    if(ui->tableWidget_listProducts->currentColumn() != -1)
    {
        QMessageBox::StandardButton option;
        option = QMessageBox::question(this, "Remove", "Do you want to remove this product?", QMessageBox::Yes | QMessageBox::No);
        if(option == QMessageBox::Yes)
        {
            ui->tableWidget_listProducts->removeRow(ui->tableWidget_listProducts->currentRow());
            ui->lineEdit_total->setText(QString::number(CalculateTotalSale(ui->tableWidget_listProducts, 4)));
            ui->tableWidget_listProducts->setCurrentCell(-1, -1);
        }
    }
    else
    {
        QMessageBox::information(this, "Error", "Select a product first");
    }
}

void SalesWindow::on_pushButton_searchProduct_clicked()
{
    InsertProductIntoTableWidget();
}

void SalesWindow::on_pushButton_editProduct_clicked()
{
    if(ui->tableWidget_listProducts->currentRow() != -1)
    {
        int idProduct = ui->tableWidget_listProducts->item(ui->tableWidget_listProducts->currentRow(), 0)->text().toInt();
        QString description = ui->tableWidget_listProducts->item(ui->tableWidget_listProducts->currentRow(), 1)->text();
        float salePrice = ui->tableWidget_listProducts->item(ui->tableWidget_listProducts->currentRow(), 2)->text().toFloat();
        int quantity = ui->tableWidget_listProducts->item(ui->tableWidget_listProducts->currentRow(), 3)->text().toInt();

        EditProductFromSaleWindow *editProductFromSaleWindow;
        editProductFromSaleWindow = new EditProductFromSaleWindow(idProduct, description, salePrice, quantity);
        editProductFromSaleWindow->exec();

        ui->tableWidget_listProducts->setItem(ui->tableWidget_listProducts->currentRow(), 2, new QTableWidgetItem(QString::number(editProductFromSaleWindow->salePrice)));
        ui->tableWidget_listProducts->setItem(ui->tableWidget_listProducts->currentRow(), 3, new QTableWidgetItem(QString::number(editProductFromSaleWindow->quantity)));

        float total = editProductFromSaleWindow->quantity * editProductFromSaleWindow->salePrice;
        ui->tableWidget_listProducts->setItem(ui->tableWidget_listProducts->currentRow(), 4, new QTableWidgetItem(QString::number(total)));

        total = CalculateTotalSale(ui->tableWidget_listProducts, 4);
        ui->lineEdit_total->setText(QString::number(total));
        ui->tableWidget_listProducts->setCurrentCell(-1, -1);
    }
    else
    {
        QMessageBox::information(this, "Error", "Select a product first");
    }
}

void SalesWindow::EraseTableWidget(QTableWidget *tableWidget)
{
    while(tableWidget->rowCount() > 0)
    {
        tableWidget->removeRow(0);
    }
}

void SalesWindow::on_pushButton_finalizeSale_clicked()
{
    if(ui->tableWidget_listProducts->rowCount() != 0)
    {
        int id_collaborator = MainWindow::id_collaborator;
        int id_payment_type = 1;
        float total = CalculateTotalSale(ui->tableWidget_listProducts, 4);
        QString date = QDate::currentDate().toString("yyyy-MM-dd") +  QTime::currentTime().toString(":hh:mm:ss");
        QSqlQuery query;
        query.prepare("INSERT INTO tb_sales (date, id_collaborator, total_value, id_payment_type) VALUES ('" + date
                      + "', " + QString::number(id_collaborator) + ", " +  QString::number(total) + ", "
                      + QString::number(id_payment_type) + ")");

        if(query.exec())
        {
            query.prepare("SELECT id FROM tb_sales ORDER BY id DESC LIMIT 1");
            if(query.exec())
            {
                query.first();
                int id_sale = query.value(0).toInt();

                for(int i = 0; i < ui->tableWidget_listProducts->rowCount(); i++)
                {
                    int id_product = ui->tableWidget_listProducts->item(i, 0)->text().toInt();
                    float sale_price = ui->tableWidget_listProducts->item(i, 2)->text().toFloat();
                    int quantity = ui->tableWidget_listProducts->item(i, 3)->text().toInt();
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
            EraseTableWidget(ui->tableWidget_listProducts);
            ui->lineEdit_total->setText("0");
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

