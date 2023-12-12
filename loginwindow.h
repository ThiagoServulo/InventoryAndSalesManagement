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
    int id;
    bool logged;
    QString name, access;
    DatabaseConnection dbConnection;

    // Functions
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_pushButton_Login_clicked();
    void on_pushButton_Cancel_clicked();

private:
    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
