#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"
#include "saleswindow.h"
#include "salesmanagementwindow.h"
#include "inventorymanagementwindow.h"
#include "collaboratosmanagementwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    userLogged = false;
    ui->label_name->setText("No user logged in");
    lockedPadlock.addFile(":/images/lock.png");
    unlockedPadlock.addFile(":/images/unlock.png");

    ui->pushButton_Block->setText("");
    ui->pushButton_Block->setIcon(lockedPadlock);
    ui->statusbar->addWidget(ui->pushButton_Block);
    ui->statusbar->addWidget(ui->label_name);
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
        userLogged = loginWindow.isLogged();
        name_collaborator = loginWindow.getName();
        access_collaborator = loginWindow.getAccess();
        if(userLogged)
        {
            ui->pushButton_Block->setIcon(unlockedPadlock);
            ui->label_name->setText(name_collaborator);
        }
    }
    else
    {
        userLogged = false;
        ui->label_name->setText("No user logged in");
        ui->pushButton_Block->setIcon(lockedPadlock);
    }
}


void MainWindow::on_pushButton_NewSale_clicked()
{
    if(userLogged)
    {
        SalesWindow salesWindow;
        salesWindow.exec();
    }
    else
    {
        QMessageBox::information(this, "Information", "No collaborators logged in yet");
    }
}


void MainWindow::on_actionInventory_triggered()
{
    if(userLogged && access_collaborator == 'A')
    {
        InventoryManagementWindow inventoryManagementWindow;
        inventoryManagementWindow.exec();
    }
    else
    {
        QMessageBox::information(this, "Information", "Unauthorized access");
    }
}


void MainWindow::on_actionCollaborators_triggered()
{
    if(userLogged && access_collaborator == 'A')
    {
        CollaboratosManagementWindow collaboratorsManagementWindow;
        collaboratorsManagementWindow.exec();
    }
    else
    {
        QMessageBox::information(this, "Information", "Unauthorized access");
    }
}


void MainWindow::on_actionSales_triggered()
{
    if(userLogged && access_collaborator == 'A')
    {
        SalesManagementWindow salesManagementWindow;
        salesManagementWindow.exec();
    }
    else
    {
        QMessageBox::information(this, "Information", "Unauthorized access");
    }
}

