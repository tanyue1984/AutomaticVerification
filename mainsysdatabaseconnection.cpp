#include "mainsysdatabaseconnection.h"




MainSysDatabaseConnection* MainSysDatabaseConnection::Connect = nullptr;
QMutex MainSysDatabaseConnection::mutex;

MainSysDatabaseConnection::MainSysDatabaseConnection()
{

    // 连接数据库
       db = QSqlDatabase::addDatabase("QODBC");
       db.setHostName("111.198.10.15");
       db.setDatabaseName("casic_metering_xichang");
       db.setPort(21906);
       db.setUserName("zdjdcs");
       db.setPassword("11111111");
      /* db.setHostName("localhost");
       db.setDatabaseName("zdjd");
       db.setPort(3306);
       db.setUserName("root");
       db.setPassword("123456");*/


}
MainSysDatabaseConnection::~MainSysDatabaseConnection() {
    // 在析构函数中关闭数据库连接
    close();
}
bool MainSysDatabaseConnection::open() {
    return db.open();
}
bool MainSysDatabaseConnection::isopen() {
    return db.isOpen();
}

void MainSysDatabaseConnection::close() {
    db.close();
}

QSqlDatabase& MainSysDatabaseConnection::getDatabase() {
    return db;
}
MainSysDatabaseConnection* MainSysDatabaseConnection::getInstance()
{
    if (!Connect) {
        QMutexLocker locker(&mutex);
        if (!Connect) {
            Connect = new MainSysDatabaseConnection();
        }
    }
    return Connect;
}
