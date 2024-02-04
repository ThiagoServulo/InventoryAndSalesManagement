#ifndef SALESWINDOW_H
#define SALESWINDOW_H

#include <QDialog>
#include <QTableWidget>
#include "databaseconnection.h"

namespace Ui {
class SalesWindow;
}

class SalesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SalesWindow(QWidget *parent = nullptr);
    ~SalesWindow();

private slots:
    void on_lineEdit_idProduct_returnPressed();    
    void on_pushButton_removeProduct_clicked();
    void on_pushButton_searchProduct_clicked();
    void on_pushButton_editProduct_clicked();  
    void on_pushButton_finalizeSale_clicked();

private:
    Ui::SalesWindow *ui;
    DatabaseConnection dbConnection;
    void UpdateProductQuantiy(int id_product, int quantity);
    void EraseTableWidget(QTableWidget *tableWidget);
    void InsertProductIntoTableWidget();
    void InitFieldsWindow();
    float CalculateTotalSale(QTableWidget *tableWidget, int column);
};

#endif // SALESWINDOW_H
