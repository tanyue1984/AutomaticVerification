#include "databaseconnection.h"




DatabaseConnection* DatabaseConnection::Connect = nullptr;
QMutex DatabaseConnection::mutex;

DatabaseConnection::DatabaseConnection()
{

    // 连接数据库
       localdb = QSqlDatabase::addDatabase("QODBC");
       localdb.setHostName("localhost");
       localdb.setDatabaseName("zdjd");
       localdb.setPort(3306);
       localdb.setUserName("root");
       localdb.setPassword("123456");

       Maindb = QSqlDatabase::addDatabase("QODBC","Main");
       Maindb.setHostName("111.198.10.15");
       Maindb.setDatabaseName("casic_metering_xichang_zd");
       Maindb.setPort(21906);
       Maindb.setUserName("zdjdcs");
       Maindb.setPassword("11111111");
//       Maindb = QSqlDatabase::addDatabase("QODBC","Main");
//       Maindb.setHostName("27.99.50.209");
//       Maindb.setDatabaseName("test");
//       Maindb.setPort(21906);
//       Maindb.setUserName("casic_203");
//       Maindb.setPassword("Casic_203");
//       Maindb = QSqlDatabase::addDatabase("QODBC","Main");
//       Maindb.setHostName("localhost");
//       Maindb.setDatabaseName("zdjd");
//       Maindb.setPort(3306);
//       Maindb.setUserName("root");
//       Maindb.setPassword("123456");



}
DatabaseConnection::~DatabaseConnection() {
    // 在析构函数中关闭数据库连接
    close();
}
bool DatabaseConnection::open() {
    return localdb.open();
}
bool DatabaseConnection::openmain() {
    return Maindb.open();
}
bool DatabaseConnection::isopen() {
    return localdb.isOpen();
}
bool DatabaseConnection::isopenmain() {
    return Maindb.isOpen();
}
void DatabaseConnection::close() {
    localdb.close();
}
void DatabaseConnection::closemain() {
    Maindb.close();
}
QSqlDatabase& DatabaseConnection::getDatabase() {
    return localdb;
}
QSqlDatabase& DatabaseConnection::getDatabaseMain() {
    return Maindb;
}
DatabaseConnection* DatabaseConnection::getInstance()
{
    if (!Connect) {
        QMutexLocker locker(&mutex);
        if (!Connect) {
            Connect = new DatabaseConnection();
        }
    }
    return Connect;
}
