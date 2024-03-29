#ifndef SALESMANAGEMENTWINDOW_H
#define SALESMANAGEMENTWINDOW_H

#include <QDialog>

namespace Ui { class SalesManagementWindow; }

class SalesManagementWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SalesManagementWindow(QWidget *parent = nullptr);
    ~SalesManagementWindow();

private slots:
    void on_tableWidget_sales_itemSelectionChanged();
    void on_pushButton_filter_clicked();
    void on_pushButton_allSales_clicked();
    void on_pushButton_removeSale_clicked();
    void on_pushButton_export_clicked();

private:
    Ui::SalesManagementWindow *ui;
    void ShowAllSalesIntoTableWidget();
};

#endif // SALESMANAGEMENTWINDOW_H
