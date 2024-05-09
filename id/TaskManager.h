#ifndef TASKMANAGER_H
#define TASKMANAGER_H
#include <QString>
#include <QMap>
#include <QDate>
#include <QDateTime>
#include <QMutex>
#include "basecommonapi.h"

class TaskManager {
private:
    static QMap<QString, int> dailyTaskCounts; // 日期 -> 任务单数量
    static QMap<QString, int> saveResultdatanoList; // 日期 -> 结果保存
    static QMap<QString, int> checkResultDataNoMap; // 日期 -> 自动核查数据单数量
    static QDate currentDate;
    static QString KeyDate;
    //ResultKeyDate
    static QString ResultKeyDate;
    static QString checkDataKeyDate;
public:
   static int getTaskCountForToday();
   static bool AddCurrenTaskCount();
   static QString generateTask(QString Number);
   static bool AddResultDataNoCount();
   static QString getsaveResultdatano();
   /******** 20240502 tanyue添加 ********/
   // 生成新的自动核查数据单编号
   static QString getCheckResultDataNo();
   /******** 20240502 ********/
};


#endif // TASKMANAGER_H
