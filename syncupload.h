
#ifndef SYNCUPLOAD_H
#define SYNCUPLOAD_H
#include <QRunnable>
#include <QtSql>
#include <QDebug>
#include "basecommonapi.h"
#include <QObject>
#include <QThread>
#include <QThreadPool>
#include <QList>



class syncupload : public QObject, public QRunnable
{
    Q_OBJECT
public:
//    syncupload(QSqlDatabase cloud, QSqlDatabase local, QString tableName,const QList<QString>& sqlList);
        syncupload(QSqlDatabase cloud, QSqlDatabase local, QString tableName,QString ssql,QMap<QString,QString> IdToSqlMap);
    SyncRunStatus syncstate;


//signals:
//    void taskCompleted(int success, QString tableName, const QString& errorMessage);

    QSqlDatabase cloud;
    QSqlDatabase local;
    QString tableName;
//    QList<QString> idList;
    QString ssql;
    QMap<QString,QString> IdToSqlMap;
    void run() override;
    void SysncUploadCount();
    void UpdateState(int state);
signals:
    void taskCompleted(int success,QString tableName, const QString& errorMessage);
    void TextState();
};



#endif // SYNCUPLOAD_H
