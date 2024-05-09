
#include "syncupload.h"

syncupload::syncupload(QSqlDatabase cloud, QSqlDatabase local, QString tableName,QString ssql,QMap<QString,QString> IdToSqlMap)
    : cloud(cloud), local(local), tableName(tableName),ssql(ssql),IdToSqlMap(IdToSqlMap)
{}

void syncupload::run()
{
    this->SysncUploadCount();
}

void syncupload::SysncUploadCount()
{
    if(syncstate==SyncRunStatus::Stopped){return;};
        while (syncstate==SyncRunStatus::Paused) {
           QThread::sleep(1);
        }
    QSqlQuery runSql(cloud);
//    for (const QString& ssql : this->sqlList) {
           if (runSql.exec(IdToSqlMap.value(ssql))) {
               QSqlQuery localQuery(local);
               localQuery.exec("UPDATE zd_data_sync SET sync_type = 1  WHERE id= " + ssql);

               qDebug() << "执行 SQL 成功";
                    emit taskCompleted(2,tableName, "cg表："+tableName);
               qDebug() << "当前线程:" << QThread::currentThreadId();
           } else {
               qDebug() << "执行 SQL 失败:" << runSql.lastError().text();
               emit taskCompleted(1,tableName,tableName+"|错误:"+runSql.lastError().text());
        qDebug() << "当前线程:" << QThread::currentThreadId();
           }
//       }
           emit TextState();
}
void syncupload::UpdateState(int state)
 {
    if(state==0)
    {
       syncstate=SyncRunStatus::Running;
    }else if(state==1){
        syncstate=SyncRunStatus::Paused;
    }else{
          syncstate=SyncRunStatus::Stopped;
    }
 }

