#include "collaboratorsmanagementwindow.h"
#include "ui_collaboratosmanagementwindow.h"
#include "utilities.h"
#include "mainwindow.h"
#include "databaseconnection.h"
#include <QSql>
#include <QMessageBox>
#include <QTableWidget>

CollaboratosManagementWindow::CollaboratosManagementWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CollaboratosManagementWindow)
{
    ui->setupUi(this);

    Utilities utilities;

    // Window layout
    QIcon iconWindow;
    iconWindow.addFile(":/images/login.png");
    this->setWindowIcon(iconWindow);
    this->setWindowTitle("Collaborators Management");
    this->setFixedSize(644, 514);

    // Update combo box with the access type options
    DatabaseConnection dbConnection;
    if(dbConnection.open())
    {
        QSqlQuery query;
        query.prepare("SELECT * FROM tb_access_type");

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
        QMessageBox::warning(this, "Error", "Unable to connect database to update combo boxes");
    }

    // Clear tab fields
    ClearNewCollaboratorTabFields();
    ClearCollaboratorManagementTabFields(true);

    // Configure table widget
    QStringList headerLabels = {"Id", "Name", "Username"};
    utilities.TableWidgetBasicConfigurations(ui->tableWidget_collaboratorsManagement_collaborators,
                                             headerLabels);

    // Configure radio button
    ui->radioButton_collaboratorsManagement_idCollaborator->setChecked(true);

    // Define first tab
    ui->tabWidget->setCurrentIndex(0);

    // Configure regex to line edit fields
    utilities.ConfigureRegexLineEdit(ui->lineEdit_newCollaborator_name, 1);
    utilities.ConfigureRegexLineEdit(ui->lineEdit_newCollaborator_username, 4);
    utilities.ConfigureRegexLineEdit(ui->lineEdit_newCollaborator_telephone, 2);
    utilities.ConfigureRegexLineEdit(ui->lineEdit_collaboratorsManagement_name, 1);
    utilities.ConfigureRegexLineEdit(ui->lineEdit_collaboratorsManagement_telephone, 2);

    // Set telephone field length
    ui->lineEdit_newCollaborator_telephone->setMaxLength(11);
    ui->lineEdit_collaboratorsManagement_telephone->setMaxLength(11);
}


CollaboratosManagementWindow::~CollaboratosManagementWindow()
{
    delete ui;
}


bool CollaboratosManagementWindow::CheckCollaboratorFields(QString name, QString telephone, QString accessType)
{
    // Fields should not be empty
    if(name == "" || telephone == "" || accessType == "")
    {
        return false;
    }

    // Name must have more than 6 characters
    if(name.length() < 6)
    {
        QMessageBox::warning(this, "Error", "Name must have more than 6 characters");
        return false;
    }

    // Telephone must have 11 characters
    if(telephone.length() != 11)
    {
        QMessageBox::warning(this, "Error", "Telephone must have 11 characters");
        return false;
    }

    // The fields are valid
    return true;
}


void CollaboratosManagementWindow::on_pushButton_newCollaborator_save_clicked()
{
    // Get fields
    QString name = ui->lineEdit_newCollaborator_name->text();
    QString username = ui->lineEdit_newCollaborator_username->text();
    QString telephone = ui->lineEdit_newCollaborator_telephone->text();
    QString accessType = ui->comboBox_newCollaborator_accessType->currentText();
    int accessTypeId;

    // Check fields
    if(!CheckCollaboratorFields(name, telephone, accessType))
    {
        return;
    }

    // Check username
    if(username.length() < 6)
    {
        QMessageBox::warning(this, "Error", "Username must have more than 6 characters");
        return;
    }
    else
    {
        if(int status = UsernameExists(username) != 0)
        {
            if(status > 0)
            {
                QMessageBox::warning(this, "Error", "This username already exists");
            }

            return;
        }
    }

    // Save new collaborator
    DatabaseConnection dbConnection;
    if(!dbConnection.open())
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to save new collaborator");
    }
    else
    {
        // Get id for the access type chosen into the combo box
        QSqlQuery query;
        query.prepare("SELECT id FROM tb_access_type WHERE type = '" + accessType + "'");

        // Execute query
        if(!query.exec())
        {
            QMessageBox::warning(this, "Error", "Unable to read access type table");
            dbConnection.close();
            return;
        }
        else
        {
            query.first();
            accessTypeId = query.value(0).toInt();
        }

        // Insert new collaborator
        query.prepare("INSERT INTO tb_collaborators (name, username, password, telephone, access) "
                      "VALUES ('" + name + "', '" + username + "', '" + QString::number(1234)
                      + "', '" + telephone + "', " + QString::number(accessTypeId) + ")");

        if(!query.exec())
        {
            QMessageBox::warning(this, "Error", "Unable to save new collaborator into database");
        }
        else
        {
            QMessageBox::information(this, "Success", "New collaborator saved into database");
            ClearNewCollaboratorTabFields();
        }

        dbConnection.close();
    }
}


