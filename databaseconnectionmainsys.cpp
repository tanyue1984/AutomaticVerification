#include "databaseconnectionmainsys.h"




DatabaseConnectionMainSys* DatabaseConnectionMainSys::Connect = nullptr;
QMutex DatabaseConnectionMainSys::mutex;

DatabaseConnectionMainSys::DatabaseConnectionMainSys()
{

    // 连接数据库
       dbmain = QSqlDatabase::addDatabase("QODBC");
       dbmain.setHostName("111.198.10.15");
       dbmain.setDatabaseName("casic_metering_xichang");
       dbmain.setPort(21906);
       dbmain.setUserName("zdjdcs");
       dbmain.setPassword("11111111");
      /* db.setHostName("localhost");
       db.setDatabaseName("zdjd");
       db.setPort(3306);
       db.setUserName("root");
       db.setPassword("123456");*/


}
DatabaseConnectionMainSys::~DatabaseConnectionMainSys() {
    // 在析构函数中关闭数据库连接
    close();
}
bool DatabaseConnectionMainSys::open() {
    return dbmain.open();
}
bool DatabaseConnectionMainSys::isopen() {
    return dbmain.isOpen();
}

void DatabaseConnectionMainSys::close() {
    dbmain.close();
}

QSqlDatabase& DatabaseConnectionMainSys::getDatabase() {
    return dbmain;
}
DatabaseConnectionMainSys* DatabaseConnectionMainSys::getInstance()
{
    if (!Connect) {
        QMutexLocker locker(&mutex);
        if (!Connect) {
            Connect = new DatabaseConnectionMainSys();
        }
    }
    return Connect;
}
