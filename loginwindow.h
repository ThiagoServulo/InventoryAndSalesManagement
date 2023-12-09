#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include "databaseconnection.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    // Properties
    DatabaseConnection dbConnection;

    // Functions
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    bool logged();

private slots:
    void on_pushButton_Login_clicked();

private:
    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
