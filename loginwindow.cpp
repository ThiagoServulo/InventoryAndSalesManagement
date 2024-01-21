#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    iconWindow.addFile(":/images/login.png");
    this->setWindowIcon(iconWindow);
    this->setWindowTitle("Login");
    this->setFixedSize(400, 157);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_pushButton_Login_clicked()
{
    QString username = ui->lineEdit_Username->text();
    QString password = ui->lineEdit_Password->text();

    if(username == "" || password == "")
    {
        QMessageBox::information(this, "Information", "Enter your credentials to log in");
        return;
    }

    if(!dbConnection.open())
    {
        QMessageBox::warning(this, "Error", "Unable to connect database");
    }
    else
    {
        QSqlQuery query;
        query.prepare("SELECT * FROM tb_collaborators WHERE username = '" + username + "'");
        if(query.exec())
        {
            query.first();
            if(query.value(1).toString() != "")
            {
                if(query.value(3).toString() == password)
                {
                    MainWindow::userLogged = true;
                    MainWindow::id_collaborator = query.value(0).toInt();
                    MainWindow::name_collaborator = query.value(1).toString();
                    MainWindow::access_collaborator = query.value(5).toInt();
                    dbConnection.close();
                    close();
                    return;
                }
                else
                {
                    QMessageBox::warning(this, "Error", "Password incorrect");
                }
            }
            else
            {
                QMessageBox::warning(this, "Error", "Username not found");
            }
        }
        else
        {
            QMessageBox::warning(this, "Error", "Query error");
        }
        dbConnection.close();
    }
}

void LoginWindow::on_pushButton_Cancel_clicked()
{
    MainWindow::userLogged = false;
    close();
}
