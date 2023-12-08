#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString path = qApp->applicationDirPath();
    QString db = path + "/database/InventoryManagement.db";
    database.setDatabaseName(db);

    userLogged=false;
    lockedPadlock.addFile(":/images/lock.png");
    unlockedPadlock.addFile(":/images/unlock.png");

    ui->pushButton_Block->setText("");
    ui->pushButton_Block->setIcon(lockedPadlock);
    ui->statusbar->addWidget(ui->pushButton_Block);

    if(!database.open())
    {
        QMessageBox::warning(this, "Error", "Unable to connect database");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_Block_clicked()
{
    if(!userLogged)
    {
        LoginWindow loginWindow;
        loginWindow.exec();
    }
    else
    {
        userLogged = true;
        ui->pushButton_Block->setIcon(unlockedPadlock);
    }
}

