#include "create_tasks_serve.h"

CreateTasks::CreateTasks()
{

}
QList<CreateTaskList>CreateTasks::getCreateTaskList(){
    QList<CreateTaskList> result;
    QSqlQuery query;
      query.exec("SELECT bi.id,bi.order_no  ,bi.customer_name ,dept.FULL_NAME ,"
                 "bi.require_over_time , bi.deliverer ,bi.is_urgent  FROM biz_business_order_info bi,sys_dept dept WHERE bi.customer_id = dept.ID");
      if (query.exec()) {
             while (query.next()) {
                 CreateTaskList task;
                 task.id = query.value("id").toString();
                 task.order_no = query.value("order_no").toString();
                 task.customer_name = query.value("customer_name").toString();
                 task.DEPT_FULL_NAME = query.value("FULL_NAME").toString();
                 task.require_over_time = query.value("require_over_time").toString();
                 task.deliverer = query.value("deliverer").toString();
                 task.is_urgent = query.value("is_urgent").toString();
                 result.append(task);
             }
         } else {
             qDebug() << "Query execution failed: " << query.lastError().text();
         }
      return result;
}

QList<CatTask>CreateTasks::getCatTask(QString orderNo)
{
    QList<CatTask> result;
    QSqlQuery query;
     query.exec("SELECT bi.id, bi.order_no, bi.customer_name, dept.FULL_NAME, bi.require_over_time,"
                "bi.deliverer, bi.deliverer_tel, bi.undertaker_name, bi.undertake_time, bi.is_urgent,"
                "bi.receive_illustrate FROM biz_business_order_info bi LEFT JOIN sys_dept dept ON bi.customer_id = dept.ID"
                " WHERE bi.order_no = :order_no");

     // 将参数绑定到占位符
      query.bindValue(":order_no", orderNo);
      if (query.exec()) {
             while (query.next()) {
                 CatTask task;
                 task.id = query.value("id").toString();
                 task.order_no = query.value("order_no").toString();
                 task.customer_name = query.value("customer_name").toString();
                 task.DEPT_FULL_NAME = query.value("FULL_NAME").toString();
                 task.require_over_time = query.value("require_over_time").toString();
                 task.deliverer = query.value("deliverer").toString();
                 task.deliverer_tel = query.value("deliverer_tel").toString();
                 task.undertaker_name = query.value("undertaker_name").toString();
                 task.undertake_time = query.value("undertake_time").toString();
                 task.is_urgent = query.value("is_urgent").toString();
                 task.receive_illustrate = query.value("receive_illustrate").toString();
                 result.append(task);
             }
         } else {
             qDebug() << "Query execution failed: " << query.lastError().text();
         }

         return result;
}

QList<CatTaskList>CreateTasks::getCatTaskList(QString orderId)
{
    QList<CatTaskList> result;
    QSqlQuery query;
     query.exec("SELECT br.id , br.sample_name ,br.sample_model ,ei.manufacture_no,ei.manufacturer ,br.appendix_descn ,"
                "br.special_require ,br.measure_complete_time ,bbdmi.conclusion ,ei.limit_instruction FROM biz_business_order_sample_relation br"
                " Left JOIN eqpt_equipment_info ei ON br.sample_no = ei.equipment_no "
                "Left JOIN biz_business_device_measure_info bbdmi ON  br.order_id = bbdmi.order_id "
                "WHERE br.order_id = :order_id");
     // 将参数绑定到占位符
      query.bindValue(":order_id", orderId);
      if (query.exec()) {
             while (query.next()) {
                 CatTaskList task;
                 task.id = query.value("id").toString();
                 task.sample_name = query.value("sample_name").toString();
                 task.sample_model = query.value("sample_model").toString();
                 task.manufacture_no = query.value("manufacture_no").toString();
                 task.manufacturer = query.value("manufacturer").toString();
                 task.appendix_descn = query.value("appendix_descn").toString();
                 task.special_require = query.value("special_require").toString();
                 task.measure_complete_time = query.value("measure_complete_time").toString();
                 task.conclusion = query.value("conclusion").toString();
                 task.limit_instruction = query.value("limit_instruction").toString();
                 result.append(task);
             }
         } else {
             qDebug() << "Query execution failed: " << query.lastError().text();
         }

         return result;
}
