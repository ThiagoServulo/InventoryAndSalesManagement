#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H
#include <QtSql>

class DatabaseConnection{

public:
    // Properties
    QSqlDatabase database;

    // Functions
    DatabaseConnection();
    void close();
    bool open();
};


#endif // DATABASECONNECTION_H
