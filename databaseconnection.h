#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H
#include <QtSql>

class DatabaseConnection{

public:
    DatabaseConnection();
    void close();
    bool open();
    bool isOpen();

private:
    QSqlDatabase database;
    int createDatabase(QString databasePath);
};


#endif // DATABASECONNECTION_H
