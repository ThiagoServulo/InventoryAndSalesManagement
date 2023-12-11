#ifndef COLLABORATOSMANAGEMENTWINDOW_H
#define COLLABORATOSMANAGEMENTWINDOW_H

#include <QDialog>

namespace Ui {
class CollaboratosManagementWindow;
}

class CollaboratosManagementWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CollaboratosManagementWindow(QWidget *parent = nullptr);
    ~CollaboratosManagementWindow();

private:
    Ui::CollaboratosManagementWindow *ui;
};

#endif // COLLABORATOSMANAGEMENTWINDOW_H
