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
    QIcon lockedPadlock;
    QIcon unlockedPadlock;
    bool userLogged;
    QString name_collaborator, access_collaborator;

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Block_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
