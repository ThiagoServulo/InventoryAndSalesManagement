#include "definepasswordwindow.h"
#include "ui_definepasswordwindow.h"
#include "databaseconnection.h"
#include <QMessageBox>


DefinePasswordWindow::DefinePasswordWindow(QString username, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DefinePasswordWindow)
{
    ui->setupUi(this);

    // Window layout
    QIcon iconWindow;
    iconWindow.addFile(":/images/login.png");
    this->setWindowIcon(iconWindow);
    this->setWindowTitle("Define password");
    this->setFixedSize(366, 168);

    // Update username class variable
    this->username = username;

    // Init icons
    eyeCloseIcon.addFile(":/images/eye_close.png");
    eyeOpenIcon.addFile(":/images/eye_open.png");

    // Init buttons and line edit
    ui->pushButton_eye_confirmPassword->setIcon(eyeOpenIcon);
    ui->pushButton_eye_password->setIcon(eyeOpenIcon);
    ui->lineEdit_confirmPassword->setEchoMode(QLineEdit::Password);
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
}


DefinePasswordWindow::~DefinePasswordWindow()
{
    delete ui;
}


void DefinePasswordWindow::on_pushButton_confirm_clicked()
{
    // Check the password length
    if((ui->lineEdit_password->text().length() < 5) || (ui->lineEdit_password->text().length() > 10))
    {
        QMessageBox::warning(this, "Warning", "A password must have between 5 and 10 characters");
        return;
    }

    // Check if the passwords are the same
    if(ui->lineEdit_password->text() != ui->lineEdit_confirmPassword->text())
    {
        QMessageBox::warning(this, "Warning", "The passwords must be the same");
        return;
    }

    // The password is valid, update database
    DatabaseConnection dbConnection;
    if(!dbConnection.open())
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to update password");
    }
    else
    {
        //QSqlQuery query;
        //query.prepare("SELECT id FROM tb_access_type WHERE type = '" + accessType + "'");
        qDebug() << username;
        dbConnection.close();
    }

    accept();
    close();
}


void DefinePasswordWindow::on_pushButton_eye_password_clicked()
{
    if(ui->lineEdit_password->echoMode() == QLineEdit::Password)
    {
        // Show password
        ui->lineEdit_password->setEchoMode(QLineEdit::Normal);
        ui->pushButton_eye_password->setIcon(eyeCloseIcon);
    }
    else
    {
        // Hide password
        ui->lineEdit_password->setEchoMode(QLineEdit::Password);
        ui->pushButton_eye_password->setIcon(eyeOpenIcon);
    }
}


void DefinePasswordWindow::on_pushButton_eye_confirmPassword_clicked()
{
    if(ui->lineEdit_confirmPassword->echoMode() == QLineEdit::Password)
    {
        // Show password
        ui->lineEdit_confirmPassword->setEchoMode(QLineEdit::Normal);
        ui->pushButton_eye_confirmPassword->setIcon(eyeCloseIcon);
    }
    else
    {
        // Hide password
        ui->lineEdit_confirmPassword->setEchoMode(QLineEdit::Password);
        ui->pushButton_eye_confirmPassword->setIcon(eyeOpenIcon);
    }
}

