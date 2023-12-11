#ifndef INVENTORYMANAGEMENTWINDOW_H
#define INVENTORYMANAGEMENTWINDOW_H

#include <QDialog>

namespace Ui {
class InventoryManagementWindow;
}

class InventoryManagementWindow : public QDialog
{
    Q_OBJECT

public:
    explicit InventoryManagementWindow(QWidget *parent = nullptr);
    ~InventoryManagementWindow();

private:
    Ui::InventoryManagementWindow *ui;
};

#endif // INVENTORYMANAGEMENTWINDOW_H