int CollaboratosManagementWindow::UsernameExists(QString username)
{
    // Init status
    int status = -1;

    DatabaseConnection dbConnection;
    if(dbConnection.open())
    {
        // Build query
        QSqlQuery query;
        query.prepare("SELECT id FROM tb_collaborators WHERE username = '" + username + "'");

        // Execute query
        if(!query.exec())
        {
            QMessageBox::warning(this, "Error", "Unable to read access type table");
        }
        else
        {
            // Init quantity
            int quantity = 0;

            //Check if the username already exists
            while(query.next())
            {
                ++quantity;
            }

            // Update status
            status = quantity;
        }

        dbConnection.close();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to check if username exists");
    }

    return status;
}


void CollaboratosManagementWindow::ClearNewCollaboratorTabFields()
{
    // Clear fields
    ui->lineEdit_newCollaborator_name->clear();
    ui->lineEdit_newCollaborator_username->clear();
    ui->lineEdit_newCollaborator_telephone->clear();
    ui->comboBox_newCollaborator_accessType->setCurrentIndex(-1);
    ui->lineEdit_newCollaborator_name->setFocus();
}


void CollaboratosManagementWindow::ClearCollaboratorManagementTabFields(bool cleanFilterField)
{
    // Clear fields
    ui->lineEdit_collaboratorsManagement_name->clear();
    ui->lineEdit_collaboratorsManagement_telephone->clear();
    ui->comboBox_collaboratorsManagement_accessType->setCurrentIndex(-1);

    // Clear filter field
    if(cleanFilterField)
    {
        ui->lineEdit_collaboratorsManagement_filter->clear();
    }
}


void CollaboratosManagementWindow::UpdateCollaboratorsManagementTableWidget()
{
    DatabaseConnection dbConnection;
    if(dbConnection.open())
    {
        // Build query
        QSqlQuery query;
        query.prepare("SELECT id, name, username FROM tb_collaborators WHERE username != 'admin' ORDER BY id");

        // Update table
        Utilities utilities;
        if(!utilities.QueryToUpdateTableWidget(&query, ui->tableWidget_collaboratorsManagement_collaborators))
        {
            QMessageBox::warning(this, "Error", "Unable to read collaborators from database");
        }

        dbConnection.close();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to update collaborators management table");
    }
}


void CollaboratosManagementWindow::on_tabWidget_currentChanged(int index)
{
    // Check the user access
    if(index == 1 && MainWindow::access_collaborator != 1)
    {
        ui->tabWidget->setCurrentIndex(0);
        QMessageBox::information(this, "Information", "You don't have access");
        return;
    }

    // Clear table widget selection
    ui->tableWidget_collaboratorsManagement_collaborators->clearSelection();

    if(index == 1)
    {
        // Update table
        UpdateCollaboratorsManagementTableWidget();
    }

    // Clear collaborator management tab fields
    ClearCollaboratorManagementTabFields(true);
}


