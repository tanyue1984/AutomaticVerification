#ifndef TECHNIQUE_FILE_SERVE_H
#define TECHNIQUE_FILE_SERVE_H
#include <QtSql>
#include <QDebug>
#include <QString>
#include "QList"

//技术文件
class Technique_File
{
public:
    QString file_distribute_no;
    QString file_name;
    QString version_no;
    QString active_date;
    QString remark;
};

class Technique_File_Serve
{
public:
    Technique_File_Serve();
    QList<Technique_File> getTechnique_File();
};

#endif // TECHNIQUE_FILE_SERVE_H
