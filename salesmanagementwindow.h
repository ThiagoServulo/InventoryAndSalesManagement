#ifndef SALESMANAGEMENTWINDOW_H
#define SALESMANAGEMENTWINDOW_H

#include <QDialog>
#include "databaseconnection.h"

namespace Ui {
class SalesManagementWindow;
}

class SalesManagementWindow : public QDialog
{
    Q_OBJECT

public:
    DatabaseConnection dbConnection;
    explicit SalesManagementWindow(QWidget *parent = nullptr);
    ~SalesManagementWindow();

private slots:
    void on_tableWidget_sales_itemSelectionChanged();

    void on_pushButton_filter_clicked();

    void on_pushButton_allSales_clicked();

private:
    Ui::SalesManagementWindow *ui;
    void ShowAllSales();
};

#endif // SALESMANAGEMENTWINDOW_H
