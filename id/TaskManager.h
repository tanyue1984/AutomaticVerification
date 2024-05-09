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
    static QDate currentDate;
    static QString KeyDate;
    //ResultKeyDate
    static QString ResultKeyDate;
public:


   static int getTaskCountForToday();
   static bool AddCurrenTaskCount();
   static QString generateTask(QString Number);
   static bool AddResultDataNoCount();
   static QString getsaveResultdatano();
};


#endif // TASKMANAGER_H
