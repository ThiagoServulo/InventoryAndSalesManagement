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

private:
    Ui::SalesManagementWindow *ui;
    void ShowAllSales();
};

#endif // SALESMANAGEMENTWINDOW_H
