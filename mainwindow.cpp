#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"
#include "saleswindow.h"
#include "salesmanagementwindow.h"
#include "inventorymanagementwindow.h"
#include "collaboratosmanagementwindow.h"
#include <QMessageBox>

// Globals variables
int MainWindow::id_collaborator;
QString MainWindow::name_collaborator;
int MainWindow::access_collaborator;
QString MainWindow::username_collaborator;
bool MainWindow::userLogged;

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
    if(userLogged && access_collaborator == 1)
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
    if(userLogged && access_collaborator == 1)
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
    if(userLogged && access_collaborator == 1)
    {
        SalesManagementWindow salesManagementWindow;
        salesManagementWindow.exec();
    }
    else
    {
        QMessageBox::information(this, "Information", "Unauthorized access");
    }
}

