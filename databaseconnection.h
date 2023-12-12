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
    bool isOpen();
};


#endif // DATABASECONNECTION_H
