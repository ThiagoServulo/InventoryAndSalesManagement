#include "collaboratosmanagementwindow.h"
#include "ui_collaboratosmanagementwindow.h"

CollaboratosManagementWindow::CollaboratosManagementWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CollaboratosManagementWindow)
{
    ui->setupUi(this);
}

CollaboratosManagementWindow::~CollaboratosManagementWindow()
{
    delete ui;
}
