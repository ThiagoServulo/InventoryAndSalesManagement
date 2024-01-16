#ifndef INVENTORYMANAGEMENTWINDOW_H
#define INVENTORYMANAGEMENTWINDOW_H

#include <QDialog>
#include <QTableWidget>
#include "databaseconnection.h"

namespace Ui {
class InventoryManagementWindow;
}

class InventoryManagementWindow : public QDialog
{
    Q_OBJECT

public:
    explicit InventoryManagementWindow(QWidget *parent = nullptr);
    ~InventoryManagementWindow();

private slots:
    void on_pushButton_newProduct_save_clicked();
    void on_pushButton_newProduct_cancel_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_tableWidget_inventoryManagement_itemSelectionChanged();
    void on_pushButton_inventoryManagement_save_clicked();
    void on_pushButton_inventoryManagement_remove_clicked();
    void on_pushButton_inventoryManagement_search_clicked();

private:
    Ui::InventoryManagementWindow *ui;
    DatabaseConnection dbConnection;
    int ProductExists(int id);
    void UpdateInventoryManagementTableWidget();
    void ClearNewProductTabFields();
    bool CheckValidProduct(float purchasePrice, float salePrice, int quantity, QString description, QString supplier);
    void ClearInventoryManagementTabFields();
};

#endif // INVENTORYMANAGEMENTWINDOW_H
