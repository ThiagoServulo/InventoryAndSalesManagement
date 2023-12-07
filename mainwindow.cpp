#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    block=false;

    lockedPadlock.addFile(":/images/lock.png");
    unlockedPadlock.addFile(":/images/unlock.png");

    ui->pushButton_Block->setText("");
    ui->pushButton_Block->setIcon(unlockedPadlock);

    ui->statusbar->addWidget(ui->pushButton_Block);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_Block_clicked()
{
    if(block)
    {
        // Call block window
    }
    else
    {
        block = true;
        ui->pushButton_Block->setIcon(lockedPadlock);
    }
}

