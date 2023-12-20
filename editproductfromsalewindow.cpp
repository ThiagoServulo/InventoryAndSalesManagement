#include "editproductfromsalewindow.h"
#include "ui_editproductfromsalewindow.h"
#include <QRegularExpressionValidator>

EditProductFromSaleWindow::EditProductFromSaleWindow(int idProduct, QString description, float salePrice, int quantity, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditProductFromSaleWindow)
{
    ui->setupUi(this);

    ui->lineEdit_idProduct->setEnabled(false);
    ui->lineEdit_description->setEnabled(false);

    this->salePrice = salePrice;
    this->quantity = quantity;

    ui->lineEdit_idProduct->setText(QString::number(idProduct));
    ui->lineEdit_description->setText(description);
    ui->lineEdit_salePrice->setText(QString::number(salePrice));
    ui->lineEdit_quantity->setText(QString::number(quantity));

    QValidator *validatorFloat = new QRegularExpressionValidator(QRegularExpression("[0-9]*\\.?[0-9]*"));
    ui->lineEdit_salePrice->setValidator(validatorFloat);

    QValidator *validatorInt = new QRegularExpressionValidator(QRegularExpression("[0-9]*"));
    ui->lineEdit_quantity->setValidator(validatorInt);
}

EditProductFromSaleWindow::~EditProductFromSaleWindow()
{
    delete ui;
}

void EditProductFromSaleWindow::on_pushButton_confirm_clicked()
{
    this->salePrice = ui->lineEdit_salePrice->text().toFloat();
    this->quantity = ui->lineEdit_quantity->text().toInt();
    close();
}

void EditProductFromSaleWindow::on_pushButton_cancel_clicked()
{
    close();
}

