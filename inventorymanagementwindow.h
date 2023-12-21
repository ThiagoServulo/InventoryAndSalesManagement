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
    // Properties
    DatabaseConnection dbConnection;

    // Functions
    explicit InventoryManagementWindow(QWidget *parent = nullptr);
    ~InventoryManagementWindow();
    void CleanTableWidget(QTableWidget *tableWidget);

private slots:
    void on_pushButton_saveNewProduct_clicked();
    void on_pushButton_cancel_clicked();
    void ClearNewProductTabFields();
    void on_tabWidget_currentChanged(int index);
    void UpdateIMTableWidget();
    void on_tableWidget_im_inventory_itemSelectionChanged();
    void on_pushButton_im_save_clicked();
    void on_pushButton_im_remove_clicked();
    void on_pushButton_im_search_clicked();

private:
    Ui::InventoryManagementWindow *ui;
};

#endif // INVENTORYMANAGEMENTWINDOW_H