void CollaboratosManagementWindow::on_tableWidget_collaboratorsManagement_collaborators_itemSelectionChanged()
{
    DatabaseConnection dbConnection;
    if(dbConnection.open())
    {
        // Get current row
        int currentRow = ui->tableWidget_collaboratorsManagement_collaborators->currentRow();

        // Get collaborator id
        int id = ui->tableWidget_collaboratorsManagement_collaborators->item(currentRow, 0)->text().toInt();

        // Build query
        QSqlQuery query;
        query.prepare("SELECT name, username, telephone, access FROM tb_collaborators WHERE id = " +
                      QString::number(id));

        // Execute query
        if(query.exec())
        {
            // Get collaborator information
            query.first();
            ui->lineEdit_collaboratorsManagement_name->setText(query.value(0).toString());
            ui->lineEdit_collaboratorsManagement_telephone->setText(query.value(2).toString());

            // Get collaborator access type
            query.prepare("SELECT type FROM tb_access_type WHERE id = " +
                          QString::number(query.value(3).toInt()));
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

        dbConnection.close();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to read collaborator information");
    }
}


void CollaboratosManagementWindow::on_pushButton_collaboratorsManagement_filter_clicked()
{
    // Clear filds and table
    ui->tableWidget_collaboratorsManagement_collaborators->clearSelection();
    ClearCollaboratorManagementTabFields(false);

    DatabaseConnection dbConnection;
    if(dbConnection.open())
    {
        // Block signals to ignore items selections changed
        ui->tableWidget_collaboratorsManagement_collaborators->blockSignals(true);

        QSqlQuery query;

        // Check filter type
        if(ui->lineEdit_collaboratorsManagement_filter->text() == "")
        {
            if(ui->radioButton_collaboratorsManagement_idCollaborator->isChecked())
            {
                query.prepare("SELECT id, name, username FROM tb_collaborators WHERE username "
                              "!= 'admin' ORDER BY id");
            }
            else
            {
                query.prepare("SELECT id, name, username FROM tb_collaborators WHERE username "
                              "!= 'admin' ORDER BY name");
            }
        }
        else
        {
            if(ui->radioButton_collaboratorsManagement_idCollaborator->isChecked())
            {
                query.prepare("SELECT id, name, username FROM tb_collaborators WHERE username "
                              "!= 'admin' WHERE id = " +
                              ui->lineEdit_collaboratorsManagement_filter->text());
            }
            else
            {
                query.prepare("SELECT id, name, username FROM tb_collaborators WHERE username "
                              "!= 'admin' WHERE name LIKE '%" +
                              ui->lineEdit_collaboratorsManagement_filter->text() + "%'");
            }
        }

        // Clean line edit
        ui->lineEdit_collaboratorsManagement_filter->clear();

        // Execute query
        if(query.exec())
        {
            // Insert into table widget
            Utilities utilities;
            utilities.QueryToInsertFieldsIntoTableWidget(&query,
                                                         ui->tableWidget_collaboratorsManagement_collaborators);
        }
        else
        {
            QMessageBox::warning(this, "Error", "Error to filter collaborators from database");
        }

        dbConnection.close();

        // Restore signals
        ui->tableWidget_collaboratorsManagement_collaborators->blockSignals(false);
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to filter collaborators");
    }
}


void CollaboratosManagementWindow::on_pushButton_collaboratorsManagement_save_clicked()
{
    // Check whether the collaborator is selected
    if(ui->tableWidget_collaboratorsManagement_collaborators->currentRow() == -1)
    {
        QMessageBox::information(this, "Information", "Select a collaborator first");
        return;
    }

    DatabaseConnection dbConnection;
    if(dbConnection.open())
    {
        // Get fields
        int currentRow = ui->tableWidget_collaboratorsManagement_collaborators->currentRow();
        int id = ui->tableWidget_collaboratorsManagement_collaborators->item(currentRow, 0)->text().toInt();
        QString name = ui->lineEdit_collaboratorsManagement_name->text();
        QString telephone = ui->lineEdit_collaboratorsManagement_telephone->text();
        QString accessType = ui->comboBox_collaboratorsManagement_accessType->currentText();
        int accessTypeId;

        // Check fields
        if(!CheckCollaboratorFields(name, telephone, accessType))
        {
            dbConnection.close();
            return;
        }

        // Get the access type id
        QSqlQuery query;
        query.prepare("SELECT id FROM tb_access_type WHERE type = '" + accessType + "'");

        // Execute query
        if(query.exec())
        {
            query.next();
            accessTypeId = query.value(0).toInt();
        }
        else
        {
            QMessageBox::warning(this, "Error", "Unable to read the access type from database");
            dbConnection.close();
            return;
        }

        // Update collaborator information
        query.prepare("UPDATE tb_collaborators SET name = '" + name + "', telephone = '" +
                      telephone + "', access = " + QString::number(accessTypeId) +
                      " WHERE id = " + QString::number(id));

        // Execute query
        if(query.exec())
        {
            // Update table widget
            ui->tableWidget_collaboratorsManagement_collaborators->clearSelection();
            UpdateCollaboratorsManagementTableWidget();
            ClearCollaboratorManagementTabFields(true);
            QMessageBox::information(this, "Success", "Collaborator information updated");
        }
        else
        {
            QMessageBox::warning(this, "Error", "Unable to update collaborator information from database");
        }

        dbConnection.close();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to update collaborator");
    }
}


void CollaboratosManagementWindow::on_pushButton_collaboratorsManagement_remove_clicked()
{
    // Get current row
    int currentRow = ui->tableWidget_collaboratorsManagement_collaborators->currentRow();

    // Check if it is a valid row
    if(currentRow == -1)
    {
        QMessageBox::information(this, "Information", "Select a collaborator first");
        return;
    }

    DatabaseConnection dbConnection;
    if(dbConnection.open())
    {
        QMessageBox::StandardButton button = QMessageBox::question(this, "Remove",
                                                                   "Do you want to remove this collaborator?",
                                                                   QMessageBox::Yes | QMessageBox::No);
        if(button == QMessageBox::Yes)
        {
            // Block signals to ignore items selections changed
            ui->tableWidget_collaboratorsManagement_collaborators->blockSignals(true);

            // Get collaborator id
            int id = ui->tableWidget_collaboratorsManagement_collaborators->item(currentRow, 0)->text().toInt();

            // Remove collaborator from database
            QSqlQuery query;
            query.prepare("DELETE FROM tb_collaborators WHERE id = " + QString::number(id));

            // Execute query
            if(query.exec())
            {
                ui->tableWidget_collaboratorsManagement_collaborators->removeRow(currentRow);
                ClearCollaboratorManagementTabFields(true);
                ui->tableWidget_collaboratorsManagement_collaborators->setCurrentCell(-1, -1);
                QMessageBox::information(this, "Success", "Collaborator removed with success");
            }
            else
            {
                QMessageBox::warning(this, "Error", "Unable to remove collaborator from database");
            }

            dbConnection.close();

            // Restore signals
            ui->tableWidget_collaboratorsManagement_collaborators->blockSignals(false);
        }
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to remove collaborator");
    }
}


void CollaboratosManagementWindow::on_pushButton_newCollaborator_cancel_clicked()
{
    // Clear fields
    ClearNewCollaboratorTabFields();
}


void CollaboratosManagementWindow::on_pushButton_collaboratorsManagement_seeSales_clicked()
{
    // Get current row
    int currentRow = ui->tableWidget_collaboratorsManagement_collaborators->currentRow();

    // Check if it is a valid row
    if(currentRow == -1)
    {
        QMessageBox::information(this, "Information", "Select a collaborator first");
        return;
    }

    DatabaseConnection dbConnection;
    if(dbConnection.open())
    {
        // Get collaborator id
        int idCollaborator = ui->tableWidget_collaboratorsManagement_collaborators->item(currentRow, 0)->text().toInt();

        // Reset collaborator's from database's password
        QSqlQuery query;
        query.prepare("SELECT id FROM tb_sales WHERE id_collaborator = " + QString::number(idCollaborator));

        // Execute query
        if(query.exec())
        {
            // Build message
            QString message = "";
            while(query.next())
            {
                message += " " + query.value(0).toString();
            }

            // Show sales
            if(message == "")
            {
                QMessageBox::information(this, "Information", "This collaborator hasn't made any sales yet");
            }
            else
            {
                QMessageBox::information(this, "Information", "Collaborator's sales: " + message);
            }
        }
        else
        {
            QMessageBox::warning(this, "Error", "Unable to get collaborator's sales from database");
        }

        dbConnection.close();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to get collaborats sales");
    }
}


void CollaboratosManagementWindow::on_pushButton_resetPassword_clicked()
{
    // Get current row
    int currentRow = ui->tableWidget_collaboratorsManagement_collaborators->currentRow();

    // Check if it is a valid row
    if(currentRow == -1)
    {
        QMessageBox::information(this, "Information", "Select a collaborator first");
        return;
    }

    DatabaseConnection dbConnection;
    if(dbConnection.open())
    {
        QMessageBox::StandardButton button = QMessageBox::question(this, "Remove",
                                                                   "Do you want to reset the collaborator's password?",
                                                                   QMessageBox::Yes | QMessageBox::No);
        if(button == QMessageBox::Yes)
        {
            // Get collaborator id
            int id = ui->tableWidget_collaboratorsManagement_collaborators->item(currentRow, 0)->text().toInt();

            // Reset collaborator's from database's password
            QSqlQuery query;
            query.prepare("UPDATE tb_collaborators SET password = '1234' WHERE id = " + QString::number(id));

            // Execute query
            if(query.exec())
            {
                QMessageBox::information(this, "Success", "Collaborator's password reseted with success");
            }
            else
            {
                QMessageBox::warning(this, "Error", "Unable to reset password from database");
            }

            dbConnection.close();
        }
    }
    else
    {
        QMessageBox::warning(this, "Error", "Unable to connect database to reset password");
    }
}
