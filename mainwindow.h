#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Properties
    QIcon lockedPadlock;
    QIcon unlockedPadlock;
    static bool userLogged;
    static QString name_collaborator, access_collaborator, username_collaborator;
    static int id_collaborator;

    // Functions
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Block_clicked();
    void on_pushButton_NewSale_clicked();
    void on_actionInventory_triggered();
    void on_actionCollaborators_triggered();
    void on_actionSales_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
