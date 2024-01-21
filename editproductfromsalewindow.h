#ifndef EDITPRODUCTFROMSALEWINDOW_H
#define EDITPRODUCTFROMSALEWINDOW_H

#include <QDialog>

namespace Ui {
class EditProductFromSaleWindow;
}

class EditProductFromSaleWindow : public QDialog
{
    Q_OBJECT

public:
    int quantity;
    float salePrice;
    explicit EditProductFromSaleWindow(int idProduct, QString description, float salePrice, int quantity, QWidget *parent = nullptr);
    ~EditProductFromSaleWindow();

private slots:
    void on_pushButton_confirm_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::EditProductFromSaleWindow *ui;
    QIcon iconWindow;

};

#endif // EDITPRODUCTFROMSALEWINDOW_H
