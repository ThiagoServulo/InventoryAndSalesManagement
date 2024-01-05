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

    // Update combo box with the access type options
    if(dbConnection.open())
    {
        QSqlQuery query;
        query.prepare("SELECT * FROM tb_access_type");

        Utilities utilities;

        if(!utilities.QueryToInsertComboBoxElements(&query, ui->comboBox_collaboratorsManagement_accessType))
        {
            QMessageBox::warning(this, "Error", "Unable to update collaborators management combo box");
        }

        if(!utilities.QueryToInsertComboBoxElements(&query, ui->comboBox_newCollaborator_accessType))
        {
            QMessageBox::warning(this, "Error", "Unable to update new collaborator combo box");
        }

        dbConnection.close();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to connect database");
    }

    // Clear tab fields
    ClearNewCollaboratorTabFields();
    ClearCollaboratorManagementTabFields();

    // Configure password fields
    ui->lineEdit_newCollaborator_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_newCollaborator_confirmPassword->setEchoMode(QLineEdit::Password);

    ui->tableWidget_collaboratorsManagement_collaborators->setColumnCount(2);
    ui->tableWidget_collaboratorsManagement_collaborators->setColumnWidth(0, 80);
    ui->tableWidget_collaboratorsManagement_collaborators->setColumnWidth(1, 150);
    ui->tableWidget_collaboratorsManagement_collaborators->setStyleSheet("QTableView {selection-background-color: red}");
    ui->tableWidget_collaboratorsManagement_collaborators->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_collaboratorsManagement_collaborators->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_collaboratorsManagement_collaborators->verticalHeader()->setVisible(false);
    ui->tableWidget_collaboratorsManagement_collaborators->setHorizontalHeaderLabels({"Id", "Name"});
    ui->radioButton_collaboratorsManagement_idCollaborator->setChecked(true);
    ui->tabWidget->setCurrentIndex(0);
}

CollaboratosManagementWindow::~CollaboratosManagementWindow()
{
    delete ui;
}

void CollaboratosManagementWindow::on_pushButton_newCollaborator_save_clicked()
{
    QString name = ui->lineEdit_newCollaborator_name->text();
    QString username = ui->lineEdit_newCollaborator_username->text();
    QString telephone = ui->lineEdit_newCollaborator_telephone->text();
    QString password = ui->lineEdit_newCollaborator_password->text();
    QString confirmPassword = ui->lineEdit_newCollaborator_confirmPassword->text();
    QString accessType = ui->comboBox_newCollaborator_accessType->currentText();
    int accessTypeId;

    if(name == "" || username == "" || telephone == "" || password == "" || confirmPassword == "" || accessType == "")
    {
        return;
    }

    if(password != confirmPassword)
    {
        QMessageBox::warning(this, "Error", "The passwords are differents");
        return;
    }

    // TODO: Fazer validações dos campos

    if(!dbConnection.open())
    {
        QMessageBox::warning(this, "Error", "Unable to connect database");
    }
    else
    {
        // Get id for the access type chosen into the combo box
        QSqlQuery query;
        query.prepare("SELECT id FROM tb_access_type WHERE type = '" + accessType + "'");

        if(!query.exec())
        {
            QMessageBox::warning(this, "Error", "Unable to read access type table");
            return;
        }
        else
        {
            query.first();
            accessTypeId = query.value(0).toInt();
        }

        query.prepare("INSERT INTO tb_collaborators (name, username, password, telephone, access) "
                      "VALUES ('" + name + "', '" + username + "', '" + password + "', '"
                      + telephone + "', " + QString::number(accessTypeId) + ")");

        if(!query.exec())
        {
            QMessageBox::warning(this, "Error", "Unable to save new collaborator into database");
        }
        else
        {
            QMessageBox::information(this, "Success", "New collaborator save into database");
            ClearNewCollaboratorTabFields();
        }
        dbConnection.close();
    }
}

void CollaboratosManagementWindow::ClearNewCollaboratorTabFields()
{
    ui->lineEdit_newCollaborator_name->clear();
    ui->lineEdit_newCollaborator_username->clear();
    ui->lineEdit_newCollaborator_telephone->clear();
    ui->lineEdit_newCollaborator_password->clear();
    ui->lineEdit_newCollaborator_confirmPassword->clear();
    ui->comboBox_newCollaborator_accessType->setCurrentIndex(-1);
    ui->lineEdit_newCollaborator_name->setFocus();
}

void CollaboratosManagementWindow::ClearCollaboratorManagementTabFields()
{
    ui->lineEdit_collaboratorsManagement_name->clear();
    ui->lineEdit_collaboratorsManagement_filter->clear();
    ui->lineEdit_collaboratorsManagement_telephone->clear();
    ui->lineEdit_collaboratorsManagement_username->clear();
    ui->comboBox_collaboratorsManagement_accessType->setCurrentIndex(-1);
}

void CollaboratosManagementWindow::UpdateCMTableWidget()
{
    if(!dbConnection.open()) //*//
    {
        QMessageBox::warning(this, "Error", "Unable to connect database");
    }

    QSqlQuery query;
    query.prepare("SELECT id, name FROM tb_collaborators ORDER BY id");
    Utilities utilities;
    if(!utilities.QueryToUpdateTableWidget(&query, ui->tableWidget_collaboratorsManagement_collaborators))
    {
        QMessageBox::warning(this, "Error", "Unable to read collaborators from database");
    }
}

