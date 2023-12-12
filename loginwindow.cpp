#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    logged = false;
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_pushButton_Login_clicked()
{
    if(!dbConnection.open())
    {
        QMessageBox::warning(this, "Error", "Unable to connect database");
    }
    else
    {
        QString username = ui->lineEdit_Username->text();
        QString password = ui->lineEdit_Password->text();
        QSqlQuery query;
        query.prepare("SELECT * FROM tb_collaborators WHERE username = '" + username +
                      "' AND password = '" + password + "'");
        if(query.exec())
        {
            query.first();
            if(query.value(1).toString() != "")
            {
                MainWindow::userLogged = true;
                MainWindow::id_collaborator = query.value(0).toInt();
                MainWindow::name_collaborator = query.value(1).toString();
                MainWindow::access_collaborator = query.value(5).toString();
                dbConnection.close();
                close();
            }
            else
            {
                QMessageBox::warning(this, "Error", "Collaborator not found");
            }
        }
        else
        {
            QMessageBox::warning(this, "Error", "The query returned empty");
        }
    }
    dbConnection.close();
}

void LoginWindow::on_pushButton_Cancel_clicked()
{
    MainWindow::userLogged = false;
    close();
}
