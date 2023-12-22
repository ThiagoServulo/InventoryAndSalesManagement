#include "collaboratosmanagementwindow.h"
#include "ui_collaboratosmanagementwindow.h"
#include "utilities.h"
#include <QSql>
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

    ClearNewCollaboratorTabFields();

    ui->comboBox_nc_accessType->addItem("A");
    ui->comboBox_nc_accessType->addItem("B");
    ui->lineEdit_nc_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_nc_confirmPassword->setEchoMode(QLineEdit::Password);
    ui->lineEdit_nc_name->setFocus();

    ui->tableWidget_cm_collaborators->setColumnCount(2);
    ui->tableWidget_cm_collaborators->setColumnWidth(0, 80);
    ui->tableWidget_cm_collaborators->setColumnWidth(1, 150);
    ui->tableWidget_cm_collaborators->setStyleSheet("QTableView {selection-background-color: red}");
    ui->tableWidget_cm_collaborators->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_cm_collaborators->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_cm_collaborators->verticalHeader()->setVisible(false);
    ui->tableWidget_cm_collaborators->setHorizontalHeaderLabels({"Id", "Name"});
    ui->radioButton_cm_idCollaborator->setChecked(true);
    ui->tabWidget->setCurrentIndex(0);
}

CollaboratosManagementWindow::~CollaboratosManagementWindow()
{
    delete ui;
}

void CollaboratosManagementWindow::on_pushButton_nc_save_clicked()
{
    if(ui->lineEdit_nc_name->text() != "" && ui->lineEdit_nc_name->text() != "" &&
        ui->lineEdit_nc_telephone->text() != "" && ui->lineEdit_nc_password->text() != "" &&
        ui->lineEdit_nc_confirmPassword->text() != "")
    {
        if(ui->lineEdit_nc_password->text() == ui->lineEdit_nc_confirmPassword->text())
        {
            QString name = ui->lineEdit_nc_name->text();
            QString username = ui->lineEdit_nc_username->text();
            QString telephone = ui->lineEdit_nc_telephone->text();
            QString password = ui->lineEdit_nc_password->text();
            QString accessType = ui->comboBox_nc_accessType->currentText();

            QSqlQuery query;
            query.prepare("INSERT INTO tb_collaborators (name, username, password, telephone, access) "
                          "VALUES ('" + name + "', '" + username + "', '" + password + "', '"
                          + telephone + "', '" + accessType + "')");

            qDebug() << "INSERT INTO tb_collaborators (name, username, password, telephone, accessType) "
                        "VALUES ('" + name + "', '" + username + "', '" + password + "', '"
                            + telephone + "', '" + accessType + "')";

            if(!query.exec())
            {
                QMessageBox::warning(this, "Error", "Unable to save new collaborator into database");
            }
            else
            {
                QMessageBox::information(this, "Success", "New collaborator save into database");
                ClearNewCollaboratorTabFields();
            }
        }
        else
        {
            QMessageBox::warning(this, "Error", "The passwords aren't the same");
        }
    }
}

void CollaboratosManagementWindow::ClearNewCollaboratorTabFields()
{
    ui->lineEdit_nc_name->clear();
    ui->lineEdit_nc_username->clear();
    ui->lineEdit_nc_telephone->clear();
    ui->lineEdit_nc_password->clear();
    ui->lineEdit_nc_confirmPassword->clear();
    ui->comboBox_nc_accessType->setCurrentIndex(-1);
}

void CollaboratosManagementWindow::UpdateCMTableWidget()
{
    QSqlQuery query;
    query.prepare("SELECT id, name FROM tb_collaborators ORDER BY id");
    Utilities utilities;
    if(!utilities.QueryToUpdateTableWidget(&query, ui->tableWidget_cm_collaborators))
    {
        QMessageBox::warning(this, "Error", "Unable to read collaborators from database");
    }
}

void CollaboratosManagementWindow::on_tabWidget_currentChanged(int index)
{
    if(index == 1)
    {
        UpdateCMTableWidget();
    }
}

