#include "editproductfromsalewindow.h"
#include "ui_editproductfromsalewindow.h"
#include <QRegularExpressionValidator>
#include "utilities.h"

EditProductFromSaleWindow::EditProductFromSaleWindow(int idProduct, QString description, float salePrice, int quantity, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditProductFromSaleWindow)
{
    ui->setupUi(this);

    // Window layout
    iconWindow.addFile(":/images/sale.png");
    this->setWindowIcon(iconWindow);
    this->setWindowTitle("Edit product sales");
    this->setFixedSize(340, 218);

    // Not enable change id and description
    ui->lineEdit_idProduct->setEnabled(false);
    ui->lineEdit_description->setEnabled(false);

    // Init parameters
    this->salePrice = salePrice;
    this->quantity = quantity;

    // Fill fields
    ui->lineEdit_idProduct->setText(QString::number(idProduct));
    ui->lineEdit_description->setText(description);
    ui->lineEdit_salePrice->setText(QString::number(salePrice));
    ui->lineEdit_quantity->setText(QString::number(quantity));

    // Configure regex
    Utilities utilities;
    utilities.ConfigureRegexLineEdit(ui->lineEdit_salePrice, 3);
    utilities.ConfigureRegexLineEdit(ui->lineEdit_quantity, 2);
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

