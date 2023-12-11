#ifndef SALESWINDOW_H
#define SALESWINDOW_H

#include <QDialog>

namespace Ui {
class SalesWindow;
}

class SalesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SalesWindow(QWidget *parent = nullptr);
    ~SalesWindow();

private:
    Ui::SalesWindow *ui;
};

#endif // SALESWINDOW_H
