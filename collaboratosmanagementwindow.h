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
    void on_pushButton_nc_save_clicked();
    void on_tabWidget_currentChanged(int index);

    void on_tableWidget_cm_collaborators_itemSelectionChanged();

    void on_pushButton_cm_filter_clicked();

    void on_pushButton_cm_save_clicked();

    void on_pushButton_cm_remove_clicked();

private:
    Ui::CollaboratosManagementWindow *ui;
    void UpdateCMTableWidget();
    void ClearNewCollaboratorTabFields();
    void ClearCollaboratorManagementTabFields();

};

#endif // COLLABORATOSMANAGEMENTWINDOW_H
