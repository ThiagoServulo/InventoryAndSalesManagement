#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"
#include "definepasswordwindow.h"
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    // Window layout
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

    // Check if username and password ate valid
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
                    // Get user logged information
                    MainWindow::userLogged = true;
                    MainWindow::id_collaborator = query.value(0).toInt();
                    MainWindow::name_collaborator = query.value(1).toString();
                    MainWindow::access_collaborator = query.value(5).toInt();
                    dbConnection.close();

                    // Check if the password must be redefined
                    if(password == "1234")
                    {
                        QMessageBox::warning(this, "Warning", "You need to redefine your password");

                        // Show define password window
                        DefinePasswordWindow definePasswordWindow;
                        QObject::connect(&definePasswordWindow, &QDialog::finished, [&](int result)
                        {
                            // Only perform login if password reset is accepted
                            if (result == QDialog::Accepted)
                            {
                                close();
                            }
                        });
                        definePasswordWindow.exec();
                    }

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
