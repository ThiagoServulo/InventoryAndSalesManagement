#ifndef INVENTORYMANAGEMENTWINDOW_H
#define INVENTORYMANAGEMENTWINDOW_H

#include <QDialog>
#include "databaseconnection.h"

namespace Ui {
class InventoryManagementWindow;
}

class InventoryManagementWindow : public QDialog
{
    Q_OBJECT

public:
    // Properties
    DatabaseConnection dbConnection;

    // Functions
    explicit InventoryManagementWindow(QWidget *parent = nullptr);
    ~InventoryManagementWindow();

private slots:
    void on_pushButton_saveNewProduct_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::InventoryManagementWindow *ui;
};

#endif // INVENTORYMANAGEMENTWINDOW_H
