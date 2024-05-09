#include "depar_task_server.h"

sloveCh::sloveCh()
{

}

QList<Dept> sloveCh::getAllData() {
    QList<Dept> result;

    QSqlQuery query;

    query.exec("SELECT  bi.id ,br.sample_name , br.sample_model ,ei.help_instruction ,ei.manufacture_no ,ei.manufacturer ,bi.customer_name ,dept.FULL_NAME,bi.require_over_time ,bi.is_urgent FROM biz_business_order_info bi LEft JOIN biz_business_order_sample_relation br ON bi.id = br.order_id LEft JOIN eqpt_equipment_info ei ON br.sample_no = ei.equipment_no LEft JOIN sys_dept dept ON ei.dept_id = dept.ID LEft JOIN sys_user user ON user.ID = bi.create_user_id");

    while (query.next()) {
        Dept Dept;
        Dept.id = query.value("id").toString();
        Dept.sample_name = query.value("sample_name").toString();
        Dept.sample_model = query.value("sample_model").toString();
        Dept.help_instruction = query.value("help_instruction").toString();
        Dept.manufacture_no = query.value("manufacture_no").toString();
        Dept.manufacturer = query.value("manufacturer").toString();
        Dept.customer_name = query.value("customer_name").toString();
        Dept.FULL_NAME = query.value("FULL_NAME").toString();
        Dept.require_over_time = query.value("require_over_time").toString();
        Dept.is_urgent = query.value("is_urgent").toString();
        result.append(Dept);
    }

    return result;
}
