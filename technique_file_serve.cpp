#include "technique_file_serve.h"

Technique_File_Serve::Technique_File_Serve()
{

}
QList<Technique_File> Technique_File_Serve::getTechnique_File()
{
    QList<Technique_File> result;
    QSqlQuery query;
     query.exec("SELECT brtf.file_distribute_no ,brtf.file_name ,brtf.version_no ,brtf.active_date ,brtf.remark "
                " FROM biz_resource_technique_file brtf");

     if (query.exec()) {
          while (query.next()) {
              Technique_File file;
              file.file_distribute_no = query.value("file_distribute_no").toString();
              file.file_name = query.value("file_name").toString();
              file.version_no = query.value("version_no").toString();
              file.active_date = query.value("active_date").toString();
              file.remark = query.value("remark").toString();

              result.append(file);
          }
      } else {
          qDebug() << "Query execution failed: " << query.lastError().text();
      }

      return result;

}