void CollaboratosManagementWindow::on_tabWidget_currentChanged(int index)
{
    ui->tableWidget_collaboratorsManagement_collaborators->clearSelection();
    if(index == 1)
    { 
        UpdateCMTableWidget();
    }
    ClearCollaboratorManagementTabFields();
}

void CollaboratosManagementWindow::on_tableWidget_collaboratorsManagement_collaborators_itemSelectionChanged()
{
    if(!dbConnection.open()) //*//
    {
        QMessageBox::warning(this, "Error", "Unable to connect database");
    }

    int id = ui->tableWidget_collaboratorsManagement_collaborators->item(ui->tableWidget_collaboratorsManagement_collaborators->currentRow(), 0)->text().toInt();
    QSqlQuery query;
    query.prepare("SELECT name, username, telephone, access FROM tb_collaborators WHERE id = " + QString::number(id));
    if(query.exec())
    {
        query.first();
        ui->lineEdit_collaboratorsManagement_name->setText(query.value(0).toString());
        ui->lineEdit_collaboratorsManagement_username->setText(query.value(1).toString());
        ui->lineEdit_collaboratorsManagement_telephone->setText(query.value(2).toString());

        query.prepare("SELECT type FROM tb_access_type WHERE id = " + QString::number(query.value(3).toInt()));
        if(query.exec())
        {
            query.first();
            ui->comboBox_collaboratorsManagement_accessType->setCurrentText(query.value(0).toString());
        }
        else
        {
            QMessageBox::warning(this, "Error", "Unable to read collaborator access type from database");
        }
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to read collaborator information from database");
    }
}


void CollaboratosManagementWindow::on_pushButton_collaboratorsManagement_filter_clicked()
{
    ui->tableWidget_collaboratorsManagement_collaborators->clearSelection();
    if(!dbConnection.open()) //*//
    {
        QMessageBox::warning(this, "Error", "Unable to connect database");
    }

    QSqlQuery query;
    if(ui->lineEdit_collaboratorsManagement_filter->text() == "")
    {
        if(ui->radioButton_collaboratorsManagement_idCollaborator->isChecked())
        {
            query.prepare("SELECT id, name FROM tb_collaborators ORDER BY id");
        }
        else
        {
            query.prepare("SELECT id, name FROM tb_collaborators ORDER BY name");
        }
    }
    else
    {
        if(ui->radioButton_collaboratorsManagement_idCollaborator->isChecked())
        {
            query.prepare("SELECT id, name FROM tb_collaborators WHERE id = " + ui->lineEdit_collaboratorsManagement_filter->text());
        }
        else
        {
            query.prepare("SELECT id, name FROM tb_collaborators WHERE name LIKE '%" + ui->lineEdit_collaboratorsManagement_filter->text() + "%'");
        }
    }

    ui->lineEdit_collaboratorsManagement_filter->clear();

    if(query.exec())
    {
        Utilities utilities;
        utilities.QueryToInsertFieldsIntoTableWidget(&query, ui->tableWidget_collaboratorsManagement_collaborators);
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to filter collaborators from database");
    }
}

void CollaboratosManagementWindow::on_pushButton_collaboratorsManagement_save_clicked()
{
    if(!dbConnection.open()) //*//
    {
        QMessageBox::warning(this, "Error", "Unable to connect database");
    }

    if(ui->lineEdit_collaboratorsManagement_name->text() == "")
    {
        QMessageBox::warning(this, "Error", "Select a collaborator");
    }
    else
    {
        int id = ui->tableWidget_collaboratorsManagement_collaborators->item(ui->tableWidget_collaboratorsManagement_collaborators->currentRow(), 0)->text().toInt();
        QString name = ui->lineEdit_collaboratorsManagement_name->text();
        QString username = ui->lineEdit_collaboratorsManagement_username->text();
        QString telephone = ui->lineEdit_collaboratorsManagement_telephone->text();
        QString accessType = ui->comboBox_collaboratorsManagement_accessType->currentText();

        QSqlQuery query;
        query.prepare("UPDATE tb_collaborators SET name = '" + name + "', username = '" + username +
                      "', telephone = '" + telephone + "', access = '" + accessType +
                      "' WHERE id = " + QString::number(id));

        if(query.exec())
        {
            UpdateCMTableWidget();
            ClearCollaboratorManagementTabFields();
            QMessageBox::information(this, "Success", "Collaborator information updated");
        }
        else
        {
            QMessageBox::warning(this, "Error", "Unable to update collaborator information from database");
        }
    }

}

void CollaboratosManagementWindow::on_pushButton_collaboratorsManagement_remove_clicked()
{
    if(!dbConnection.open()) //*//
    {
        QMessageBox::warning(this, "Error", "Unable to connect database");
    }

    QMessageBox::StandardButton button = QMessageBox::question(this, "Remove", "Do you want to remove this collaborator?", QMessageBox::Yes | QMessageBox::No);
    if(button == QMessageBox::Yes)
    {
        int id = ui->tableWidget_collaboratorsManagement_collaborators->item(ui->tableWidget_collaboratorsManagement_collaborators->currentRow(), 0)->text().toInt();
        QSqlQuery query;
        query.prepare("DELETE FROM tb_collaborators WHERE id = " + QString::number(id));
        if(query.exec())
        {
            ui->tableWidget_collaboratorsManagement_collaborators->removeRow(ui->tableWidget_collaboratorsManagement_collaborators->currentRow());
            ClearCollaboratorManagementTabFields();
            QMessageBox::information(this, "Success", "Collaborator removed with success");
        }
        else
        {
            QMessageBox::warning(this, "Error", "Unable to remove collaborator from database");
        }
    }
}

