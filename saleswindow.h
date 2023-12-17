#ifndef SALESWINDOW_H
#define SALESWINDOW_H

#include <QDialog>
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

private:
    Ui::SalesWindow *ui;
};

#endif // SALESWINDOW_H
