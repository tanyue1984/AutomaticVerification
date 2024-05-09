#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H
#include <QtSql>

class DatabaseConnection
{
public:
    DatabaseConnection();
    ~DatabaseConnection();
    bool open();
    void close();
    QSqlDatabase& getDatabase();
private:
    QSqlDatabase db;
};

#endif // DATABASECONNECTION_H
