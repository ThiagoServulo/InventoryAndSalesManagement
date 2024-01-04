#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H
#include <QtSql>

class DatabaseConnection{

public:
    QSqlDatabase database;
    DatabaseConnection();
    void close();
    bool open();
    bool isOpen();
    bool SendQuery(QString command, QSqlQuery *query);
};


#endif // DATABASECONNECTION_H
