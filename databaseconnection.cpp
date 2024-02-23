#include "databaseconnection.h"
#include <QMessageBox>
#include <QFileInfo>

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
    // Init database configurations
    QString path = qApp->applicationDirPath();
    QString db = path + "/database/InventoryManagement.db";
    QFileInfo fileInfo(db);

    // Check if the file exists
    if (!fileInfo.exists())
    {
        if(!createDatabase(db))
        {
            return false;
        }
    }

    // Check if the database is open
    if(this->isOpen())
    {
        return true;
    }
    else
    {
        // Set database name
        database.setDatabaseName(db);

        // Return status
        return database.open();
    }
}


bool DatabaseConnection::isOpen()
{
    return database.isOpen();
}


int DatabaseConnection::createDatabase(QString databasePath)
{
    // Set database name
    database.setDatabaseName(databasePath);

    // Open database
    if (!database.open())
    {
        qDebug() << "Error to create database";
        return false;
    }

    // Create table tb_access_type
    QSqlQuery query;
    if (!query.exec("CREATE TABLE tb_access_type ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "type TEXT NOT NULL,"
                    "description TEXT"
                    ")"))
    {
        qDebug() << "Error to create tb_access_type table: " << query.lastError().text();
        return false;
    }

    // Create table tb_collaborators
    if (!query.exec("CREATE TABLE tb_collaborators ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "name TEXT NOT NULL,"
                    "username TEXT NOT NULL,"
                    "password TEXT NOT NULL,"
                    "telephone TEXT NOT NULL,"
                    "access INTEGER NOT NULL"
                    ")"))
    {
        qDebug() << "Error to create tb_collaborators table: " << query.lastError().text();
        return false;
    }

    // Create table tb_inventory
    if (!query.exec("CREATE TABLE tb_inventory ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "description TEXT NOT NULL,"
                    "supplier TEXT NOT NULL,"
                    "quantity INTEGER NOT NULL,"
                    "purchase_price REAL NOT NULL,"
                    "sale_price NUMERIC NOT NULL"
                    ")"))
    {
        qDebug() << "Error to create tb_inventory table: " << query.lastError().text();
        return false;
    }

    // Create table tb_products_sales
    if (!query.exec("CREATE TABLE tb_products_sales ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "id_sale INTEGER NOT NULL,"
                    "id_product INTEGER NOT NULL,"
                    "quantity INTEGER NOT NULL,"
                    "sale_price REAL NOT NULL"
                    ")"))
    {
        qDebug() << "Error to create tb_products_sales table: " << query.lastError().text();
        return false;
    }

    // Create table tb_sales
    if (!query.exec("CREATE TABLE tb_sales ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "date TEXT NOT NULL,"
                    "id_collaborator INTEGER NOT NULL,"
                    "total_value REAL NOT NULL"
                    ")"))
    {
        qDebug() << "Error to create tb_sales table: " << query.lastError().text();
        return false;
    }

    // Insert admin user into tb_collaborators
    if (!query.exec("INSERT INTO tb_collaborators (name, username, password, telephone, access) "
                    "VALUES ('admin', 'admin', 'admin', '', 1)"))
    {
        qDebug() << "Error to insert admin user into tb_collaborators: " << query.lastError().text();
        return false;
    }

    // Insert access type A
    if (!query.exec("INSERT INTO tb_access_type (type, description) "
                    "VALUES ('A', 'Full access')"))
    {
        qDebug() << "Error to insert access type A into tb_access_type: " << query.lastError().text();
        return false;
    }

    // Insert access type B
    if (!query.exec("INSERT INTO tb_access_type (type, description) "
                    "VALUES ('B', 'Can create, but cannot edit')"))
    {
        qDebug() << "Error to insert access type B into tb_access_type: " << query.lastError().text();
        return false;
    }

    // Insert access type C
    if (!query.exec("INSERT INTO tb_access_type (type, description) "
                    "VALUES ('C', 'Access only to sale page')"))
    {
        qDebug() << "Error to insert access type C into tb_access_type: " << query.lastError().text();
        return false;
    }

    return true;
}
