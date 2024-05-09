#include "synctask.h"
#include "QMap"
#include <QtSql>
#include "basecommonapi.h"



SyncTask::SyncTask( QSqlDatabase srcDb,
                    QSqlDatabase dstDb,
                    QString tableName)
    :srcDb(srcDb),dstDb(dstDb), tableName(tableName)
{

   //

}
void SyncTask::run()
{

    this->syncTable();
}

void SyncTask::syncTable()
{

   if(syncstate==SyncRunStatus::Stopped){return;};

       while (syncstate==SyncRunStatus::Paused) {
           //等待
          QThread::sleep(1);
       }

     emit taskCompleted(2,tableName, "表："+tableName);
    // 清空目标数据库中的表数据
     QSqlQuery clearQuery(dstDb);

     clearQuery.prepare("DELETE FROM "+tableName);

     if (!clearQuery.exec()) {
         qDebug() << "删除错误："  << clearQuery.lastError().text();
         //错误处理
         //触发信号
         emit taskCompleted(1,tableName,tableName+"|删除本地表时错误:"+clearQuery.lastError().text());
         return;
     }
     // 从源数据库复制数据到目标数据库
     QSqlQuery copyQuery(srcDb);
     copyQuery.prepare("SELECT * FROM " + tableName);
     if (!copyQuery.exec()) {
         qDebug() << tableName+"查询错误：" << copyQuery.lastError().text();

        emit taskCompleted(1, tableName,tableName+"|查询主系统表时错误:"+copyQuery.lastError().text());


        return;
     }

     while (copyQuery.next()) {
       if(syncstate==SyncRunStatus::Stopped){return;};
        // 构建插入语句并执行
        QStringList values;
        QString ValueJoin;
        QSqlRecord record;
        QString ResValue;
        record=copyQuery.record();
        for (int i = 0; i < copyQuery.record().count(); ++i) {
           if(syncstate==SyncRunStatus::Stopped){return;};
           while (syncstate==SyncRunStatus::Paused) {
               //等待
              QThread::sleep(1);
           }
            //处理时间类型
            if (record.fieldName(i).count("date")||record.fieldName(i).count("TIME")
                    ||record.fieldName(i).count("time")
                    ||record.fieldName(i).count("vaild")||
                    record.fieldName(i).count("BIRTHDAY")||
                    record.fieldName(i).count("person_id") ||
                    record.fieldName(i).count("valid") ){
                  if(copyQuery.value(i).toString()==""){
                      if(i==copyQuery.record().count()-1)
                      {
                          ValueJoin+="NULL";


                      }else{
                          ValueJoin+="NULL,";
                      }

                      continue;
                   };
             }
            //处理内容的 "\T
            ResValue=copyQuery.value(i).toString();
            // 替换双引号
            ResValue = ResValue.replace("\"", " ");

            ResValue = ResValue.replace("\\", "\\\\");
            // 替换字母T
            ResValue = ResValue.replace("T", " ");

            if(i==copyQuery.record().count()-1)
            {
               ValueJoin+="\""+ResValue+"\"";


            }else{
              ValueJoin+="\""+ResValue+"\""+",";
            }

        }
        QSqlQuery insertQuery(dstDb);
        QString str="INSERT INTO " + tableName + " VALUES (" + ValueJoin + ")";
        insertQuery.prepare(str);
        if (!insertQuery.exec()) {

           emit taskCompleted(1, tableName,tableName+"|添加本地表数据时错误:"+insertQuery.lastError().text()+"\t"+ValueJoin);
           qDebug() <<tableName+"1" << copyQuery.lastError().text();
           return;
            // 或者可以记录错误并继续
        }
   }
     emit taskCompleted(0, tableName,"表："+tableName);
     emit TextState();
}
void SyncTask::UpdateState(int state)
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
