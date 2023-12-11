#ifndef SALESMANAGEMENTWINDOW_H
#define SALESMANAGEMENTWINDOW_H

#include <QDialog>

namespace Ui {
class SalesManagementWindow;
}

class SalesManagementWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SalesManagementWindow(QWidget *parent = nullptr);
    ~SalesManagementWindow();

private:
    Ui::SalesManagementWindow *ui;
};

#endif // SALESMANAGEMENTWINDOW_H
