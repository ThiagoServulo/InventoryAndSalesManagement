#ifndef COLLABORATOSMANAGEMENTWINDOW_H
#define COLLABORATOSMANAGEMENTWINDOW_H

#include <QDialog>
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

private:
    Ui::CollaboratosManagementWindow *ui;
};

#endif // COLLABORATOSMANAGEMENTWINDOW_H
