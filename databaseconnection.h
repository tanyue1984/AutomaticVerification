#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H
#include <QtSql>
#include <QObject>
#include <QMutex>

class DatabaseConnection
{


public:
    //获取数据库实例
    static DatabaseConnection* getInstance();

    DatabaseConnection();
    ~DatabaseConnection();
    bool open();
    bool isopen();
    bool openmain();
    void close();
    void closemain();
    bool isopenmain();
    QSqlDatabase& getDatabase();
     QSqlDatabase& getDatabaseMain();
private:
    //单列模式 保证DatabaseConnection的唯一性
    static DatabaseConnection* Connect;
    //QMutex 来确保在多线程环境下单例的正确创建。
    static QMutex mutex;

    //这是拷贝构造函数的声明，后面跟着 = delete; 表示这个构造函数被删除了。
    //这意味着你不能通过拷贝另一个 DatabaseConnection 对象来创建新的 DatabaseConnection 对象。
    //如果尝试这样做，编译器会报错。
    DatabaseConnection(const DatabaseConnection&) = delete;
    //这是拷贝赋值运算符的声明，同样后面跟着 = delete; 表示这个运算符被删除了。
    //这意味着你不能将一个 DatabaseConnection 对象赋值给另一个 DatabaseConnection 对象。
    //如果尝试这样做，编译器同样会报错。
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;
    //数据库db类
    QSqlDatabase Maindb;
    QSqlDatabase localdb;
};

#endif // DATABASECONNECTION_H
