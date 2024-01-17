#include "utilities.h"
#include <QMessageBox>
#include <QHeaderView>

Utilities::Utilities()
{

}

bool Utilities::QueryToUpdateTableWidget(QSqlQuery *query, QTableWidget *tableWidget)
{
    if(query->exec())
    {
        QueryToInsertFieldsIntoTableWidget(query, tableWidget);
        return true;
    }
    return false;
}

void Utilities::QueryToInsertFieldsIntoTableWidget(QSqlQuery *query, QTableWidget *tableWidget)
{
    int line = 0;
    CleanTableWidget(tableWidget);
    while(query->next())
    {
        tableWidget->insertRow(line);
        for(int i = 0; i < tableWidget->columnCount(); i++)
        {
            tableWidget->setItem(line, i, new QTableWidgetItem(query->value(i).toString()));
        }
        tableWidget->setRowHeight(line, 20);
        line++;
    }
    tableWidget->setCurrentCell(-1, -1);
}

void Utilities::CleanTableWidget(QTableWidget *tableWidget)
{
    while(tableWidget->rowCount())
    {
        tableWidget->removeRow(0);
    }
}

bool Utilities::QueryToInsertComboBoxElements(QSqlQuery *query, QComboBox *comboBox)
{
    if(query->exec())
    {
        comboBox->clear();
        comboBox->clearEditText();

        while(query->next())
        {
            comboBox->addItem(query->value(1).toString());
        }

        return true;
    }
    return false;
}

void Utilities::TableWidgetBasicConfigurations(QTableWidget *tableWidget, QStringList &headerLabels)
{
    tableWidget->setColumnCount(headerLabels.size());
    tableWidget->setHorizontalHeaderLabels(headerLabels);
    tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: #535455; }");
    tableWidget->setStyleSheet("QTableView { selection-background-color: #535455; }");
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void Utilities::ConfigureRegexLineEdit(QLineEdit *lineEdit, int type)
{
    QString regex;

    switch (type)
    {
        case 1: // string
            regex = "[a-zA-ZÀ-ÖØ-öø-ÿ ]*";
        break;

        case 2: // int
            regex = "[0-9]*";
        break;

        case 3: // float
            regex = "[0-9]*\\.?[0-9]*";
        break;

        default: // unknown
            return;
    }

    QValidator *validator = new QRegularExpressionValidator(QRegularExpression(regex));
    lineEdit->setValidator(validator);
}
