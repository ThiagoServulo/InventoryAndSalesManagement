#ifndef DEFINEPASSWORDWINDOW_H
#define DEFINEPASSWORDWINDOW_H

#include <QDialog>

namespace Ui {
class DefinePasswordWindow;
}

class DefinePasswordWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DefinePasswordWindow(QWidget *parent = nullptr);
    ~DefinePasswordWindow();

private slots:
    void on_pushButton_confirm_clicked();

private:
    Ui::DefinePasswordWindow *ui;
};

#endif // DEFINEPASSWORDWINDOW_H
