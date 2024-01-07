#include "databaseconnection.h"
#include <QMessageBox>

DatabaseConnection::DatabaseConnection()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
}

void DatabaseConnection::close()
{
    database.close();
}

bool DatabaseConnection::open()
{
    if(this->isOpen())
    {
        return true;
    }
    else
    {
        QString path = qApp->applicationDirPath();
        QString db = path + "/database/InventoryManagement.db";
        database.setDatabaseName(db);

        return database.open();
    }
}

bool DatabaseConnection::isOpen()
{
    return database.isOpen();
}
