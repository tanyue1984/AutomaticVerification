#ifndef SYNCDIALOG_H
#define SYNCDIALOG_H

#include <QDialog>
#include "databaseconnection.h"
#include "basecommonapi.h"
#include "databaseconnectionmainsys.h"
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QSqlDatabase>
#include "synctask.h"
#include "syncupload.h"

namespace Ui {
class SyncDialog;
}

class SyncDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SyncDialog(QWidget *parent = nullptr,QString type=NULL);
    ~SyncDialog();

private slots:
    void on_button_Start_clicked();


    void connectTaskSignals(SyncTask *task);
    void connectTaskSignalsByUp(syncupload *upload);
    void onTaskCompleted(int success,QString tableName, const QString& errorMessage);
    void onTextState();
    void on_button_Close_clicked();
    void StopSync();
    void on_button_step_clicked();
    void upLoadData();

public:
    static int m_totalTasks;
    static int filecount;
    static int stepcount;

    QThreadPool *threadPool;
    QSqlDatabase dstDb;
    QSqlDatabase srcDb;
    double progressDecrement;

    QString zdlogId="";

    BaseCommonApi dbmang;
    QList<zdDataSync> zdDataSyncList;
    QList<QString> querydstList;
    QString synctype;
signals:
    void StopRequested(int state); // 定义暂停 停止请求信号
private:
    Ui::SyncDialog *ui;
};

#endif // SYNCDIALOG_H
