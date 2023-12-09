#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
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
            // Create verification
        }
    }
    dbConnection.close();
}

