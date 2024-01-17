#ifndef UTILITIES_H
#define UTILITIES_H

#include <QSqlQuery>
#include <QTableWidget>
#include <QComboBox>
#include <QLineEdit>

class Utilities
{
public:
    Utilities();
    bool QueryToUpdateTableWidget(QSqlQuery *query, QTableWidget *tableWidget);
    void QueryToInsertFieldsIntoTableWidget(QSqlQuery *query, QTableWidget *tableWidget);
    void CleanTableWidget(QTableWidget *tableWidget);
    bool QueryToInsertComboBoxElements(QSqlQuery *query, QComboBox *comboBox);
    void TableWidgetBasicConfigurations(QTableWidget *tableWidget, QStringList &headerLabels);
    void ConfigureRegexLineEdit(QLineEdit *lineEdit, int type);
};

#endif // UTILITIES_H
