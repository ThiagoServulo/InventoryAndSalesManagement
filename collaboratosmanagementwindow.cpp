#include "collaboratosmanagementwindow.h"
#include "ui_collaboratosmanagementwindow.h"
#include <QMessageBox>

CollaboratosManagementWindow::CollaboratosManagementWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CollaboratosManagementWindow)
{
    ui->setupUi(this);

    if(!dbConnection.open())
    {
        QMessageBox::warning(this, "Error", "Unable to connect database");
    }

    ui->comboBox_nc_accessType->addItem("A");
    ui->comboBox_nc_accessType->addItem("B");
    ui->lineEdit_nc_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_nc_confirmPassword->setEchoMode(QLineEdit::Password);
    ui->lineEdit_nc_name->setFocus();
}

CollaboratosManagementWindow::~CollaboratosManagementWindow()
{
    delete ui;
}

void CollaboratosManagementWindow::on_pushButton_nc_save_clicked()
{

}

