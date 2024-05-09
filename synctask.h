#ifndef SYNCTASK_H
#define SYNCTASK_H

#include <QRunnable>
#include <QtSql>
#include <QDebug>
#include "basecommonapi.h"
#include <QObject>


class SyncTask :public QObject,public QRunnable  {
Q_OBJECT
public:

    SyncTask(QSqlDatabase srcDb,QSqlDatabase dstDb,  QString tableName);


    void run() override;
    void UpdateState(int state);

signals:
    void taskCompleted(int success,QString tableName, const QString& errorMessage);
    void TextState();


private:
    void syncTable();

private:
    QSqlDatabase srcDb;
    QSqlDatabase dstDb;
    QString tableName;
    SyncRunStatus syncstate;
};
#endif // STANDARD_DEVICEITEM_H
