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
    // Properties
    DatabaseConnection dbConnection;

    // Functions
    explicit SalesWindow(QWidget *parent = nullptr);
    ~SalesWindow();

private slots:
    void on_lineEdit_idProduct_returnPressed();
    void InsertProductIntoTableWidget();
    void InitFieldsWindow();
    float CalculateTotalSale(QTableWidget *tableWidget, int column);
    void on_pushButton_removeProduct_clicked();
    void on_pushButton_searchProduct_clicked();

    void on_pushButton_editProduct_clicked();

private:
    Ui::SalesWindow *ui;
};

#endif // SALESWINDOW_H
