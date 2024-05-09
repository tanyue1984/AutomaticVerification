#include "data_base_connect_serve.h"

DatabaseConnection::DatabaseConnection()
{
    // 连接数据库
       db = QSqlDatabase::addDatabase("QODBC");
       db.setHostName("127.0.0.1");
       db.setPort(3306);
       db.setDatabaseName("trigger_2");
       db.setUserName("root");
       db.setPassword("123456");

}
DatabaseConnection::~DatabaseConnection() {
    // 在析构函数中关闭数据库连接
    close();
}
bool DatabaseConnection::open() {
    return db.open();
}

void DatabaseConnection::close() {
    db.close();
}

QSqlDatabase& DatabaseConnection::getDatabase() {
    return db;
}
