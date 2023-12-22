#include "utilities.h"
#include <QMessageBox>

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
