#include "syncdialog.h"
#include "ui_syncdialog.h"
#include "syncupload.h"
#include <QRunnable>

int SyncDialog::m_totalTasks;
int SyncDialog::stepcount;
int SyncDialog::filecount;


SyncDialog::SyncDialog(QWidget *parent,QString type) :
    QDialog(parent),
    synctype(type),
    ui(new Ui::SyncDialog)
{
    ui->setupUi(this);



}

SyncDialog::~SyncDialog()
{
    delete ui;
}

void SyncDialog::on_button_Start_clicked()
{
    if(synctype == "1"){
            upLoadData();
    }else {
        //qint64 id = dbmang.generateId();
         //zdlogId=QString::number(id);
         zdDataSyncList.clear();
         querydstList.clear();
         //从主系统下载
         //连接两个数据库
         bool IsOpen=false;
         bool IsOpenMain=false;

         DatabaseConnection *Conn=DatabaseConnection::getInstance();
         //DatabaseConnectionMainSys *MainConn=DatabaseConnectionMainSys::getInstance();
         if(!Conn->isopen()){IsOpen=Conn->open();}else{IsOpen=true;};
         if(!Conn->isopenmain()){IsOpenMain=Conn->openmain();}else{IsOpenMain=true;};

         // 记录任务数量

         m_totalTasks=0;
         stepcount=0;
         filecount=0;
         // 获取第一个数据库的所有表名
         dstDb=Conn->getDatabase();
         srcDb=Conn->getDatabaseMain();
         QSqlQuery querydst(dstDb);
         querydst.exec("SHOW TABLES");
         QSqlQuery query(srcDb);
         query.exec("SHOW TABLES");

         while(querydst.next()){
             QString tableName = querydst.value(0).toString();
             if(tableName.contains("act_") ||tableName.contains("flw_ru_")){continue;};
             querydstList.append(tableName);};
         // 创建一个线程池
         threadPool= QThreadPool::globalInstance();
         threadPool->setMaxThreadCount(5); // 设置线程池中的最大线程数
         // 遍历表名，为每个表创建一个任务并提交到线程池
         BaseCommonApi::SaveLogInfo(0,"开始下载同步");

         while (query.next()) {
                 QString tableName = query.value(0).toString();
                 if(!querydstList.contains(tableName)){continue;};
                 if(tableName.contains("ACT_")
                  ||tableName.contains("FLW_RU_")
                  ||tableName.contains("zd"))
                 {continue;};
                 SyncTask *task = new SyncTask(srcDb, dstDb, tableName);
                 connectTaskSignals(task);
                 // 提交任务到线程池
                 threadPool->start(task);
                 ++m_totalTasks;
                 //

              }
         //总数量
        ui->lab_sum->setText(QString::number(m_totalTasks));
        progressDecrement=static_cast<double>(100) / m_totalTasks;
}


}
void SyncDialog::upLoadData()
{
    zdDataSyncList.clear();
    querydstList.clear();
    //从主系统下载
    //连接两个数据库
    bool IsOpen=false;
    bool IsOpenMain=false;

    DatabaseConnection *Conn=DatabaseConnection::getInstance();
    //DatabaseConnectionMainSys *MainConn=DatabaseConnectionMainSys::getInstance();
    if(!Conn->isopen()){IsOpen=Conn->open();}else{IsOpen=true;};
    if(!Conn->isopenmain()){IsOpenMain=Conn->openmain();}else{IsOpenMain=true;};

    // 记录任务数量
    m_totalTasks=0;
    stepcount=0;
    filecount=0;

    dstDb=Conn->getDatabase();
    srcDb=Conn->getDatabaseMain();
//    QList<QString> sqlList;
    QMap<QString,QString> idToSqlMap;
    qDebug() << "开始" << QThread::currentThreadId();
    QSqlQuery selectSql(dstDb);
    bool isSuccess = selectSql.exec("SELECT * FROM  zd_data_sync  WHERE sync_type = '0' ");
    if (!isSuccess)
        return;
    while (selectSql.next()) {
        QString id = selectSql.value("id").toString();
        QString ssql = selectSql.value("ssql").toString();
//        sqlList.append(ssql);
        idToSqlMap.insert(id,ssql);
    }
//    syncupload *syncload = new syncupload(srcDb, dstDb, "zd_data_sync",sqlList);

//    for(const QString& ssql : sqlList){
//                syncupload *syncload = new syncupload(srcDb, dstDb, "zd_data_sync",ssql);
//                threadPool= QThreadPool::globalInstance();
//                threadPool->setMaxThreadCount(QThread::idealThreadCount());
//                connectTaskSignalsByUp(syncload);
//                threadPool->start(syncload);
//                ++m_totalTasks;
//    }

    for(const QString& id : idToSqlMap.keys()){
        syncupload *syncload = new syncupload(srcDb, dstDb, "zd_data_sync",id,idToSqlMap);
        threadPool= QThreadPool::globalInstance();
        threadPool->setMaxThreadCount(QThread::idealThreadCount());
        connectTaskSignalsByUp(syncload);
        threadPool->start(syncload);
        ++m_totalTasks;
    }


    //总数量
   ui->lab_sum->setText(QString::number(m_totalTasks));
   progressDecrement=static_cast<double>(100) / m_totalTasks;
}

