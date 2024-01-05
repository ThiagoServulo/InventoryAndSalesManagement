#ifndef COLLABORATOSMANAGEMENTWINDOW_H
#define COLLABORATOSMANAGEMENTWINDOW_H

#include <QDialog>
#include <QTableWidget>
#include "databaseconnection.h"

namespace Ui {
class CollaboratosManagementWindow;
}

class CollaboratosManagementWindow : public QDialog
{
    Q_OBJECT

public:
    // Properties
    DatabaseConnection dbConnection;

    // Functions
    explicit CollaboratosManagementWindow(QWidget *parent = nullptr);
    ~CollaboratosManagementWindow();

private slots:
    void on_pushButton_newCollaborator_save_clicked();
    void on_tabWidget_currentChanged(int index);

    void on_tableWidget_collaboratorsManagement_collaborators_itemSelectionChanged();

    void on_pushButton_collaboratorsManagement_filter_clicked();

    void on_pushButton_collaboratorsManagement_save_clicked();

    void on_pushButton_collaboratorsManagement_remove_clicked();

private:
    Ui::CollaboratosManagementWindow *ui;
    void UpdateCollaboratorsManagementTableWidget();
    void ClearNewCollaboratorTabFields();
    void ClearCollaboratorManagementTabFields();

};

#endif // COLLABORATOSMANAGEMENTWINDOW_H
