#ifndef CREATETASKS_H
#define CREATETASKS_H
#include <QtSql>
#include <QDebug>
#include <QString>
#include "QList"
//创建任务单-首页
class CreateTaskList
{
public:
    QString id;
    QString order_no;
    QString customer_name;
    QString DEPT_FULL_NAME;
    QString require_over_time;
    QString deliverer;
    QString is_urgent;
};
//创建任务单-查看任务单
class CatTask
{
public:
    QString id;
    QString order_no;
    QString customer_name;
    QString DEPT_FULL_NAME;
    QString require_over_time;
    QString deliverer;
    QString deliverer_tel;
    QString undertaker_name;
    QString undertake_time;
    QString is_urgent;
    QString receive_illustrate;
};
class  CatTaskList
{
public:
    QString id;
    QString sample_name;
    QString sample_model;
    QString manufacture_no;
    QString manufacturer;
    QString appendix_descn;
    QString special_require;
    QString measure_complete_time;
    QString conclusion;
    QString limit_instruction;
};


class CreateTasks
{
public:
    CreateTasks();
    QList<CreateTaskList>getCreateTaskList();
    QList<CatTask>getCatTask(QString orderNo);
    QList<CatTaskList>getCatTaskList(QString orderId);
};

#endif // CREATETASKS_H
