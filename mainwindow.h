#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static bool userLogged;
    static QString name_collaborator;
    static int id_collaborator, access_collaborator;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Block_clicked();
    void on_actionInventory_triggered();
    void on_actionCollaborators_triggered();
    void on_actionSales_triggered();
    void on_lineEdit_idProduct_returnPressed();
    void on_pushButton_finalizeSale_clicked();
    void on_pushButton_searchProduct_clicked();
    void on_pushButton_editProduct_clicked();
    void on_pushButton_removeProduct_clicked();
    void on_actionAbout_triggered();
    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
    QIcon lockedPadlock;
    QIcon unlockedPadlock;
    float CalculateTotalSale(QTableWidget *tableWidget, int column);
    void InitFieldsWindow();
    void UpdateProductQuantiy(int id_product, int quantity);
    void InsertProductIntoTableWidget();
};
#endif // MAINWINDOW_H
