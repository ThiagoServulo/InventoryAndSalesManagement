#ifndef UTILITIES_H
#define UTILITIES_H

#include <QSqlQuery>
#include <QTableWidget>
#include <QComboBox>

class Utilities
{
public:
    Utilities();
    bool QueryToUpdateTableWidget(QSqlQuery *query, QTableWidget *tableWidget);
    void QueryToInsertFieldsIntoTableWidget(QSqlQuery *query, QTableWidget *tableWidget);
    void CleanTableWidget(QTableWidget *tableWidget);
    bool QueryToInsertComboBoxElements(QSqlQuery *query, QComboBox *comboBox);
};

#endif // UTILITIES_H
