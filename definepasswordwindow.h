#ifndef DEFINEPASSWORDWINDOW_H
#define DEFINEPASSWORDWINDOW_H

#include <QDialog>
#include <QIcon>
#include <QString>

namespace Ui {
class DefinePasswordWindow;
}

class DefinePasswordWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DefinePasswordWindow(QString username, QWidget *parent = nullptr);
    ~DefinePasswordWindow();

private slots:
    void on_pushButton_confirm_clicked();
    void on_pushButton_eye_password_clicked();
    void on_pushButton_eye_confirmPassword_clicked();

private:
    Ui::DefinePasswordWindow *ui;
    QIcon eyeCloseIcon;
    QIcon eyeOpenIcon;
    QString username;
};

#endif // DEFINEPASSWORDWINDOW_H