void SyncDialog::connectTaskSignalsByUp(syncupload *upload){
    connect(upload, &syncupload::taskCompleted, this, &SyncDialog::onTaskCompleted);
    connect(upload, &syncupload::TextState, this, &SyncDialog::onTextState);
    connect(this,&SyncDialog::StopRequested,upload,&syncupload::UpdateState);
}


void SyncDialog::connectTaskSignals(SyncTask *task) {
    connect(task, &SyncTask::taskCompleted, this, &SyncDialog::onTaskCompleted);
    connect(task, &SyncTask::TextState, this, &SyncDialog::onTextState);
    connect(this,&SyncDialog::StopRequested,task,&SyncTask::UpdateState);
   //当前正在执行的
}
void SyncDialog::onTaskCompleted(int success,QString tableName, const QString& errorMessage)
{
    if(success!=2){
        if(querydstList.contains(tableName)){querydstList.removeAll(tableName);};
        //添加同步表
        //任务完成 任务失败 都需要添加
        qint64 id = dbmang.generateId();
        zdDataSync zdDataSyncentity;
        zdDataSyncentity.id=QString::number(id);;
        zdDataSyncentity.remark=errorMessage;
        zdDataSyncentity.log_sync_id="";
        zdDataSyncentity.sync_type=QString::number(success+1);
        zdDataSyncList.append(zdDataSyncentity);
       // dbmang.InsertzdDataSync(zdDataSyncentity);
    };
    QString mess;
    if (success==0) {
        // 任务成功完成
         mess = "任务完成";
         mess+=errorMessage;

    } else if(success==1){
        // 任务执行失败
        mess = "任务失败";
        mess +=errorMessage;
        filecount++;
        //失败数量
        ui->lab_fail->setText(QString::number(filecount));;
    }else{
        mess = "正在执行";
        mess +=errorMessage+".....";
    }
    //log保存
    BaseCommonApi::SaveLogInfo(0,mess);
    //界面listview显示
    ui->listWidget->addItem(mess);
    // 当添加新项后，滚动到底部
    ui->listWidget->scrollToBottom();

}
void SyncDialog::onTextState()
{

    stepcount++;
    //成功数量
    ui->lab_succ->setText(QString::number(stepcount));;
    //更新进度条
    int progress = static_cast<int>(progressDecrement * stepcount);
    ui->progressBar->setValue(progress);
    if(stepcount>=m_totalTasks)
    {
        this->StopSync();
    }

}

void SyncDialog::on_button_Close_clicked()
{
   emit StopRequested(2);
   threadPool->waitForDone();
   this->StopSync();
   srcDb.close();
   this->close();
}
void SyncDialog::StopSync()
{
    ui->progressBar->setValue(100);
    srcDb.close();
    //dstDb.close();

    //添加日志表
    zdLog zlog;
    zlog.id=QString::number( dbmang.generateId());;
    zlog.type="1";
    if(filecount==0)
    {
        zlog.is_sync="成功";
    }else
    {
        zlog.is_sync="失败";
    }
    zlog.sync_time=QDateTime::currentDateTime();
    zlog.remark="错误数量"+QString::number(filecount);
    dbmang.InsertzdLog(zlog);
    //添加记录表
    for (int i=0;i<zdDataSyncList.count();i++)
    {
        zdDataSyncList[i].log_sync_id=zlog.id;
        dbmang.InsertzdDataSync(zdDataSyncList[i]);
    }
    if(querydstList.count()!=0)
    {
        for (int i=0;i<querydstList.count();i++)
        {
            zdDataSync entity;
            entity.id=QString::number(dbmang.generateId());
            entity.sync_type="0";
            entity.log_sync_id=zlog.id;
            entity.remark="表："+querydstList[i];
            dbmang.InsertzdDataSync(entity);
        }
    }
    BaseCommonApi::SaveLogInfo(0,"结束下载同步");
    QMessageBox::warning(this, "提示", "结束！");
}

void SyncDialog::on_button_step_clicked()
{
    if(ui->button_step->text()=="暂停")
    {
       ui->button_step->setText("继续");
       emit StopRequested(1);


    }else{
        ui->button_step->setText("暂停");
       emit StopRequested(0);
    };
}
