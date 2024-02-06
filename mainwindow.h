#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QtSql>
#include "databaseconnection.h"

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
    static QString name_collaborator, username_collaborator;
    static int id_collaborator, access_collaborator;

    // Functions
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Block_clicked();
    void on_actionInventory_triggered();
    void on_actionCollaborators_triggered();
    void on_actionSales_triggered();
    void on_lineEdit_idProduct_4_returnPressed();
    void on_pushButton_finalizeSale_4_clicked();
    void on_pushButton_searchProduct_4_clicked();
    void on_pushButton_editProduct_4_clicked();
    void on_pushButton_removeProduct_4_clicked();

private:
    Ui::MainWindow *ui;
    //DatabaseConnection dbConnection;
    void EraseTableWidget(QTableWidget *tableWidget);
    float CalculateTotalSale(QTableWidget *tableWidget, int column);
    void InitFieldsWindow();
    void UpdateProductQuantiy(int id_product, int quantity);
    void InsertProductIntoTableWidget();
};
#endif // MAINWINDOW_H
