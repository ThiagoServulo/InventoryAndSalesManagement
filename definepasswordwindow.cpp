#include "definepasswordwindow.h"
#include "ui_definepasswordwindow.h"

DefinePasswordWindow::DefinePasswordWindow(QWidget *parent) :
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

    QIcon eyeCloseIcon(":/images/eye_close.png");
    QIcon eyeOpenIcon(":/images/eye_open.png");

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
    accept();
    close();
}

