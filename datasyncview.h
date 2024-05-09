#ifndef DATASYNCVIEW_H
#define DATASYNCVIEW_H

#include <QWidget>
#include <QObject>
#include "synctask.h"
#include "databaseconnection.h"
#include "basecommonapi.h"
#include "databaseconnectionmainsys.h"
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QSqlDatabase>

namespace Ui {
class DataSyncView;
}

class DataSyncView : public QWidget
{
    Q_OBJECT

public:
    explicit DataSyncView(QWidget *parent = nullptr);
    ~DataSyncView();
private slots:
    void initForm();        //初始化界面数据

    void on_DownloadDataButton_clicked();
    void OnButtonSelectClicked(zdLog log);
    void on_button_return_clicked();

    void on_UploadDataButton_clicked();

public:


private:
    Ui::DataSyncView *ui;
};

#endif // DATASYNCVIEW_H
