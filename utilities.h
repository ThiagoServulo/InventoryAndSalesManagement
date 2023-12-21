#ifndef UTILITIES_H
#define UTILITIES_H

#include <QSqlQuery>
#include <QTableWidget>

class Utilities
{
public:
    Utilities();
    bool QueryToUpdateTableWidget(QSqlQuery *query, QTableWidget *tableWidget);
    void QueryToInsertFieldsIntoTableWidget(QSqlQuery *query, QTableWidget *tableWidget);
    void CleanTableWidget(QTableWidget *tableWidget);
};

#endif // UTILITIES_H
