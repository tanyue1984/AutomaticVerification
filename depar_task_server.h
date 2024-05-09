#ifndef SLOVECH_H
#define SLOVECH_H

#include <QtSql>
#include <QDebug>
#include <QString>
#include <vector>
#include "QList"
#include "databaseconnection.h"


 class Dept {
 public:
    QString id;
    QString sample_name;
    QString sample_model;
    QString help_instruction;
    QString manufacture_no;
    QString manufacturer;
    QString customer_name;
    QString FULL_NAME;
    QString require_over_time;
    QString is_urgent;
};
class sloveCh
{
public:
    sloveCh();
    QList<Dept> getAllData();

private:
    QSqlDatabase db;

};


#endif // SLOVECH_H
