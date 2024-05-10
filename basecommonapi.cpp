#include "basecommonapi.h"

#include <QByteArray>
#include <QDateTime>
#include <QDebug>
#include <QProcess>
#include <QString>

#include "global.h"

BaseCommonApi::BaseCommonApi() { }
// const QString BaseCommonApi::temporary_user_id="1634107227953819649";
// const QString BaseCommonApi::measure_person_id="1695282436182188034";
static SyncRunStatus syncstate = SyncRunStatus::Stopped;

// sys_user表的id转换为measure_person_id 检定人id（meter_staff表id）
// StaffInfo BaseCommonApi::ConvertStaffInfo(QString userId)
//{
//     userId=temporary_user_id;
//     StaffInfo result;
//     QSqlQuery query;
//     query.prepare("SELECT brsi.* from biz_resource_staff_info brsi "
//                   " JOIN sys_user ON sys_user.ACCOUNT = brsi.account "
//                   " WHERE sys_user.id = :userId ");

//     query.bindValue(":userId", userId);
//     if (query.exec()) {
//         while (query.next()) {
//             StaffInfo info;
//             info.id = query.value("id").toString();
//             info.account = query.value("account").toString();
//             info.staff_no = query.value("staff_no").toString();
//             info.staff_name = query.value("staff_name").toString();
//             info.picture_file = query.value("picture_file").toString();
//             info.id_card = query.value("id_card").toString();
//             info.birthday = query.value("birthday").toString();
//             info.native_place = query.value("native_place").toString();
//             info.education = query.value("education").toString();
//             info.record_date = query.value("record_date").toString();
//             info.officer_no = query.value("officer_no").toString();
//             info.gender = query.value("gender").toString();
//             info.graduation_date = query.value("graduation_date").toString();
//             info.nation = query.value("nation").toString();
//             info.degree = query.value("degree").toString();
//             info.dept_id = query.value("dept_id").toString();
//             info.dept_name = query.value("dept_name").toString();
//             info.station = query.value("station").toString();
//             info.major = query.value("major").toString();
//             info.certificate_number =
//             query.value("certificate_number").toString(); info.engage_date =
//             query.value("engage_date").toString(); info.working_age =
//             query.value("working_age").toString(); info.title_initial_date =
//             query.value("title_initial_date").toString(); info.remark =
//             query.value("remark").toString(); info.create_time =
//             query.value("create_time").toDateTime(); info.update_time =
//             query.value("update_time").toDateTime(); result=info;

//         }
////          qDebug() << "sql代码： " << query.lastQuery();
//     }else {
//         qDebug() << "Query execution failed: " << query.lastError().text();
//     }

//         return result;
//}

bool SysUserLoginByCount(QString account, QString password = nullptr)
{
    QSqlQuery query;
    query.prepare(
        "SELECT count(*) as num	 FROM sys_user"
        " WHERE ACCOUNT = :account ");
    query.bindValue(":account", account);
    bool isSuccess = query.exec();
    if (isSuccess && query.next()) {
        int num = query.value("num").toInt();
        if (num > 0)
            return true;
    }
    return false;
}
bool getMeasurePersonId(QString account)
{
    QSqlQuery query;
    query.prepare(
        "SELECT * FROM biz_resource_staff_info WHERE account = :account ");
    query.bindValue(":account", account);
    if (query.exec()) {
        while (query.next()) {
            loginMeasurePersonId = query.value("id").toString();
        }
    }
    if (loginMeasurePersonId != "0") {
        return true;
    }
    return false;
}

bool BaseCommonApi::SysUserLogin(QString account, QString password = nullptr)
{
    bool isCount = SysUserLoginByCount(account, password);
    QSqlQuery query;
    query.prepare(
        "SELECT *	FROM sys_user "
        " WHERE ACCOUNT = :ACCOUNT ");
    query.bindValue(":ACCOUNT", account);
    //    query.bindValue(":PASSWORD",password);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << query.lastError().text();
        return false;
    }
    //    QCryptographicHash hash(QCryptographicHash::Md5);
    //    hash.addData("gph2o");
    //    QByteArray result = hash.result();
    //    QString md5 = result.toHex();

    //    qDebug() << "MD5:" << md5;

    //    hash.reset();
    //    hash.addData(result);
    //    hash.addData("1234567");

    //    QByteArray pwdRst = hash.result();

    //    for (int i = 0; i < 999; i++) {
    //        hash.reset();
    //        hash.addData(pwdRst);

    //        pwdRst = hash.result();
    //    }

    //    qDebug() << "===" << pwdRst.toHex();
    if (isSuccess && isCount && getMeasurePersonId(account)) {
        while (query.next()) {
            loginUserId = query.value("ID").toString();
            loginUserName = query.value("NAME").toString();
            loginUserDeptId = query.value("DEPT_ID").toString();
        }
        return true;
    } else {
        return false;
    }
}

// 获取部门任务数据
QList<Dept> BaseCommonApi::getAllData(QString userId)
{
    QList<Dept> result;
    QSqlQuery query;
    //    QString deptId=ConvertStaffInfo(temporary_user_id).dept_id;
    query.prepare(
        "SELECT   blei.id AS id, blei.sample_id as "
        "equipmentId,eei.equipment_name AS sampleName, eei.model AS "
        "sampleModel,eei.help_instruction AS helpInstruction, "
        " eei.manufacture_no AS manufactureNo, "
        "eei.manufacturer,bo.customer_name,sd.SIMPLE_NAME,bo.require_over_time, "
        "bo.is_urgent,blei.measure_status,bo.id as orderId,bo.create_user_id "
        "FROM biz_business_lab_executive_info blei  "
        "JOIN eqpt_equipment_info eei ON eei.id = blei.sample_id "
        "LEFT JOIN sys_dept sd ON blei.measure_dept_id = sd.ID "
        " JOIN biz_business_order_info bo ON bo.id = blei.order_id   "
        " WHERE blei.measure_dept_id = (SELECT brsi.dept_id from "
        "biz_resource_staff_info brsi"
        " JOIN sys_user ON sys_user.ACCOUNT = brsi.account"
        " WHERE sys_user.id = :userId ) "
        " ORDER BY blei.update_time DESC");
    // 将参数绑定到占位符
    query.bindValue(":userId", userId);

    if (query.exec()) {
        while (query.next()) {
            Dept dept;
            dept.id = query.value("id").toString();
            dept.sample_name = query.value("sampleName").toString();
            dept.sample_id = query.value("equipmentId").toString();
            dept.sample_model = query.value("sampleModel").toString();
            dept.help_instruction = query.value("helpInstruction").toString();
            dept.manufacture_no = query.value("manufactureNo").toString();
            dept.manufacturer = query.value("manufacturer").toString();
            dept.customer_name = query.value("customer_name").toString();
            dept.FULL_NAME = query.value("SIMPLE_NAME").toString();
            dept.require_over_time = query.value("require_over_time").toDateTime();
            dept.is_urgent = query.value("is_urgent").toString();
            dept.measure_status = query.value("measure_status").toString();
            dept.orderId = query.value("orderId").toString();
            dept.create_user_id = query.value("create_user_id").toString();
            result.append(dept);
            //        qDebug() <<"---+"<<dept.sample_id;
        }

    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    return result;
}
// 获取我的任务  参数登录用户的id 和 任务状态
// blei.measure_status 3 为检定中的状态
// blei.measure_status 4 为检定完
// blei.measure_status 10 已退回
QList<MyTask> BaseCommonApi::getMyTasks(QString userId)
{
    QList<MyTask> result;
    QSqlQuery query;
    query.prepare(
        "SELECT blei.id AS id, eei.id AS sample_id, eei.equipment_no AS "
        "sampleNo, eei.equipment_name AS sampleName, eei.model AS sampleModel, "
        "eei.check_cycle,"
        " eei.manufacture_no AS manufactureNo, eei.manufacturer, "
        "eei.help_instruction AS helpInstruction, bo.order_no AS orderNo, bo.id "
        "AS orderId, "
        "bo.customer_id,"
        " bo.customer_name, bo.deliverer, bo.is_urgent, bo.require_over_time, "
        "blei.require_certifications, blei.measure_status, bo.customer_address,"
        " blei.measure_person_id, eei.RFID AS labelBind, eei.certificate_valid "
        "AS certificateValid, sd.ID AS measureDeptId, sd.SIMPLE_NAME AS "
        "measureDeptName"
        " FROM biz_business_lab_executive_info blei"
        " JOIN eqpt_equipment_info eei ON eei.id = blei.sample_id"
        " JOIN biz_business_order_info bo ON bo.id = blei.order_id"
        " LEFT JOIN sys_dept sd ON blei.measure_dept_id = sd.ID"
        " WHERE blei.measure_person_id = (SELECT brsi.id from "
        "biz_resource_staff_info brsi"
        " JOIN sys_user ON sys_user.ACCOUNT = brsi.account"
        " WHERE sys_user.id = :userId )"
        " AND blei.measure_status = 3 "
        " ORDER BY blei.update_time DESC");

    // 将参数绑定到占位符
    query.bindValue(":userId", userId);
    // query.bindValue(":status", status);
    if (query.exec()) {
        while (query.next()) {
            MyTask task;
            task.id = query.value("id").toString();
            task.sample_id = query.value("sample_id").toString();
            task.sampleNo = query.value("sampleNo").toString();
            task.sampleName = query.value("sampleName").toString();
            task.sampleModel = query.value("sampleModel").toString();
            task.check_cycle = query.value("check_cycle").toString();
            task.manufactureNo = query.value("manufactureNo").toString();
            task.manufacturer = query.value("manufacturer").toString();
            task.help_instruction = query.value("helpInstruction").toString();
            task.orderNo = query.value("orderNo").toString();
            task.orderId = query.value("order_id").toString();
            task.customer_id = query.value("customer_id").toString();
            task.customer_name = query.value("customer_name").toString();
            task.deliverer = query.value("deliverer").toString();
            task.is_urgent = query.value("is_urgent").toString();
            task.require_over_time = query.value("require_over_time").toDateTime();
            task.require_certifications = query.value("require_certifications").toString();
            task.measure_status = query.value("measure_status").toString();
            task.customer_address = query.value("customer_address").toString();
            task.measure_person_id = query.value("measure_person_id").toString();
            task.labelBind = query.value("labelBind").toString();
            task.certificateValid = query.value("certificateValid").toString();
            task.measureDeptId = query.value("measureDeptId").toString();
            task.FULL_NAME = query.value("measureDeptName").toString();
            loginMeasureDeptName = query.value("measureDeptName").toString();
            result.append(task);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
}
// 查看最新结果保存数据
SaveDeviceMeasureInfo BaseCommonApi::getNewResultData()
{
    SaveDeviceMeasureInfo result;
    QSqlQuery query;
    query.prepare(
        "select * from biz_business_device_measure_info as bz ORDER BY  "
        "bz.create_time DESC LIMIT 1");
    if (query.exec()) {
        while (query.next()) {
            result.id = query.value("id").toString();
            result.data_no = query.value("data_no").toString();
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
}
// 查看当前最新的任务单
QList<CreateTaskList> BaseCommonApi::getCreateTaskListNew()
{
    QList<CreateTaskList> result;
    QSqlQuery query;
    query.prepare(
        "select * from biz_business_order_info as bz ORDER BY  bz.create_time "
        "DESC LIMIT 1");
    if (query.exec()) {
        while (query.next()) {
            CreateTaskList task;
            task.id = query.value("id").toString();
            task.order_no = query.value("order_no").toString();
            task.customer_name = query.value("customer_name").toString();
            task.DEPT_FULL_NAME = query.value("FULL_NAME").toString();
            task.require_over_time = query.value("require_over_time").toDateTime();
            task.deliverer = query.value("deliverer").toString();
            task.is_urgent = query.value("is_urgent").toString();
            result.append(task);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
}
// 获取创建任务首页数据
QList<CreateTaskList> BaseCommonApi::getCreateTaskList(QString userId)
{
    userId = loginUserId;
    QList<CreateTaskList> result;
    QSqlQuery query;
    query.prepare(
        "SELECT "
        "bi.id,bi.order_no,bi.customer_name,dept.FULL_NAME,bi.require_over_time,"
        "bi.deliverer,bi.is_urgent "
        " FROM biz_business_order_info bi LEFT JOIN sys_dept dept ON "
        "bi.customer_id = dept.ID "
        " WHERE bi.create_user_id = :userId ORDER BY bi.create_time DESC");
    // 将参数绑定到占位符
    query.bindValue(":userId", userId);
    if (query.exec()) {
        while (query.next()) {
            CreateTaskList task;
            task.id = query.value("id").toString();
            task.order_no = query.value("order_no").toString();
            task.customer_name = query.value("customer_name").toString();
            task.DEPT_FULL_NAME = query.value("FULL_NAME").toString();
            task.require_over_time = query.value("require_over_time").toDateTime();
            task.deliverer = query.value("deliverer").toString();
            task.is_urgent = query.value("is_urgent").toString();
            result.append(task);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
}
// 获取创建任务首页数据
QList<CreateTaskList> getCreateTaskList(QString userId)
{
    QList<CreateTaskList> result;
    QSqlQuery query;
    query.prepare(
        "SELECT "
        "bi.id,bi.order_no,bi.customer_name,dept.FULL_NAME,bi.require_over_time,"
        "bi.deliverer,bi.is_urgent "
        " FROM biz_business_order_info bi LEFT JOIN sys_dept dept ON "
        "bi.customer_id = dept.ID "
        " WHERE bi.create_user_id =  :userId  ");
    // 将参数绑定到占位符
    query.bindValue(":userId", userId);
    if (query.exec()) {
        while (query.next()) {
            CreateTaskList task;
            task.id = query.value("id").toString();
            task.order_no = query.value("order_no").toString();
            task.customer_name = query.value("customer_name").toString();
            task.DEPT_FULL_NAME = query.value("FULL_NAME").toString();
            task.require_over_time = query.value("require_over_time").toDateTime();
            task.deliverer = query.value("deliverer").toString();
            task.is_urgent = query.value("is_urgent").toString();
            result.append(task);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    return result;
}

// 创建任务单-查看/编辑 顶部数据  传参该条数据的id
QList<CatTask> BaseCommonApi::getCatTask(QString id)
{
    QList<CatTask> result;
    QSqlQuery query;
    query.prepare(
        "SELECT bi.id, bi.order_no ,bi.customer_name ,dept.FULL_NAME "
        ",bi.require_over_time ,"
        " bi.deliverer ,bi.deliverer_tel ,bi.undertaker_name ,bi.undertake_time "
        ",  bi.is_urgent ,"
        " bi.receive_illustrate  FROM biz_business_order_info bi LEFT JOIN "
        "sys_dept dept ON bi.customer_id = dept.ID"
        " WHERE  bi.id = :id  ");
    // 将参数绑定到占位符
    query.bindValue(":id", id);
    if (query.exec()) {
        while (query.next()) {
            CatTask task;
            task.id = query.value("id").toString();
            task.order_no = query.value("order_no").toString();
            task.customer_name = query.value("customer_name").toString();
            task.DEPT_FULL_NAME = query.value("FULL_NAME").toString();
            task.require_over_time = query.value("require_over_time").toDateTime();
            task.deliverer = query.value("deliverer").toString();
            task.deliverer_tel = query.value("deliverer_tel").toString();
            task.undertaker_name = query.value("undertaker_name").toString();
            task.undertake_time = query.value("undertake_time").toDateTime();
            task.is_urgent = query.value("is_urgent").toString();
            task.receive_illustrate = query.value("receive_illustrate").toString();
            result.append(task);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    return result;
}

// 获取创建任务 -任务单列表
QList<CatTaskList> BaseCommonApi::getCatTaskList(QString orderId)
{
    QList<CatTaskList> result;
    QSqlQuery query;
    query.prepare(
        "SELECT br.id , br.sample_name ,br.sample_model "
        ",ei.manufacture_no,ei.manufacturer ,br.appendix_descn ,"
        "br.special_require ,br.measure_complete_time ,bbdmi.conclusion "
        ",ei.limit_instruction FROM biz_business_order_sample_relation br"
        " Left JOIN eqpt_equipment_info ei ON br.sample_no = ei.equipment_no "
        "Left JOIN biz_business_device_measure_info bbdmi ON  br.order_id = "
        "bbdmi.order_id "
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
            task.measure_complete_time = query.value("measure_complete_time").toDateTime();
            task.conclusion = query.value("conclusion").toString();
            task.limit_instruction = query.value("limit_instruction").toString();
            result.append(task);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    return result;
}

// 先查询指定下拉框的字段
QList<SelectSpecifyFields> BaseCommonApi::getSelectSpecifyFields()
{
    QList<SelectSpecifyFields> result;

    QSqlQuery query;
    bool IsSucc = query.exec(
        "SELECT bi.id,bi.order_no  ,bi.customer_name ,dept.FULL_NAME ,"
        "bi.require_over_time , bi.deliverer ,bi.is_urgent  "
        "FROM biz_business_order_info bi,sys_dept dept WHERE bi.customer_id = "
        "dept.ID");
    if (IsSucc) {
        while (query.next()) {
            SelectSpecifyFields customerInfo;
            customerInfo.id = query.value("id").toString();
            customerInfo.customer_no = query.value("customer_no").toString();
            customerInfo.customer_name = query.value("customer_name").toString();
            customerInfo.FULL_NAME = query.value("FULL_NAME").toString();
            result.append(customerInfo);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    return result;
}
// 新增任务单
bool BaseCommonApi::InsertTaskBasic(InsertTask insertTask)
{
    // 1640229293790650370
    insertTask.create_user_id = loginUserId;
    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_business_order_info (id,"
        "order_no, customer_name, require_over_time, deliverer, deliverer_tel,"
        "undertaker_name, undertake_time, is_urgent, receive_illustrate, "
        "customer_id,create_user_id"
        "   ) VALUES (:id, "
        "     :order_no, :customer_name, :require_over_time, "
        "    :deliverer, :deliverer_tel, :undertaker_name, :undertake_time, "
        "    :is_urgent, "
        "    :receive_illustrate,"
        "   :customer_id,"
        "   :create_user_id"
        " );");

    query.bindValue(":id", insertTask.id);
    query.bindValue(":order_no", insertTask.order_no);
    query.bindValue(":customer_name", insertTask.customer_name);
    query.bindValue(":require_over_time", insertTask.require_over_time);
    query.bindValue(":deliverer", insertTask.deliverer);
    query.bindValue(":deliverer_tel", insertTask.deliverer_tel);
    query.bindValue(":undertaker_name", insertTask.undertaker_name);
    query.bindValue(":undertake_time", insertTask.undertake_time);
    query.bindValue(":is_urgent", insertTask.is_urgent);
    query.bindValue(":receive_illustrate", insertTask.receive_illustrate);
    query.bindValue(":customer_id", insertTask.customer_id);
    query.bindValue(":Create_user_id", insertTask.create_user_id);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << "InsertTaskBasic query failed: " << query.lastError().text();
    }

    return isSuccess;
}
// 新增任务单样品表
bool BaseCommonApi::InsertOrderSampleRelation(OrderSampleRelation orderSample)
{
    orderSample.create_user_id = loginUserId;
    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_business_order_sample_relation (id,order_id,sample_id,"
        "sample_name, sample_model,measure_complete_time,create_time,update_time)"
        "VALUES (:id,:order_id,:sample_id,"
        ":sample_name, "
        ":sample_model,:measure_complete_time,:create_time,:update_time);");

    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", orderSample.id);
    query.bindValue(":order_id", orderSample.order_id);
    query.bindValue(":sample_id", orderSample.sample_id);
    query.bindValue(":sample_name", orderSample.sample_name);
    query.bindValue(":sample_model", orderSample.sample_model);
    query.bindValue(":measure_complete_time", orderSample.measure_complete_time);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);

    bool isSuccess = query.exec();

    if (isSuccess) {
        QString ssql = "INSERT INTO biz_business_order_sample_relation (id,order_id,sample_id,"
                       "sample_name, "
                       "sample_model,measure_complete_time,create_time,update_time)"
                       "VALUES (:1,:2,:3,"
                       ":4, :5,:6,:7,:8);";
        saveLogToSql(ssql, &orderSample);
    } else {
        qDebug() << " query failed: " << query.lastError().text();
    }

    return isSuccess;
}
// 新增任务单 信息记录表
bool BaseCommonApi::InsertTaskInfoRecord(
    biz_business_lab_executive_info eneity)
{
    eneity.create_user_id = loginUserId;
    eneity.measure_person_id = loginMeasurePersonId;
    QSqlQuery query;
    query.prepare(
        "INSERT "
        "biz_business_lab_executive_info(id,order_id,sample_id,measure_dept_id,"
        "measure_person_id,create_user,measure_status,is_auto"
        ")VALUES"
        "(:id,:order_id,:sample_id,:measure_dept_id,:measure_person_id,"
        ":create_user_id,:measure_status,:is_auto,:create_time,:update_time)");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    // Bind values to the parameters
    query.bindValue(":id", eneity.id);
    query.bindValue(":order_id", eneity.order_id);
    query.bindValue(":sample_id", eneity.sample_id);
    query.bindValue(":measure_dept_id", eneity.measure_dept_id);
    query.bindValue(":measure_person_id", loginMeasurePersonId);
    query.bindValue(":create_user_id", eneity.create_user_id);
    query.bindValue(":measure_status", "3");
    query.bindValue(":is_auto", "0");
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();

    if (isSuccess) {
        QString ssql = "INSERT "
                       "biz_business_lab_executive_info(id,order_id,sample_id,measure_dept_id,"
                       "measure_person_id,create_user,measure_status,is_auto,create_time,"
                       "update_time"
                       ")VALUES"
                       "(:1,:2,:3,:4,:5,"
                       ":6,:7,:8,:9,:10);";
        saveLogToSql(ssql, &eneity);
    } else {
        qDebug() << " query failed: " << query.lastError().text();
    }

    return isSuccess;
}
// 更新 任务单
bool BaseCommonApi::UpdateTask(InsertTask updateTask)
{
    QSqlQuery query;
    query.prepare(
        "UPDATE biz_business_order_info"
        " SET order_no = :order_no,customer_name = :customer_name,"
        " require_over_time = :require_over_time,deliverer = :deliverer,"
        "   deliverer_tel = :deliverer_tel,undertaker_name = :undertaker_name,"
        " undertake_time = :undertake_time, is_urgent = :is_urgent,"
        "  receive_illustrate = :receive_illustrate, customer_id = :customer_id "
        " WHERE id = :id;");
    query.bindValue(":order_no", updateTask.order_no);
    query.bindValue(":customer_name", updateTask.customer_name);
    query.bindValue(":require_over_time", updateTask.require_over_time);
    query.bindValue(":deliverer", updateTask.deliverer);
    query.bindValue(":deliverer_tel", updateTask.deliverer_tel);
    query.bindValue(":undertaker_name", updateTask.undertaker_name);
    query.bindValue(":undertake_time", updateTask.undertake_time);
    query.bindValue(":is_urgent", updateTask.is_urgent);
    query.bindValue(":receive_illustrate", updateTask.receive_illustrate);
    query.bindValue(":customer_id", updateTask.customer_id);
    query.bindValue(":id", updateTask.id);

    bool isSuccess = query.exec();
    if (isSuccess) {
        QString ssql = "UPDATE biz_business_order_info"
                       " SET order_no = :1,customer_name = :2,"
                       " require_over_time = :3,deliverer = :4,"
                       "   deliverer_tel = :5,undertaker_name = :6,"
                       " undertake_time = :7, is_urgent = :8,"
                       "  receive_illustrate = :9, customer_id = :10 "
                       " WHERE id = :11 ;";
        saveLogToSql(ssql, &updateTask);
    } else {
        qDebug() << "query failed: " << query.lastError().text();
    }

    return isSuccess;
}
// 单独查询任务单表
// 更新 任务单
InsertTask BaseCommonApi::SelectTaskSingle(QString order_id)
{
    InsertTask result;
    QSqlQuery query;
    query.prepare("SELECT * From biz_business_order_info WHERE id=:order_id");
    // 将参数绑定到占位符
    query.bindValue(":order_id", order_id);
    if (query.exec()) {
        while (query.next()) {
            result.id = query.value("id").toString();
            result.customer_name = query.value("customer_name").toString();
            result.require_over_time = query.value("require_over_time").toString();
            result.deliverer = query.value("deliverer").toString();
            result.deliverer_tel = query.value("deliverer_tel").toString();
            result.undertaker_name = query.value("undertaker_name").toString();
            result.undertake_time = query.value("undertake_time").toString();
            bool is_urgent = false;
            if (query.value("is_urgent").toString() == "是") {
                is_urgent = true;
            } else {
                is_urgent = false;
            };
            result.is_urgent = is_urgent;
            result.receive_illustrate = query.value("receive_illustrate").toString();
            result.customer_id = query.value("customer_id").toString();
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    return result;
}
// 删除任务单
bool BaseCommonApi::DeleteTask(QString id)
{
    QSqlQuery query;

    bool allSuccessful = true;
    // 定义一个删除函数，用于复用删除逻辑
    auto deleteRecord = [&](const QString& tableName) {
        query.prepare("DELETE FROM " + tableName + " WHERE order_id = :id");
        query.bindValue(":id", id);
        if (query.exec()) {
            zdDataSync sync;
            QString ssql_result = "DELETE FROM  " + tableName + "WHERE order_id =" + id;
            sync.ssql = ssql_result;
            sync.id = QString::number(BaseCommonApi().generateId());
            sync.sync_type = "0";
            bool isSuccess = BaseCommonApi::InsertzdDataSync(sync);
            if (!isSuccess) {
                qDebug() << "save log fail";
            }
        } else {
            qDebug() << "删除" << tableName
                     << "表中的记录失败: " << query.lastError().text();
            allSuccessful = false;
        }
    };
    deleteRecord("biz_business_order_info");
    deleteRecord("biz_business_order_sample_relation");
    deleteRecord("biz_business_lab_executive_info");
    return allSuccessful;
}
// 删除任务单信息记录表同一任务ID的设备
bool BaseCommonApi::DeleteTaskInfoRecordDevice(QString order_id)
{
    QSqlQuery query;

    // 删除任务单样品表数据
    query.prepare(
        "DELETE FROM biz_business_lab_executive_info WHERE order_id = :id");
    query.bindValue(":id", order_id);

    if (query.exec()) {
        zdDataSync sync;
        QString ssql_result = "DELETE FROM biz_business_lab_executive_info WHERE order_id =" + order_id;
        sync.ssql = ssql_result;
        sync.id = QString::number(BaseCommonApi().generateId());
        sync.sync_type = "0";
        bool isSuccess = BaseCommonApi::InsertzdDataSync(sync);
        if (!isSuccess) {
            qDebug() << "save log fail";
        }
    } else {
        qDebug() << "删除失败 " << query.lastError().text();
        return false;
    }

    return true;
}
// 删除任务详情表同一任务ID的设备
bool BaseCommonApi::DeleteTaskSameDevice(QString order_id)
{
    QSqlQuery query;

    // 删除任务单样品表数据
    query.prepare(
        "DELETE FROM biz_business_order_sample_relation WHERE order_id = :id");
    query.bindValue(":id", order_id);

    if (query.exec()) {
        zdDataSync sync;
        QString ssql_result = "DELETE FROM biz_business_order_sample_relation WHERE order_id =" + order_id;
        sync.ssql = ssql_result;
        sync.id = QString::number(BaseCommonApi().generateId());
        sync.sync_type = "0";
        bool isSuccess = BaseCommonApi::InsertzdDataSync(sync);
        if (!isSuccess) {
            qDebug() << "save log fail";
        }
    } else {
        qDebug() << "删除失败 " << query.lastError().text();
        return false;
    }

    return true;
}
// 删除任务单内设备
bool BaseCommonApi::DeleteTaskDevice(QString id)
{
    QSqlQuery query;

    // 删除任务单样品表数据
    query.prepare(
        "DELETE FROM biz_business_order_sample_relation WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        zdDataSync sync;
        QString ssql_result = "DELETE FROM biz_business_order_sample_relation WHERE id = " + id;
        sync.ssql = ssql_result;
        sync.id = QString::number(BaseCommonApi().generateId());
        sync.sync_type = "0";
        bool isSuccess = BaseCommonApi::InsertzdDataSync(sync);
        if (!isSuccess) {
            qDebug() << "save log fail";
        }
    } else {
        qDebug() << "删除失败 " << query.lastError().text();
        return false;
    }

    return true;
}
// 获取标准设备管理 首页
QList<BizDevice> BaseCommonApi::getBizDeviceManage()
{
    QList<BizDevice> result;
    QSqlQuery query;
    bool isSuccess = query.exec(
        "SELECT bei.id ,bei.equipment_name ,bei.model ,bei.manufacture_no "
        ",bei.manufacturer  FROM biz_equipment_info bei");
    if (isSuccess) {
        while (query.next()) {
            BizDevice device;
            device.id = query.value("id").toString();
            device.equipment_name = query.value("equipment_name").toString();
            device.model = query.value("model").toString();
            device.manufacture_no = query.value("manufacture_no").toString();
            device.manufacturer = query.value("manufacturer").toString();
            result.append(device);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    return result;
}

// 获取标准设备管理-基本信息
QList<CatDeviceInfor> BaseCommonApi::getCatDeviceInfor(QString id)
{
    QList<CatDeviceInfor> result;
    QSqlQuery query;
    query.prepare(
        "SELECT bei.equipment_name ,bei.model ,bei.manufacture_no "
        ",bei.measure_valid_date ,"
        "bei.product_country , bei.manufacturer , bei.produce_date, "
        "bei.purchase_date ,bei.unit_price , bei.usage_status , "
        "bei.quality_status , "
        "bei.dept_name , bei.director_name , bei.supplier_name , bei.location , "
        "bei.type , bei.standard_type , bei.category , bei.level , "
        "bei.measure_cycle , bei.unpack_record_name , bei.remark  FROM "
        "biz_equipment_info bei WHERE bei.id = :id");
    // 将参数绑定到占位符
    query.bindValue(":id", id);
    if (query.exec()) {
        while (query.next()) {
            CatDeviceInfor device;
            device.equipment_name = query.value("equipment_name").toString();
            device.model = query.value("model").toString();
            device.manufacture_no = query.value("manufacture_no").toString();
            device.measure_valid_date = query.value("measure_valid_date").toString();
            device.product_country = query.value("product_country").toString();
            device.manufacturer = query.value("manufacturer").toString();
            device.produce_date = query.value("produce_date").toString();
            device.purchase_date = query.value("purchase_date").toString();
            device.unit_price = query.value("unit_price").toString();
            device.usage_status = query.value("usage_status").toString();
            device.quality_status = query.value("quality_status").toString();
            device.dept_name = query.value("dept_name").toString();
            device.director_name = query.value("director_name").toString();
            device.supplier_name = query.value("supplier_name").toString();
            device.location = query.value("location").toString();
            device.type = query.value("type").toString();
            device.standard_type = query.value("standard_type").toString();
            device.category = query.value("category").toString();
            device.level = query.value("level").toString();
            device.measure_cycle = query.value("measure_cycle").toString();
            device.unpack_record_name = query.value("unpack_record_name").toString();
            device.remark = query.value("remark").toString();

            result.append(device);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    return result;
}

//   //核查直流稳压电源检定装置 待修改  禁用 2024年3月20日13:55:12 查询的不对
// QList<CheckDCPower>BaseCommonApi::getCheckDCPower(QString
// equipment_id,QStringList *headList)
//{

////    equipment_id="1693435753272557570";
//    QList<CheckDCPower> result;
//    QSqlQuery query;
//     query.prepare("SELECT besciddp.id
//     ,besciddp.params,besciddp.unit,resolution,besciddp.urel,besciddp.check_type,datum_point,cycle_number
//     "
//                  " ,datum_point,resistance_value,resistance_value_unit,remark
//                  FROM biz_equipment_standard_check_item_data_dc_power
//                  besciddp WHERE equipment_id = :equipment_id ");
//     // 将参数绑定到占位符  1693435753272557570
//      query.bindValue(":equipment_id", equipment_id);
//      if (query.exec()) {
//             while (query.next()) {
//                 CheckDCPower checkDCPower;
//                 checkDCPower.id = query.value("id").toString();
//                 checkDCPower.params = query.value("params").toString();
//                 checkDCPower.check_point =
//                 query.value("check_point").toString(); checkDCPower.unit =
//                 query.value("unit").toString(); checkDCPower.resolution =
//                 query.value("resolution").toString(); checkDCPower.urel =
//                 query.value("urel").toString(); checkDCPower.check_type =
//                 query.value("check_type").toString();
//                 checkDCPower.datum_point =
//                 query.value("datum_point").toString();
//                 checkDCPower.cycle_number =
//                 query.value("cycle_number").toString();
//                 checkDCPower.resistance_value =
//                 query.value("resistance_value").toString();
//                 checkDCPower.resistance_value_unit =
//                 query.value("resistance_value_unit").toString();
//                 checkDCPower.remark = query.value("remark").toString();

//                 result.append(checkDCPower);
//             }
//         } else {
//             qDebug() << "Query execution failed: " <<
//             query.lastError().text();
//         }

//      *headList
//      <<"序号"<<"核查项目"<<"核查点"<<"单位"<<"分辨力"<<"Urel"<<"核查类型"<<"是否为基准点(1/0)"<<"循环次数"
//               <<"电阻值/频率"<<"电阻值单位/频率单位"<<"核查项备注";

//         return result;
//}
// 根据id获取装置名称  2024年4月23日17:53:40  主系统数据库已变
// QString BaseCommonApi::getApparatus(QString id,QString name)
//{
//    QString result;
//    QSqlQuery query;
//    query.prepare("SELECT besi.standard_name FROM biz_equipment_standard_info
//    besi WHERE besi.id = ( "
//                   "SELECT bei.meter_standard_id FROM biz_equipment_info bei
//                   WHERE bei.id = :id )");
//        // 将参数绑定到占位符
//         query.bindValue(":id", id);
//         if (query.exec()) {
//                while (query.next()) {
//                     result = query.value("standard_name").toString();
//                }
//         }   else {
//             result = "Not Found";
//         }
//         return  result;
//}

//  被检设备管理 首页
QList<TestedDevice> BaseCommonApi::getTestedDevice()
{
    QList<TestedDevice> result;
    QSqlQuery query;
    bool isSuccess = query.exec(
        "SELECT DISTINCT  eei.id ,eei.equipment_name ,eei.model "
        ",eei.help_instruction ,eei.manufacture_no ,eei.manufacturer "
        ",bboi.customer_name ,sys_dept.FULL_NAME ,eei.meter_identify "
        ",eei.check_cycle ,eei.certificate_valid   from eqpt_equipment_info eei "
        " LEFT JOIN sys_dept ON sys_dept.ID = eei.dept_id "
        " LEFT JOIN biz_business_order_info bboi ON bboi.customer_id = "
        "eei.company_id");
    if (isSuccess) {
        while (query.next()) {
            TestedDevice testedDevice;
            testedDevice.id = query.value("id").toString();
            testedDevice.equipment_name = query.value("equipment_name").toString();
            testedDevice.model = query.value("model").toString();
            testedDevice.help_instruction = query.value("help_instruction").toString();
            testedDevice.manufacture_no = query.value("manufacture_no").toString();
            testedDevice.manufacturer = query.value("manufacturer").toString();
            testedDevice.FULL_NAME = query.value("FULL_NAME").toString();
            testedDevice.customer_name = query.value("customer_name").toString();
            testedDevice.meter_identify = query.value("meter_identify").toString();
            testedDevice.check_cycle = query.value("check_cycle").toString();
            testedDevice.certificate_valid = query.value("certificate_valid").toString();

            result.append(testedDevice);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    return result;
}

// 被检设备管理查看设备信息
QList<TestedCatDevice> BaseCommonApi::getTestedCatDevice(QString id)
{
    QList<TestedCatDevice> result;
    QSqlQuery query;
    query.prepare(
        "SELECT eei.equipment_name ,eei.model ,eei.help_instruction "
        ",eei.manufacture_no ,eei.product_country ,eei.manufacturer "
        ",eei.product_date ,eei.purchase_date ,eei.unit_price ,eei.usage_status "
        ",eei.quality_condition ,sys_dept.FULL_NAME ,"
        "eei.category , eei.check_cycle ,eei.level , eei.remark  from "
        "eqpt_equipment_info eei LEFT JOIN sys_dept ON sys_dept.ID = eei.dept_id "
        "WHERE eei.id = :id ");
    // 将参数绑定到占位符
    query.bindValue(":id", id);
    if (query.exec()) {
        while (query.next()) {
            TestedCatDevice testedCatDevice;
            testedCatDevice.equipment_name = query.value("equipment_name").toString();
            testedCatDevice.model = query.value("model").toString();
            testedCatDevice.help_instruction = query.value("help_instruction").toString();
            testedCatDevice.manufacture_no = query.value("manufacture_no").toString();
            testedCatDevice.product_country = query.value("product_country").toString();
            testedCatDevice.manufacturer = query.value("manufacturer").toString();
            testedCatDevice.product_date = query.value("product_date").toString();
            testedCatDevice.purchase_date = query.value("purchase_date").toString();
            testedCatDevice.unit_price = query.value("unit_price").toString();
            testedCatDevice.usage_status = query.value("usage_status").toString();
            testedCatDevice.quality_condition = query.value("quality_condition").toString();
            testedCatDevice.FULL_NAME = query.value("FULL_NAME").toString();
            testedCatDevice.category = query.value("category").toString();
            testedCatDevice.check_cycle = query.value("check_cycle").toString();
            testedCatDevice.level = query.value("level").toString();
            testedCatDevice.remark = query.value("remark").toString();

            result.append(testedCatDevice);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    return result;
}
////核查 判断哪个装置表
// JudgeCalibrationTable BaseCommonApi::getJudgeVerifyTable(QString deviceId)
//{
//     JudgeCalibrationTable result;
//     QSqlQuery query;
//     query.prepare("SELECT besi.standard_name,bescic.belong_standard_equipment
//     FROM biz_equipment_standard_info besi "
//                  "  JOIN biz_equipment_standard_check_item_category bescic ON
//                  bescic.belong_standard_equipment_name = besi.standard_name "
//                   "  WHERE besi.id = ( SELECT bei.meter_standard_id FROM
//                   biz_equipment_info bei WHERE bei.id = :deviceId )");
//     // 将参数绑定到占位符  '1692796512381222914'
//      query.bindValue(":deviceId", deviceId);
//      if (query.exec()) {
//          while (query.next()) {
//              JudgeCalibrationTable calibration;
//              calibration.belong_standard_equipment =
//              query.value("belong_standard_equipment").toString();
//    //          QString equipmentValue =
//    query.value("belong_standard_equipment").toString();

//             result = calibration;
//         }
//     } else {
//         qDebug() << "Query execution failed: " << query.lastError().text();
//     }

//     return result;
//}

// 判断 检定 哪个装置表 传参 设备id
QList<JudgeCalibrationTable> BaseCommonApi::getJudgeCalibrationTable(
    QString id)
{
    //    id="1720260188257480706";
    QList<JudgeCalibrationTable> result;
    QSqlQuery query;
    query.prepare(
        "SELECT id,belong_standard_equipment  FROM "
        "biz_business_device_measure_item_info bbdmii WHERE "
        "bbdmii.device_model_id =(SELECT eei.model_id  from eqpt_equipment_info "
        "eei WHERE eei.id =  :id )");
    // query.prepare("SELECT id,belong_standard_equipment  FROM
    // biz_business_device_measure_item_info bbdmii "
    //               "WHERE bbdmii.device_model_id = 1714161758107967489");
    // 将参数绑定到占位符
    query.bindValue(":id", id);
    if (query.exec()) {
        while (query.next()) {
            JudgeCalibrationTable calibration;
            calibration.id = query.value("id").toString();
            calibration.belong_standard_equipment = query.value("belong_standard_equipment").toString();
            //          QString equipmentValue =
            //          query.value("belong_standard_equipment").toString();

            result.append(calibration);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    return result;
}

// 标准装置字典类型
/**
 * 多功能校准源标准装置 = 1
 * 直流稳压电源检定装置 = 2
 * 多功能电气安全校准器标准装置 = 3
 * 0.02级活塞式压力计标准装置 = 4
 * 二等铂电阻温度计标准装置 = 5
 * 安全阀校验装置 = 6
 * 精密露点仪标准装置 = 7
 * E2等级砝码标准装置 = 8
 * 频谱分析仪标准装置 = 9
 * 信号发生器标准装置 = 10
 * 微波衰减标准装置 = 11
 * 频率短期稳定度标准装置 =12
 * 示波器标准装置 = 13
 * 铯原子频率标准装置 =14
 * 小功率标准装置 = 15
 * 低频信号发生器标准装置 = 16
 * S参数标准装置 = 17
 * 失真度标准装置 = 18
 */

// 根据被检设备ID获取当前设备的 检定项目及检定点
QMap<QString, QList<QStringList>> BaseCommonApi::Search_CalibrationTableClass(
    QString DeiverId, SequentialMap<QString, QStringList>* HeadList)
{
    QMap<QString, QList<QStringList>> DataList;
    DataList.clear();
    QList<JudgeCalibrationTable> CliTable = BaseCommonApi::getJudgeCalibrationTable(DeiverId);
    if (CliTable.count() == 0)
        return DataList;
    QList<DeviceMeasureEquipmentCatory> deviceCatory = BaseCommonApi::getDeviceMeasureEquipmentCatory(DeiverId);
    QString deviceCatoryName = deviceCatory[0].item_category_name;
    switch (CliTable[0].belong_standard_equipment.toInt()) {
    case 1:
        // 多功能校准源标准装置
        DataList = BaseCommonApi::getVerMultiCalClass(deviceCatoryName,
            CliTable[0].id, HeadList);
        break;

    case 2:
        // 直流稳压电源检定装置
        DataList = BaseCommonApi::getDeviceMeasureItemDataDcPower(
            deviceCatoryName, CliTable[0].id, HeadList);
        break;
    case 4:
        // 0.02级活塞式压力计标准装置
        DataList = BaseCommonApi::getDeviceMeasureCalibratorPistonGauge(
            deviceCatoryName, CliTable[0].id, HeadList);
        break;
    case 5:
        // 二等铂电阻温度计标准装置
        DataList = BaseCommonApi::getDeviceMeasureItemDataResistanceThermometer(
            deviceCatoryName, CliTable[0].id, HeadList);
        break;
    case 9:
        // 频谱分析仪装置
        DataList = BaseCommonApi::getDeviceMeasureItemDataSpectrumAnalyzer(
            deviceCatoryName, CliTable[0].id, HeadList);
        break;
    case 10:
        // 信号装置
        DataList = BaseCommonApi::getDeviceMeasureItemDataSignalGenerator(
            deviceCatoryName, CliTable[0].id, HeadList);
        break;
    case 13:
        // 示波器标准装置
        DataList = BaseCommonApi::getDeviceOscilloscopeCalibration(
            deviceCatoryName, CliTable[0].id, HeadList);
        break;
    // 微波衰减标准装置
    case 11:
        DataList = BaseCommonApi::getDeviceMicrowaveAttenuation(
            deviceCatoryName, CliTable[0].id, HeadList);
        break;
    // 低频信号发生器标准装置
    case 16:
        DataList = BaseCommonApi::getDeviceMeasureItemDataLFSignalGenerator(
            deviceCatoryName, CliTable[0].id, HeadList);
        break;
    // 小功率标准装置
    case 15:
        DataList = BaseCommonApi::getDeviceSmallPower(deviceCatoryName,
            CliTable[0].id, HeadList);
        break;
    case 17:
        DataList = BaseCommonApi::getDeviceMeasureItemDataSParam(deviceCatoryName,
            CliTable[0].id, HeadList);
        break;
    // 铯原子频率标准装置
    case 14:
        DataList = BaseCommonApi::getDeviceCesiumAtomicFrequency(deviceCatoryName,
            CliTable[0].id, HeadList);
        break;
    }

    return DataList;
};
// 查询测试页面中 生成结果处理的表头及数据
QMap<QString, QList<QStringList>> BaseCommonApi::Search_GenerateTableClass(
    QString DeiverId, SequentialMap<QString, QStringList>* HeadList)
{
    QMap<QString, QList<QStringList>> DataList;
    DataList.clear();
    QList<JudgeCalibrationTable> CliTable = BaseCommonApi::getJudgeCalibrationTable(DeiverId);
    if (CliTable.count() == 0)
        return DataList;
    QList<DeviceMeasureEquipmentCatory> deviceCatory = BaseCommonApi::getDeviceMeasureEquipmentCatory(DeiverId);
    QString deviceCatoryName = deviceCatory[0].item_category_name;
    switch (CliTable[0].belong_standard_equipment.toInt()) {
    case 4:
        DataList = BaseCommonApi::getDeviceMeasureCalibratorPistonGaugeGenerate(
            deviceCatoryName, CliTable[0].id, HeadList);
        break;
    case 13:
        // 示波器标准装置 = 13
        // DataList=BaseCommonApi::getDeviceOscilloscopeCalibration(deviceCatoryName,CliTable[0].id,HeadList);
        break;
    }

    return DataList;
}

// 根据被检设备ID获取当前设备的 检定项目及检定点  测试函数
// QList<QStringList> BaseCommonApi::Search_CalibrationTable(QString
// DeiverId,QStringList *HeadList)
//{

//     QList<QStringList> DataList;
//     DataList.clear();
//     QList<JudgeCalibrationTable>
//     CliTable=BaseCommonApi::getJudgeCalibrationTable(DeiverId);
//     if(CliTable.count()==0)return DataList;
//     switch (CliTable[0].belong_standard_equipment.toInt())
//     {
//        case 1:
//       //CliTable[0].id
//         //1717086652508487681
//         DataList=BaseCommonApi::getVerMultiCal(CliTable[0].id,HeadList);
////     case 4:
////
/// DataList=BaseCommonApi::getDeviceMeasureCalibratorPistonGauge(CliTable[0].id,HeadList);
//     }

//     return  DataList;
//};
// 核查设备对应的装置表  核查项及核查点   传参设备id
// QList<QStringList> BaseCommonApi::Search_VerifyTable(QString
// DeiverId,QStringList *HeadList )
//{
//         QList<QStringList> DataList;
//         JudgeCalibrationTable
//         CliTable=BaseCommonApi::getJudgeVerifyTable(DeiverId);
//         if(CliTable.belong_standard_equipment.isEmpty() ||
//         CliTable.belong_standard_equipment.isNull()) return DataList;
//         switch(CliTable.belong_standard_equipment.toInt())
//         {
//            case 1: //多功能校准源
//               DataList =
//               BaseCommonApi::getVerificationItemsMultiList(DeiverId,HeadList);
//             break;
//         }
//              return  DataList;
//};
// 判断检定数据管理 结果信息表
QList<QStringList> BaseCommonApi::Search_CalibrationDataTable(
    QString DeiverId, QString dataId, QStringList* HeadList)
{
    QList<QStringList> DataList;
    QList<JudgeCalibrationTable> CliTable = BaseCommonApi::getJudgeCalibrationTable(DeiverId);
    if (CliTable.count() == 0)
        return DataList;
    switch (CliTable[0].belong_standard_equipment.toInt()) {
    case 1: // 多功能校准源
        DataList = BaseCommonApi::getVerificationMultiCalResultData(dataId, HeadList);
        break;
    }
    return DataList;
};
////判断核查数据管理 结果信息表
// QList<QStringList> BaseCommonApi::Search_VerifyDataTable(QString
// DeiverId,QString dataId,QStringList *HeadList)
//{
//     QList<QStringList> DataList;
////    JudgeCalibrationTable
/// CliTable=BaseCommonApi::getJudgeVerifyTable(DeiverId);
//    if(CliTable.belong_standard_equipment.isEmpty() ||
//    CliTable.belong_standard_equipment.isNull()) return DataList;
//    switch(CliTable.belong_standard_equipment.toInt())
//    {
//       case 1: //多功能校准源
//        DataList =
//        BaseCommonApi::getCheckDataManageResultManageMult(dataId,HeadList);
//        break;
//    }
//         return  DataList;
//};
// 被检设备管理-检定项目及检定点-多功能校准源
QList<QStringList> BaseCommonApi::getVerMultiCal(QString itemId,
    QStringList* headList)
{
    QMap<QString, QStringList> dailyTaskCounts;
    QMap<QString, QList<QStringList>> dailyTaskCounts1;

    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();

    QSqlQuery query;
    query.prepare(
        "SELECT bbdmidc.*,bbdmic.category_name FROM "
        "biz_business_device_measure_item_data_calibrator bbdmidc "
        " JOIN biz_business_device_measure_item_info bbdmii ON bbdmii.id = "
        "bbdmidc.item_id "
        " JOIN biz_business_device_measure_item_category bbdmic ON bbdmic.id = "
        "bbdmii.item_category_id "
        " WHERE item_id = :itemId");
    // 将参数绑定到占位符
    query.bindValue(":itemId", itemId);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_id").toString()); // 检定项目ID
            result.append(query.value("params").toString()); // 检查项目
            result.append(query.value("capacity").toString());
            result.append(query.value("unit").toString());
            result.append(query.value("frequency").toString());
            result.append(query.value("frequency_unit").toString());
            result.append(query.value("range_range").toString());
            result.append(query.value("standard_value").toString());
            result.append(query.value("resolution").toString());
            result.append(query.value("maximum_error").toString());
            result.append(query.value("error_param_a").toString());
            result.append(query.value("error_param_b").toString());
            result.append(query.value("line_mode").toString());
            result.append(query.value("coil_enlarge").toString());
            result.append(query.value("indicating_value").toString());
            result.append(query.value("accuracy_a").toString());
            result.append(query.value("full_scale_value").toString());
            result.append(query.value("full_scale_value_unit").toString());
            result.append(query.value("measure_indication_value").toString());
            result.append(query.value("accuracy_level").toString());
            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());
            result.append(query.value("category_name").toString());
            resultData.append(result);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    *headList << "序号"
              << "检定项id"
              << "参数"
              << "量"
              << "单位"
              << "频率"
              << "频率单位"
              << "量程"
              << "标准值"
              << "分辨力"
              << "最大允许误差(公式)"
              << "误差参数a"
              << "误差参数b"
              << "两线/四线模式"
              << "线圈放大"
              << "示值"
              << "准确度a"
              << "指针式仪表满量程值"
              << "指针式仪表满量程值单位"
              << "被检表示值"
              << "准确度等级"
              << "创建时间"
              << "更新时间"
              << "检定项分类";
    return resultData;
}
// 被检设备管理-检定项目及检定点-多功能校准源
QMap<QString, QList<QStringList>> BaseCommonApi::getVerMultiCalClass(
    QString itemCatoryName, QString itemId,
    SequentialMap<QString, QStringList>* headList)
{
    QMap<QString, QList<QStringList>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();

    QSqlQuery query;
    query.prepare(
        "SELECT * FROM biz_business_device_measure_item_data_calibrator bbdmidc "
        "WHERE item_id = :itemId order by bbdmidc.create_time ASC");
    // 将参数绑定到占位符
    query.bindValue(":itemId", itemId);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_id").toString()); // 检定项目ID
            result.append(query.value("params").toString()); // 检查项目
            result.append(query.value("capacity").toString()); // 量
            result.append(query.value("unit").toString()); // 单位
            result.append(query.value("frequency").toString()); // 频率
            result.append(query.value("frequency_unit").toString()); // 频率单位
            result.append(query.value("range_range").toString()); // 量程
            result.append(query.value("standard_value").toString()); // 标准值
            result.append(query.value("resolution").toString()); // 分辨力
            result.append(
                query.value("maximum_error").toString()); // 最大允许误差(公式)
            result.append(query.value("error_param_a").toString()); // 误差参数a
            result.append(query.value("error_param_b").toString()); // 误差参数b
            result.append(query.value("line_mode").toString()); // 两线/四线模式
            result.append(query.value("coil_enlarge").toString()); // 线圈放大
            result.append(query.value("indicating_value").toString()); // 示值
            result.append(query.value("accuracy_a").toString()); // 准确度a
            result.append(
                query.value("full_scale_value").toString()); // 指针式仪表满量程值
            result.append(query.value("full_scale_value_unit")
                              .toString()); // 指针式仪表满量程值单位
            result.append(
                query.value("measure_indication_value").toString()); // 被检表示值
            result.append(query.value("accuracy_level").toString()); // 准确度等级
            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());
            resultData.append(result);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    // 进行检定项目列头转换
    DataMapList = GetDataReturnItemList(itemCatoryName, resultData, DataMapList, headList);
    return DataMapList;
}

QMap<QString, QList<QStringList>> BaseCommonApi::GetDataReturnItemList(
    QString itemCatoryName, QList<QStringList> ResultData,
    QMap<QString, QList<QStringList>> DataMapList,
    SequentialMap<QString, QStringList>* HeadMapList)
{
    QList<QStringList> DCVList;
    QList<QStringList> ACVList;
    QList<QStringList> DCIList;
    QList<QStringList> ACIList;
    QList<QStringList> RList;
    QStringList DCVhead;
    QStringList ACVhead;
    QStringList DCIhead;
    QStringList ACIhead;
    QStringList Rhead;
    if (itemCatoryName.contains("数字多用表")) {
        for (const QStringList& row : ResultData) {
            QStringList head;
            QStringList Data;
            QString VerificationItem = NULL;
            if (!row.isEmpty()) { // 确保行不为空
                if (row[2] == "DCV") {
                    VerificationItem = "DCV";
                    // 添加列头
                    head << "序号"
                         << "检定项目"
                         << "量"
                         << "单位"
                         << "量程"
                         << "标准值"
                         << "示值"
                         << "绝对误差"
                         << "最大允许误差"
                         << "最大允许误差公式"
                         << "分辨力"
                         << "a"
                         << "b";
                    DCVhead = head;
                    // HeadMapList->insert(VerificationItem, head);
                    //  添加数据
                    Data.append(row[0]); // ID
                    Data.append(row[2]); // 检定项目
                    Data.append(row[3]); // 量
                    Data.append(row[4]); // 单位
                    Data.append(row[7]); // 量程
                    Data.append(row[8]); // 标准值
                    Data.append(row[15]); // 示值
                    Data.append(""); // 绝对误差
                    Data.append(row[10]); // 最大允许误差 软件界面计算
                    Data.append(row[10]); // 最大允许误差公式 软件界面计算
                    // 最大允许误差 根据公式计算 目前 先都按 a*标准值 + b*量程
                    /*try {
                        //保证数据库内数据没问题
                        Data.append(QString::number((row[11].toDouble()*row[8].toDouble())
                                                 +
                    (row[12].toDouble()*row[7].toDouble()))); } catch(const std::exception
                    &e) { Data.append("0");
                    }*/
                    Data.append(row[9]); // 分辨力
                    Data.append(row[11]); // a
                    Data.append(row[12]); // b
                    DCVList.append(Data);
                } else if (row[2] == "ACV") {
                    VerificationItem = "ACV";
                    // 添加列头

                    head << "序号"
                         << "检定项目"
                         << "量"
                         << "单位"
                         << "频率"
                         << "频率单位"
                         << "量程"
                         << "标准值"
                         << "示值"
                         << "绝对误差"
                         << "最大允许误差"
                         << "最大允许误差公式"
                         << "分辨力"
                         << "a"
                         << "b";
                    ACVhead = head;
                    // HeadMapList->insert(VerificationItem, head);
                    //  添加数据
                    Data.append(row[0]); // ID
                    Data.append(row[2]); // 检定项目
                    Data.append(row[3]); // 量
                    Data.append(row[4]); // 单位
                    Data.append(row[5]); // 频率
                    Data.append(row[6]); // 频率单位
                    Data.append(row[7]); // 量程
                    Data.append(row[8]); // 标准值
                    Data.append(row[15]); // 示值
                    Data.append(""); // 绝对误差 软件界面计算
                    Data.append(row[10]); // 最大允许误差 软件界面计算
                    Data.append(row[10]); // 最大允许误差公式 软件界面计算
                    // 最大允许误差 根据公式计算 目前 先都按 a*标准值 + b*量程
                    /*try {
                        //保证数据库内数据没问题
                        Data.append(QString::number((row[11].toDouble()*row[8].toDouble())
                                                 +
                    (row[12].toDouble()*row[7].toDouble()))); } catch(const std::exception
                    &e) { Data.append("0");
                    }*/
                    Data.append(row[9]); // 分辨力
                    Data.append(row[11]); // a
                    Data.append(row[12]); // b
                    ACVList.append(Data);
                } else if (row[2] == "DCI") {
                    VerificationItem = "DCI";
                    // 添加列头

                    head << "序号"
                         << "检定项目"
                         << "量"
                         << "单位"
                         << "量程"
                         << "标准值"
                         << "示值"
                         << "绝对误差"
                         << "最大允许误差"
                         << "最大允许误差公式"
                         << "分辨力"
                         << "a"
                         << "b";
                    DCIhead = head;
                    // HeadMapList->insert(VerificationItem, head);

                    // 添加数据
                    Data.append(row[0]); // ID
                    Data.append(row[2]); // 检定项目
                    Data.append(row[3]); // 量
                    Data.append(row[4]); // 单位
                    // Data.append(row[5]);  //频率
                    Data.append(row[7]); // 量程
                    Data.append(row[8]); // 标准值
                    Data.append(row[15]); // 示值
                    Data.append(""); // 绝对误差 软件界面计算
                    Data.append(row[10]); // 最大允许误差 软件界面计算
                    Data.append(row[10]); // 最大允许误差公式 软件界面计算
                    // 最大允许误差 根据公式计算 目前 先都按 a*标准值 + b*量程
                    /*try {
                        //保证数据库内数据没问题
                        Data.append(QString::number((row[11].toDouble()*row[8].toDouble())
                                                 +
                    (row[12].toDouble()*row[7].toDouble()))); } catch(const std::exception
                    &e) { Data.append("0");
                    }*/
                    Data.append(row[9]); // 分辨力
                    Data.append(row[11]); // a
                    Data.append(row[12]); // b
                    DCIList.append(Data);

                } else if (row[2] == "ACI") {
                    VerificationItem = "ACI";
                    // 添加列头

                    head << "序号"
                         << "检定项目"
                         << "量"
                         << "单位"
                         << "频率"
                         << "频率单位"
                         << "量程"
                         << "标准值"
                         << "示值"
                         << "绝对误差"
                         << "最大允许误差"
                         << "最大允许误差公式"
                         << "分辨力"
                         << "a"
                         << "b";

                    ACIhead = head;
                    // HeadMapList->insert(VerificationItem, head);

                    // 添加数据

                    Data.append(row[0]); // ID
                    Data.append(row[2]); // 检定项目
                    Data.append(row[3]); // 量
                    Data.append(row[4]); // 单位
                    Data.append(row[5]); // 频率
                    Data.append(row[6]); // 频率单位
                    Data.append(row[7]); // 量程
                    Data.append(row[8]); // 标准值
                    Data.append(row[15]); // 示值
                    Data.append(""); // 绝对误差 软件界面计算
                    Data.append(row[10]); // 最大允许误差 软件界面计算
                    Data.append(row[10]); // 最大允许误差公式 软件界面计算
                    // 最大允许误差 根据公式计算 目前 先都按 a*标准值 + b*量程
                    /*try {
                        //保证数据库内数据没问题
                        Data.append(QString::number((row[11].toDouble()*row[8].toDouble())
                                                 +
                    (row[12].toDouble()*row[7].toDouble()))); } catch(const std::exception
                    &e) { Data.append("0");
                    }*/
                    Data.append(row[9]); // 分辨力
                    Data.append(row[11]); // a
                    Data.append(row[12]); // b
                    ACIList.append(Data);
                } else if (row[2] == "R") {
                    VerificationItem = "R";
                    // 添加列头

                    head << "序号"
                         << "检定项目"
                         << "量"
                         << "单位"
                         << "两线/四线"
                         << "量程"
                         << "标准值"
                         << "示值"
                         << "绝对误差"
                         << "最大允许误差"
                         << "最大允许误差公式"
                         << "分辨力"
                         << "a"
                         << "b";
                    Rhead = head;
                    // HeadMapList->insert(VerificationItem, head);
                    //  添加数据

                    Data.append(row[0]); // ID
                    Data.append(row[2]); // 检定项目
                    Data.append(row[3]); // 量
                    Data.append(row[4]); // 单位
                    Data.append(row[13]); // 两线/四线
                    Data.append(row[7]); // 量程
                    Data.append(row[8]); // 标准值
                    Data.append(row[15]); // 示值
                    Data.append(""); // 绝对误差 软件界面计算
                    Data.append(row[10]); // 最大允许误差 软件界面计算
                    Data.append(row[10]); // 最大允许误差公式 软件界面计算
                    // 最大允许误差 根据公式计算 目前 先都按 a*标准值 + b*量程
                    /* try {
                         //保证数据库内数据没问题
                         Data.append(QString::number((row[11].toDouble()*row[8].toDouble())
                                                  +
                     (row[12].toDouble()*row[7].toDouble()))); } catch(const
                     std::exception &e) { Data.append("0");
                     }*/
                    Data.append(row[9]); // 分辨力
                    Data.append(row[11]); // a
                    Data.append(row[12]); // b
                    RList.append(Data);
                }
            }
        }

        if (DCVList.count() != 0) {
            DataMapList.insert("DCV", DCVList);
            HeadMapList->insert("DCV", DCVhead);
        };
        if (ACVList.count() != 0) {
            DataMapList.insert("ACV", ACVList);
            HeadMapList->insert("ACV", ACVhead);
        };
        if (DCIList.count() != 0) {
            DataMapList.insert("DCI", DCIList);
            HeadMapList->insert("DCI", DCIhead);
        };
        if (ACIList.count() != 0) {
            DataMapList.insert("ACI", ACIList);
            HeadMapList->insert("ACI", ACIhead);
        };
        if (RList.count() != 0) {
            DataMapList.insert("R", RList);
            HeadMapList->insert("R", Rhead);
        };
    }
    return DataMapList;
}
// 检定数据管理 首页
QList<VerificationData> BaseCommonApi::getVerificationData()
{
    QList<VerificationData> result;
    QSqlQuery query;
    /*bool isSuccess = query.exec("SELECT bbdmi.*, eei.id,bboi.customer_name,
       eemi.help_instruction, bbdmii.item_category_id, bbdmii.item_category_name,
       " " bbdmii.belong_standard_equipment,
       bboi.customer_address,_user.NAME,eei.meter_identify,dept.FULL_NAME" " FROM
       biz_business_device_measure_info bbdmi " " JOIN
       biz_business_device_measure_item_info bbdmii ON bbdmii.id = bbdmi.item_id "
                                " LEFT JOIN sys_user _user ON bbdmi.create_user_id
       = _user.id " " LEFT JOIN eqpt_equipment_info eei ON bbdmi.sample_id =
       eei.id " " LEFT JOIN sys_dept dept ON eei.dept_id = dept.ID " " JOIN
       eqpt_equipment_model_info eemi ON eemi.id = bbdmii.device_model_id " " JOIN
       biz_business_order_sample_relation bbosr ON (bbosr.order_id =
       bbdmi.order_id AND bbosr.sample_id = " " bbdmi.sample_id) JOIN
       biz_business_order_info bboi ON bbosr.order_id = bboi.id " " ORDER BY
       bbdmi.create_time DESC");*/
    bool isSuccess = query.exec(
        "SELECT bbdmi.*, eei.id,bboi.customer_name, eemi.help_instruction, "
        "bbdmii.item_category_id, bbdmii.item_category_name, "
        " bbdmii.belong_standard_equipment, "
        "bboi.customer_address,_user.NAME,eei.meter_identify,dept.FULL_NAME "
        " FROM biz_business_device_measure_info bbdmi "
        "  LEFT JOIN biz_business_device_measure_item_info bbdmii ON bbdmii.id = "
        "bbdmi.item_id "
        "  LEFT JOIN sys_user _user ON bbdmi.create_user_id = _user.id "
        " LEFT JOIN eqpt_equipment_info eei ON bbdmi.sample_id = eei.id "
        " LEFT JOIN sys_dept dept ON eei.dept_id = dept.ID "
        " LEFT JOIN eqpt_equipment_model_info eemi ON eemi.id = "
        "bbdmii.device_model_id "
        " LEFT JOIN biz_business_order_sample_relation bbosr ON (bbosr.order_id "
        "= bbdmi.order_id AND bbosr.sample_id = "
        " bbdmi.sample_id) "
        " LEFT JOIN biz_business_order_info bboi ON bbosr.order_id = bboi.id "
        " ORDER BY bbdmi.create_time DESC");
    if (isSuccess) {
        while (query.next()) {
            VerificationData verificationData;
            verificationData.id = query.value("id").toString();
            verificationData.sample_id = query.value("sample_id").toString();
            verificationData.item_id = query.value("item_id").toString();
            verificationData.sample_name = query.value("sample_name").toString();
            verificationData.model = query.value("model").toString();
            verificationData.help_instruction = query.value("help_instruction").toString();
            verificationData.manufacture_no = query.value("manufacture_no").toString();
            verificationData.manufacturer = query.value("manufacturer").toString();
            verificationData.customer_name = query.value("customer_name").toString();
            verificationData.FULL_NAME = query.value("FULL_NAME").toString();
            verificationData.NAME = query.value("NAME").toString();
            verificationData.trace_date = query.value("trace_date").toDate();
            verificationData.measure_valid_date = query.value("measure_valid_date").toDateTime();
            verificationData.meter_identify = query.value("meter_identify").toString();
            verificationData.restriction_instruction = query.value("restriction_instruction").toString();
            result.append(verificationData);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
}
// 检定数据管理-检定基本信息-标准设备 参数：首页数据的id
QList<VerificationBasicDataDevice>
BaseCommonApi::getVerificationBasicDataDevice(QString data_id)
{
    QList<VerificationBasicDataDevice> result;
    QSqlQuery query;
    query.prepare(
        "SELECT bei.id AS equipmentId, bei.equipment_no, bei.equipment_name, "
        "bei.manufacture_no, bei.model, bei.measure_valid_date, "
        "bett.uncertainty "
        " FROM biz_business_device_measure_equipment bbdme "
        " JOIN biz_equipment_info bei ON bei.id = bbdme.equipment_id "
        " Left JOIN biz_equipment_technical_target bett ON bei.equipment_no = "
        "bett.equipment_no "
        " WHERE bbdme.data_id = :data_id");
    // 将参数绑定到占位符
    query.bindValue(":data_id", data_id);
    if (query.exec()) {
        while (query.next()) {
            VerificationBasicDataDevice dataDetails;
            dataDetails.equipmentId = query.value("equipmentId").toString();
            dataDetails.equipment_no = query.value("equipment_no").toString();
            dataDetails.equipment_name = query.value("equipment_name").toString();
            dataDetails.model = query.value("model").toString();
            dataDetails.measure_valid_date = query.value("measure_valid_date").toDateTime();
            dataDetails.uncertainty = query.value("uncertainty").toString();
            result.append(dataDetails);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    return result;
}
// 检定数据管理-检定基本信息  参数item_id 首页单条数据中包含
VerificationBasicDataDetails BaseCommonApi::getVerificationBasicDataDetails(
    QString item_id)
{
    VerificationBasicDataDetails result;
    QSqlQuery query;
    query.prepare(
        "SELECT bbdmi.*, bboi.customer_name, eemi.help_instruction, "
        "bbdmii.item_category_name, "
        "bboi.order_no,eemtte.based_documents,_user.NAME"
        " FROM biz_business_device_measure_info bbdmi "
        " JOIN biz_business_device_measure_item_info bbdmii ON bbdmii.id = "
        "bbdmi.item_id "
        " JOIN eqpt_equipment_model_info eemi ON eemi.id = "
        "bbdmii.device_model_id "
        " JOIN biz_business_order_sample_relation bbosr ON (bbosr.order_id = "
        "bbdmi.order_id AND bbosr.sample_id = bbdmi.sample_id) "
        " JOIN biz_business_order_info bboi ON bbosr.order_id = bboi.id "
        " LEFT JOIN eqpt_equipment_model_technical_target eemtte ON "
        "bbdmii.device_model_id = eemtte.model_id "
        " LEFT JOIN sys_user _user ON bbdmi.create_user_id = _user.id "
        " WHERE bbdmi.item_id = :item_id "
        " ORDER BY bbdmi.update_time DESC"
        " LIMIT 1");
    // 将参数绑定到占位符
    query.bindValue(":item_id", item_id);
    if (query.exec()) {
        while (query.next()) {
            VerificationBasicDataDetails dataDetails;
            dataDetails.customer_name = query.value("customer_name").toString();
            dataDetails.humidity = query.value("humidity").toString();
            dataDetails.temperature = query.value("temperature").toString();
            //                 dataDetails.model = query.value("model").toString();
            dataDetails.measure_address = query.value("measure_address").toString();
            dataDetails.measure_valid_date = query.value("measure_valid_date").toDateTime();
            dataDetails.conclusion = query.value("conclusion").toString();
            dataDetails.restriction_instruction = query.value("restriction_instruction").toString();
            dataDetails.based_documents = query.value("based_documents").toString();
            dataDetails.trace_date = query.value("trace_date").toDate();
            dataDetails.NAME = query.value("NAME").toString();
            dataDetails.appearance_function_check = query.value("appearance_function_check").toString();
            dataDetails.appearance_remark = query.value("appearance_remark").toString();
            result = dataDetails;
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    return result;
}
// 检定数据管理-检定结果信息 SaveDeviceMeasureCalibrator 多功能校准源结果数据表
QList<QStringList> BaseCommonApi::getVerificationMultiCalResultData(
    QString dataId, QStringList* HeadList)
{
    QList<QStringList> resultData;
    QStringList result;
    QSqlQuery query;
    query.prepare(
        "SELECT * FROM biz_business_device_measure_data_calibrator WHERE data_id "
        "= :dataId ");
    // 将参数绑定到占位符
    query.bindValue(":dataId", dataId);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("params").toString());
            result.append(query.value("capacity").toString());
            result.append(query.value("unit").toString());
            result.append(query.value("frequency").toString());
            result.append(query.value("range_range").toString());
            result.append(query.value("standard_value").toString());
            result.append(query.value("indicating_value").toString());
            result.append(query.value("resolution").toString());
            result.append(query.value("maximum_error").toString());
            result.append(query.value("error_param_a").toString());
            result.append(query.value("error_param_b").toString());
            result.append(query.value("other_outcome").toString());
            resultData.append(result);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    *HeadList << "id"
              << "参数"
              << "量"
              << "单位"
              << "频率"
              << "量程"
              << "标准值"
              << "示值"
              << "分辨力"
              << "最大允许误差"
              << "误差参数a"
              << "误差参数b"
              << "合格率";

    return resultData;
}

// 核查数据管理 首页
QList<CheckData> BaseCommonApi::getCheckData()
{
    QList<CheckData> result;
    QSqlQuery query;
    bool isSuccess = query.exec(
        "SELECT besci.*, besi.standard_name AS standardEquipmentName, "
        "beit.equipment_name AS equipmentName, beit.model AS equipmentModel, "
        " bescic.category_name AS itemCategoryName, "
        "bescic.belong_standard_equipment AS "
        "belongStandardEquipment,beit.manufacture_no,"
        " beit.manufacturer,besci.create_user_name "
        " FROM biz_equipment_standard_check_info besci "
        " JOIN biz_equipment_standard_info besi ON besci.standard_equipment_id = "
        "besi.id "
        " JOIN biz_equipment_info beit ON besci.equipment_id = beit.id  "
        " LEFT JOIN biz_equipment_standard_check_item_category bescic ON "
        "bescic.id = besci.item_category_id ORDER BY besci.create_time DESC ");
    if (isSuccess) {
        while (query.next()) {
            CheckData dataManage;
            dataManage.id = query.value("id").toString();
            dataManage.equipmentName = query.value("equipmentName").toString();
            dataManage.equipmentModel = query.value("equipmentModel").toString();
            dataManage.manufacture_no = query.value("manufacture_no").toString();
            //             dataManage.meter_standard_name =
            //             query.value("meter_standard_name").toString();
            dataManage.equipment_id = query.value("equipment_id").toString();
            dataManage.create_user_name = query.value("create_user_name").toString();
            dataManage.check_date = query.value("check_date").toDate();
            dataManage.dept_name = query.value("dept_name").toString();
            dataManage.manufacturer = query.value("manufacturer").toString();
            result.append(dataManage);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    return result;
}

// 核查数据管理-核查基本信息-核查件  参数为 首页id
QList<CheckDataManageBasicInfo>
BaseCommonApi::getCheckDataManageBasicInfoDevice(QString data_id)
{
    QList<CheckDataManageBasicInfo> result;
    QSqlQuery query;
    query.prepare(
        "SELECT bei.* FROM biz_equipment_standard_check_info_equipment bescie "
        " JOIN biz_equipment_info bei ON bei.id = bescie.check_equipment_id "
        " WHERE bescie.data_id = :data_id ");

    // 将参数绑定到占位符
    query.bindValue(":data_id", data_id);
    if (query.exec()) {
        while (query.next()) {
            CheckDataManageBasicInfo basicInfo;
            basicInfo.equipment_name = query.value("equipment_name").toString();
            basicInfo.model = query.value("model").toString();
            basicInfo.manufacture_no = query.value("manufacture_no").toString();
            basicInfo.manufacturer = query.value("manufacturer").toString();
            result.append(basicInfo);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    return result;
}

// 核查数据管理-核查基本信息-基本信息
CheckDataManageBasicInfo BaseCommonApi::getCheckDataManageBasicInfo(
    QString equipment_id)
{
    CheckDataManageBasicInfo result;
    QSqlQuery query;
    query.prepare(
        "SELECT * FROM biz_equipment_standard_check_info WHERE equipment_id = "
        ":equipment_id");

    // 将参数绑定到占位符
    query.bindValue(":equipment_id", equipment_id);
    if (query.exec()) {
        while (query.next()) {
            CheckDataManageBasicInfo basicInfo;
            basicInfo.temperature = query.value("temperature").toString();
            basicInfo.humidity = query.value("humidity").toString();
            basicInfo.check_date = query.value("check_date").toDate();
            basicInfo.check_address = query.value("check_address").toString();
            basicInfo.create_user_name = query.value("create_user_name").toString();
            basicInfo.conclusion = query.value("conclusion").toString();
            result = basicInfo;
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    return result;
}

// 核查数据管理-核查结果信息-核查项多功能校准源数据   参数首页数据的id
QList<QStringList> BaseCommonApi::getCheckDataManageResultManageMult(
    QString data_id, QStringList* HeadList)
{
    QList<QStringList> resultDate;
    QStringList result;
    QSqlQuery query;
    query.prepare(
        "SELECT * FROM biz_equipment_standard_check_data_calibrator WHERE "
        "data_id = :data_id ");

    query.bindValue(":data_id", data_id);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("data_id").toString());
            result.append(query.value("check_type").toString());
            result.append(query.value("test_type").toString());
            result.append(query.value("params").toString());
            result.append(query.value("unit").toString());
            result.append(query.value("frequency").toString());
            result.append(query.value("frequency_unit").toString());
            result.append(query.value("check_point").toString());

            result.append(query.value("test_value_one_date").toString());
            result.append(query.value("test_value_one").toString());

            result.append(query.value("test_value_two_date").toString());
            result.append(query.value("test_value_two").toString());

            result.append(query.value("test_value_three_date").toString());
            result.append(query.value("test_value_three").toString());

            result.append(query.value("test_value_four_date").toString());
            result.append(query.value("test_value_four").toString());

            result.append(query.value("test_value_five_date").toString());
            result.append(query.value("test_value_five").toString());

            result.append(query.value("test_value_six_date").toString());
            result.append(query.value("test_value_six").toString());

            result.append(query.value("average_value").toString());
            result.append(query.value("standard_deviation").toString());
            result.append(query.value("relative_repeatability").toString());
            result.append(query.value("urel").toString());
            result.append(query.value("less_than").toString());
            result.append(query.value("qualified").toString());

            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());

            resultDate.append(result);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    *HeadList << "id"
              << "核查数据管理基础信息表id"
              << "核查类型"
              << "核查点类型"
              << "核查项目"
              << "单位"
              << "频率"
              << "频率单位"
              << "核查点"
              << "核查读数1日期"
              << "核查读数1(示值)"
              << "核查读数2日期（稳定性）"
              << "核查读数2（示值）"
              << "核查读数3日期（稳定性）  "
              << "核查读数3（示值）"
              << "核查读数4日期（稳定性）"
              << "核查读数4（示值）"
              << "核查读数5日期（稳定性）"
              << "核查读数5（示值）"
              << "核查读数6日期（稳定性）"
              << "核查读数6（本次示值）"
              << "核查读数平均值(算数平均值)"
              << "Sn(x)(标准偏差)"
              << "Sn(x)/核查读数平均值(相对重复性)"
              << "UREL"
              << "是否小于相对扩展不确定度Urel(0/1)"
              << "稳定性考核是否合格"
              << "创建时间"
              << "更新时间";
    return resultDate;
}
// 技术文件管理
QList<Technique_File> BaseCommonApi::getTechnique_File()
{
    QList<Technique_File> result;
    QSqlQuery query;
    bool isSuccess = query.exec(
        "SELECT brtf.id,brtf.file_distribute_no ,brtf.file_name ,brtf.version_no "
        ",brtf.active_date ,brtf.remark "
        " FROM biz_resource_technique_file brtf");

    if (isSuccess) {
        while (query.next()) {
            Technique_File file;
            file.id = query.value("id").toString();
            file.file_distribute_no = query.value("file_distribute_no").toString();
            file.file_name = query.value("file_name").toString();
            file.version_no = query.value("version_no").toString();
            file.active_date = query.value("active_date").toDateTime();
            file.remark = query.value("remark").toString();

            result.append(file);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    return result;
}
// 检定程序管理首页查询
QList<VerificationProgramInfo> BaseCommonApi::getVerificationProgramInfoMain()
{
    QList<VerificationProgramInfo> result;
    QSqlQuery query;
    bool isSuccess = query.exec("SELECT *  FROM zd_verification_program_info");
    if (isSuccess) {
        while (query.next()) {
            VerificationProgramInfo programInfo;
            programInfo.id = query.value("id").toString();
            programInfo.program_name = query.value("program_name").toString();
            programInfo.create_name = query.value("create_name").toString();
            programInfo.create_time = query.value("create_time").toDateTime();
            programInfo.remark = query.value("remark").toString();
            programInfo.belong_standard_equipment = query.value("belong_standard_equipment").toString();
            programInfo.equipment_item_category = query.value("equipment_item_category").toString();
            programInfo.equipment_name = query.value("equipment_name").toString();
            programInfo.model = query.value("model").toString();
            programInfo.help_instruction = query.value("help_instruction").toString();
            result.append(programInfo);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
}
// 查询检定项分类  参数 装置字典键  例如 1 ，2  可传空值 “” 查询所有
QList<itemCategory> BaseCommonApi::getitemCategoryList(QString item)
{
    QList<itemCategory> result;
    QSqlQuery query;
    if (item.isEmpty() || item.isNull()) {
        query.prepare("SELECT * FROM biz_business_device_measure_item_category");
    } else {
        query.prepare(
            "SELECT * FROM biz_business_device_measure_item_category  WHERE  "
            "belong_standard_equipment = :item ");
        query.bindValue(":item", item);
    }
    if (query.exec()) {
        while (query.next()) {
            itemCategory category;
            category.id = query.value("id").toString();
            category.category_no = query.value("category_no").toString();
            category.category_name = query.value("category_name").toString();
            category.create_time = query.value("create_time").toDateTime();
            category.device_type = query.value("device_type").toString();
            category.belong_standard_equipment = query.value("belong_standard_equipment").toString();
            category.measure_category = query.value("measure_category").toString();
            category.technology_file = query.value("technology_file").toString();
            category.update_time = query.value("update_time").toDateTime();
            category.lab_x = query.value("lab_x").toString();
            category.lab_h = query.value("lab_h").toString();
            category.wiring_diagram = query.value("wiring_diagram").toString();
            result.append(category);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
}

// 检定程序管理-编辑 查询
VerificationProgramInfo BaseCommonApi::getVerificationProgramInfo(QString id)
{
    VerificationProgramInfo result;
    QSqlQuery query;
    query.prepare("SELECT * FROM zd_verification_program_info WHERE id = :id ");
    query.bindValue(":id", id);
    if (query.exec()) {
        while (query.next()) {
            VerificationProgramInfo programInfo;
            programInfo.id = query.value("id").toString();
            programInfo.program_name = query.value("program_name").toString();
            programInfo.create_name = query.value("create_name").toString();
            programInfo.create_time = query.value("create_time").toDateTime();
            programInfo.remark = query.value("remark").toString();
            programInfo.img_path = query.value("img_path").toString();
            programInfo.tested_device_id = query.value("tested_device_id").toString();
            programInfo.standard_device_id = query.value("standard_device_id").toString();
            programInfo.update_time = query.value("update_time").toDateTime();
            programInfo.belong_standard_equipment = query.value("belong_standard_equipment").toString();
            programInfo.equipment_item_category = query.value("equipment_item_category").toString();
            programInfo.equipment_name = query.value("equipment_name").toString();
            programInfo.model = query.value("model").toString();
            programInfo.help_instruction = query.value("help_instruction").toString();
            programInfo.biz_standard_name = query.value("biz_standard_name").toString();
            programInfo.biz_standard_id = query.value("biz_standard_id").toString();
            result = programInfo;
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
}

// 查询设备通信接口
QList<VerificationProgramConfigInterface>
BaseCommonApi::getVerificationProgramConfigInterface(QString deviceId)
{
    QList<VerificationProgramConfigInterface> result;
    QSqlQuery query;
    query.prepare(
        "SELECT id,device_id ,communication_type ,serial_port ,visa ,create_time "
        ",update_time "
        "FROM zd_communication_setting WHERE device_id =   :deviceId  ");
    query.bindValue(":deviceId", deviceId);
    if (query.exec()) {
        while (query.next()) {
            VerificationProgramConfigInterface configInterface;
            configInterface.id = query.value("id").toString();
            configInterface.device_id = query.value("device_id").toString();
            configInterface.communication_type = query.value("communication_type").toString();
            configInterface.serial_port = query.value("serial_port").toString();
            configInterface.visa = query.value("visa").toString();
            configInterface.create_time = query.value("create_time").toDateTime();
            configInterface.update_time = query.value("update_time").toDateTime();

            result.append(configInterface);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
};

// 新建检定程序
bool BaseCommonApi::InsertVerificationProgramInfo(
    VerificationProgramInfo verInfo)
{
    QSqlQuery query;
    query.prepare(
        " INSERT INTO zd_verification_program_info "
        "(id,program_name,create_name,create_time, "
        " remark,img_path,tested_device_id,standard_device_id,update_time,belong_"
        "standard_equipment,"
        " equipment_item_category,equipment_name,model,help_instruction,biz_"
        "standard_name,biz_standard_id)"
        " VALUES (:id, :program_name, :create_name, :create_time, :remark, "
        ":img_path, :tested_device_id, "
        " :standard_device_id,:update_time,:belong_standard_equipment,:equipment_"
        "item_category,"
        " :equipment_name,:model,:help_instruction,:biz_standard_name,:biz_"
        "standard_id);");
    QString formattedDateTime = verInfo.create_time.toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", verInfo.id);
    query.bindValue(":program_name", verInfo.program_name);
    query.bindValue(":create_name", verInfo.create_name);
    query.bindValue(":create_time", formattedDateTime);
    query.bindValue(":remark", verInfo.remark);
    query.bindValue(":img_path", verInfo.img_path);
    query.bindValue(":tested_device_id", verInfo.tested_device_id);
    query.bindValue(":standard_device_id", verInfo.standard_device_id);
    query.bindValue(":update_time", formattedDateTime);
    query.bindValue(":belong_standard_equipment",
        verInfo.belong_standard_equipment);
    query.bindValue(":equipment_item_category", verInfo.equipment_item_category);
    query.bindValue(":equipment_name", verInfo.equipment_name);
    query.bindValue(":model", verInfo.model);
    query.bindValue(":help_instruction", verInfo.help_instruction);
    query.bindValue(":biz_standard_name", verInfo.biz_standard_name);
    query.bindValue(":biz_standard_id", verInfo.biz_standard_id);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << "InsertTaskBasic query failed: " << query.lastError().text();
    }

    return isSuccess;
};

// 新增通讯接口
bool BaseCommonApi::InsertVerificationProgramConfigInterface(
    VerificationProgramConfigInterface configInterdace)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO zd_communication_setting "
        "(id,device_id,communication_type,serial_port,visa,create_time,update_"
        "time) "
        "  VALUES "
        "(:id,:device_id,:communication_type,:serial_port,:visa,:create_time,:"
        "update_time)");
    QString formattedDateTime = configInterdace.create_time.toString("yyyy-MM-dd HH:mm:ss");
    QString formattedUpdateDateTime = configInterdace.update_time.toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", configInterdace.id);
    query.bindValue(":device_id", configInterdace.device_id);
    query.bindValue(":communication_type", configInterdace.communication_type);
    query.bindValue(":serial_port", configInterdace.serial_port);
    query.bindValue(":visa", configInterdace.visa);
    query.bindValue(":create_time", formattedDateTime);
    query.bindValue(":update_time", formattedUpdateDateTime);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << "InsertTaskBasic query failed: " << query.lastError().text();
    }

    return isSuccess;
};

// 更新检定程序
bool BaseCommonApi::UpdateVerificationProgramInfo(
    VerificationProgramInfo verInfo)
{
    QString biz_standard_name; // 标准器的装置名称按，分割
    QString biz_standard_id; // 标准器的装置id按，分割
    QSqlQuery query;
    query.prepare(
        "UPDATE zd_verification_program_info "
        " SET program_name = :program_name,"
        "create_name = :create_name, "
        "create_time = :create_time,"
        "remark = :remark,"
        "img_path = :img_path,"
        "tested_device_id = :tested_device_id,"
        "standard_device_id = :standard_device_id ,"
        "update_time = :update_time, "
        "belong_standard_equipment = :belong_standard_equipment,"
        "equipment_item_category = :equipment_item_category,"
        "equipment_name = :equipment_name,"
        "model=:model,"
        "help_instruction=:help_instruction, "
        "biz_standard_name=:biz_standard_name, "
        "biz_standard_id=:biz_standard_id "
        " WHERE id = :id;");
    QString formattedDateTime = verInfo.create_time.toString("yyyy-MM-dd HH:mm:ss");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":update_time", currentDateTime);
    query.bindValue(":program_name", verInfo.program_name);
    query.bindValue(":create_name", verInfo.create_name);
    query.bindValue(":create_time", formattedDateTime);
    query.bindValue(":remark", verInfo.remark);
    query.bindValue(":img_path", verInfo.img_path);
    query.bindValue(":tested_device_id", verInfo.tested_device_id);
    query.bindValue(":standard_device_id", verInfo.standard_device_id);
    query.bindValue(":belong_standard_equipment",
        verInfo.belong_standard_equipment);
    query.bindValue(":equipment_item_category", verInfo.equipment_item_category);
    query.bindValue(":equipment_name", verInfo.equipment_name);
    query.bindValue(":model", verInfo.model);
    query.bindValue(":help_instruction", verInfo.help_instruction);
    query.bindValue(":biz_standard_name", verInfo.biz_standard_id);
    query.bindValue(":biz_standard_id", verInfo.biz_standard_name);
    query.bindValue(":id", verInfo.id);
    bool isSuccess = query.exec();
    qDebug() << "执行的sql：" << query.lastQuery();
    if (!isSuccess) {
        qDebug() << "Insert query failed: " << query.lastError().text();
    }
    return isSuccess;
};
// 更新通讯接口
bool BaseCommonApi::UpdateVerificationProgramConfigInterface(
    VerificationProgramConfigInterface configInterdace)
{
    QSqlQuery query;
    query.prepare(
        "UPDATE zd_communication_setting SET communication_type = "
        ":communication_type, "
        " serial_port = :serial_port,visa = :visa,update_time = :update_time "
        " WHERE device_id = :device_id;");
    QString formattedDateTime = configInterdace.update_time.toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":communication_type", configInterdace.communication_type);
    query.bindValue(":serial_port", configInterdace.serial_port);
    query.bindValue(":visa", configInterdace.visa);
    query.bindValue(":update_time", formattedDateTime);
    query.bindValue(":device_id", configInterdace.device_id);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << "Insert query failed: " << query.lastError().text();
    }
    return isSuccess;
};
// 删除检定程序
bool BaseCommonApi::DeleteVerificationProgramInfo(QString id)
{
    QSqlQuery query;
    // 删除检定程序
    query.prepare("DELETE FROM zd_verification_program_info WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "删除失败: " << query.lastError().text();
        return false;
    }
    return true;
};

// 查询指令库  参数 检定程序id
QList<InstructionLib> BaseCommonApi::getInstructionLib(
    QString verification_program_id)
{
    QList<InstructionLib> result;
    QSqlQuery query;
    QString sql = "SELECT * FROM zd_instruction_library WHERE device_id = %1 ";
    QString ssql = sql.arg(verification_program_id);
    if (query.exec(ssql)) {
        while (query.next()) {
            InstructionLib instruction;
            instruction.id = query.value("id").toString();
            instruction.device_id = query.value("device_id").toString();
            instruction.device_name = query.value("device_name").toString();
            instruction.instruct_config = query.value("instruct_config").toString();
            instruction.instruct_read = query.value("instruct_read").toString();
            instruction.type = query.value("type").toString();
            instruction.belong_vername = query.value("belong_vername").toString();
            instruction.belong_checkname = query.value("belong_checkname").toString();
            instruction.create_time = query.value("create_time").toDateTime();
            instruction.update_time = query.value("update_time").toDateTime();
            //               instruction.verification_program_id =
            //               query.value("verification_program_id").toString();
            instruction.param = query.value("param").toString();
            result.append(instruction);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
};
// 查询检定项用于在新增指令时筛选 例如DCV ACI
// 如果equipment_item_category_id为空查询所有
QList<Params> BaseCommonApi::getParams(QString belong_standard_equipment_id,
    QString equipment_item_category_id)
{
    QList<Params> result;
    QSqlQuery query;
    QList<itemCategory> standard_item = getitemCategoryList(belong_standard_equipment_id);
    switch (belong_standard_equipment_id.toInt()) {
    case 1:
        if (equipment_item_category_id.isNull() || equipment_item_category_id.isEmpty()) {
            query.prepare(
                "SELECT DISTINCT bbdmidc.params FROM "
                "biz_business_device_measure_item_data_calibrator bbdmidc "
                " JOIN biz_business_device_measure_item_info bbdmii ON bbdmii.id= "
                "bbdmidc.item_id ");
        } else {
            query.prepare(
                "SELECT DISTINCT bbdmidc.params FROM "
                "biz_business_device_measure_item_data_calibrator bbdmidc "
                " JOIN biz_business_device_measure_item_info bbdmii ON bbdmii.id= "
                "bbdmidc.item_id "
                " WHERE bbdmii.item_category_id = :equipment_item_category_id ");
        }
        query.bindValue(":equipment_item_category_id",
            equipment_item_category_id);
        if (query.exec()) {
            while (query.next()) {
                Params param;
                param.param = query.value("params").toString();
                result.append(param);
            }
        } else {
            qDebug() << "Query execution failed: " << query.lastError().text();
        }
        break;
    case 2:
        if (equipment_item_category_id.isNull() || equipment_item_category_id.isEmpty()) {
            query.prepare(
                "SELECT DISTINCT  data_type FROM "
                "biz_business_device_measure_item_data_dc_power bbdmiddp "
                " JOIN biz_business_device_measure_item_info bbdmii ON bbdmii.id= "
                "bbdmiddp.item_id");
        } else {
            query.prepare(
                "SELECT DISTINCT  data_type FROM "
                "biz_business_device_measure_item_data_dc_power bbdmiddp "
                " JOIN biz_business_device_measure_item_info bbdmii ON bbdmii.id= "
                "bbdmiddp.item_id "
                "WHERE bbdmii.item_category_id = :equipment_item_category_id ");
        }
        query.bindValue(":equipment_item_category_id",
            equipment_item_category_id);
        if (query.exec()) {
            while (query.next()) {
                Params param;
                QString data_type = query.value("data_type").toString();
                switch (data_type.toInt()) {
                case 1:
                    param.param = "电源电压调整率";
                    break;
                case 2:
                    param.param = "负载调整率";
                    break;
                case 3:
                    param.param = "电压示值误差";
                    break;
                case 4:
                    param.param = "电流示值误差(直接测量)";
                    break;
                case 5:
                    param.param = "电流示值误差(间接测量)";
                    break;
                case 6:
                    param.param = "纹波电压";
                    break;
                case 7:
                    param.param = "输出电压短期稳定性";
                    break;
                }
                result.append(param);
            }
        }
        break;
    case 4: {
        bool isExit = false;
        QString itemName;
        if (!equipment_item_category_id.isNull() && !equipment_item_category_id.isEmpty()) {
            for (int i = 0; i < standard_item.length(); i++) {
                if (equipment_item_category_id.contains(standard_item[i].id)) {
                    isExit = true;
                    itemName = standard_item[i].category_name;
                    break; // 找到匹配项后退出循环
                }
            }
            if (isExit && itemName.contains("一般压力表")) {
                Params param;
                param.param = "示值误差/回程误差/轻敲位移";
                result.append(param);
            }
        } else {
            Params param;
            param.param = "示值误差/回程误差/轻敲位移";
            Params param1;
            param1.param = "示值误差";
            Params param2;
            param2.param = "回程误差";
            Params param3;
            param3.param = "轻敲位移";
            QList<Params> paramList;
            paramList.append(param1);
            paramList.append(param2);
            paramList.append(param3);
            for (int i = 0; i < paramList.length(); i++) {
                result.append(paramList[i]);
            }
        }
        break; // 确保每个case结束都有break语句
    }
    case 5: {
        if (!equipment_item_category_id.isNull() || equipment_item_category_id.isEmpty()) {
            query.prepare(
                "SELECT DISTINCT bbdmidrt.data_type,bbdmii.item_category_name FROM "
                "biz_business_device_measure_item_data_resistance_thermometer "
                "bbdmidrt"
                " JOIN biz_business_device_measure_item_info bbdmii ON bbdmii.id= "
                "bbdmidrt.item_id"
                " WHERE bbdmii.item_category_id = :equipment_item_category_id ");
            query.bindValue(":equipment_item_category_id",
                equipment_item_category_id);
            if (query.exec()) {
                while (query.next()) {
                    Params param;
                    param.param = query.value("data_type").toString();
                    param.itemCategoryName = query.value("item_category_name").toString();
                    if (param.param == "1" && param.itemCategoryName.contains("恒温槽"))
                        param.param = "波动性";
                    else if (param.param == "2" && param.itemCategoryName.contains("恒温槽")) {
                        param.param = "均匀性";
                    } else if (param.param == "1" && param.itemCategoryName.contains("工业铂铜热电阻")) {
                        param.param = "允差";
                    } else if (param.param == "1" && param.itemCategoryName.contains("工业铂铜热电阻")) {
                        param.param = "结果表格";
                    }
                    result.append(param);
                }
            } else {
                qDebug() << "Query execution failed: " << query.lastError().text();
            }
            break;
        case 9: {
            query.prepare("");
        }
        }
        break;
    }
    }
    return result;
};

// 新增指令库
bool BaseCommonApi::InsertInstructionLib(InstructionLib instructionLib)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO zd_instruction_library "
        "(id,device_id,device_name,instruct_config,instruct_read,"
        "type,belong_vername,belong_checkname,create_time,update_time,"
        "verification_program_id,param) "
        " VALUES "
        "(:id,:device_id,:device_name,:instruct_config,:instruct_read,:type,:"
        "belong_vername,"
        ":belong_checkname,:create_time,:update_time,:verification_program_id,:"
        "param)");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", instructionLib.id);
    query.bindValue(":device_id", instructionLib.device_id);
    query.bindValue(":device_name", instructionLib.device_name);
    query.bindValue(":instruct_config", instructionLib.instruct_config);
    query.bindValue(":instruct_read", instructionLib.instruct_read);
    query.bindValue(":type", instructionLib.type);
    query.bindValue(":belong_vername", instructionLib.belong_vername);
    query.bindValue(":belong_checkname", instructionLib.belong_checkname);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    query.bindValue(":verification_program_id", instructionLib.program_id);
    query.bindValue(":param", instructionLib.param);
    bool isSuccess = query.exec();
    //        qDebug() << "InsertTaskBasic query failed: " << query.lastQuery();
    if (!isSuccess) {
        QString str = query.lastError().text();
        qDebug() << "InsertTaskBasic query failed: " << query.lastError().text();
    }

    return isSuccess;
};
// 删除指令库
bool BaseCommonApi::DeleteInstructionLib(QString id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM zd_instruction_library WHERE id = :id ");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "删除失败: " << query.lastError().text();
        return false;
    }
    return true;
};
// 更新指令库
bool BaseCommonApi::UpdateInstructionLib(InstructionLib instructionLib)
{
    QSqlQuery query;
    query.prepare(
        "UPDATE zd_instruction_library SET device_id=:device_id,"
        "device_name = :device_name,instruct_config = "
        ":instruct_config,instruct_read = :instruct_read,"
        " type = :type,belong_vername = :belong_vername,belong_checkname = "
        ":belong_checkname,"
        "update_time = "
        ":update_time,verification_program_id=:verification_program_id,param=:"
        "param WHERE id = :id");

    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":device_id", instructionLib.device_id);
    query.bindValue(":device_name", instructionLib.device_name);
    query.bindValue(":instruct_config", instructionLib.instruct_config);
    query.bindValue(":instruct_read", instructionLib.instruct_read);
    query.bindValue(":type", instructionLib.type);
    query.bindValue(":belong_vername", instructionLib.belong_vername);
    query.bindValue(":belong_checkname", instructionLib.belong_checkname);
    query.bindValue(":update_time", currentDateTime);
    query.bindValue(":verification_program_id", instructionLib.program_id);
    query.bindValue(":param", instructionLib.param);
    query.bindValue(":id", instructionLib.id);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << "Insert query failed: " << query.lastError().text();
    }
    return isSuccess;
};
// 雪花算法生成id
qint64 BaseCommonApi::generateId() { return this->idWorker.nextId(); };
QString BaseCommonApi::GenerateTaskNumber()
{
    QList<CreateTaskList> TaskList = BaseCommonApi::getCreateTaskListNew();
    return TaskManager::generateTask(TaskList.first().order_no);
};
bool BaseCommonApi::AddTaskNumber()
{
    return TaskManager::AddCurrenTaskCount();
};
// 自动检定-根据被检设备 查询检定项分类
QList<DeviceMeasureEquipmentCatory>
BaseCommonApi::getDeviceMeasureEquipmentCatory(QString deviceId)
{
    QList<DeviceMeasureEquipmentCatory> result;
    QSqlQuery query;
    query.prepare(
        " SELECT bbdmii.*, eemi.category AS deviceType, eemi.equipment_name AS "
        "deviceName, "
        " eemi.model AS model, eemi.help_instruction AS helpInstruction, "
        "eemi.instructions_file AS instructionsFile, "
        " eemi.check_cycle AS checkCycle, eemi.id AS eqptDeviceModelId, "
        "eemi.category AS eqptDeviceType "
        " FROM eqpt_equipment_model_info eemi "
        " LEFT JOIN biz_business_device_measure_item_info bbdmii ON eemi.id = "
        "bbdmii.device_model_id "
        " JOIN eqpt_equipment_info eei ON eei.model_id = eemi.id "
        " WHERE  eei.id = :deviceId "
        " ORDER BY eemi.create_time DESC ");
    query.bindValue(":deviceId", deviceId);
    if (query.exec()) {
        while (query.next()) {
            DeviceMeasureEquipmentCatory catory;
            catory.id = query.value("id").toString();
            catory.device_model_id = query.value("device_model_id").toString();
            catory.belong_standard_equipment = query.value("belong_standard_equipment").toString();
            catory.help_field_instruction = query.value("help_field_instruction").toString();
            catory.item_category_id = query.value("item_category_id").toString();
            catory.item_category_name = query.value("item_category_name").toString();
            catory.deviceType = query.value("deviceType").toString();
            catory.deviceName = query.value("deviceName").toString();
            catory.model = query.value("model").toString();
            catory.helpInstruction = query.value("helpInstruction").toString();
            catory.instructionsFile = query.value("instructionsFile").toString();
            catory.checkCycle = query.value("checkCycle").toString();
            catory.eqptDeviceModelId = query.value("eqptDeviceModelId").toString();
            catory.eqptDeviceType = query.value("eqptDeviceType").toString();
            result.append(catory);
        }
        //        qDebug() << query.lastQuery();
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
};
////自动检定-根据被检设备 查询检定项分类 在查询检定程序
// 根据getDeviceMeasureEquipmentCatory返回的item_category_id查询检定程序
QList<VerificationProgramInfo>
BaseCommonApi::getVerificationProgramInfoByDeviceId(QString deviceId)
{
    QList<DeviceMeasureEquipmentCatory> catory;
    QSqlQuery query;
    QList<VerificationProgramInfo> result;
    catory = BaseCommonApi::getDeviceMeasureEquipmentCatory(deviceId);
    if (catory.length() <= 0) {
        return result;
    }
    QString equipment_item_category = catory[0].item_category_id;
    query.prepare(
        "SELECT * from zd_verification_program_info zvpi WHERE "
        "zvpi.equipment_item_category = :equipment_item_category");
    query.bindValue(":equipment_item_category", equipment_item_category);
    if (query.exec()) {
        while (query.next()) {
            VerificationProgramInfo info;
            info.id = query.value("id").toString();
            info.program_name = query.value("program_name").toString();
            info.create_name = query.value("create_name").toString();
            info.create_time = query.value("create_time").toDateTime();
            info.remark = query.value("remark").toString();
            info.img_path = query.value("img_path").toString();
            info.tested_device_id = query.value("tested_device_id").toString();
            info.standard_device_id = query.value("standard_device_id").toString();
            info.update_time = query.value("update_time").toDateTime();
            info.belong_standard_equipment = query.value("belong_standard_equipment").toString();
            info.equipment_item_category = query.value("equipment_item_category").toString();
            info.equipment_name = query.value("equipment_name").toString();
            info.model = query.value("model").toString();
            info.help_instruction = query.value("help_instruction").toString();
            result.append(info);
        }

    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
};

// 自动检定-结果保存-biz_business_device_measure_info 基本信息表
bool BaseCommonApi::InsertSaveDeviceMeasureInfo(
    SaveDeviceMeasureInfo saveInfo)
{
    saveInfo.data_no = TaskManager::getsaveResultdatano();

    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_business_device_measure_info ( "
        " id, data_no, measure_category, order_id, sample_id, sample_name, "
        "model, "
        " manufacture_no, manufacturer, item_id, trace_date, measure_valid_date, "
        " measure_address, temperature, humidity, data_source, ipri, ipri_unit, "
        " isec, isec_unit, vpri, vpri_unit, vsec, vsec_unit, "
        "appearance_function_check, "
        " appearance, appearance_remark, pointer_deflection_stability, "
        " pointer_deflection_stability_remark, tightness, tightness_remark,"
        " insulation_resistance, insulation_resistance_remark, measure_medium,"
        " division, zero_drift_result, work_line_equation, sealing_test, "
        " insulation_resistance_value, angle_adjust_error, actual_division_value,"
        " actual_division_value_unit, conclusion, restriction_instruction, "
        "outcome,"
        " other_outcome, remark, other_remark, create_user_id, measure_person_id,"
        " create_user_name,meter_identify, create_time, update_time "
        " ) VALUES ( "
        " :id, :data_no, :measure_category, :order_id, :sample_id, :sample_name, "
        ":model, "
        " :manufacture_no, :manufacturer, :item_id, :trace_date, "
        ":measure_valid_date, "
        " :measure_address, :temperature, :humidity, :data_source, :ipri, "
        ":ipri_unit, "
        " :isec, :isec_unit, :vpri, :vpri_unit, :vsec, :vsec_unit, "
        ":appearance_function_check, "
        " :appearance, :appearance_remark, :pointer_deflection_stability,  "
        " :pointer_deflection_stability_remark, :tightness, :tightness_remark, "
        " :insulation_resistance, :insulation_resistance_remark, "
        ":measure_medium, "
        " :division, :zero_drift_result, :work_line_equation, :sealing_test, "
        " :insulation_resistance_value, :angle_adjust_error, "
        ":actual_division_value, "
        " :actual_division_value_unit, :conclusion, :restriction_instruction, "
        ":outcome, "
        " :other_outcome, :remark, :other_remark, :create_user_id, "
        ":measure_person_id, "
        " :create_user_name,:meter_identify, :create_time, :update_time);");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    //       query.bindValue(":id", saveInfo.id); //id 自动生成
    query.bindValue(":id", saveInfo.id);
    query.bindValue(":data_no", saveInfo.data_no);
    query.bindValue(":measure_category", saveInfo.measure_category);
    query.bindValue(":order_id", saveInfo.order_id);
    query.bindValue(":sample_id", saveInfo.sample_id);
    query.bindValue(":sample_name", saveInfo.sample_name);
    query.bindValue(":model", saveInfo.model);
    query.bindValue(":manufacture_no", saveInfo.manufacture_no);
    query.bindValue(":manufacturer", saveInfo.manufacturer);
    query.bindValue(":item_id", saveInfo.item_id);
    query.bindValue(":trace_date",
        saveInfo.trace_date.toString(
            "yyyy-MM-dd")); // 根据数据库实际日期格式转换
    query.bindValue(":measure_valid_date",
        saveInfo.measure_valid_date.toString("yyyy-MM-dd"));
    query.bindValue(":measure_address", saveInfo.measure_address);
    query.bindValue(":temperature", saveInfo.temperature);
    query.bindValue(":humidity", saveInfo.humidity);
    query.bindValue(":data_source", saveInfo.data_source);
    query.bindValue(":ipri", saveInfo.ipri);
    query.bindValue(":ipri_unit", saveInfo.ipri_unit);
    query.bindValue(":isec", saveInfo.isec);
    query.bindValue(":isec_unit", saveInfo.isec_unit);
    query.bindValue(":vpri", saveInfo.vpri);
    query.bindValue(":vpri_unit", saveInfo.vpri_unit);
    query.bindValue(":vsec", saveInfo.vsec);
    query.bindValue(":vsec_unit", saveInfo.vsec_unit);
    query.bindValue(":appearance_function_check",
        saveInfo.appearance_function_check);
    query.bindValue(":appearance", saveInfo.appearance);
    query.bindValue(":appearance_remark", saveInfo.appearance_remark);
    query.bindValue(":pointer_deflection_stability",
        saveInfo.pointer_deflection_stability);
    query.bindValue(":pointer_deflection_stability_remark",
        saveInfo.pointer_deflection_stability_remark);
    query.bindValue(":tightness", saveInfo.tightness);
    query.bindValue(":tightness_remark", saveInfo.tightness_remark);
    query.bindValue(":insulation_resistance", saveInfo.insulation_resistance);
    query.bindValue(":insulation_resistance_remark",
        saveInfo.insulation_resistance_remark);
    query.bindValue(":measure_medium", saveInfo.measure_medium);
    query.bindValue(":division", saveInfo.division);
    query.bindValue(":zero_drift_result", saveInfo.zero_drift_result);
    query.bindValue(":work_line_equation", saveInfo.work_line_equation);
    query.bindValue(":sealing_test", saveInfo.sealing_test);
    query.bindValue(":insulation_resistance_value",
        saveInfo.insulation_resistance_value);
    query.bindValue(":angle_adjust_error", saveInfo.angle_adjust_error);
    query.bindValue(":actual_division_value", saveInfo.actual_division_value);
    query.bindValue(":actual_division_value_unit",
        saveInfo.actual_division_value_unit);
    query.bindValue(":conclusion", saveInfo.conclusion);
    query.bindValue(":restriction_instruction", saveInfo.restriction_instruction);
    query.bindValue(":outcome", saveInfo.outcome);
    query.bindValue(":other_outcome", saveInfo.other_outcome);
    query.bindValue(":remark", saveInfo.remark);
    query.bindValue(":other_remark", saveInfo.other_remark);
    query.bindValue(":create_user_id", saveInfo.create_user_id);
    query.bindValue(":measure_person_id", saveInfo.measure_person_id);
    query.bindValue(":create_user_name", saveInfo.create_user_name);
    query.bindValue(":meter_identify", saveInfo.meter_identify);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << "InsertTaskBasic query failed: " << query.lastError().text();
    } else {
        TaskManager::AddResultDataNoCount();
        QString ssql = "INSERT INTO biz_business_device_measure_info ( "
                       " id, data_no, measure_category, order_id, sample_id, sample_name, "
                       "model, "
                       " manufacture_no, manufacturer, item_id, trace_date, "
                       "measure_valid_date, "
                       " measure_address, temperature, humidity, data_source, ipri, "
                       "ipri_unit, "
                       " isec, isec_unit, vpri, vpri_unit, vsec, vsec_unit, "
                       "appearance_function_check, "
                       " appearance, appearance_remark, pointer_deflection_stability, "
                       " pointer_deflection_stability_remark, tightness, tightness_remark,"
                       " insulation_resistance, insulation_resistance_remark, measure_medium,"
                       " division, zero_drift_result, work_line_equation, sealing_test, "
                       " insulation_resistance_value, angle_adjust_error, "
                       "actual_division_value,"
                       " actual_division_value_unit, conclusion, restriction_instruction, "
                       "outcome,"
                       " other_outcome, remark, other_remark, create_user_id, "
                       "measure_person_id,"
                       " create_user_name,meter_identify,create_time, update_time "
                       " ) VALUES ( "
                       " :1, :2, :3, :4, :5, :6, :7, "
                       " :8, :9, :10, :11, :12, "
                       " :13, :14, :15, :16, :17, :18, "
                       " :19, :20, :21, :22, :23, :24, :25, "
                       " :26, :27, :28,  "
                       " :29, :30, :31, "
                       " :32, :33, :34, "
                       " :35, :36, :37, :38, "
                       " :39, :40, :41, "
                       " :42, :43, :44, :45, "
                       " :46, :47, :48, :49, :50, "
                       " :51, :52, :53,:54);";
        saveLogToSql(ssql, &saveInfo);
    }

    return isSuccess;
};

// 计量检定业务-受检设备检定管理-检定数据管理-所使用的标准、主要测量设备
bool BaseCommonApi::InsertDeviceMeasureEquipment(
    DeviceMeasureEquipment deviceMeasure)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_business_device_measure_equipment "
        " (id,data_id,equipment_id,standard_id,standard_name,create_time,update_"
        "time)"
        " VALUES"
        " (:id,:data_id,:equipment_id,:standard_id,:standard_name,:create_time,:"
        "update_time)");

    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    //       QString createTime = deviceMeasure.create_time.toString("yyyy-MM-dd
    //       HH:mm:ss");
    query.bindValue(":id", deviceMeasure.id);
    query.bindValue(":data_id", deviceMeasure.data_id);
    query.bindValue(":equipment_id", deviceMeasure.equipment_id);
    query.bindValue(":standard_id", deviceMeasure.standard_id);
    query.bindValue(":standard_name", deviceMeasure.standard_name);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (isSuccess) {
        QString ssql = "INSERT INTO biz_business_device_measure_equipment "
                       " (id,data_id,equipment_id,standard_id,standard_name,create_time,"
                       "update_time) "
                       " VALUES "
                       " (:1,:2,:3,:4,:5,:6,:7);";
        saveLogToSql(ssql, &deviceMeasure);
    } else {
        qDebug() << "InsertTaskBasic query failed: " << query.lastError().text();
    }

    return isSuccess;
};
bool BaseCommonApi::saveLogToSql(QString ssql, BaseClass* entity)
{
    zdDataSync sync;
    QString resultsql;
    QString chart = "/";
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    if (!ssql.isEmpty()) {
        ssql = ssql.replace(":", "%");
    }
    if (entity == nullptr) {
        resultsql = QString(ssql).arg("'" + currentDateTime + "'");
    }
    InsertTask* orderInfo = dynamic_cast<InsertTask*>(entity);
    OrderSampleRelation* orderSampleRelation = dynamic_cast<OrderSampleRelation*>(entity);
    biz_business_lab_executive_info* labExecutiveInfo = dynamic_cast<biz_business_lab_executive_info*>(entity);
    SaveDeviceMeasureInfo* deviceMeasureInfo = dynamic_cast<SaveDeviceMeasureInfo*>(entity);
    DeviceMeasureEquipment* deviceMeasureEquipment = dynamic_cast<DeviceMeasureEquipment*>(entity);
    SaveDeviceMeasureCalibrator* deviceMeasureCalibrator = dynamic_cast<SaveDeviceMeasureCalibrator*>(entity);
    // 计量检定业务-受检设备检定管理-检定数据管理检定数据历史修改记录[新增]
    MeasureModifyRecord* deviceModifyRecord = dynamic_cast<MeasureModifyRecord*>(entity);
    // 【结果保存】压力
    DeviceMeasureDataPistonGauge* devicePistonGaugeData = dynamic_cast<DeviceMeasureDataPistonGauge*>(entity);
    // 【结果保存】直流稳压电源
    bizBusinessDeviceMeasureDataDcPower* deviceDCPowerData = dynamic_cast<bizBusinessDeviceMeasureDataDcPower*>(entity);
    // 【结果保存】示波器
    DeviceOscilloscopeCalibration* deviceOscilloscopeData = dynamic_cast<DeviceOscilloscopeCalibration*>(entity);
    // 【结果保存】二等铂电阻
    DeviceMeasureDataResistanceThermometer* deviceResistanceThermometerData = dynamic_cast<DeviceMeasureDataResistanceThermometer*>(entity);
    // 【结果保存】频谱分析仪
    DeviceMeasureDataSpectrumAnalyzer* deviceSpectrumAnalyzerData = dynamic_cast<DeviceMeasureDataSpectrumAnalyzer*>(entity);
    // 【结果保存】信号发生器
    DeviceMeasureDataSignalGenerator* deviceSignalGeneratorData = dynamic_cast<DeviceMeasureDataSignalGenerator*>(entity);
    if (orderInfo != nullptr) {
        resultsql = QString(ssql)
                        .arg("'" + orderInfo->order_no + "'")
                        .arg("'" + orderInfo->customer_name + "'")
                        .arg("'" + orderInfo->require_over_time + "'")
                        .arg("'" + orderInfo->deliverer + "'")
                        .arg("'" + orderInfo->deliverer_tel + "'")
                        .arg("'" + orderInfo->undertaker_name + "'")
                        .arg("'" + orderInfo->undertake_time + "'")
                        .arg("'" + QString::number(orderInfo->is_urgent) + "'")
                        .arg("'" + orderInfo->receive_illustrate + "'")
                        .arg("'" + orderInfo->customer_id + "'")
                        .arg("'" + orderInfo->id + "'");
    }
    if (orderSampleRelation != nullptr) {
        resultsql = QString(ssql)
                        .arg("'" + orderSampleRelation->id + "'")
                        .arg("'" + orderSampleRelation->order_id + "'")
                        .arg("'" + orderSampleRelation->sample_id + "'")
                        .arg("'" + orderSampleRelation->sample_name + "'")
                        .arg("'" + orderSampleRelation->sample_model + "'")
                        .arg("'" + orderSampleRelation->measure_complete_time.toString() + "'")
                        .arg("'" + currentDateTime + "'")
                        .arg("'" + currentDateTime + "'");
    }
    if (labExecutiveInfo != nullptr) {
        resultsql = QString(ssql)
                        .arg("'" + labExecutiveInfo->id + "'")
                        .arg("'" + labExecutiveInfo->order_id + "'")
                        .arg("'" + labExecutiveInfo->sample_id + "'")
                        .arg("'" + labExecutiveInfo->measure_dept_id + "'")
                        .arg("'" + labExecutiveInfo->measure_person_id + "'")
                        .arg("'" + labExecutiveInfo->create_user_id + "'")
                        .arg("'" + labExecutiveInfo->measure_status + "'")
                        .arg("'" + labExecutiveInfo->is_auto + "'")
                        .arg("'" + currentDateTime + "'")
                        .arg("'" + currentDateTime + "'");
    }
    if (deviceMeasureInfo != nullptr) {
        resultsql = QString(ssql)
                        .arg("'" + deviceMeasureInfo->id + "'")
                        .arg("'" + deviceMeasureInfo->data_no + "'")
                        .arg("'" + deviceMeasureInfo->measure_category + "'")
                        .arg("'" + deviceMeasureInfo->order_id + "'")
                        .arg("'" + deviceMeasureInfo->sample_id + "'")
                        .arg("'" + deviceMeasureInfo->sample_name + "'")
                        .arg("'" + deviceMeasureInfo->model + "'")
                        .arg("'" + deviceMeasureInfo->manufacture_no + "'")
                        .arg("'" + deviceMeasureInfo->manufacturer + "'")
                        .arg("'" + deviceMeasureInfo->item_id + "'")
                        .arg("'" + deviceMeasureInfo->trace_date.toString("yyyy-MM-dd") + "'")
                        .arg("'" + deviceMeasureInfo->measure_valid_date.toString("yyyy-MM-dd HH:mm:ss") + "'")
                        .arg("'" + deviceMeasureInfo->measure_address + "'")
                        .arg(deviceMeasureInfo->temperature.toDouble())
                        .arg(deviceMeasureInfo->humidity.toDouble())
                        .arg("'" + deviceMeasureInfo->data_source + "'")
                        .arg("'" + deviceMeasureInfo->ipri + "'")
                        .arg("'" + deviceMeasureInfo->ipri_unit + "'")
                        .arg("'" + deviceMeasureInfo->isec + "'")
                        .arg("'" + deviceMeasureInfo->isec_unit + "'")
                        .arg("'" + deviceMeasureInfo->vpri + "'")
                        .arg("'" + deviceMeasureInfo->vpri_unit + "'")
                        .arg("'" + deviceMeasureInfo->vsec + "'")
                        .arg("'" + deviceMeasureInfo->vsec_unit + "'")
                        .arg("'" + deviceMeasureInfo->appearance_function_check + "'")
                        .arg(deviceMeasureInfo->appearance == ""
                                ? 1
                                : deviceMeasureInfo->appearance.toInt())
                        .arg("'" + deviceMeasureInfo->appearance_remark + "'")
                        .arg(deviceMeasureInfo->pointer_deflection_stability == ""
                                ? 1
                                : deviceMeasureInfo->pointer_deflection_stability.toInt())
                        .arg("'" + deviceMeasureInfo->pointer_deflection_stability_remark + "'")
                        .arg(deviceMeasureInfo->tightness == ""
                                ? 1
                                : deviceMeasureInfo->tightness.toInt())
                        .arg("'" + deviceMeasureInfo->tightness_remark + "'")
                        .arg(deviceMeasureInfo->insulation_resistance == ""
                                ? 1
                                : deviceMeasureInfo->insulation_resistance.toInt())
                        .arg("'" + deviceMeasureInfo->insulation_resistance_remark + "'")
                        .arg("'" + deviceMeasureInfo->measure_medium + "'")
                        .arg(deviceMeasureInfo->division.toDouble())
                        .arg("'" + deviceMeasureInfo->zero_drift_result + "'")
                        .arg("'" + deviceMeasureInfo->work_line_equation + "'")
                        .arg(deviceMeasureInfo->sealing_test == ""
                                ? 1
                                : deviceMeasureInfo->sealing_test.toInt())
                        .arg("'" + deviceMeasureInfo->insulation_resistance_value + "'")
                        .arg(deviceMeasureInfo->angle_adjust_error == ""
                                ? 1
                                : deviceMeasureInfo->angle_adjust_error.toInt())
                        .arg("'" + deviceMeasureInfo->actual_division_value + "'")
                        .arg("'" + deviceMeasureInfo->actual_division_value_unit + "'")
                        .arg("'" + deviceMeasureInfo->conclusion + "'")
                        .arg("'" + deviceMeasureInfo->restriction_instruction + "'")
                        .arg("'" + deviceMeasureInfo->outcome + "'")
                        .arg("'" + deviceMeasureInfo->other_outcome + "'")
                        .arg("'" + deviceMeasureInfo->remark + "'")
                        .arg("'" + deviceMeasureInfo->other_remark + "'")
                        .arg("'" + deviceMeasureInfo->create_user_id + "'")
                        .arg("'" + deviceMeasureInfo->measure_person_id + "'")
                        .arg("'" + deviceMeasureInfo->create_user_name + "'")
                        .arg("'" + deviceMeasureInfo->meter_identify + "'")
                        .arg("'" + currentDateTime + "'")
                        .arg("'" + currentDateTime + "'");
    }
    if (deviceMeasureEquipment != nullptr) {
        resultsql = QString(ssql)
                        .arg("'" + deviceMeasureEquipment->id + "'")
                        .arg("'" + deviceMeasureEquipment->data_id + "'")
                        .arg("'" + deviceMeasureEquipment->equipment_id + "'")
                        .arg("'" + deviceMeasureEquipment->standard_id + "'")
                        .arg("'" + deviceMeasureEquipment->standard_name + "'")
                        .arg("'" + currentDateTime + "'")
                        .arg("'" + currentDateTime + "'");
    }
    if (deviceMeasureCalibrator != nullptr) {
        resultsql = QString(ssql)
                        .arg("'" + deviceMeasureCalibrator->id + "'")
                        .arg("'" + deviceMeasureCalibrator->data_id + "'")
                        .arg("'" + deviceMeasureCalibrator->item_data_id + "'")
                        .arg("'" + deviceMeasureCalibrator->maximum_error_formula + "'")
                        .arg("'" + deviceMeasureCalibrator->error_param_a + "'")
                        .arg("'" + deviceMeasureCalibrator->error_param_b + "'")
                        .arg("'" + deviceMeasureCalibrator->params + "'")
                        .arg("'" + deviceMeasureCalibrator->capacity + "'")
                        .arg("'" + deviceMeasureCalibrator->unit + "'")
                        .arg("'" + deviceMeasureCalibrator->frequency + "'")
                        .arg("'" + deviceMeasureCalibrator->frequency_unit + "'")
                        .arg("'" + deviceMeasureCalibrator->range_range + "'")
                        .arg("'" + deviceMeasureCalibrator->standard_value + "'")
                        .arg("'" + deviceMeasureCalibrator->indicating_value + "'")
                        .arg("'" + deviceMeasureCalibrator->maximum_error + "'")
                        .arg("'" + deviceMeasureCalibrator->absolute_error + "'")
                        .arg("'" + deviceMeasureCalibrator->resolution + "'")
                        .arg(deviceMeasureCalibrator->line_mode == ""
                                ? "'" + chart + "'"
                                : "'" + deviceMeasureCalibrator->line_mode + "'")
                        .arg("'" + deviceMeasureCalibrator->remark + "'")
                        .arg("'" + deviceMeasureCalibrator->an + "'")
                        .arg("'" + deviceMeasureCalibrator->bn + "'")
                        .arg("'" + deviceMeasureCalibrator->cn + "'")
                        .arg("'" + deviceMeasureCalibrator->delta_an + "'")
                        .arg("'" + deviceMeasureCalibrator->delta_bn + "'")
                        .arg("'" + deviceMeasureCalibrator->delta_cn + "'")
                        .arg("'" + deviceMeasureCalibrator->measure_indication_value + "'")
                        .arg("'" + deviceMeasureCalibrator->standard_value_upper_limit + "'")
                        .arg("'" + deviceMeasureCalibrator->accuracy_level + "'")
                        .arg("'" + currentDateTime + "'")
                        .arg("'" + currentDateTime + "'");
        //        qDebug()<<">>"<<resultsql;
    }
    if (deviceModifyRecord != nullptr) {
        resultsql = QString(ssql)
                        .arg("'" + deviceModifyRecord->id + "'")
                        .arg("'" + deviceModifyRecord->data_id + "'")
                        .arg("'" + deviceModifyRecord->source_system + "'")
                        .arg("'" + deviceModifyRecord->modify_content + "'")
                        .arg("'" + deviceModifyRecord->create_user_id + "'")
                        .arg("'" + deviceModifyRecord->create_user_name + "'")
                        .arg("'" + currentDateTime + "'")
                        .arg("'" + currentDateTime + "'");
    }
    if (devicePistonGaugeData != nullptr) {
        resultsql = QString(ssql)
                        .arg("'" + devicePistonGaugeData->id + "'")
                        .arg("'" + devicePistonGaugeData->data_id + "'")
                        .arg("'" + devicePistonGaugeData->data_type + "'")
                        .arg("'" + devicePistonGaugeData->data_category + "'")
                        .arg("'" + devicePistonGaugeData->indicating_value + "'")
                        .arg("'" + devicePistonGaugeData->before_friction_forward_stroke + "'")
                        .arg("'" + devicePistonGaugeData->before_friction_reverse_stroke + "'")
                        .arg("'" + devicePistonGaugeData->after_friction_forward_stroke + "'")
                        .arg("'" + devicePistonGaugeData->after_friction_reverse_stroke + "'")
                        .arg("'" + devicePistonGaugeData->variation_forward_stroke + "'")
                        .arg("'" + devicePistonGaugeData->variation_reverse_stroke + "'")
                        .arg(devicePistonGaugeData->theoretical_output_value == ""
                                ? "NULL"
                                : "'" + devicePistonGaugeData->theoretical_output_value + "'")
                        .arg("'" + devicePistonGaugeData->first_forward_stroke + "'")
                        .arg("'" + devicePistonGaugeData->first_reverse_stroke + "'")
                        .arg("'" + devicePistonGaugeData->second_forward_stroke + "'")
                        .arg("'" + devicePistonGaugeData->second_reverse_stroke + "'")
                        .arg("'" + devicePistonGaugeData->third_forward_stroke + "'")
                        .arg("'" + devicePistonGaugeData->third_reverse_stroke + "'")
                        .arg("'" + devicePistonGaugeData->indicating_error + "'")
                        .arg("'" + devicePistonGaugeData->return_error + "'")
                        .arg("'" + devicePistonGaugeData->average_forward_stroke + "'")
                        .arg("'" + devicePistonGaugeData->average_reverse_stroke + "'")
                        .arg("'" + devicePistonGaugeData->average_value + "'")
                        .arg("'" + devicePistonGaugeData->average_return_error + "'")
                        .arg("'" + devicePistonGaugeData->item + "'")
                        .arg("'" + devicePistonGaugeData->max_indicating_error + "'")
                        .arg("'" + devicePistonGaugeData->max_return_error + "'")
                        .arg("'" + devicePistonGaugeData->max_friction_error + "'")
                        .arg("'" + devicePistonGaugeData->maximum_error_absolute + "'")
                        .arg("'" + devicePistonGaugeData->forward_stroke_switch_value + "'")
                        .arg("'" + devicePistonGaugeData->reverse_stroke_switch_value + "'")
                        .arg("'" + devicePistonGaugeData->point_deviation + "'")
                        .arg("'" + devicePistonGaugeData->different_gap + "'")
                        .arg("'" + devicePistonGaugeData->max_point_deviation + "'")
                        .arg("'" + devicePistonGaugeData->allow_point_deviation + "'")
                        .arg("'" + devicePistonGaugeData->max_different_gap + "'")
                        .arg("'" + devicePistonGaugeData->allow_different_gap + "'")
                        .arg("'" + devicePistonGaugeData->zero_drift_one + "'")
                        .arg("'" + devicePistonGaugeData->zero_drift_two + "'")
                        .arg("'" + devicePistonGaugeData->zero_drift_three + "'")
                        .arg("'" + devicePistonGaugeData->zero_drift_four + "'")
                        .arg("'" + devicePistonGaugeData->zero_drift_five + "'")
                        .arg("'" + devicePistonGaugeData->intercept_a + "'")
                        .arg("'" + devicePistonGaugeData->slope_b + "'")
                        .arg("'" + devicePistonGaugeData->sensitivity_b + "'")
                        .arg("'" + devicePistonGaugeData->repeatability_r + "'")
                        .arg("'" + devicePistonGaugeData->hysteresis_h + "'")
                        .arg("'" + devicePistonGaugeData->linear_l + "'")
                        .arg("'" + devicePistonGaugeData->basic_error_a + "'")
                        .arg("'" + devicePistonGaugeData->periodic_stability_s_b + "'")
                        .arg("'" + currentDateTime + "'")
                        .arg("'" + currentDateTime + "'");
        //       qDebug()<<resultsql;
    }
    if (deviceDCPowerData != nullptr) {
        resultsql = QString(ssql)
                        .arg("'" + deviceDCPowerData->id + "'")
                        .arg("'" + deviceDCPowerData->data_id + "'")
                        .arg("'" + deviceDCPowerData->data_type + "'")
                        .arg("'" + deviceDCPowerData->data_type_type + "'")
                        .arg("'" + deviceDCPowerData->output_channel + "'")
                        .arg("'" + deviceDCPowerData->unit + "'")
                        .arg("'" + deviceDCPowerData->voltage_regulator_output_value + "'")
                        .arg("'" + deviceDCPowerData->voltage_max_difference + "'")
                        .arg("'" + deviceDCPowerData->technical_index + "'")
                        .arg("'" + deviceDCPowerData->technical_index_symbol + "'")
                        .arg("'" + deviceDCPowerData->voltage_regulation + "'")
                        .arg("'" + deviceDCPowerData->load_situation + "'")
                        .arg("'" + deviceDCPowerData->voltage_output_value + "'")
                        .arg("'" + deviceDCPowerData->voltage_difference + "'")
                        .arg("'" + deviceDCPowerData->load_regulation + "'")
                        .arg("'" + deviceDCPowerData->measure_indication_value + "'")
                        .arg("'" + deviceDCPowerData->standard_value + "'")
                        .arg("'" + deviceDCPowerData->maximum_error + "'")
                        .arg("'" + deviceDCPowerData->maximum_error_unit + "'")
                        .arg("'" + deviceDCPowerData->absolute_error + "'")
                        .arg(deviceDCPowerData->resolution.toDouble())
                        .arg("'" + deviceDCPowerData->standard_voltage_value + "'")
                        .arg("'" + deviceDCPowerData->standard_voltage_value_unit + "'")
                        .arg("'" + deviceDCPowerData->standard_resistance_value + "'")
                        .arg("'" + deviceDCPowerData->standard_resistance_value_unit + "'")
                        .arg("'" + deviceDCPowerData->ripple_voltage + "'")
                        .arg("'" + deviceDCPowerData->output_voltage_max_change + "'")
                        .arg("'" + deviceDCPowerData->short_term_stability + "'")
                        .arg(deviceDCPowerData->exceed_mark.toInt())
                        .arg("'" + currentDateTime + "'")
                        .arg("'" + currentDateTime + "'");
    }
    if (deviceOscilloscopeData != nullptr) {
        resultsql = QString(ssql)
                        .arg("'" + deviceOscilloscopeData->id + "'")
                        .arg("'" + deviceOscilloscopeData->data_id + "'")
                        .arg("'" + deviceOscilloscopeData->data_type + "'")
                        .arg("'" + deviceOscilloscopeData->cycle_signal + "'")
                        .arg("'" + deviceOscilloscopeData->cycle_signal_unit + "'")
                        .arg("'" + deviceOscilloscopeData->time_base_absolute_error + "'")
                        .arg("'" + deviceOscilloscopeData->time_base_absolute_error_unit + "'")
                        .arg("'" + deviceOscilloscopeData->time_base_relative_error + "'")
                        .arg("'" + deviceOscilloscopeData->time_base_relative_error_unit + "'")
                        .arg("'" + deviceOscilloscopeData->technical_index_symbol + "'")
                        .arg("'" + deviceOscilloscopeData->technical_index + "'")
                        .arg("'" + deviceOscilloscopeData->technical_index_unit + "'")
                        .arg("'" + deviceOscilloscopeData->conclusion + "'")
                        .arg("'" + deviceOscilloscopeData->thoroughfare + "'")
                        .arg("'" + deviceOscilloscopeData->impedance + "'")
                        .arg("'" + deviceOscilloscopeData->impedance_unit + "'")
                        .arg("'" + deviceOscilloscopeData->vertical_deflect_coefficient + "'")
                        .arg("'" + deviceOscilloscopeData->vertical_deflect_coefficient_unit + "'")
                        .arg("'" + deviceOscilloscopeData->standard_value + "'")
                        .arg("'" + deviceOscilloscopeData->standard_value_unit + "'")
                        .arg("'" + deviceOscilloscopeData->measure_value + "'")
                        .arg("'" + deviceOscilloscopeData->measure_value_unit + "'")
                        .arg("'" + deviceOscilloscopeData->relative_error + "'")
                        .arg("'" + deviceOscilloscopeData->error_value + "'")
                        .arg("'" + deviceOscilloscopeData->band_width + "'")
                        .arg("'" + deviceOscilloscopeData->band_width_unit + "'")
                        .arg("'" + deviceOscilloscopeData->reference_amplitude + "'")
                        .arg("'" + deviceOscilloscopeData->reference_amplitude_unit + "'")
                        .arg("'" + deviceOscilloscopeData->measure_amplitude + "'")
                        .arg("'" + deviceOscilloscopeData->measure_amplitude_unit + "'")
                        .arg("'" + deviceOscilloscopeData->decibel_number + "'")
                        .arg("'" + deviceOscilloscopeData->trigger_type + "'")
                        .arg("'" + deviceOscilloscopeData->signal_frequency + "'")
                        .arg("'" + deviceOscilloscopeData->signal_frequency_unit + "'")
                        .arg("'" + deviceOscilloscopeData->item_type + "'")
                        .arg("'" + deviceOscilloscopeData->nominal_value + "'")
                        .arg("'" + deviceOscilloscopeData->nominal_value_unit + "'")
                        .arg("'" + deviceOscilloscopeData->top_measure_value + "'")
                        .arg("'" + deviceOscilloscopeData->top_measure_value_unit + "'")
                        .arg("'" + deviceOscilloscopeData->bottom_measure_value + "'")
                        .arg("'" + deviceOscilloscopeData->bottom_measure_value_unit + "'")
                        .arg("'" + deviceOscilloscopeData->scan_type + "'")
                        .arg("'" + deviceOscilloscopeData->horizontal_scale_gear + "'")
                        .arg("'" + deviceOscilloscopeData->horizontal_scale_gear_unit + "'")
                        .arg("'" + deviceOscilloscopeData->vertical_scale_gear + "'")
                        .arg("'" + deviceOscilloscopeData->vertical_scale_gear_unit + "'")
                        .arg("'" + deviceOscilloscopeData->frequency_upper + "'")
                        .arg("'" + deviceOscilloscopeData->frequency_upper_unit + "'")
                        .arg("'" + deviceOscilloscopeData->output_signal_amplitude + "'")
                        .arg("'" + deviceOscilloscopeData->rise_time + "'")
                        .arg("'" + deviceOscilloscopeData->correct_rise_time + "'")
                        .arg("'" + deviceOscilloscopeData->signal_type + "'")
                        .arg("'" + deviceOscilloscopeData->nominal_amplitude_value + "'")
                        .arg("'" + deviceOscilloscopeData->nominal_amplitude_value_unit + "'")
                        .arg("'" + deviceOscilloscopeData->nominal_frequency_value + "'")
                        .arg("'" + deviceOscilloscopeData->nominal_frequency_value_unit + "'")
                        .arg("'" + deviceOscilloscopeData->measure_amplitude_value + "'")
                        .arg("'" + deviceOscilloscopeData->measure_amplitude_value_unit + "'")
                        .arg("'" + deviceOscilloscopeData->measure_frequency_value + "'")
                        .arg("'" + deviceOscilloscopeData->frequency_deviation + "'")
                        .arg("'" + deviceOscilloscopeData->amplitude_deviation + "'")
                        .arg("'" + deviceOscilloscopeData->exceed_mark + "'")
                        .arg("'" + currentDateTime + "'")
                        .arg("'" + currentDateTime + "'");
    }
    if (deviceResistanceThermometerData != nullptr) {
        resultsql = QString(ssql)
                        .arg("'" + deviceResistanceThermometerData->id + "'")
                        .arg("'" + deviceResistanceThermometerData->data_id + "'")
                        .arg("'" + deviceResistanceThermometerData->data_type + "'")
                        .arg("'" + deviceResistanceThermometerData->data_category + "'")
                        .arg("'" + deviceResistanceThermometerData->upper_lower_point + "'")
                        .arg("'" + deviceResistanceThermometerData->location + "'")
                        .arg("'" + deviceResistanceThermometerData->unit + "'")
                        .arg("'" + deviceResistanceThermometerData->one_value + "'")
                        .arg("'" + deviceResistanceThermometerData->two_value + "'")
                        .arg("'" + deviceResistanceThermometerData->three_value + "'")
                        .arg("'" + deviceResistanceThermometerData->four_value + "'")
                        .arg("'" + deviceResistanceThermometerData->five_value + "'")
                        .arg("'" + deviceResistanceThermometerData->six_value + "'")
                        .arg("'" + deviceResistanceThermometerData->seven_value + "'")
                        .arg("'" + deviceResistanceThermometerData->eight_value + "'")
                        .arg("'" + deviceResistanceThermometerData->nine_value + "'")
                        .arg("'" + deviceResistanceThermometerData->ten_value + "'")
                        .arg("'" + deviceResistanceThermometerData->eleven_value + "'")
                        .arg("'" + deviceResistanceThermometerData->twelve_value + "'")
                        .arg("'" + deviceResistanceThermometerData->thirteen_value + "'")
                        .arg("'" + deviceResistanceThermometerData->fourteen_value + "'")
                        .arg("'" + deviceResistanceThermometerData->fifteen_value + "'")
                        .arg("'" + deviceResistanceThermometerData->max_value + "'")
                        .arg("'" + deviceResistanceThermometerData->min_value + "'")
                        .arg("'" + deviceResistanceThermometerData->volatility + "'")
                        .arg("'" + deviceResistanceThermometerData->upper_temperature_different + "'")
                        .arg("'" + deviceResistanceThermometerData->lower_temperature_different + "'")
                        .arg("'" + deviceResistanceThermometerData->max_horizontal_temperature_different + "'")
                        .arg("'" + deviceResistanceThermometerData->max_work_temperature_different + "'")
                        .arg("'" + deviceResistanceThermometerData->calibrated_point + "'")
                        .arg("'" + deviceResistanceThermometerData->indicating_value_forward_stroke + "'")
                        .arg("'" + deviceResistanceThermometerData->indicating_value_reverse_stroke + "'")
                        .arg("'" + deviceResistanceThermometerData->measure_indication_value_forward_stroke + "'")
                        .arg("'" + deviceResistanceThermometerData->measure_indication_value_reverse_stroke + "'")
                        .arg("'" + deviceResistanceThermometerData->indicating_error_forward_stroke + "'")
                        .arg("'" + deviceResistanceThermometerData->indicating_error_reverse_stroke + "'")
                        .arg("'" + deviceResistanceThermometerData->maximum_error + "'")
                        .arg("'" + deviceResistanceThermometerData->urel + "'")
                        .arg("'" + deviceResistanceThermometerData->rank_level + "'")
                        .arg("'" + deviceResistanceThermometerData->nominal_temperature + "'")
                        .arg("'" + deviceResistanceThermometerData->item + "'")
                        .arg("'" + deviceResistanceThermometerData->allow_error_lower + "'")
                        .arg("'" + deviceResistanceThermometerData->allow_error_upper + "'")
                        .arg("'" + deviceResistanceThermometerData->measure_value + "'")
                        .arg("'" + currentDateTime + "'")
                        .arg("'" + currentDateTime + "'");
    }
    if (deviceSpectrumAnalyzerData != nullptr) {
        resultsql = QString(ssql)
                        .arg("'" + deviceSpectrumAnalyzerData->id + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->data_id + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->data_type + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->nominal_value + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->unit + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->sweep_width + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->sweep_width_unit + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->horizontal_resolution + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->horizontal_resolution_unit + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->resolution_bandwidth + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->resolution_bandwidth_unit + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->one_value + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->two_value + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->three_value + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->four_value + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->five_value + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->six_value + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->seven_value + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->eight_value + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->nine_value + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->ten_value + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->upper_index_twice + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->upper_index_twice_unit + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->upper_index_thrice + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->upper_index_thrice_unit + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->upper_index_quartic + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->upper_index_quartic_unit + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->lower_index + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->lower_index_unit + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->average_value + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->upper_index + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->upper_index_unit + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->conclusion + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->convert_deviation + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->lower_convert_deviation + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->upper_convert_deviation + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->standard_attenuator_attenuation + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->standard_attenuator_attenuation_unit + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->signal_source_amplitude + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->signal_source_amplitude_unit + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->vertical_scale + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->frequency + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->frequency_unit + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->input_frequency + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->start_frequency + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->start_frequency_unit + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->end_frequency + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->end_frequency_unit + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->revised_result + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->input_attenuator + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->input_attenuator_unit + "'")
                        .arg(
                            "'" + deviceSpectrumAnalyzerData->input_attenuator_conversion_effect + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->spectrum_level + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->spectrum_level_unit + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->gain_compression + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->exceed_mark + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->exceed_mark_one + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->exceed_mark_two + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->measure_result + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->measure_result_unit + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->technical_index + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->technical_index_left + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->technical_index_right + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->corrected_value + "'")
                        .arg("'" + deviceSpectrumAnalyzerData->type_value + "'")
                        .arg("'" + currentDateTime + "'")
                        .arg("'" + currentDateTime + "'");
    }
    if (deviceSignalGeneratorData != nullptr) {
        resultsql = QString(ssql)
                        .arg("'" + deviceSignalGeneratorData->id + "'")
                        .arg("'" + deviceSignalGeneratorData->data_id + "'")
                        .arg("'" + deviceSignalGeneratorData->data_type + "'")
                        .arg("'" + deviceSignalGeneratorData->nominal_value + "'")
                        .arg("'" + deviceSignalGeneratorData->unit + "'")
                        .arg("'" + deviceSignalGeneratorData->upper_index + "'")
                        .arg("'" + deviceSignalGeneratorData->upper_thd + "'")
                        .arg("'" + deviceSignalGeneratorData->lower_index + "'")
                        .arg("'" + deviceSignalGeneratorData->measure_value + "'")
                        .arg("'" + deviceSignalGeneratorData->measure_value_unit + "'")
                        .arg("'" + deviceSignalGeneratorData->relative_error + "'")
                        .arg("'" + deviceSignalGeneratorData->urel + "'")
                        .arg("'" + deviceSignalGeneratorData->frequency + "'")
                        .arg("'" + deviceSignalGeneratorData->frequency_unit + "'")
                        .arg("'" + deviceSignalGeneratorData->power + "'")
                        .arg("'" + deviceSignalGeneratorData->power_unit + "'")
                        .arg("'" + deviceSignalGeneratorData->amplitude + "'")
                        .arg("'" + deviceSignalGeneratorData->amplitude_unit + "'")
                        .arg("'" + deviceSignalGeneratorData->high_pass_filter + "'")
                        .arg("'" + deviceSignalGeneratorData->high_pass_filter_unit + "'")
                        .arg("'" + deviceSignalGeneratorData->lower_pass_filter + "'")
                        .arg("'" + deviceSignalGeneratorData->lower_pass_filter_unit + "'")
                        .arg("'" + deviceSignalGeneratorData->modulation_rate + "'")
                        .arg("'" + deviceSignalGeneratorData->modulation_rate_unit + "'")
                        .arg("'" + deviceSignalGeneratorData->thd + "'")
                        .arg("'" + deviceSignalGeneratorData->upper_index_symbol + "'")
                        .arg("'" + deviceSignalGeneratorData->upper_index_unit + "'")
                        .arg("'" + deviceSignalGeneratorData->remain_amplitude_modulation + "'")
                        .arg("'" + deviceSignalGeneratorData->harmonic_number + "'")
                        .arg("'" + deviceSignalGeneratorData->fundamental_amplitude + "'")
                        .arg("'" + deviceSignalGeneratorData->fundamental_amplitude_unit + "'")
                        .arg("'" + deviceSignalGeneratorData->harmonic_amplitude + "'")
                        .arg("'" + deviceSignalGeneratorData->harmonic_amplitude_unit + "'")
                        .arg("'" + deviceSignalGeneratorData->offset_frequency + "'")
                        .arg("'" + deviceSignalGeneratorData->offset_frequency_unit + "'")
                        .arg("'" + deviceSignalGeneratorData->exceed_mark + "'")
                        .arg("'" + deviceSignalGeneratorData->exceed_mark_one + "'")
                        .arg("'" + currentDateTime + "'")
                        .arg("'" + currentDateTime + "'");
    }
    sync.ssql = resultsql;
    sync.id = QString::number(BaseCommonApi().generateId());
    sync.sync_type = "0";
    bool isSuccess = BaseCommonApi::InsertzdDataSync(sync);
}
// 自动检定-结果保存-检定数据管理多功能校准源数据(自动检定输出或业务创建)
bool BaseCommonApi::InsertVerMultiCal(
    SaveDeviceMeasureCalibrator measureCalibrator)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_business_device_measure_data_calibrator "
        " (id,data_id,item_data_id,maximum_error_formula,error_param_a,error_"
        "param_b,params,capacity,unit,frequency,frequency_unit,"
        "range_range,standard_value,indicating_value,maximum_error,absolute_"
        "error,resolution,line_mode,remark,an,bn,cn,delta_an,"
        "delta_bn,delta_cn,measure_indication_value,standard_value_upper_limit,"
        "accuracy_level,create_time,update_time) "
        " VALUES "
        " (:id,:data_id,:item_data_id,:maximum_error_formula,:error_param_a,:"
        "error_param_b,:params,:capacity,:unit,:frequency,:frequency_unit,:range_"
        "range,:standard_value,:indicating_value,:maximum_error,:absolute_error,:"
        "resolution,:line_mode,:remark,:an,:bn,:cn,:delta_an,:delta_bn,:delta_cn,"
        ":measure_indication_value,:standard_value_upper_limit,:accuracy_level,:"
        "create_time,:update_time);");

    //       QString formattedUpdateDateTime =
    //       measureCalibrator.create_time.toString("yyyy-MM-dd HH:mm:ss");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", measureCalibrator.id);
    query.bindValue(":data_id", measureCalibrator.data_id);
    query.bindValue(":item_data_id", measureCalibrator.item_data_id);
    query.bindValue(":maximum_error_formula",
        measureCalibrator.maximum_error_formula);
    query.bindValue(":error_param_a", measureCalibrator.error_param_a);
    query.bindValue(":error_param_b", measureCalibrator.error_param_b);
    query.bindValue(":params", measureCalibrator.params);
    query.bindValue(":capacity", measureCalibrator.capacity);
    query.bindValue(":unit", measureCalibrator.unit);
    query.bindValue(":frequency", measureCalibrator.frequency);
    query.bindValue(":frequency_unit", measureCalibrator.frequency_unit);
    query.bindValue(":range_range", measureCalibrator.range_range);
    query.bindValue(":standard_value", measureCalibrator.standard_value);
    query.bindValue(
        ":indicating_value",
        measureCalibrator
            .measure_indication_value); // 暂时改为被检表示值2024年3月27日09:43:27
                                        // 待修改
    query.bindValue(":maximum_error", measureCalibrator.maximum_error);
    query.bindValue(":absolute_error", measureCalibrator.absolute_error);
    query.bindValue(":resolution", measureCalibrator.resolution);
    query.bindValue(":line_mode", measureCalibrator.line_mode);
    query.bindValue(":remark", measureCalibrator.remark);
    query.bindValue(":an", measureCalibrator.an);
    query.bindValue(":bn", measureCalibrator.bn);
    query.bindValue(":cn", measureCalibrator.cn);
    query.bindValue(":delta_an", measureCalibrator.delta_an);
    query.bindValue(":delta_bn", measureCalibrator.delta_bn);
    query.bindValue(":delta_cn", measureCalibrator.delta_cn);
    query.bindValue(":measure_indication_value",
        measureCalibrator.measure_indication_value);
    query.bindValue(":standard_value_upper_limit",
        measureCalibrator.standard_value_upper_limit);
    query.bindValue(":accuracy_level", measureCalibrator.accuracy_level);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    qDebug() << "保存的示值为：" << measureCalibrator.indicating_value;
    if (isSuccess) {
        QString sql = "INSERT INTO biz_business_device_measure_data_calibrator "
                      " (id,data_id,item_data_id,maximum_error_formula,error_param_a,error_"
                      "param_b,params,capacity,unit,frequency,frequency_unit,"
                      "range_range,standard_value,indicating_value,maximum_error,absolute_"
                      "error,resolution,line_mode,remark,an,bn,cn,delta_an,"
                      "delta_bn,delta_cn,measure_indication_value,standard_value_upper_limit,"
                      "accuracy_level,create_time,update_time) "
                      " VALUES "
                      " (:1,:2,:3,:4,:5,:6,:7,:8,:9,:10,:11,:12,:13,:14,:15,:16,:17,:18,:19,:"
                      "20,:21,:22,:23,:24,:25,:26,:27,:28,:29,:30);";
        saveLogToSql(sql, &measureCalibrator);
    }
    return isSuccess;
};
// 核查程序管理首页查询
QList<CheckProgramManage> BaseCommonApi::getCheckProgramManage()
{
    QList<CheckProgramManage> result;
    QSqlQuery query;
    bool isSuccess = query.exec(
        "SELECT id,check_name,belong_standard_equipment_name,create_name,remark "
        "FROM zd_verification_program_manage");
    if (isSuccess) {
        while (query.next()) {
            CheckProgramManage programInfo;
            programInfo.id = query.value("id").toString();
            programInfo.check_name = query.value("check_name").toString();
            programInfo.belong_standard_equipment_name = query.value("belong_standard_equipment_name").toString();
            programInfo.create_name = query.value("create_name").toString();
            //            programInfo.create_time =
            //            query.value("create_time").toDateTime();
            programInfo.remark = query.value("remark").toString();
            result.append(programInfo);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
};

// 核查程序管理-编辑查询单个 参数核查程序id
CheckProgramManage BaseCommonApi::getCheckProgramManageById(QString id)
{
    CheckProgramManage result;
    QSqlQuery query;
    query.prepare(
        "SELECT "
        "id,check_name,belong_standard_equipment_name,category_name,create_name,"
        "create_time,remark,img_path,"
        " standard_device_id,verification_documents_id,belong_standard_equipment,"
        "equipment_type,standard_id FROM zd_verification_program_manage "
        "WHERE id = :id ");
    query.bindValue(":id", id);
    if (query.exec()) {
        while (query.next()) {
            CheckProgramManage programInfo;
            programInfo.id = query.value("id").toString();
            programInfo.check_name = query.value("check_name").toString();
            programInfo.belong_standard_equipment_name = query.value("belong_standard_equipment_name").toString();
            programInfo.category_name = query.value("category_name").toString();
            programInfo.create_name = query.value("create_name").toString();
            programInfo.create_time = query.value("create_time").toDateTime();
            programInfo.remark = query.value("remark").toString();
            programInfo.img_path = query.value("img_path").toString();
            programInfo.standard_device_id = query.value("standard_device_id").toString();
            programInfo.verification_documents_id = query.value("verification_documents_id").toString();
            programInfo.belong_standard_equipment = query.value("belong_standard_equipment").toString();
            programInfo.equipment_type = query.value("equipment_type").toString();
            programInfo.standard_id = query.value("standard_id").toString();
            //            programInfo.update_time =
            //            query.value("update_time").toDateTime();
            result = programInfo;
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
};

// 核查程序管理-编辑查询 选择设备 选择标准
QList<CheckAndTestedDevice> BaseCommonApi::getCheckDeviceList()
{
    QList<CheckAndTestedDevice> result;
    QSqlQuery query;
    bool isSuccess = query.exec(
        "SELECT bei.id ,bei.equipment_name ,bei.model ,bei.manufacture_no "
        ",bei.manufacturer FROM biz_equipment_info bei");
    if (isSuccess) {
        while (query.next()) {
            CheckAndTestedDevice device;
            device.id = query.value("id").toString();
            device.equipment_name = query.value("equipment_name").toString();
            device.model = query.value("model").toString();
            device.manufacture_no = query.value("manufacture_no").toString();
            device.manufacturer = query.value("manufacturer").toString();
            result.append(device);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
};
////核查程序管理-编辑查询 选择设备 选择核查件 为标准和被检的并集
// QList<CheckAndTestedDevice>
// BaseCommonApi::getCheckAndTestedDeviceList(QString standardId)
//{
//     QList<CheckAndTestedDevice> result;
//     QSqlQuery query;
//     bool isSuccess = query.exec("SELECT   "
//                                 " bei.id ,  bei.equipment_name ,   bei.model
//                                 , bei.manufacture_no , bei.manufacturer "
//                             " FROM    biz_equipment_info bei   UNION ALL
//                             SELECT   "
//                               "  eei.id ,   eei.equipment_name ,   eei.model
//                               ,   eei.manufacture_no ,  eei.manufacturer  "
//                             " FROM     eqpt_equipment_info eei   LEFT JOIN
//                             sys_dept ON sys_dept.ID = eei.dept_id;");
//     if (isSuccess) {
//         while (query.next()) {
//             CheckAndTestedDevice device;
//             device.id = query.value("id").toString();
//             device.equipment_name = query.value("equipment_name").toString();
//             device.model = query.value("model").toString();
//             device.manufacture_no = query.value("manufacture_no").toString();
//             device.manufacturer = query.value("manufacturer").toString();
//             result.append(device);
//         }
//     } else {
//         qDebug() << "Query execution failed: " << query.lastError().text();
//     }
//    return result;
// };

// 新增核查程序
bool BaseCommonApi::InsertCheckProgramManage(CheckProgramManage check)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO zd_verification_program_manage "
        "(id,check_name,create_name,remark,img_path,standard_device_id,"
        " verification_documents_id,belong_standard_equipment_name,belong_"
        "standard_equipment,category_name,equipment_type,standard_id,"
        " create_time,update_time) "
        " VALUES "
        "(:id,:check_name,:create_name,:remark,:img_path,:standard_device_id,:"
        "verification_documents_id,"
        ":belong_standard_equipment_name,:belong_standard_equipment,:category_"
        "name,:equipment_type,:standard_id,:create_time,:update_time)");
    QString formattedDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", check.id);
    query.bindValue(":check_name", check.check_name);
    query.bindValue(":create_name", check.create_name);
    query.bindValue(":remark", check.remark);
    query.bindValue(":img_path", check.img_path);
    query.bindValue(":standard_device_id", check.standard_device_id);
    query.bindValue(":verification_documents_id",
        check.verification_documents_id);
    query.bindValue(":belong_standard_equipment_name",
        check.belong_standard_equipment_name);
    query.bindValue(":belong_standard_equipment",
        check.belong_standard_equipment);
    query.bindValue(":category_name", check.category_name);
    query.bindValue(":equipment_type", check.equipment_type);
    query.bindValue(":standard_id", check.standard_id);
    query.bindValue(":create_time", formattedDateTime);
    query.bindValue(":update_time", formattedDateTime);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << "InsertTaskBasic query failed: " << query.lastError().text();
    }

    return isSuccess;
};

// 更新核查程序
bool BaseCommonApi::UpdateCheckProgramManage(CheckProgramManage check)
{
    QSqlQuery query;
    query.prepare(
        " UPDATE zd_verification_program_manage "
        " SET check_name =:check_name, create_name = :create_name, remark = "
        ":remark, img_path =:img_path, "
        " standard_device_id =:standard_device_id,"
        " verification_documents_id=:verification_documents_id,"
        " belong_standard_equipment_name=:belong_standard_equipment_name,"
        " belong_standard_equipment=:belong_standard_equipment,"
        " category_name=:category_name,"
        " equipment_type=:equipment_type,"
        " standard_id = :standard_id,"
        " update_time=:update_time"
        " WHERE  id = :id;");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":check_name", check.check_name);
    query.bindValue(":create_name", check.create_name);
    query.bindValue(":remark", check.remark);
    query.bindValue(":img_path", check.img_path);
    query.bindValue(":standard_device_id", check.standard_device_id);
    query.bindValue(":verification_documents_id",
        check.verification_documents_id);
    query.bindValue(":belong_standard_equipment_name",
        check.belong_standard_equipment_name);
    query.bindValue(":belong_standard_equipment",
        check.belong_standard_equipment);
    query.bindValue(":category_name", check.category_name);
    query.bindValue(":equipment_type", check.equipment_type);
    query.bindValue(":standard_id", check.standard_id);
    query.bindValue(":update_time", currentDateTime);
    query.bindValue(":id", check.id);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << "InsertOrderSampleRelation query failed: "
                 << query.lastError().text();
    }
    return isSuccess;
};
// 删除核查程序
bool BaseCommonApi::DeleteCheckProgramManage(QString id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM zd_verification_program_manage WHERE id = :id");
    query.bindValue(":id", id);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << "InsertOrderSampleRelation query failed: "
                 << query.lastError().text();
    }
    return isSuccess;
};
////    自动核查-根据 设备id查询核查件设备信息
// CheckAndTestedDevice BaseCommonApi::getCheckAndTestedDeviceById(QString
// deviceId)
//{
//     CheckAndTestedDevice result;
//     QSqlQuery query;
//        query.prepare("SELECT   bei.id ,   bei.equipment_name , bei.model ,
//        bei.manufacture_no ,"
//                      " bei.manufacturer   FROM  biz_equipment_info bei WHERE
//                      bei.id = :deviceId " " UNION ALL " " SELECT  eei.id ,
//                      eei.equipment_name ,  eei.model, eei.manufacture_no ,
//                      eei.manufacturer  " " FROM   eqpt_equipment_info eei
//                      LEFT JOIN  sys_dept ON sys_dept.ID = eei.dept_id WHERE
//                      eei.id = :deviceId");
//          query.bindValue(":deviceId", deviceId);
//          bool isSuccess = query.exec();
//          if (isSuccess) {
//              while (query.next()) {
//                  CheckAndTestedDevice device;
//                  device.id = query.value("id").toString();
//                  device.equipment_name =
//                  query.value("equipment_name").toString(); device.model =
//                  query.value("model").toString(); device.manufacture_no =
//                  query.value("manufacture_no").toString();
//                  device.manufacturer =
//                  query.value("manufacturer").toString(); result=device;
//              }
//          } else {
//              qDebug() << "Query execution failed: " <<
//              query.lastError().text();
//          }
//         return result;
// };
// 自动核查-结果保存-设备与装置-标准装置管理-核查数据管理0.02级活塞式压力计数据表
// bool
// BaseCommonApi::InsertStandardCheckDatePistonGauge(StandardCheckDatePistonGauge
// gauge)
//{
//     QSqlQuery query;
//     query.prepare("INSERT INTO biz_equipment_standard_check_data_piston_gauge
//     "
//                   "
//                   (id,data_id,unit,check_point,first_forward_stroke,first_reverse_stroke,second_forward_stroke,second_reverse_stroke,third_forward_stroke,third_reverse_stroke,average_value,"
//                   "average_average_value,max_indicating_error,max_return_error,params,test_value_one,measure_value_one,test_value_two,measure_value_two,test_value_three,measure_value_three,"
//                   "test_value_four,measure_value_four,test_value_five,measure_value_five,test_value_six,measure_value_six,maximum_error,create_time,update_time)
//                   " " VALUES " "
//                   (:id,:data_id,:unit,:check_point,:first_forward_stroke,:first_reverse_stroke,:second_forward_stroke,:second_reverse_stroke,:third_forward_stroke,:third_reverse_stroke,:average_value,:average_average_value,:max_indicating_error,:max_return_error,:params,:test_value_one,:measure_value_one,:test_value_two,:measure_value_two,:test_value_three,:measure_value_three,:test_value_four,:measure_value_four,:test_value_five,:measure_value_five,:test_value_six,:measure_value_six,:maximum_error,:create_time,:update_time)
//                   ");
//     QString currentDateTime =
//     QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
//     query.bindValue(":id",gauge.id);
//     query.bindValue(":data_id",gauge.data_id);
//     query.bindValue(":unit",gauge.unit);
//     query.bindValue(":check_point",gauge.check_point);
//     query.bindValue(":first_forward_stroke",gauge.first_forward_stroke);
//     query.bindValue(":first_reverse_stroke",gauge.first_reverse_stroke);
//     query.bindValue(":second_forward_stroke",gauge.second_forward_stroke);
//     query.bindValue(":second_reverse_stroke",gauge.second_reverse_stroke);
//     query.bindValue(":third_forward_stroke",gauge.third_forward_stroke);
//     query.bindValue(":third_reverse_stroke",gauge.third_reverse_stroke);
//     query.bindValue(":average_value",gauge.average_value);
//     query.bindValue(":average_average_value",gauge.average_average_value);
//     query.bindValue(":max_indicating_error",gauge.max_indicating_error);
//     query.bindValue(":max_return_error",gauge.max_return_error);
//     query.bindValue(":params",gauge.params);
//     query.bindValue(":test_value_one",gauge.test_value_one);
//     query.bindValue(":measure_value_one",gauge.measure_value_one);
//     query.bindValue(":test_value_two",gauge.test_value_two);
//     query.bindValue(":measure_value_two",gauge.measure_value_two);
//     query.bindValue(":test_value_three",gauge.test_value_three);
//     query.bindValue(":measure_value_three",gauge.measure_value_three);
//     query.bindValue(":test_value_four",gauge.test_value_four);
//     query.bindValue(":measure_value_four",gauge.measure_value_four);
//     query.bindValue(":test_value_five",gauge.test_value_five);
//     query.bindValue(":measure_value_five",gauge.measure_value_five);
//     query.bindValue(":test_value_six",gauge.test_value_six);
//     query.bindValue(":measure_value_six",gauge.measure_value_six);
//     query.bindValue(":maximum_error",gauge.maximum_error);
//     query.bindValue(":create_time",currentDateTime);
//     query.bindValue(":update_time",currentDateTime);
//     bool isSuccess = query.exec();
//     if (!isSuccess) {
//         qDebug() << "InsertTaskBasic query failed: " <<
//         query.lastError().text();
//     }

//    return isSuccess;

//};
//    //自动核查-结果保存-基本信息
// bool BaseCommonApi::InsertCheckDataManageBasicInfo(CheckDataManageBasicInfo
// baseInfo)
//{
//    QSqlQuery query;
//    query.prepare("INSERT INTO biz_equipment_standard_check_info "
//                 "
//                 (id,data_no,check_date,check_address,temperature,humidity,check_accord,stability_examine,standard_equipment_id"
//                  ",equipment_id,item_category_id,conclusion,remark,create_user_id,create_user_name,create_time,update_time)
//                  "
//                 "  VALUES "
//                 "
//                 (:id,:data_no,:check_date,:check_address,:temperature,:humidity,:check_accord,:stability_examine,:standard_equipment_id,"
//                  ":equipment_id,:item_category_id,:conclusion,:remark,:create_user_id,:create_user_name,:create_time,:update_time)
//                  ");
//    QString checkData = baseInfo.check_date.toString("yyyy-MM-dd");
//    QString currentDateTime =
//    QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
//      query.bindValue(":id",baseInfo.id);
//      query.bindValue(":data_no",baseInfo.data_no);
//      query.bindValue(":check_date",checkData);
//      query.bindValue(":check_address",baseInfo.check_address);
//      query.bindValue(":temperature",baseInfo.temperature);
//      query.bindValue(":humidity",baseInfo.humidity);
//      query.bindValue(":check_accord",baseInfo.check_accord);
//      query.bindValue(":stability_examine",baseInfo.stability_examine);
//      query.bindValue(":standard_equipment_id",baseInfo.standard_equipment_id);
//      query.bindValue(":equipment_id",baseInfo.equipment_id);
//      query.bindValue(":item_category_id",baseInfo.item_category_id);
//      query.bindValue(":conclusion",baseInfo.conclusion);
//      query.bindValue(":remark",baseInfo.remark);
//      query.bindValue(":create_user_id",baseInfo.create_user_id);
//      query.bindValue(":create_user_name",baseInfo.create_user_name);
//      query.bindValue(":create_time",currentDateTime);
//      query.bindValue(":update_time",currentDateTime);
//      bool isSuccess = query.exec();
//      if (!isSuccess) {
//          qDebug() << "InsertTaskBasic query failed: " <<
//          query.lastError().text();
//      }

//      return isSuccess;
//};
// 自动检定和自动核查 查询实验室及温湿度
QList<LocationInfo> BaseCommonApi::getLocationInfo()
{
    QList<LocationInfo> result;
    QSqlQuery query;
    bool isSuccess = query.exec(
        "SELECT *  FROM env_location_info eli LEFT JOIN ( SELECT temperature, "
        "humidity, location_id AS  locationId "
        "  FROM env_environment_record_data  ) ethd ON ethd.locationId = eli.id "
        " ORDER BY update_time DESC");
    if (isSuccess) {
        while (query.next()) {
            LocationInfo info;
            info.id = query.value("id").toString();
            info.location_no = query.value("location_no").toString();
            info.location_name = query.value("location_name").toString();
            info.location_position = query.value("location_position").toString();
            info.create_user_id = query.value("create_user_id").toString();
            info.create_user = query.value("create_user").toString();
            info.create_time = query.value("create_time").toDateTime();
            info.update_time = query.value("update_time").toDateTime();
            info.location_lab = query.value("location_lab").toString();
            info.temperature = query.value("temperature").toString();
            info.humidity = query.value("humidity").toString();
            info.location_id = query.value("location_id").toString();
            result.append(info);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
};
// 查询出所有实验室地点，用于自动检定/核查 结果保存 地点下拉框查询
QList<LocationInfo> BaseCommonApi::getLocationInfoList()
{
    QList<LocationInfo> result;
    QSqlQuery query;
    bool isSuccess = query.exec("SELECT *   FROM   env_location_info");
    if (isSuccess) {
        while (query.next()) {
            LocationInfo info;
            info.id = query.value("id").toString();
            info.location_no = query.value("location_no").toString();
            info.location_name = query.value("location_name").toString();
            info.location_position = query.value("location_position").toString();
            info.create_user_id = query.value("create_user_id").toString();
            info.create_user = query.value("create_user").toString();
            info.create_time = query.value("create_time").toDateTime();
            info.update_time = query.value("update_time").toDateTime();
            info.location_lab = query.value("location_lab").toString();
            result.append(info);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
};
// 查询所有用户，用于自动检定/核查 结果保存
QList<User> BaseCommonApi::getUser()
{
    QList<User> result;
    QSqlQuery query;
    bool isSuccess = query.exec("SELECT * FROM sys_user");
    if (isSuccess) {
        while (query.next()) {
            User user;
            user.ID = query.value("ID").toString();
            user.NAME = query.value("NAME").toString();
            user.DEPT_ID = query.value("DEPT_ID").toString();

            result.append(user);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
};
//    设备与装置-标准装置管理-核查数据基本信息关联的核查标准设备，即给设备做核查的设备
bool BaseCommonApi::InsertStandCheckInfoEquipent(
    StandCheckInfoEquipent equiment)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_equipment_standard_check_info_equipment "
        " (id,check_equipment_id,data_id) "
        " VALUES "
        " (:id,:check_equipment_id,:data_id)");
    query.bindValue(":id", equiment.id);
    query.bindValue(":check_equipment_id", equiment.check_equipment_id);
    query.bindValue(":data_id", equiment.data_id);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << "InsertTaskBasic query failed: " << query.lastError().text();
    }

    return isSuccess;
};

////自动核查--结果保存-设备与装置-标准装置管理-核查数据管理多功能校准源数据表
// bool BaseCommonApi::InsertStandCheckDataCalibrator(StandCheckDataCalibrator
// calibrator)
//{
//     QSqlQuery query;
//     query.prepare("INSERT INTO biz_equipment_standard_check_data_calibrator "
//                   "(id, data_id, check_type, test_type, params, unit,
//                   frequency, frequency_unit, check_point, "
//                   "test_value_one_date, test_value_one, test_value_two_date,
//                   test_value_two, " "test_value_three_date, test_value_three,
//                   test_value_four_date, test_value_four, "
//                   "test_value_five_date, test_value_five,
//                   test_value_six_date, test_value_six, average_value, "
//                   "standard_deviation, relative_repeatability, urel,
//                   less_than, qualified, create_time, update_time) " "VALUES "
//                   "(:id, :data_id, :check_type, :test_type, :params, :unit,
//                   :frequency, :frequency_unit, :check_point, "
//                   ":test_value_one_date, :test_value_one,
//                   :test_value_two_date, :test_value_two, "
//                   ":test_value_three_date, :test_value_three,
//                   :test_value_four_date, :test_value_four, "
//                   ":test_value_five_date, :test_value_five,
//                   :test_value_six_date, :test_value_six, :average_value, "
//                   ":standard_deviation, :relative_repeatability, :urel,
//                   :less_than, :qualified, :create_time, :update_time)");
//     QString currentDateTime =
//     QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
//     // 绑定所有值
//     query.bindValue(":id", calibrator.id);
//     query.bindValue(":data_id", calibrator.data_id);
//     query.bindValue(":check_type", calibrator.check_type);
//     query.bindValue(":test_type", calibrator.test_type);
//     query.bindValue(":params", calibrator.params);
//     query.bindValue(":unit", calibrator.unit);
//     query.bindValue(":frequency", calibrator.frequency);
//     query.bindValue(":frequency_unit", calibrator.frequency_unit);
//     query.bindValue(":check_point", calibrator.check_point);
//     query.bindValue(":test_value_one_date",
//     calibrator.test_value_one_date.toString("yyyy-MM-dd"));
//     query.bindValue(":test_value_one", calibrator.test_value_one);
//     query.bindValue(":test_value_two_date",
//     calibrator.test_value_two_date.toString("yyyy-MM-dd"));
//     query.bindValue(":test_value_two", calibrator.test_value_two);
//     query.bindValue(":test_value_three_date",
//     calibrator.test_value_three_date.toString("yyyy-MM-dd"));
//     query.bindValue(":test_value_three", calibrator.test_value_three);
//     query.bindValue(":test_value_four_date",
//     calibrator.test_value_four_date.toString("yyyy-MM-dd"));
//     query.bindValue(":test_value_four", calibrator.test_value_four);
//     query.bindValue(":test_value_five_date",
//     calibrator.test_value_five_date.toString("yyyy-MM-dd"));
//     query.bindValue(":test_value_five", calibrator.test_value_five);
//     query.bindValue(":test_value_six_date",
//     calibrator.test_value_six_date.toString("yyyy-MM-dd"));
//     query.bindValue(":test_value_six", calibrator.test_value_six);
//     query.bindValue(":average_value", calibrator.average_value);
//     query.bindValue(":standard_deviation", calibrator.standard_deviation);
//     query.bindValue(":relative_repeatability",
//     calibrator.relative_repeatability); query.bindValue(":urel",
//     calibrator.urel); query.bindValue(":less_than", calibrator.less_than);
//     query.bindValue(":qualified", calibrator.qualified);
//     query.bindValue(":create_time", currentDateTime);
//     query.bindValue(":update_time", currentDateTime);
//     bool isSuccess = query.exec();
//     if (!isSuccess) {
//         qDebug() << "InsertTaskBasic query failed: " <<
//         query.lastError().text();
//     }

//    return isSuccess;
//};
// 根据用户ID查询 当前部门ID和部门名称全程
QList<Dept> BaseCommonApi::SelectDeptData(QString UserId)
{
    UserId = loginUserId;
    QList<Dept> result;
    QSqlQuery query;
    bool isSuccess = query.exec(QString("SELECT dept.ID,dept.FULL_NAME FROM sys_dept as "
                                        "dept WHERE dept.CREATE_USER=%1")
                                    .arg(UserId));
    if (isSuccess) {
        while (query.next()) {
            Dept info;
            info.id = query.value("id").toString();
            info.FULL_NAME = query.value("FULL_NAME").toString();
            result.append(info);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
};
// 查询出所有委托方
QList<biz_resource_customer_info> BaseCommonApi::SelectCustomerData()
{
    QList<biz_resource_customer_info> result;
    QSqlQuery query;
    bool isSuccess = query.exec("SELECT * FROM biz_resource_customer_info");
    if (isSuccess) {
        while (query.next()) {
            biz_resource_customer_info info;
            info.id = query.value("id").toString();
            info.customer_name = query.value("customer_name").toString();
            result.append(info);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
};
// 计量检定业务-受检设备检定管理-检定数据管理检定数据历史修改记录[新增]
bool BaseCommonApi::InsertModifyRecord(MeasureModifyRecord record)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_business_device_measure_modify_record "
        " (id, data_id,source_system, modify_content,   create_user_id,  "
        "create_user_name,   create_time,update_time ) "
        " VALUES (:id,  :data_id,  :source_system,  :modify_content,  "
        ":create_user_id,  :create_user_name,  :create_time,  :update_time  )");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    // 绑定所有值
    query.bindValue(":id", record.id);
    query.bindValue(":data_id", record.data_id);
    query.bindValue(":source_system", record.source_system);
    query.bindValue(":modify_content", record.modify_content);
    query.bindValue(":create_user_id", record.create_user_id);
    query.bindValue(":create_user_name", record.create_user_name);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (isSuccess) {
        QString ssql = "INSERT INTO biz_business_device_measure_modify_record "
                       " (id, data_id,source_system, modify_content,   create_user_id,  "
                       "create_user_name,   create_time,update_time ) "
                       " VALUES (:1,  :2,  :3,  :4,  :5,  :6,  :7,  :8  );";
        saveLogToSql(ssql, &record);
    } else {
        qDebug() << "Insert query failed: " << query.lastError().text();
    }

    return isSuccess;
};
// 计量检定业务-受检设备检定管理-检定数据管理检定数据历史修改记录[查询]  参数
// 检定数据管理基础信息表id
QList<MeasureModifyRecord> BaseCommonApi::getModifyRecord(QString data_id)
{
    QList<MeasureModifyRecord> result;
    QSqlQuery query;
    query.prepare(
        "SELECT * FROM biz_business_device_measure_modify_record WHERE data_id = "
        ":data_id ");
    query.bindValue(":data_id", data_id);
    if (query.exec()) {
        while (query.next()) {
            MeasureModifyRecord record;
            record.id = query.value("id").toString();
            record.data_id = query.value("data_id").toString();
            record.source_system = query.value("source_system").toString();
            record.modify_content = query.value("modify_content").toString();
            record.create_user_id = query.value("create_user_id").toString();
            record.create_user_name = query.value("create_user_name").toString();
            record.create_time = query.value("create_time").toDateTime();
            record.update_time = query.value("update_time").toDateTime();
            result.append(record);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
};
// 设备与装置-标准装置管理-核查数据管理核查数据历史修改记录
bool BaseCommonApi::InsertCheckModifyRecord(MeasureModifyRecord record)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_equipment_standard_check_modify_record "
        " (id, data_id,source_system, modify_content,   create_user_id,  "
        "create_user_name,   create_time,update_time ) "
        " VALUES (:id,  :data_id,  :source_system,  :modify_content,  "
        ":create_user_id,  :create_user_name,  :create_time,  :update_time  )");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    // 绑定所有值
    query.bindValue(":id", record.id);
    query.bindValue(":data_id", record.data_id);
    query.bindValue(":source_system", record.source_system);
    query.bindValue(":modify_content", record.modify_content);
    query.bindValue(":create_user_id", record.create_user_id);
    query.bindValue(":create_user_name", record.create_user_name);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << "Insert query failed: " << query.lastError().text();
    }

    return isSuccess;
};

// 设备与装置-标准装置管理-核查数据管理核查数据历史修改记录[查询] 参数
// 检定数据管理基础信息表id
QList<MeasureModifyRecord> BaseCommonApi::getMeasureModifyRecord(
    QString data_id)
{
    QList<MeasureModifyRecord> result;
    QSqlQuery query;
    query.prepare(
        "SELECT * FROM biz_equipment_standard_check_modify_record WHERE data_id "
        "= :data_id ");
    query.bindValue(":data_id", data_id);
    if (query.exec()) {
        while (query.next()) {
            MeasureModifyRecord record;
            record.id = query.value("id").toString();
            record.data_id = query.value("data_id").toString();
            record.source_system = query.value("source_system").toString();
            record.modify_content = query.value("modify_content").toString();
            record.create_user_id = query.value("create_user_id").toString();
            record.create_user_name = query.value("create_user_name").toString();
            record.create_time = query.value("create_time").toDateTime();
            record.update_time = query.value("update_time").toDateTime();
            result.append(record);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
};

// 保存log信息
// log文件
bool BaseCommonApi::SaveLogInfo(int pathIndex, QString Info)
{
    // 获取应用程序的根目录
    QString path = QCoreApplication::applicationDirPath();
    // QString path ="C:\\AutoVerScheme";
    switch (pathIndex) {
    case 0:
        path += "\\File\\log\\sync_info.txt";
        break;
    case 1:
        path += "\\File\\log\\textlog.txt";
        break;
    }
    QByteArray utf8Dat;
    QString Message = Info;
    QFile file(path);
    // 如果目录不存在，则创建目录
    // 获取文件所在的目录
    QDir logDir = QDir(QFileInfo(path).absolutePath());
    if (!logDir.exists()) {
        if (!logDir.mkpath(".")) {
            qDebug() << "Failed to create log directory:" << logDir.absolutePath();
            return false;
        }
    }

    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "无法打开文件：" << file.errorString();
        QString str = file.errorString();
        return false;
    }
    QTextStream out(&file);
    utf8Dat = Message.toUtf8();
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "\t"
        << utf8Dat << "\n";
    file.close();
    return true;
};

// 新增日志表数据
bool BaseCommonApi::InsertzdLog(zdLog log)
{
    log.userid = loginUserId;
    QSqlQuery query;
    query.prepare(
        "INSERT INTO "
        "zd_log(id,is_sync,sync_time,remark,userid,type,create_time,update_time) "
        " VALUES(:id,:is_sync,:sync_time,:remark,:userid,:type,:create_time,:"
        "update_time) ");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    // 绑定所有值
    query.bindValue(":id", log.id);
    query.bindValue(":is_sync", log.is_sync);
    query.bindValue(":sync_time", log.sync_time.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":remark", log.remark);
    query.bindValue(":userid", log.userid);
    query.bindValue(":type", log.type);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << "Insert query failed: " << query.lastError().text();
    }

    return isSuccess;
};
// 根据userid查询日志表数据
QList<zdLog> BaseCommonApi::getzdLogByuserId(QString userid)
{
    userid = loginUserId;
    QList<zdLog> result;
    QSqlQuery query;
    query.prepare("SELECT * FROM zd_log WHERE userid = :userid ");
    query.bindValue(":userid", userid);
    if (query.exec()) {
        while (query.next()) {
            zdLog log;
            log.id = query.value("id").toString();
            log.is_sync = query.value("is_sync").toString();
            log.sync_time = query.value("sync_time").toDateTime();
            log.remark = query.value("remark").toString();
            log.type = query.value("type").toString();
            log.create_time = query.value("create_time").toDateTime();
            log.update_time = query.value("update_time").toDateTime();
            result.append(log);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
};
// 新增数据同步表
bool BaseCommonApi::InsertzdDataSync(zdDataSync data)
{
    data.userid = loginUserId;
    QSqlQuery query;
    query.prepare(
        "INSERT INTO "
        "zd_data_sync(id,type,ssql,userid,remark,create_time,update_time,log_"
        "sync_id,sync_type) "
        " VALUES "
        " (:id,:type,:ssql,:userid,:remark,:create_time,:update_time,:log_sync_"
        "id,:sync_type) ");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    // 绑定所有值
    query.bindValue(":id", data.id);
    query.bindValue(":type", data.type);
    query.bindValue(":ssql", data.ssql);
    query.bindValue(":remark", data.remark);
    query.bindValue(":userid", data.userid);
    query.bindValue(":log_sync_id", data.log_sync_id);
    query.bindValue(":sync_type", data.sync_type);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << "Insert query failed: " << query.lastError().text();
    }

    return isSuccess;
};
// 根据userid查询数据同步表
QList<zdDataSync> BaseCommonApi::getzdDataSyncByuserId(QString log_sync_id)
{
    QList<zdDataSync> result;
    QSqlQuery query;
    query.prepare("SELECT * FROM zd_data_sync WHERE log_sync_id= :log_sync_id ");
    query.bindValue(":log_sync_id", log_sync_id);
    if (query.exec()) {
        while (query.next()) {
            zdDataSync data;
            data.id = query.value("id").toString();
            data.type = query.value("type").toString();
            data.ssql = query.value("ssql").toString();
            data.remark = query.value("remark").toString();
            data.userid = query.value("userid").toString();
            data.log_sync_id = query.value("log_sync_id").toString();
            data.sync_type = query.value("sync_type").toString();
            data.create_time = query.value("create_time").toDateTime();
            data.update_time = query.value("update_time").toDateTime();
            result.append(data);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
};

// 【查询】检定项0.02活塞式压力计数据(包含字典的字段都存为字典value)
QMap<QString, QList<QStringList>>
BaseCommonApi::getDeviceMeasureCalibratorPistonGauge(
    QString itemCatoryName, QString itemId,
    SequentialMap<QString, QStringList>* headList)
{
    QMap<QString, QList<QStringList>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();

    QSqlQuery query;
    query.prepare(
        "SELECT "
        "bbdmidpg.*,bbdmii.appearance_function_check,bbdmicp.indication_error,"
        "bbdmicp.measure_medium, "
        " bbdmicp.division,bbdmicp.deviation_and_switching,bbdmicp.display_value_"
        "one,bbdmicp.display_value_two,bbdmicp.display_value_three, "
        " bbdmicp.insulation_resistance,bbdmicp.appearance,bbdmicp.tightness,"
        "bbdmicp.pointer_deflection_stability,"
        "bbdmidpg.create_time,bbdmidpg.update_time "
        " FROM biz_business_device_measure_item_data_piston_gauge bbdmidpg "
        " JOIN biz_business_device_measure_item_info bbdmii ON bbdmii.id = "
        "bbdmidpg.item_id "
        " JOIN biz_business_device_measure_item_config_piston_gauge bbdmicp ON "
        "bbdmicp.item_id = bbdmidpg.item_id "
        " WHERE bbdmidpg.item_id =   :itemId ORDER BY bbdmidpg.create_time ASC");
    // 将参数绑定到占位符
    query.bindValue(":itemId", itemId);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_id").toString()); // 检定项目ID
            result.append(query.value("indicating_value").toString()); // 标准器示值
            result.append(query.value("return_error").toString()); // 回程误差允许值
            result.append(
                query.value("friction_error").toString()); // 轻敲位移允许值
            result.append(
                query.value("indicating_error").toString()); // 示值误差允许值
            result.append(query.value("zero_position").toString()); // 零位是否带止销
            result.append(
                query.value("measure_medium").toString()); // 检定介质(字典value)
            result.append(query.value("accuracy_class").toString()); // 准确度等级
            result.append(query.value("maximum_error").toString()); // 最大允许误差
            result.append(
                query.value("theoretical_output_value").toString()); // 理论输出值
            result.append(query.value("theoretical_output_value_unit")
                              .toString()); // 理论输出值单位
            result.append(query.value("appearance_function_check")
                              .toString()); // 外观及功能性检查(1/0)
            result.append(query.value("indication_error")
                              .toString()); // 示值误差、回程误差、轻敲位移
            result.append(
                query.value("measure_medium").toString()); // 检定介质(字典value)
            result.append(query.value("division").toString()); // 分度值的五分之一
            result.append(query.value("deviation_and_switching")
                              .toString()); // 设定点偏差及切换差
            result.append(
                query.value("display_value_one").toString()); // 设定标准器示值1
            result.append(
                query.value("display_value_two").toString()); // 设定标准器示值2
            result.append(
                query.value("display_value_three").toString()); // 设定标准器示值3
            result.append(
                query.value("insulation_resistance").toString()); // 绝缘电阻
            result.append(query.value("appearance").toString()); // 外观
            result.append(query.value("tightness").toString()); // 密封性
            result.append(query.value("pointer_deflection_stability")
                              .toString()); // 指针偏转平稳性
            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());
            resultData.append(result);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    //        *headList
    //        <<"序号"<<"检定项id"<<"标准器示值"<<"回程误差允许值"<<"轻敲位移允许值"<<"示值误差允许值"<<"零位是否带止销"
    //                 <<"检定介质"<<"准确度等级"<<"最大允许误差"<<"理论输出值"<<"理论输出值单位"<<"外观及功能性检查(1/0)"<<"示值误差、回程误差、轻敲位移"
    //                <<"检定介质(字典value)"<<"分度值的五分之一"<<"创建时间"<<"更新时间";
    // 进行检定项目列头转换
    DataMapList = GetDataReturnItemListByPistonGauge(itemCatoryName, resultData,
        DataMapList, headList);
    return DataMapList;
};
// 0.02压力表头
QMap<QString, QList<QStringList>>
BaseCommonApi::GetDataReturnItemListByPistonGauge(
    QString itemCatoryName, QList<QStringList> ResultData,
    QMap<QString, QList<QStringList>> DataMapList,
    SequentialMap<QString, QStringList>* HeadMapList)
{
    QList<QStringList> IndicationErrorList;
    if (itemCatoryName.contains("一般压力表")) {
        for (const QStringList& row : ResultData) {
            QString VerificationItem = NULL;
            if (!row.isEmpty() && row[13] == "1") {
                VerificationItem = "示值误差/回程误差/轻敲位移";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "标准器示值"
                     << "轻敲前示值-正"
                     << "轻敲前示值-反"
                     << "轻敲后示值-正"
                     << "轻敲后示值-反"
                     << "轻敲前后变动量-正"
                     << "轻敲前后变动量-反";
                HeadMapList->insert(VerificationItem, head);
                // 添加数据
                QStringList Data;
                //                   Data.append(row[12]);  //外观及功能性检查
                //                   Data.append(row[13]);
                //                   //示值误差、回程误差、轻敲位移
                //                   Data.append(row[11]);  //理论输出值单位
                //                   Data.append(row[14]);  //检定介质
                //                   Data.append(row[15]);  //分度值的五分之一
                Data.append(row[0]); // ID
                Data.append(row[2]); // 标准器示值
                Data.append(row[2]); // 轻敲前示值-正
                Data.append(row[2]); //"轻敲前示值-反"
                Data.append(row[2]); // 轻敲后示值-正
                Data.append(row[2]); // 轻敲后示值-反
                Data.append("0"); // 轻敲前后变动量-正
                Data.append("0"); // 轻敲前后变动量-反
                IndicationErrorList.append(Data);
            }
        }
    }
    if (IndicationErrorList.count() != 0) {
        DataMapList.insert("示值误差/回程误差/轻敲位移", IndicationErrorList);
    };
    return DataMapList;
};
// 一般压力表  查询生成结果处理界面的头和数据
QMap<QString, QList<QStringList>>
BaseCommonApi::getDeviceMeasureCalibratorPistonGaugeGenerate(
    QString itemCatoryName, QString itemId,
    SequentialMap<QString, QStringList>* headList)
{
    QMap<QString, QList<QStringList>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();

    QSqlQuery query;
    query.prepare(
        "SELECT "
        "bbdmidpg.*,bbdmii.appearance_function_check,bbdmicp.indication_error,"
        "bbdmicp.measure_medium, "
        " bbdmicp.division,bbdmicp.deviation_and_switching,bbdmicp.display_value_"
        "one,bbdmicp.display_value_two,bbdmicp.display_value_three, "
        " bbdmicp.insulation_resistance,bbdmicp.appearance,bbdmicp.tightness,"
        "bbdmicp.pointer_deflection_stability,"
        " bbdmicp.accuracy_level_lower,bbdmicp.accuracy_level_upper"
        " FROM biz_business_device_measure_item_data_piston_gauge bbdmidpg "
        " JOIN biz_business_device_measure_item_info bbdmii ON bbdmii.id = "
        "bbdmidpg.item_id "
        " JOIN biz_business_device_measure_item_config_piston_gauge bbdmicp ON "
        "bbdmicp.item_id = bbdmidpg.item_id "
        " WHERE bbdmidpg.item_id =   :itemId ORDER BY bbdmidpg.create_time DESC");
    // 将参数绑定到占位符
    query.bindValue(":itemId", itemId);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_id").toString()); // 检定项目ID
            result.append(
                query.value("indicating_value").toString()); // 标准ji器示值
            result.append(query.value("return_error").toString()); // 回程误差允许值
            result.append(
                query.value("friction_error").toString()); // 轻敲位移允许值
            result.append(
                query.value("indicating_error").toString()); // 示值误差允许值
            result.append(query.value("zero_position").toString()); // 零位是否带止销
            result.append(
                query.value("measure_medium").toString()); // 检定介质(字典value)
            result.append(query.value("accuracy_class").toString()); // 准确度等级
            result.append(query.value("maximum_error").toString()); // 最大允许误差
            result.append(
                query.value("theoretical_output_value").toString()); // 理论输出值
            result.append(query.value("theoretical_output_value_unit")
                              .toString()); // 理论输出值单位
            result.append(query.value("appearance_function_check")
                              .toString()); // 外观及功能性检查(1/0)
            result.append(query.value("indication_error")
                              .toString()); // 示值误差、回程误差、轻敲位移
            result.append(
                query.value("measure_medium").toString()); // 检定介质(字典value)
            result.append(query.value("division").toString()); // 分度值的五分之一
            result.append(query.value("deviation_and_switching")
                              .toString()); // 设定点偏差及切换差
            result.append(
                query.value("display_value_one").toString()); // 设定标准器示值1
            result.append(
                query.value("display_value_two").toString()); // 设定标准器示值2
            result.append(
                query.value("display_value_three").toString()); // 设定标准器示值3
            result.append(
                query.value("insulation_resistance").toString()); // 绝缘电阻
            result.append(query.value("appearance").toString()); // 外观
            result.append(query.value("tightness").toString()); // 密封性
            result.append(query.value("pointer_deflection_stability")
                              .toString()); // 指针偏转平稳性
            result.append(query.value("accuracy_level_lower")
                              .toString()); // 准确度等级(量程上限90%以下)
            result.append(query.value("accuracy_level_upper")
                              .toString()); // 准确度等级(量程上限90%以上)
            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());
            resultData.append(result);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    //        *headList
    //        <<"序号"<<"检定项id"<<"标准器示值"<<"回程误差允许值"<<"轻敲位移允许值"<<"示值误差允许值"<<"零位是否带止销"
    //                 <<"检定介质"<<"准确度等级"<<"最大允许误差"<<"理论输出值"<<"理论输出值单位"<<"外观及功能性检查(1/0)"<<"示值误差、回程误差、轻敲位移"
    //                <<"检定介质(字典value)"<<"分度值的五分之一"<<"创建时间"<<"更新时间";
    // 进行检定项目列头转换
    DataMapList = GetDataReturnItemListByPistonGaugeResult(
        itemCatoryName, resultData, DataMapList, headList);
    return DataMapList;
};

QMap<QString, QList<QStringList>>
BaseCommonApi::GetDataReturnItemListByPistonGaugeResult(
    QString itemCatoryName, QList<QStringList> ResultData,
    QMap<QString, QList<QStringList>> DataMapList,
    SequentialMap<QString, QStringList>* HeadMapList)
{
    QList<QStringList> IndicationErrorList;
    if (itemCatoryName.contains("一般压力表")) {
        for (const QStringList& row : ResultData) {
            QString VerificationItem = NULL;
            if (!row.isEmpty()) {
                VerificationItem = "项目";
                // 添加列头
                QStringList head;
                head << "项目-量程上限90%以下"
                     << "项目-量程上限90%以上"
                     << "示值误差允许值"
                     << "示值误差最大值"
                     << "轻敲位移允许值"
                     << "轻敲位移最大值"
                     << "回程误差允许值"
                     << "回程误差最大值";
                HeadMapList->insert(VerificationItem, head);
                // 添加数据
                QStringList Data;
                Data.append(row[24]); // 量程上限90%以下
                Data.append(row[25]); // 量程上限90%及以上
                Data.append(row[5]); // 示值误差允许值
                Data.append(""); // 示值误差最大值
                Data.append(row[4]); // 轻敲位移允许值
                Data.append(""); // 轻敲位移最大值
                Data.append(row[3]); // 回程误差允许值
                Data.append(""); // 回程误差最大值
                //                    Data.append("");//结果
                //                    Data.append("");//输出备注
                IndicationErrorList.append(Data);
            }
        }
    }
    if (IndicationErrorList.count() != 0) {
        DataMapList.insert("项目", IndicationErrorList);
    };
    return DataMapList;
}
// 根据设备id查询 不同头数据
QList<QStringList> BaseCommonApi::searchStatus(QString deviceId)
{
    QList<QStringList> result;
    QList<JudgeCalibrationTable> tableList = getJudgeCalibrationTable(deviceId);
    QString itemId = tableList[0].id;
    QString belongStandardEquipment = tableList[0].belong_standard_equipment;
    switch (belongStandardEquipment.toInt()) {
    case 1:
        break;
    case 2:
        break;
    case 4:
        result = getDeviceMeasureCalibratorPistonGaugeBystatus(itemId);
        break;
    }
    return result;
}

// 测试界面 外观及功能性检查和检定数据头
QList<QStringList> BaseCommonApi::getDeviceMeasureCalibratorPistonGaugeBystatus(
    QString itemId)
{
    QList<QStringList> result;
    QSqlQuery query;
    QStringList strList;
    query.prepare(
        "SELECT "
        "bbdmii.appearance_function_check,bbdmicp.appearance,bbdmicp.tightness,"
        "bbdmicp.insulation_resistance,"
        " bbdmicp.zero_drift,bbdmicp.range_unit,bbdmidpg.measure_medium "
        "  ,bbdmicp.division FROM "
        "biz_business_device_measure_item_data_piston_gauge bbdmidpg "
        " JOIN biz_business_device_measure_item_config_piston_gauge bbdmicp ON "
        "bbdmicp.item_id = bbdmidpg.item_id "
        " JOIN biz_business_device_measure_item_info bbdmii ON bbdmii.id = "
        "bbdmidpg.item_id"
        " WHERE bbdmidpg.item_id = :itemId LIMIT 1");
    // 将参数绑定到占位符  1763116518209597442
    query.bindValue(":itemId", itemId);
    if (query.exec()) {
        while (query.next()) {
            strList.append(query.value("appearance_function_check")
                               .toString()); // 外观及功能性检查(1/0)
            strList.append(query.value("appearance").toString()); // 外观
            strList.append(query.value("tightness").toString()); // 密封性
            strList.append(
                query.value("insulation_resistance").toString()); // 绝缘电阻
            strList.append(query.value("zero_drift").toString()); // 零位飘移
            strList.append(query.value("pointer_deflection_stability")
                               .toString()); // 指针偏转平稳性
            strList.append(query.value("range_unit").toString()); // 单位
            strList.append(
                query.value("measure_medium").toString()); // 检定介质(字典value)
            strList.append(query.value("division").toString()); // 分度值的五分之一
            //              result.appearance_function_check =
            //              query.value("appearance_function_check").toString();
            //              result.appearance = query.value("appearance").toString();
            //              result.tightness= query.value("tightness").toString();
            //              result.insulation_resistance=
            //              query.value("insulation_resistance").toString();
            //              result.pointer_deflection_stability =
            //              query.value("pointer_deflection_stability").toString();
            //              result.zero_drift = query.value("zero_drift").toString();
            //              result.measure_medium =
            //              query.value("measure_medium").toString(); result.division
            //              = query.value("division").toString();
        }
        result.append(strList);
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
}

// 自动检定数据结果保存 参数1：装置字典1,2,3 参数2:BaseClass父类， 子类赋值
bool BaseCommonApi::SearchSelectTable(QString id, BaseClass* object)
{
    bool result = false; // 初始化为 false 或根据需要初始化为其他值
    switch (id.toInt()) {
    case 1: {
        // 多功能校准源
        SaveDeviceMeasureCalibrator* recordCal = dynamic_cast<SaveDeviceMeasureCalibrator*>(object);
        if (recordCal != nullptr) {
            result = BaseCommonApi::InsertVerMultiCal(
                *recordCal); // 注意这里使用recordCal而不是recordGauge
        } else {
            // 处理转换失败的情况
            result = false;
        }
        break;
    }
    case 2: {
        // 直流稳压电源
        bizBusinessDeviceMeasureDataDcPower* dcPower = dynamic_cast<bizBusinessDeviceMeasureDataDcPower*>(object);
        if (dcPower != nullptr) {
            result = BaseCommonApi::InsertbizBusinessDeviceMeasureDataDcPower(*dcPower);
        } else {
            // 处理转换失败的情况
            result = false;
        }
        break;
    }
    case 4: {
        // 0.02压力活塞计
        DeviceMeasureDataPistonGauge* recordGauge = dynamic_cast<DeviceMeasureDataPistonGauge*>(object);
        if (recordGauge != nullptr) {
            result = BaseCommonApi::InsertDeviceMeasureDataPistonGauge(*recordGauge);
        } else {
            // 处理转换失败的情况
            result = false;
        }
        break;
    }
    case 5: {
        // 二等铂电阻
        DeviceMeasureDataResistanceThermometer* thermometer = dynamic_cast<DeviceMeasureDataResistanceThermometer*>(object);
        if (thermometer != nullptr) {
            result = BaseCommonApi::InsertDeviceMeasureDataResistanceThermometer(
                *thermometer);
        } else {
            // 处理转换失败的情况
            result = false;
        }
        break;
    }
    case 9: {
        // 频谱
        DeviceMeasureDataSpectrumAnalyzer* analyzer = dynamic_cast<DeviceMeasureDataSpectrumAnalyzer*>(object);
        if (analyzer != nullptr) {
            result = BaseCommonApi::InsertDeviceMeasureDataSpectrumAnalyzer(*analyzer);
        } else {
            // 处理转换失败的情况
            result = false;
        }
        break;
    }
    case 10: {
        // 信号发生器
        DeviceMeasureDataSignalGenerator* generator = dynamic_cast<DeviceMeasureDataSignalGenerator*>(object);
        if (generator != nullptr) {
            result = BaseCommonApi::InsertDeviceMeasureDataSignalGenerator(*generator);
        } else {
            // 处理转换失败的情况
            result = false;
        }
        break;
    }
    case 13: {
        // 示波器
        DeviceOscilloscopeCalibration* recordGauge = dynamic_cast<DeviceOscilloscopeCalibration*>(object);
        if (recordGauge != nullptr) {
            result = BaseCommonApi::InsertDeviceOscilloscopeCalibration(*recordGauge);
        } else {
            // 处理转换失败的情况
            result = false;
        }
        break;
    }
    case 11: {
        // 微波衰减器 InsertDeviceMicrowaveAttenuation
        MicrowaveAttenuationCalibration* recordGauge = dynamic_cast<MicrowaveAttenuationCalibration*>(object);
        if (recordGauge != nullptr) {
            result = BaseCommonApi::InsertDeviceMicrowaveAttenuation(*recordGauge);
        } else {
            // 处理转换失败的情况
            result = false;
        }
    }
    case 16: {
        // 低频信号发生器
        LFSignalGeneratoCalibration* recordGauge = dynamic_cast<LFSignalGeneratoCalibration*>(object);
        if (recordGauge != nullptr) {
            result = BaseCommonApi::InsertDeviceLFSignalGenerator(*recordGauge);
        } else {
            // 处理转换失败的情况
            result = false;
        }
    }
    case 17: {
        // s参数
        DeviceMeasureDataSParam* sParam = dynamic_cast<DeviceMeasureDataSParam*>(object);
        if (sParam != nullptr) {
            result = BaseCommonApi::InsertDeviceMeasureItemDataSParam(*sParam);
        } else {
            // 处理转换失败的情况
            result = false;
        }
    }
    case 15: {
        // 小功率
        SmallPowerCalibration* sParam = dynamic_cast<SmallPowerCalibration*>(object);
        if (sParam != nullptr) {
            result = BaseCommonApi::InsertDeviceSmallPower(*sParam);
        } else {
            // 处理转换失败的情况
            result = false;
        }
    }

    // 可能还有其他的case
    default:
        result = false;
        break;
    }

    return result; // 返回结果
};
// 【结果保存】压力
bool BaseCommonApi::InsertDeviceMeasureDataPistonGauge(
    DeviceMeasureDataPistonGauge gauge)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_business_device_measure_data_piston_gauge "
        "(id, data_id, data_type, data_category, indicating_value, "
        "before_friction_forward_stroke, before_friction_reverse_stroke, "
        "after_friction_forward_stroke, after_friction_reverse_stroke, "
        "variation_forward_stroke, variation_reverse_stroke, "
        "theoretical_output_value, first_forward_stroke, first_reverse_stroke, "
        "second_forward_stroke, second_reverse_stroke, third_forward_stroke, "
        "third_reverse_stroke, indicating_error, return_error, "
        "average_forward_stroke, "
        "average_reverse_stroke, average_value, average_return_error, item, "
        "max_indicating_error, max_return_error, max_friction_error, "
        "maximum_error_absolute, "
        "forward_stroke_switch_value, reverse_stroke_switch_value, "
        "point_deviation, "
        "different_gap, max_point_deviation, allow_point_deviation, "
        "max_different_gap, "
        "allow_different_gap, zero_drift_one, zero_drift_two, zero_drift_three, "
        "zero_drift_four, zero_drift_five, intercept_a, slope_b, sensitivity_b, "
        "repeatability_r, hysteresis_h, linear_l, basic_error_a, "
        "periodic_stability_s_b, "
        "create_time, update_time) "
        "VALUES (:id, :data_id, :data_type, :data_category, :indicating_value, "
        ":before_friction_forward_stroke, :before_friction_reverse_stroke, "
        ":after_friction_forward_stroke, :after_friction_reverse_stroke, "
        ":variation_forward_stroke, :variation_reverse_stroke, "
        ":theoretical_output_value, :first_forward_stroke, "
        ":first_reverse_stroke, "
        ":second_forward_stroke, :second_reverse_stroke, :third_forward_stroke, "
        ":third_reverse_stroke, :indicating_error, :return_error, "
        ":average_forward_stroke, "
        ":average_reverse_stroke, :average_value, :average_return_error, :item, "
        ":max_indicating_error, :max_return_error, :max_friction_error, "
        ":maximum_error_absolute, "
        ":forward_stroke_switch_value, :reverse_stroke_switch_value, "
        ":point_deviation, "
        ":different_gap, :max_point_deviation, :allow_point_deviation, "
        ":max_different_gap, "
        ":allow_different_gap, :zero_drift_one, :zero_drift_two, "
        ":zero_drift_three, "
        ":zero_drift_four, :zero_drift_five, :intercept_a, :slope_b, "
        ":sensitivity_b, "
        ":repeatability_r, :hysteresis_h, :linear_l, :basic_error_a, "
        ":periodic_stability_s_b, "
        ":create_time, :update_time)");

    ////       QString formattedUpdateDateTime =
    /// measureCalibrator.create_time.toString("yyyy-MM-dd HH:mm:ss");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", gauge.id);
    query.bindValue(":data_id", gauge.data_id);
    query.bindValue(":data_type", gauge.data_type);
    query.bindValue(":data_category", gauge.data_category);
    query.bindValue(":indicating_value", gauge.indicating_value);
    query.bindValue(":before_friction_forward_stroke",
        gauge.before_friction_forward_stroke);
    query.bindValue(":before_friction_reverse_stroke",
        gauge.before_friction_reverse_stroke);
    query.bindValue(":after_friction_forward_stroke",
        gauge.after_friction_forward_stroke);
    query.bindValue(":after_friction_reverse_stroke",
        gauge.after_friction_reverse_stroke);
    query.bindValue(":variation_forward_stroke", gauge.variation_forward_stroke);
    query.bindValue(":variation_reverse_stroke", gauge.variation_reverse_stroke);
    query.bindValue(":theoretical_output_value", gauge.theoretical_output_value);
    query.bindValue(":first_forward_stroke", gauge.first_forward_stroke);
    query.bindValue(":first_reverse_stroke", gauge.first_reverse_stroke);
    query.bindValue(":second_forward_stroke", gauge.second_forward_stroke);
    query.bindValue(":second_reverse_stroke", gauge.second_reverse_stroke);
    query.bindValue(":third_forward_stroke", gauge.third_forward_stroke);
    query.bindValue(":third_reverse_stroke", gauge.third_reverse_stroke);
    query.bindValue(":indicating_error", gauge.indicating_error);
    query.bindValue(":return_error", gauge.return_error);
    query.bindValue(":average_forward_stroke", gauge.average_forward_stroke);
    query.bindValue(":average_reverse_stroke", gauge.average_reverse_stroke);
    query.bindValue(":average_value", gauge.average_value);
    query.bindValue(":average_return_error", gauge.average_return_error);
    query.bindValue(":item", gauge.item);
    query.bindValue(":max_indicating_error", gauge.max_indicating_error);
    query.bindValue(":max_return_error", gauge.max_return_error);
    query.bindValue(":max_friction_error", gauge.max_friction_error);
    query.bindValue(":maximum_error_absolute", gauge.maximum_error_absolute);
    query.bindValue(":forward_stroke_switch_value",
        gauge.forward_stroke_switch_value);
    query.bindValue(":reverse_stroke_switch_value",
        gauge.reverse_stroke_switch_value);
    query.bindValue(":point_deviation", gauge.point_deviation);
    query.bindValue(":different_gap", gauge.different_gap);
    query.bindValue(":max_point_deviation", gauge.max_point_deviation);
    query.bindValue(":allow_point_deviation", gauge.allow_point_deviation);
    query.bindValue(":max_different_gap", gauge.max_different_gap);
    query.bindValue(":allow_different_gap", gauge.allow_different_gap);
    query.bindValue(":zero_drift_one", gauge.zero_drift_one);
    query.bindValue(":zero_drift_two", gauge.zero_drift_two);
    query.bindValue(":zero_drift_three", gauge.zero_drift_three);
    query.bindValue(":zero_drift_four", gauge.zero_drift_four);
    query.bindValue(":zero_drift_five", gauge.zero_drift_five);
    query.bindValue(":intercept_a", gauge.intercept_a);
    query.bindValue(":slope_b", gauge.slope_b);
    query.bindValue(":sensitivity_b", gauge.sensitivity_b);
    query.bindValue(":repeatability_r", gauge.repeatability_r);
    query.bindValue(":hysteresis_h", gauge.hysteresis_h);
    query.bindValue(":linear_l", gauge.linear_l);
    query.bindValue(":basic_error_a", gauge.basic_error_a);
    query.bindValue(":periodic_stability_s_b", gauge.periodic_stability_s_b);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);

    bool isSuccess = query.exec();
    if (isSuccess) {
        QString ssql = "INSERT INTO biz_business_device_measure_data_piston_gauge "
                       "(id, data_id, data_type, data_category, indicating_value, "
                       "before_friction_forward_stroke, before_friction_reverse_stroke, "
                       "after_friction_forward_stroke, after_friction_reverse_stroke, "
                       "variation_forward_stroke, variation_reverse_stroke, "
                       "theoretical_output_value, first_forward_stroke, first_reverse_stroke, "
                       "second_forward_stroke, second_reverse_stroke, third_forward_stroke, "
                       "third_reverse_stroke, indicating_error, return_error, "
                       "average_forward_stroke, "
                       "average_reverse_stroke, average_value, average_return_error, item, "
                       "max_indicating_error, max_return_error, max_friction_error, "
                       "maximum_error_absolute, "
                       "forward_stroke_switch_value, reverse_stroke_switch_value, "
                       "point_deviation, "
                       "different_gap, max_point_deviation, allow_point_deviation, "
                       "max_different_gap, "
                       "allow_different_gap, zero_drift_one, zero_drift_two, "
                       "zero_drift_three, "
                       "zero_drift_four, zero_drift_five, intercept_a, slope_b, "
                       "sensitivity_b, "
                       "repeatability_r, hysteresis_h, linear_l, basic_error_a, "
                       "periodic_stability_s_b, "
                       "create_time, update_time) "
                       "VALUES (:1, :2, :3, :4, :5, "
                       ":6, :7, "
                       ":8, :9, "
                       ":10, :11, "
                       ":12, :13, :14, "
                       ":15, :16, :17, "
                       ":18, :19, :20, :21, "
                       ":22, :23, :24, :25, "
                       ":26, :27, :28, :29, "
                       ":30, :31, :32, "
                       ":33, :34, :35, :36, "
                       ":37, :38, :39, :40, "
                       ":41, :42, :43, :44, :45, "
                       ":46, :47, :48, :49, :50, "
                       ":51, :52);";
        saveLogToSql(ssql, &gauge);
    } else {
        qDebug() << "Insert data failed: " << query.lastError().text();
    }

    return isSuccess;
};
// 【查询】 0.02压力结果表数据
QList<QStringList> BaseCommonApi::getDeviceMeasureDataPistonGauge(
    QString data_id, QStringList* HeadList)
{
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();

    QSqlQuery query;
    query.prepare(
        "SELECT * FROM biz_business_device_measure_data_piston_gauge WHERE "
        "data_id= :data_id ");
    // 将参数绑定到占位符
    query.bindValue(":data_id", data_id);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(
                query.value("data_id").toString()); // 检定数据管理基础信息表id
            result.append(
                query.value("data_type")
                    .toString()); // 检定数据类型(电接点一般压力表一个检定数据中区分两个表格)(字典code)
            result.append(query.value("data_category")
                              .toString()); // 检定数据类别(1检定数据，2计算结果数据)
            result.append(query.value("indicating_value").toString()); // 标准器示值
            result.append(query.value("before_friction_forward_stroke")
                              .toString()); // 轻敲前被检设备示值-正行程
            result.append(query.value("before_friction_reverse_stroke")
                              .toString()); // 轻敲前被检设备示值-反行程
            result.append(query.value("after_friction_forward_stroke")
                              .toString()); // 轻敲后被检设备示值-正行程
            result.append(query.value("after_friction_reverse_stroke")
                              .toString()); // 轻敲后被检设备示值-反行程
            result.append(query.value("variation_forward_stroke")
                              .toString()); // 轻敲前后变动量-正行程
            result.append(query.value("variation_reverse_stroke")
                              .toString()); // 轻敲前后变动量-反行程
            result.append(
                query.value("theoretical_output_value").toString()); // 理论输出值
            result.append(
                query.value("first_forward_stroke").toString()); // 第一次正行程
            result.append(
                query.value("first_reverse_stroke").toString()); // 第一次反行程
            result.append(
                query.value("second_forward_stroke").toString()); // 第二次正行程
            result.append(
                query.value("second_reverse_stroke").toString()); // 第二次反行程
            result.append(
                query.value("third_forward_stroke").toString()); // 第三次正行程
            result.append(
                query.value("third_reverse_stroke").toString()); // 第三次反行程
            result.append(query.value("indicating_error").toString()); // 示值误差
            result.append(query.value("return_error").toString()); // 回程误差
            result.append(
                query.value("average_forward_stroke").toString()); // 正行程平均值
            result.append(
                query.value("average_reverse_stroke").toString()); // 反行程平均值
            result.append(query.value("average_value").toString()); // 平均值
            result.append(
                query.value("average_return_error").toString()); // 回程误差平均值
            result.append(query.value("item").toString()); // 项目(结果表)
            result.append(query.value("max_indicating_error")
                              .toString()); // 示值误差最大值(结果表)
            result.append(query.value("max_return_error")
                              .toString()); // 回程误差最大值(结果表)
            result.append(query.value("max_friction_error")
                              .toString()); // 轻敲位移最大值(结果表)
            result.append(query.value("maximum_error_absolute")
                              .toString()); // 最大允许误差绝对值
            result.append(query.value("forward_stroke_switch_value")
                              .toString()); // 正行程切换值(电接点一般压力表)
            result.append(query.value("reverse_stroke_switch_value")
                              .toString()); // 反行程切换值(电接点一般压力表)
            result.append(query.value("point_deviation")
                              .toString()); // 设定点偏差(电接点一般压力表)
            result.append(
                query.value("different_gap").toString()); // 切换差(电接点一般压力表)
            result.append(
                query.value("max_point_deviation")
                    .toString()); // 最大设定点偏差绝对值(结果表)(电接点一般压力表的第2个结果表)
            result.append(
                query.value("allow_point_deviation")
                    .toString()); // 允许设定点偏差绝对值(结果表)(电接点一般压力表的第2个结果表)
            result.append(
                query.value("max_different_gap")
                    .toString()); // 最大切换差(结果表)(电接点一般压力表的第2个结果表)
            result.append(
                query.value("allow_different_gap")
                    .toString()); // 允许切换差(结果表)(电接点一般压力表的第2个结果表)
            result.append(
                query.value("zero_drift_one")
                    .toString()); // 零位漂移(数字压力计手工输入的第1个示值)
            result.append(
                query.value("zero_drift_two")
                    .toString()); // 零位漂移(数字压力计手工输入的第2个示值)
            result.append(
                query.value("zero_drift_three")
                    .toString()); // 零位漂移(数字压力计手工输入的第3个示值)
            result.append(
                query.value("zero_drift_four")
                    .toString()); // 零位漂移(数字压力计手工输入的第4个示值)
            result.append(
                query.value("zero_drift_five")
                    .toString()); // 零位漂移(数字压力计手工输入的第5个示值)
            result.append(query.value("intercept_a").toString()); // 截距a(结果表)
            result.append(query.value("slope_b").toString()); // 斜率b(结果表)
            result.append(
                query.value("sensitivity_b").toString()); // 灵敏度b(结果表)
            result.append(
                query.value("repeatability_r").toString()); // 重复性ξR(结果表)
            result.append(query.value("hysteresis_h").toString()); // 迟滞ξH(结果表)
            result.append(query.value("linear_l").toString()); // 线性ξL(结果表)
            result.append(
                query.value("basic_error_a").toString()); // 基本误差A(结果表)
            result.append(query.value("periodic_stability_s_b")
                              .toString()); // 周期稳定性Sb(结果表)
            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());
            resultData.append(result);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    *HeadList << "序号"
              << "检定数据管理基础信息表id"
              << "检定数据类型"
              << "标准器示值"
              << "轻敲前被检设备示值-正行程"
              << "轻敲前被检设备示值-反行程"
              << "轻敲后被检设备示值-正行程"
              << "轻敲后被检设备示值-反行程"
              << "理论输出值"
              << "第一次正行程"
              << "第一次反行程"
              << "第二次正行程"
              << "第二次反行程"
              << "第三次正行程"
              << "第三次反行程"
              << "示值误差"
              << "回程误差"
              << "正行程平均值"
              << "反行程平均值"
              << "平均值"
              << "回程误差平均值"
              << "项目(结果表)"
              << "示值误差最大值(结果表)"
              << "轻敲位移最大值(结果表)"
              << "最大允许误差绝对值"
              << "正行程切换值(电接点一般压力表)"
              << "反行程切换值(电接点一般压力表)"
              << "设定点偏差(电接点一般压力表)"
              << "切换差(电接点一般压力表)"
              << "最大设定点偏差绝对值(结果表)(电接点一般压力表的第2个结果表)"
              << "允许设定点偏差绝对值(结果表)(电接点一般压力表的第2个结果表)"
              << "最大切换差(结果表)(电接点一般压力表的第2个结果表)"
              << "允许切换差(结果表)(电接点一般压力表的第2个结果表)"
              << "零位漂移(数字压力计手工输入的第1个示值)"
              << "零位漂移(数字压力计手工输入的第2个示值)"
              << "零位漂移(数字压力计手工输入的第3个示值)"
              << "零位漂移(数字压力计手工输入的第4个示值)"
              << "零位漂移(数字压力计手工输入的第5个示值)"
              << "截距a(结果表)"
              << "斜率b(结果表)"
              << "灵敏度b(结果表)"
              << "重复性ξR(结果表)"
              << "迟滞ξH(结果表)"
              << "线性ξL(结果表)"
              << "基本误差A(结果表)"
              << "周期稳定性Sb(结果表)"
              << "创建日期"
              << "更新日期";
    return resultData;
};

// 检定数据管理-检定结果信息-直流稳压电源
QList<QStringList> BaseCommonApi::getbizBusinessDeviceMeasureDataDcPower(
    QString data_id, QStringList* HeadList)
{
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();

    QSqlQuery query;
    query.prepare(
        "SELECT * FROM biz_business_device_measure_data_dc_power WHERE data_id= "
        ":data_id ");
    // 将参数绑定到占位符
    query.bindValue(":data_id", data_id);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());

            result.append(
                query.value("data_id").toString()); // 检定数据管理基础信息表id

            result.append(
                query.value("data_type")
                    .toString()); // 检定数据类型(一个检定数据中区分多个表格)(字典code)

            result.append(
                query.value("data_type_type")
                    .toString()); // 检定项数据类型的类型(一个检定项中一个表格切换的类型)(字典value)

            result.append(query.value("output_channel").toString()); // 输出通道

            result.append(query.value("unit").toString()); // 单位

            result.append(query.value("voltage_regulator_output_value")
                              .toString()); // 调压器输出值

            result.append(query.value("voltage_max_difference")
                              .toString()); // 被检电源输出电压最大差值

            result.append(query.value("technical_index")
                              .toString()); // 被检电源技术指标/仪器技术指标

            result.append(query.value("technical_index_symbol")
                              .toString()); // 被检电源技术指标/仪器技术指标前符号

            result.append(
                query.value("voltage_regulation").toString()); // 电源电压调整率

            result.append(query.value("load_situation").toString()); // 负载情况

            result.append(query.value("voltage_output_value")
                              .toString()); // 被检电源输出电压值

            result.append(query.value("voltage_difference").toString()); // 电压差值

            result.append(query.value("load_regulation").toString()); // 负载调整率

            result.append(
                query.value("measure_indication_value").toString()); // 被检表示值

            result.append(query.value("standard_value").toString()); // 标准值

            result.append(query.value("maximum_error").toString()); // 最大允许误差

            result.append(
                query.value("maximum_error_unit").toString()); // 最大允许误差单位

            result.append(query.value("absolute_error").toString()); // 绝对误差

            result.append(query.value("resolution").toString()); // 分辨力

            result.append(
                query.value("standard_voltage_value").toString()); // 标准电压值

            result.append(query.value("standard_voltage_value_unit")
                              .toString()); // 标准电压值单位

            result.append(
                query.value("standard_resistance_value").toString()); // 标准电阻值

            result.append(query.value("standard_resistance_value_unit")
                              .toString()); // 标准电阻值单位

            result.append(query.value("ripple_voltage").toString()); // 纹波电压

            result.append(query.value("output_voltage_max_change")
                              .toString()); // 输出电压最大变化值

            result.append(
                query.value("short_term_stability").toString()); // 短期稳定性

            result.append(query.value("exceed_mark").toString()); // 超出范围标*(1/0)

            result.append(query.value("create_time").toString());

            result.append(query.value("update_time").toString());
        }
    }
    // 待修改
    *HeadList << "序号";
}

// 【查询】检定项直流稳压电源
QMap<QString, QList<QStringList>>
BaseCommonApi::getDeviceMeasureItemDataDcPower(
    QString itemCatoryName, QString itemId,
    SequentialMap<QString, QStringList>* headList)
{
    QMap<QString, QList<QStringList>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();

    QSqlQuery query;
    query.prepare(
        " SELECT "
        "bbdmiddp.*,bbdmii.appearance_function_check,bbdmii.voltage_regulation,"
        "bbdmii.load_regulation,"
        " bbdmii.voltage_represent_error,bbdmii.current_represent_error_direct,"
        "bbdmii.current_represent_error_indirect,"
        " bbdmii.ripple_voltage,bbdmii.voltage_output_stability "
        " FROM biz_business_device_measure_item_data_dc_power bbdmiddp "
        " JOIN biz_business_device_measure_item_info bbdmii ON bbdmii.id = "
        "bbdmiddp.item_id "
        " WHERE bbdmiddp.item_id =  :itemId "
        " ORDER BY bbdmiddp.create_time ASC ");
    // 将参数绑定到占位符
    query.bindValue(":itemId", itemId);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_id").toString()); // 检定项目ID
            result.append(
                query.value("data_type")
                    .toString()); // 检定项数据类型(一个检定项中区分两个表格)(字典code)
            result.append(
                query.value("data_type_type")
                    .toString()); // 检定项数据类型的类型(一个检定项中一个表格切换的类型)(字典code)
            result.append(query.value("output_channel").toString()); // 输出通道
            result.append(query.value("unit").toString()); // 单位
            result.append(query.value("voltage_regulator_output_value")
                              .toString()); // 调压器输出值
            result.append(query.value("technical_index")
                              .toString()); // 被检电源技术指标/仪器技术指标
            result.append(query.value("technical_index_symbol")
                              .toString()); // 被检电源技术指标/仪器技术指标前符号
            result.append(query.value("load_situation").toString()); // 负载情况
            result.append(
                query.value("measure_indication_value").toString()); // 被检表示值
            result.append(
                query.value("full_scale_value").toString()); // 指针式仪表满量程值
            result.append(
                query.value("standard_resistance_value").toString()); // 标准电阻值
            result.append(query.value("standard_resistance_value_unit")
                              .toString()); // 标准电阻值单位
            result.append(query.value("error_param_a").toString()); // 误差参数a
            result.append(query.value("accuracy_a").toString()); // 准确度a
            result.append(query.value("error_param_b").toString()); // 误差参数b
            result.append(query.value("maximum_error").toString()); // 最大允许误差
            result.append(query.value("resolution").toString()); // 分辨力
            result.append(query.value("appearance_function_check")
                              .toString()); // 外观及功能性检查(1/0)
            result.append(query.value("voltage_regulation")
                              .toString()); // 电源电压调整率(1/0)(直流稳压电源)
            result.append(query.value("load_regulation")
                              .toString()); // 负载调整率(1/0)(直流稳压电源)
            result.append(query.value("voltage_represent_error")
                              .toString()); // 电压表示值误差(1/0)(直流稳压电源)
            result.append(
                query.value("current_represent_error_direct")
                    .toString()); // 电流表示值误差(直接测量)(1/0)(直流稳压电源)
            result.append(
                query.value("current_represent_error_indirect")
                    .toString()); // 电流表示值误差(间接测量)(1/0)(直流稳压电源)
            result.append(query.value("ripple_voltage")
                              .toString()); // 纹波电压(1/0)(直流稳压电源)
            result.append(query.value("voltage_output_stability")
                              .toString()); // 输出电压短期稳定性(1/0)(直流稳压电源)
            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());
            resultData.append(result);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    // 进行检定项目列头转换
    DataMapList = GetDataReturnItemListByDcPower(itemCatoryName, resultData,
        DataMapList, headList);
    return DataMapList;
};
// 直流稳压电源 检定项头
QMap<QString, QList<QStringList>> BaseCommonApi::GetDataReturnItemListByDcPower(
    QString itemCatoryName, QList<QStringList> ResultData,
    QMap<QString, QList<QStringList>> DataMapList,
    SequentialMap<QString, QStringList>* HeadMapList)
{
    QList<QStringList> VpsAdjustmentRateList;
    QList<QStringList> LoadAdjustmentRateList;
    QList<QStringList> VoltageIndicationErrorList;
    QList<QStringList> CurrentIndicationErrorDirectList;
    QList<QStringList> CurrentIndicationErrorIndirectList;
    QList<QStringList> RippleVoltageList;
    QList<QStringList> ShortTermStabilityVoltageList;

    QStringList VpsAdjustmentRateHead;
    QStringList LoadAdjustmentRateHead;
    QStringList VoltageIndicationErrorHead;
    QStringList CurrentIndicationErrorDirectHead;
    QStringList CurrentIndicationErrorIndirectHead;
    QStringList RippleVoltageHead;
    QStringList ShortTermStabilityVoltageHead;

    if (itemCatoryName.contains("直流稳压电源")) {
        for (const QStringList& row : ResultData) {
            QString VerificationItem = NULL;
            if (!row.isEmpty() && row[20] == "1" && row[2] == "1") {
                VerificationItem = "电源电压调整率";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "输出通道"
                     << "单位"
                     << "调压器输出值"
                     << "被检电源输出电压值"
                     << "被检电源输出电压最大差值"
                     << "被检电源技术指标"
                     << "被检电源技术指标前符号"
                     << "电源电压调整率";
                VpsAdjustmentRateHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(row[2]); // 检定项目
                Data.append(row[4]); // 输出通道
                Data.append(row[5]); // 单位
                Data.append(row[6]); // 调压器输出值
                Data.append(""); // 被检电源输出电压值
                Data.append(""); // 被检电源输出电压最大差值  公式
                Data.append(row[8] + row[7]); // 被检电源技术指标
                Data.append(row[8]); // 被检电源技术指标单位
                Data.append(""); // 电源电压调整率   公式
                VpsAdjustmentRateList.append(Data);
            } else if (!row.isEmpty() && row[21] == "1" && row[2] == "2") {
                VerificationItem = "负载调整率";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "输出通道"
                     << "单位"
                     << "负载情况"
                     << "被检电源输出电压值"
                     << "电压差值"
                     << "被检电源技术指标"
                     << "负载调整率";

                LoadAdjustmentRateHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(row[2]); // 检定项目
                Data.append(row[4]); // 输出通道
                Data.append(row[5]); // 单位
                Data.append(row[9]); // 负载情况
                Data.append(""); // 被检电源输出电压值
                Data.append(""); // 电压差值
                Data.append(row[8] + row[7]); // 被检电源技术指标
                Data.append(""); // 负载调整率
                LoadAdjustmentRateList.append(Data);
            } else if (!row.isEmpty() && row[22] == "1" && row[2] == "3") {
                VerificationItem = "电压示值误差";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "输出通道"
                     << "单位"
                     << "被检表示值"
                     << "标准值"
                     << "最大允许误差"
                     << "绝对误差";

                VoltageIndicationErrorHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(row[2]); // 检定项目
                Data.append(row[4]); // 输出通道
                Data.append(row[5]); // 单位
                Data.append(row[10]); // 被检表示值
                Data.append(""); // 标准值
                Data.append(row[17]); // 最大允许误差
                Data.append(""); // 绝对误差
                VoltageIndicationErrorList.append(Data);
            } else if (!row.isEmpty() && row[23] == "1" && row[2] == "4") {
                VerificationItem = "电流示值误差(直接测量)";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "输出通道"
                     << "单位"
                     << "被检表示值"
                     << "标准值"
                     << "最大允许误差"
                     << "绝对误差";
                CurrentIndicationErrorDirectHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(row[2]); // 检定项目
                Data.append(row[4]); // 输出通道
                Data.append(row[5]); // 单位
                Data.append(row[10]); // 被检表示值
                Data.append(""); // 标准值
                Data.append(row[17]); // 最大允许误差
                Data.append(""); // 绝对误差
                CurrentIndicationErrorDirectList.append(Data);
            } else if (!row.isEmpty() && row[24] == "1" && row[2] == "5") {
                VerificationItem = "电流示值误差(间接测量)";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "输出通道"
                     << "单位"
                     << "被检表示值"
                     << "标准电压值"
                     << "标准电阻值"
                     << "标准电阻值单位"
                     << "标准值"
                     << "最大允许误差"
                     << "绝对误差";
                CurrentIndicationErrorIndirectHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(row[2]); // 检定项目
                Data.append(row[4]); // 输出通道
                Data.append(row[5]); // 单位
                Data.append(row[10]); // 被检表示值
                Data.append(""); // 标准电压值
                Data.append(row[12]); // 标准电阻值
                Data.append(row[13]); // 标准电阻值单位
                Data.append(""); // 标准值
                Data.append(row[17]); // 最大允许误差
                Data.append(""); // 绝对误差
                CurrentIndicationErrorIndirectList.append(Data);
            } else if (!row.isEmpty() && row[25] == "1" && row[2] == "6") {
                VerificationItem = "纹波电压";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "输出通道"
                     << "单位"
                     << "被检电源技术指标"
                     << "纹波电压";
                RippleVoltageHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(row[2]); // 检定项目
                Data.append(row[4]); // 输出通道
                Data.append(row[5]); // 单位
                Data.append(row[8] + row[7]); // 被检电源技术指标
                Data.append(""); // 纹波电压
                RippleVoltageList.append(Data);
            } else if (!row.isEmpty() && row[25] == "1" && row[2] == "7") {
                VerificationItem = "输出电压短期稳定性";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "输出通道"
                     << "单位"
                     << "被测电源输出电压"
                     << "输出电压最大变化值"
                     << "仪器技术指标"
                     << "短期稳定性";

                ShortTermStabilityVoltageHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(row[2]); // 检定项目
                Data.append(row[4]); // 输出通道
                Data.append(row[5]); // 单位
                Data.append(""); // 被测电源输出电压
                Data.append(""); // 输出电压最大变化值
                Data.append(row[8] + row[7]); // 仪器技术指标
                //            Data.append(row[8]);//仪器技术指标单位
                Data.append(""); // 短期稳定性
                ShortTermStabilityVoltageList.append(Data);
            }
        }
    }
    if (VpsAdjustmentRateList.count() != 0) {
        DataMapList.insert("电源电压调整率", VpsAdjustmentRateList);
        HeadMapList->insert("电源电压调整率", VpsAdjustmentRateHead);
    };
    if (LoadAdjustmentRateList.count() != 0) {
        DataMapList.insert("负载调整率", LoadAdjustmentRateList);
        HeadMapList->insert("负载调整率", LoadAdjustmentRateHead);
    };
    if (VoltageIndicationErrorList.count() != 0) {
        DataMapList.insert("电压示值误差", VoltageIndicationErrorList);
        HeadMapList->insert("电压示值误差", VoltageIndicationErrorHead);
    };
    if (CurrentIndicationErrorDirectList.count() != 0) {
        DataMapList.insert("电流示值误差(直接测量)",
            CurrentIndicationErrorDirectList);
        HeadMapList->insert("电流示值误差(直接测量)", CurrentIndicationErrorDirectHead);
    };
    if (CurrentIndicationErrorIndirectList.count() != 0) {
        DataMapList.insert("电流示值误差(间接测量)",
            CurrentIndicationErrorIndirectList);
        HeadMapList->insert("电流示值误差(间接测量)", CurrentIndicationErrorIndirectHead);
    };
    if (RippleVoltageList.count() != 0) {
        DataMapList.insert("纹波电压", RippleVoltageList);
        HeadMapList->insert("纹波电压", RippleVoltageHead);
    };
    if (ShortTermStabilityVoltageList.count() != 0) {
        DataMapList.insert("输出电压短期稳定性", ShortTermStabilityVoltageList);
        HeadMapList->insert("输出电压短期稳定性", ShortTermStabilityVoltageHead);
    };
    return DataMapList;
};
// 【结果保存】直流稳压电源
bool BaseCommonApi::InsertbizBusinessDeviceMeasureDataDcPower(
    bizBusinessDeviceMeasureDataDcPower power)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_business_device_measure_data_dc_power "
        "(id, data_id, data_type, data_type_type, output_channel, unit, "
        "voltage_regulator_output_value, voltage_max_difference, "
        "technical_index, technical_index_symbol, voltage_regulation, "
        "load_situation, voltage_output_value, voltage_difference, "
        "load_regulation, measure_indication_value, standard_value, "
        "maximum_error, maximum_error_unit, absolute_error, resolution, "
        "standard_voltage_value, standard_voltage_value_unit, "
        "standard_resistance_value, standard_resistance_value_unit, "
        "ripple_voltage, "
        "output_voltage_max_change, short_term_stability, exceed_mark, "
        "create_time, update_time) "
        "VALUES "
        "(:id, :data_id, :data_type, :data_type_type, :output_channel, :unit, "
        ":voltage_regulator_output_value, :voltage_max_difference, "
        ":technical_index, :technical_index_symbol, :voltage_regulation, "
        ":load_situation, :voltage_output_value, :voltage_difference, "
        ":load_regulation, :measure_indication_value, :standard_value, "
        ":maximum_error, :maximum_error_unit, :absolute_error, :resolution, "
        ":standard_voltage_value, :standard_voltage_value_unit, "
        ":standard_resistance_value, :standard_resistance_value_unit, "
        ":ripple_voltage, "
        ":output_voltage_max_change, :short_term_stability, :exceed_mark, "
        ":create_time, :update_time)");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", power.id);
    query.bindValue(":data_id", power.data_id);
    query.bindValue(":data_type", power.data_type);
    query.bindValue(":data_type_type", power.data_type_type);
    query.bindValue(":output_channel", power.output_channel);
    query.bindValue(":unit", power.unit);
    query.bindValue(":voltage_regulator_output_value",
        power.voltage_regulator_output_value);
    query.bindValue(":voltage_max_difference", power.voltage_max_difference);
    query.bindValue(":technical_index", power.technical_index);
    query.bindValue(":technical_index_symbol", power.technical_index_symbol);
    query.bindValue(":voltage_regulation", power.voltage_regulation);
    query.bindValue(":load_situation", power.load_situation);
    query.bindValue(":voltage_output_value", power.voltage_output_value);
    query.bindValue(":voltage_difference", power.voltage_difference);
    query.bindValue(":load_regulation", power.load_regulation);
    query.bindValue(":measure_indication_value", power.measure_indication_value);
    query.bindValue(":standard_value", power.standard_value);
    query.bindValue(":maximum_error", power.maximum_error);
    query.bindValue(":maximum_error_unit", power.maximum_error_unit);
    query.bindValue(":absolute_error", power.absolute_error);
    query.bindValue(":resolution", power.resolution);
    query.bindValue(":standard_voltage_value", power.standard_voltage_value);
    query.bindValue(":standard_voltage_value_unit",
        power.standard_voltage_value_unit);
    query.bindValue(":standard_resistance_value",
        power.standard_resistance_value);
    query.bindValue(":standard_resistance_value_unit",
        power.standard_resistance_value_unit);
    query.bindValue(":ripple_voltage", power.ripple_voltage);
    query.bindValue(":output_voltage_max_change",
        power.output_voltage_max_change);
    query.bindValue(":short_term_stability", power.short_term_stability);
    query.bindValue(":exceed_mark", power.exceed_mark);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (isSuccess) {
        QString ssql = "INSERT INTO biz_business_device_measure_data_dc_power "
                       "(id, data_id, data_type, data_type_type, output_channel, unit, "
                       "voltage_regulator_output_value, voltage_max_difference, "
                       "technical_index, technical_index_symbol, voltage_regulation, "
                       "load_situation, voltage_output_value, voltage_difference, "
                       "load_regulation, measure_indication_value, standard_value, "
                       "maximum_error, maximum_error_unit, absolute_error, resolution, "
                       "standard_voltage_value, standard_voltage_value_unit, "
                       "standard_resistance_value, standard_resistance_value_unit, "
                       "ripple_voltage, "
                       "output_voltage_max_change, short_term_stability, exceed_mark, "
                       "create_time, update_time) "
                       "VALUES "
                       "(:1, :2, :3, :4, :5, :6, :7, :8, "
                       ":9, :10, :11, :12, :13, :14, "
                       ":15, :16, :17, :18, :19, :20, :21, "
                       ":22, :23, :24, :25, :26, "
                       ":27, :28, :29, :30, :31);";
        saveLogToSql(ssql, &power);
    } else {
        qDebug() << "Insert data failed: " << query.lastError().text();
    }

    return isSuccess;
};

// 检定项 示波器检定装置
QMap<QString, QList<QStringList>>
BaseCommonApi::getDeviceOscilloscopeCalibration(
    QString itemCatoryName, QString itemId,
    SequentialMap<QString, QStringList>* headList)
{
    QMap<QString, QList<QStringList>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();

    QSqlQuery query;
    query.prepare(
        "SELECT *,bbdmii.band_width as BW  FROM "
        "biz_business_device_measure_item_data_oscilloscope bbdmido"
        "  JOIN biz_business_device_measure_item_info bbdmii ON bbdmii.id = "
        "bbdmido.item_id "
        " WHERE bbdmido.item_id = :itemId  ORDER BY bbdmido.create_time ASC");
    // 将参数绑定到占位符
    query.bindValue(":itemId", itemId);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_id").toString()); // 检定项目ID
            result.append(query.value("time_base").toString()); // 时基
            result.append(
                query.value("current_gain").toString()); // 直流增益(示波器-数字)
            result.append(
                query.value("current_bias").toString()); // 直流偏置(示波器-数字)
            result.append(
                query.value("BW").toString()); // 频带宽度 (示波器-数字/模拟)
            result.append(query.value("trigger_sensitivity")
                              .toString()); // 触发灵敏度(示波器-数字/模拟)
            result.append(
                query.value("rise_time").toString()); // 上升时间(示波器-数字/模拟)
            result.append(query.value("input_resistance")
                              .toString()); // 输入电阻 (示波器-数字/模拟)
            result.append(query.value("calibrating_signal")
                              .toString()); // 校准信号(示波器-数字/模拟)
            result.append(query.value("scan_time_coefficient")
                              .toString()); // 扫描时间系数及扩展(示波器-模拟)
            result.append(query.value("cycle_signal").toString()); // 周期时标信号
            result.append(
                query.value("cycle_signal_unit").toString()); // 周期时标信号单位
            result.append(query.value("technical_index").toString()); // 技术指标
            result.append(
                query.value("technical_index_unit").toString()); // 技术指标单位
            result.append(query.value("thoroughfare").toString()); // 通道
            result.append(query.value("impedance").toString()); // 阻抗
            result.append(query.value("impedance_unit").toString()); // 阻抗单位
            result.append(query.value("vertical_deflect_coefficient")
                              .toString()); // 垂直偏转系数/div
            result.append(query.value("vertical_deflect_coefficient_unit")
                              .toString()); // 垂直偏转系数单位
            result.append(query.value("standard_value").toString()); // 标准值
            result.append(
                query.value("standard_value_unit").toString()); // 标准值单位
            result.append(query.value("band_width").toString()); // 频带宽度
            result.append(query.value("band_width_unit").toString()); // 频带宽度单位
            result.append(query.value("amplitude").toString()); // 幅度
            result.append(query.value("amplitude_unit").toString()); // 幅度单位
            result.append(query.value("trigger_type").toString()); // 触发类型
            result.append(query.value("signal_frequency").toString()); // 信号频率
            result.append(query.value("nominal_value").toString()); // 标称值
            result.append(
                query.value("nominal_value_unit").toString()); // 标称值单位
            result.append(query.value("data_type").toString()); //
            result.append(query.value("item_type").toString()); //
            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());
            resultData.append(result);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    DataMapList = GetDataReturnItemListByOscilloscope(itemCatoryName, resultData,
        DataMapList, headList);
    return DataMapList;
};
QMap<QString, QList<QStringList>>
BaseCommonApi::GetDataReturnItemListByOscilloscope(
    QString itemCatoryName, QList<QStringList> ResultData,
    QMap<QString, QList<QStringList>> DataMapList,
    SequentialMap<QString, QStringList>* HeadMapList)
{
    QList<QStringList> time_baseList;
    QList<QStringList> Current_gainList;
    QList<QStringList> Current_biasList;
    QList<QStringList> band_widthList;
    QList<QStringList> trigger_sensitivity;
    QList<QStringList> rise_timeList;
    QList<QStringList> input_resistanceList;
    QList<QStringList> calibrating_signalList;

    QStringList time_baseHead;
    QStringList Current_gainHead;
    QStringList Current_biasHead;
    QStringList band_widthHead;
    QStringList trigger_sensitivityHead;
    QStringList rise_timeHead;
    QStringList input_resistanceHead;
    QStringList calibrating_signalHead;
    if (itemCatoryName.contains("数字示波器")) {
        for (const QStringList& row : ResultData) {
            QString VerificationItem = NULL;
            QString CHNAME = "CH1";
            if (!row.isEmpty() && row[2] == "1" && row[30] == "1") {
                VerificationItem = "时基";
                QStringList head;

                head << "序号"
                     << "检定项目"
                     << "周期时标信号"
                     << "时基绝对误差△T/ns"
                     << "时基相对误差△δ"
                     << "技术指标"
                     << "结论";
                time_baseHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;

                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[11] + row[12]); // 周期时标信号
                Data.append(""); // 时基绝对误差△T/ns
                Data.append(""); // 时基相对误差△δ
                Data.append(""); // 技术指标
                Data.append(""); // 结论
                time_baseList.append(Data);
            }
            if (!row.isEmpty() && row[3] == "1" && row[30] == "2") {
                VerificationItem = "直流增益";
                QStringList head;
                // 添加列头
                head << "序号"
                     << "检定项目"
                     << "通道"
                     << "阻抗"
                     << "幅度/DIV"
                     << "标准值"
                     << "测量值"
                     << "相对误差"
                     << "技术指标"
                     << "结论";
                Current_gainHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[15]); // 通道
                Data.append(row[16] + row[17]); // 阻抗
                Data.append(row[18] + row[19]); // 垂直偏转系数(/div)
                Data.append(row[20] + row[21]); // 标准值
                Data.append(""); // 测量值
                Data.append(""); // 相对误差
                Data.append(row[13] + row[14]); // 技术指标
                Data.append(""); // 结论
                Current_gainList.append(Data);
            }
            if (!row.isEmpty() && row[4] == "1" && row[30] == "3") {
                VerificationItem = "直流偏置";
                QStringList head;
                // 添加列头
                head << "序号"
                     << "检定项目"
                     << "通道"
                     << "阻抗"
                     << "幅度/DIV"
                     << "标准值"
                     << "测量值"
                     << "误差"
                     << "相对误差"
                     << "技术指标"
                     << "结论";

                Current_biasHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[15]); // 通道
                Data.append(row[16] + row[17]); // 阻抗
                Data.append(row[18] + row[19]); // 垂直偏转系数(/div)
                Data.append(row[20] + row[21]); // 标准值
                Data.append(""); // 测量值
                Data.append(""); // 误差
                Data.append(""); // 相对误差
                Data.append(row[13] + row[14]); // 技术指标
                Data.append(""); // 结论
                Current_biasList.append(Data);
            }
            if (!row.isEmpty() && row[5] == "1" && row[30] == "4") {
                VerificationItem = "频带宽度";
                QStringList head;
                // 添加列头
                head << "序号"
                     << "检定项目"
                     << "通道"
                     << "阻抗"
                     << "幅度/DIV"
                     << "标准值"
                     << "频带宽度"
                     << "参考幅度"
                     << "参考幅度单位"
                     << "测量幅度"
                     << "测量幅度单位"
                     << "分贝数(dB)"
                     << "技术指标"
                     << "结论";
                band_widthHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[15]); // 通道
                Data.append(row[16] + row[17]); // 阻抗
                Data.append(row[18] + row[19]); // 垂直偏转系数(/div)
                Data.append(row[20] + row[21]); // 标准值
                Data.append(row[22] + row[23]); // 频带宽度
                Data.append(""); // 参考幅度
                Data.append(row[19]); // 参考幅度单位
                Data.append(""); // 测量幅度
                Data.append(row[19]); // 测量幅度单位
                Data.append(""); // 分贝数(dB)
                Data.append(row[13] + row[14]); // 技术指标
                Data.append(""); // 结论
                band_widthList.append(Data);
            }
            if (!row.isEmpty() && row[6] == "1" && row[30] == "5") {
                VerificationItem = "触发灵敏度";
                QStringList head;
                // 添加列头
                head << "序号"
                     << "检定项目"
                     << "通道"
                     << "触发类型"
                     << "信号频率"
                     << "标称值"
                     << "测量值"
                     << "结论";

                trigger_sensitivityHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[15]); // 通道
                Data.append(row[26]); // 触发类型
                Data.append(row[27]); // 信号频率
                Data.append(row[28] + row[29]); // 标称值
                Data.append(""); // 测量值
                Data.append(""); // 结论
                trigger_sensitivity.append(Data);
            }
            if (!row.isEmpty() && row[7] == "1" && row[30] == "6") {
                VerificationItem = "上升时间";
                QStringList head;
                // 添加列头
                head << "序号"
                     << "检定项目"
                     << "通道"
                     << "阻抗"
                     << "幅度/DIV"
                     << "标准值"
                     << "测量值"
                     << "单位"
                     << "技术指标"
                     << "结论";
                rise_timeHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[15]); // 通道
                Data.append(row[16] + row[17]); // 阻抗
                Data.append(row[18] + row[19]); // 垂直偏转系数(/div)
                Data.append(row[20] + row[21]); // 标准值
                Data.append(""); // 测量值
                Data.append("ns"); // 测量值单位
                Data.append(row[13] + row[14]); // 技术指标
                Data.append(""); // 结论
                rise_timeList.append(Data);
            }
            if (!row.isEmpty() && row[8] == "1" && row[30] == "7") {
                VerificationItem = "输入电阻";
                QStringList head;
                // 添加列头
                head << "序号"
                     << "检定项目"
                     << "通道"
                     << "标称值"
                     << "幅度/DIV"
                     << "测量值"
                     << "测量值单位"
                     << "技术指标"
                     << "结论";
                input_resistanceHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[15]); // 通道
                Data.append(row[28] + row[29]); // 标称值
                Data.append(row[18] + row[19]); // 垂直偏转系数(/div)
                Data.append(""); // 测量值
                Data.append(""); // 测量值单位
                Data.append(row[13] + row[14]); // 技术指标
                Data.append(""); // 结论
                input_resistanceList.append(Data);
            }
            if (!row.isEmpty() && row[9] == "1" && row[30] == "8") {
                VerificationItem = "校准信号";
                QStringList head;
                // 添加列头
                head << "序号"
                     << "检定项目"
                     << "项目"
                     << "标称值"
                     << "顶部测量值"
                     << "底部测量值"
                     << "测量值"
                     << "测量值单位"
                     << "相对误差";

                calibrating_signalHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[31]); // 项目
                Data.append(row[28] + row[29]); // 标称值
                Data.append(""); // 顶部测量值
                Data.append(""); // 底部测量值
                Data.append(""); // 测量值
                Data.append(""); // 测量值单位

                Data.append(""); // 相对误差
                calibrating_signalList.append(Data);
            }
        }
        if (time_baseList.count() != 0) {
            DataMapList.insert("时基", time_baseList);
            HeadMapList->insert("时基", time_baseHead);
        }
        if (Current_gainList.count() != 0) {
            DataMapList.insert("直流增益", Current_gainList);
            HeadMapList->insert("直流增益", Current_gainHead);
        }
        if (Current_biasList.count() != 0) {
            DataMapList.insert("直流偏置", Current_biasList);
            HeadMapList->insert("直流偏置", Current_biasHead);
        }
        if (band_widthList.count() != 0) {
            DataMapList.insert("频带宽度", band_widthList);
            HeadMapList->insert("频带宽度", band_widthHead);
        }
        if (trigger_sensitivity.count() != 0) {
            DataMapList.insert("触发灵敏度", trigger_sensitivity);
            HeadMapList->insert("触发灵敏度", trigger_sensitivityHead);
        }
        if (rise_timeList.count() != 0) {
            DataMapList.insert("上升时间", rise_timeList);
            HeadMapList->insert("上升时间", rise_timeHead);
        }
        if (input_resistanceList.count() != 0) {
            DataMapList.insert("输入电阻", input_resistanceList);
            HeadMapList->insert("输入电阻", input_resistanceHead);
        }
        if (calibrating_signalList.count() != 0) {
            DataMapList.insert("校准信号", calibrating_signalList);
            HeadMapList->insert("校准信号", calibrating_signalHead);
        }
    }
    return DataMapList;
};
// 【结果保存】 示波器
bool BaseCommonApi::InsertDeviceOscilloscopeCalibration(
    DeviceOscilloscopeCalibration scope)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_business_device_measure_data_oscilloscope ( "
        " id, data_id, data_type, cycle_signal, cycle_signal_unit,"
        " time_base_absolute_error, time_base_absolute_error_unit,"
        " time_base_relative_error, time_base_relative_error_unit,"
        " technical_index_symbol, technical_index, technical_index_unit,"
        " conclusion, thoroughfare, impedance, impedance_unit,"
        " vertical_deflect_coefficient, vertical_deflect_coefficient_unit,"
        " standard_value, standard_value_unit, measure_value, measure_value_unit,"
        " relative_error, error_value, band_width, band_width_unit,"
        " reference_amplitude, reference_amplitude_unit, measure_amplitude,"
        " measure_amplitude_unit, decibel_number, trigger_type,"
        " signal_frequency, signal_frequency_unit, item_type, nominal_value,"
        " nominal_value_unit, top_measure_value, top_measure_value_unit,"
        " bottom_measure_value, bottom_measure_value_unit, scan_type,"
        " horizontal_scale_gear, horizontal_scale_gear_unit,"
        " vertical_scale_gear, vertical_scale_gear_unit, frequency_upper,"
        " frequency_upper_unit, output_signal_amplitude, rise_time,"
        " correct_rise_time, signal_type, nominal_amplitude_value,"
        " nominal_amplitude_value_unit, nominal_frequency_value,"
        " nominal_frequency_value_unit, measure_amplitude_value,"
        " measure_amplitude_value_unit, measure_frequency_value,"
        " frequency_deviation, amplitude_deviation, exceed_mark, create_time,"
        " update_time"
        " ) VALUES ("
        " :id, :data_id, :data_type, :cycle_signal, :cycle_signal_unit,"
        " :time_base_absolute_error, :time_base_absolute_error_unit,"
        " :time_base_relative_error, :time_base_relative_error_unit,"
        " :technical_index_symbol, :technical_index, :technical_index_unit,"
        " :conclusion, :thoroughfare, :impedance, :impedance_unit,"
        " :vertical_deflect_coefficient, :vertical_deflect_coefficient_unit,"
        " :standard_value, :standard_value_unit, :measure_value, "
        ":measure_value_unit,"
        " :relative_error, :error_value, :band_width, :band_width_unit,"
        " :reference_amplitude, :reference_amplitude_unit, :measure_amplitude,"
        " :measure_amplitude_unit, :decibel_number, :trigger_type,"
        " :signal_frequency, :signal_frequency_unit, :item_type, :nominal_value,"
        " :nominal_value_unit, :top_measure_value, :top_measure_value_unit,"
        " :bottom_measure_value, :bottom_measure_value_unit, :scan_type,"
        " :horizontal_scale_gear, :horizontal_scale_gear_unit,"
        " :vertical_scale_gear, :vertical_scale_gear_unit, :frequency_upper,"
        " :frequency_upper_unit, :output_signal_amplitude, :rise_time,"
        " :correct_rise_time, :signal_type, :nominal_amplitude_value,"
        " :nominal_amplitude_value_unit, :nominal_frequency_value,"
        " :nominal_frequency_value_unit, :measure_amplitude_value,"
        " :measure_amplitude_value_unit, :measure_frequency_value,"
        " :frequency_deviation, :amplitude_deviation, :exceed_mark, :create_time,"
        " :update_time );");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", scope.id);
    query.bindValue(":data_id", scope.data_id);
    query.bindValue(":data_type", scope.data_type);
    query.bindValue(":cycle_signal", scope.cycle_signal);
    query.bindValue(":cycle_signal_unit", scope.cycle_signal_unit);
    query.bindValue(":time_base_absolute_error", scope.time_base_absolute_error);
    query.bindValue(":time_base_absolute_error_unit",
        scope.time_base_absolute_error_unit);
    query.bindValue(":time_base_relative_error", scope.time_base_relative_error);
    query.bindValue(":time_base_relative_error_unit",
        scope.time_base_relative_error_unit);
    query.bindValue(":technical_index_symbol", scope.technical_index_symbol);
    query.bindValue(":technical_index", scope.technical_index);
    query.bindValue(":technical_index_unit", scope.technical_index_unit);
    query.bindValue(":conclusion", scope.conclusion);
    query.bindValue(":thoroughfare", scope.thoroughfare);
    query.bindValue(":impedance", scope.impedance);
    query.bindValue(":impedance_unit", scope.impedance_unit);
    query.bindValue(":vertical_deflect_coefficient",
        scope.vertical_deflect_coefficient);
    query.bindValue(":vertical_deflect_coefficient_unit",
        scope.vertical_deflect_coefficient_unit);
    query.bindValue(":standard_value", scope.standard_value);
    query.bindValue(":standard_value_unit", scope.standard_value_unit);
    query.bindValue(":measure_value", scope.measure_value);
    query.bindValue(":measure_value_unit", scope.measure_value_unit);
    query.bindValue(":relative_error", scope.relative_error);
    query.bindValue(":error_value", scope.error_value);
    query.bindValue(":band_width", scope.band_width);
    query.bindValue(":band_width_unit", scope.band_width_unit);
    query.bindValue(":reference_amplitude", scope.reference_amplitude);
    query.bindValue(":reference_amplitude_unit", scope.reference_amplitude_unit);
    query.bindValue(":measure_amplitude", scope.measure_amplitude);
    query.bindValue(":measure_amplitude_unit", scope.measure_amplitude_unit);
    query.bindValue(":decibel_number", scope.decibel_number);
    query.bindValue(":trigger_type", scope.trigger_type);
    query.bindValue(":signal_frequency", scope.signal_frequency);
    query.bindValue(":signal_frequency_unit", scope.signal_frequency_unit);
    query.bindValue(":item_type", scope.item_type);
    query.bindValue(":nominal_value", scope.nominal_value);
    query.bindValue(":nominal_value_unit", scope.nominal_value_unit);
    query.bindValue(":top_measure_value", scope.top_measure_value);
    query.bindValue(":top_measure_value_unit", scope.top_measure_value_unit);
    query.bindValue(":bottom_measure_value", scope.bottom_measure_value);
    query.bindValue(":bottom_measure_value_unit",
        scope.bottom_measure_value_unit);
    query.bindValue(":scan_type", scope.scan_type);
    query.bindValue(":horizontal_scale_gear", scope.horizontal_scale_gear);
    query.bindValue(":horizontal_scale_gear_unit",
        scope.horizontal_scale_gear_unit);
    query.bindValue(":vertical_scale_gear", scope.vertical_scale_gear);
    query.bindValue(":vertical_scale_gear_unit", scope.vertical_scale_gear_unit);
    query.bindValue(":frequency_upper", scope.frequency_upper);
    query.bindValue(":frequency_upper_unit", scope.frequency_upper_unit);
    query.bindValue(":output_signal_amplitude", scope.output_signal_amplitude);
    query.bindValue(":rise_time", scope.rise_time);
    query.bindValue(":correct_rise_time", scope.correct_rise_time);
    query.bindValue(":signal_type", scope.signal_type);
    query.bindValue(":nominal_amplitude_value", scope.nominal_amplitude_value);
    query.bindValue(":nominal_amplitude_value_unit",
        scope.nominal_amplitude_value_unit);
    query.bindValue(":nominal_frequency_value", scope.nominal_frequency_value);
    query.bindValue(":nominal_frequency_value_unit",
        scope.nominal_frequency_value_unit);
    query.bindValue(":measure_amplitude_value", scope.measure_amplitude_value);
    query.bindValue(":measure_amplitude_value_unit",
        scope.measure_amplitude_value_unit);
    query.bindValue(":measure_frequency_value", scope.measure_frequency_value);
    query.bindValue(":measure_frequency_value_unit",
        scope.measure_frequency_value_unit);
    query.bindValue(":frequency_deviation", scope.frequency_deviation);
    query.bindValue(":amplitude_deviation", scope.amplitude_deviation);
    query.bindValue(":exceed_mark", scope.exceed_mark);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (isSuccess) {
        QString ssql = "INSERT INTO biz_business_device_measure_data_oscilloscope ( "
                       " id, data_id, data_type, cycle_signal, cycle_signal_unit,"
                       " time_base_absolute_error, time_base_absolute_error_unit,"
                       " time_base_relative_error, time_base_relative_error_unit,"
                       " technical_index_symbol, technical_index, technical_index_unit,"
                       " conclusion, thoroughfare, impedance, impedance_unit,"
                       " vertical_deflect_coefficient, vertical_deflect_coefficient_unit,"
                       " standard_value, standard_value_unit, measure_value, "
                       "measure_value_unit,"
                       " relative_error, error_value, band_width, band_width_unit,"
                       " reference_amplitude, reference_amplitude_unit, measure_amplitude,"
                       " measure_amplitude_unit, decibel_number, trigger_type,"
                       " signal_frequency, signal_frequency_unit, item_type, nominal_value,"
                       " nominal_value_unit, top_measure_value, top_measure_value_unit,"
                       " bottom_measure_value, bottom_measure_value_unit, scan_type,"
                       " horizontal_scale_gear, horizontal_scale_gear_unit,"
                       " vertical_scale_gear, vertical_scale_gear_unit, frequency_upper,"
                       " frequency_upper_unit, output_signal_amplitude, rise_time,"
                       " correct_rise_time, signal_type, nominal_amplitude_value,"
                       " nominal_amplitude_value_unit, nominal_frequency_value,"
                       " nominal_frequency_value_unit, measure_amplitude_value,"
                       " measure_amplitude_value_unit, measure_frequency_value,"
                       " frequency_deviation, amplitude_deviation, exceed_mark, create_time,"
                       " update_time"
                       " ) VALUES ("
                       " :1, :2, :3, :4, :5,"
                       " :6, :7,"
                       " :8, :9,"
                       " :10, :11, :12,"
                       " :13, :14, :15, :16,"
                       " :17, :18,"
                       " :19, :20, :21, :22,"
                       " :23, :24, :25, :26,"
                       " :27, :28, :29,"
                       " :30, :31, :32,"
                       " :33, :34, :35, :36,"
                       " :37, :38, :39,"
                       " :40, :41, :42,"
                       " :43, :44,"
                       " :45, :46, :47,"
                       " :48, :49, :50,"
                       " :51, :52, :53,"
                       " :54, :55,"
                       " :56, :57,"
                       " :58, :59,"
                       " :60, :61, :62, :63,"
                       " :64 );";
        saveLogToSql(ssql, &scope);
    } else {
        qDebug() << "Insert data failed: " << query.lastError().text();
    }

    return isSuccess;
}
// 检定项-二等铂电阻
QMap<QString, QList<QStringList>>
BaseCommonApi::getDeviceMeasureItemDataResistanceThermometer(
    QString itemCatoryName, QString itemId,
    SequentialMap<QString, QStringList>* headList)
{
    QMap<QString, QList<QStringList>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();

    QSqlQuery query;
    query.prepare(
        " SELECT "
        "bbdmidrt.*,bbdmii.wave_even_test,bbdmii.insulate_resistance,bbdmii."
        "angle_adjust_error,bbdmii.indication_error,"
        " bbdmii.allow_error FROM "
        "biz_business_device_measure_item_data_resistance_thermometer bbdmidrt"
        " JOIN biz_business_device_measure_item_info bbdmii ON bbdmii.id = "
        "bbdmidrt.item_id"
        " WHERE bbdmidrt.item_id = :itemId"
        " ORDER BY bbdmidrt.create_time ASC ");
    // 将参数绑定到占位符   1753003382429302786 ->均匀性
    query.bindValue(":itemId", itemId);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_id").toString()); // 检定项id
            result.append(
                query.value("data_type")
                    .toString()); // 检定项数据类型(一个检定项中区分两个表格)(字典code)
            result.append(query.value("unit").toString()); // 单位(字典value)
            result.append(query.value("upper_index").toString()); // 指标上限
            result.append(query.value("lower_index").toString()); // 指标下限
            result.append(
                query.value("measure_range_upper").toString()); // 测量范围上限
            result.append(
                query.value("measure_range_lower").toString()); // 测量范围下限
            result.append(query.value("measure_range_unit")
                              .toString()); // 测量范围单位(字典value)
            result.append(
                query.value("accuracy_level").toString()); // 准确度等级(字典value)
            result.append(query.value("calibrated_point").toString()); // 校准点
            result.append(query.value("maximum_error").toString()); // 最大允许误差
            result.append(query.value("urel").toString()); // 扩展不确定度
            result.append(query.value("level").toString()); // 级别(字典value)
            result.append(query.value("nominal_temperature").toString()); // 标称温度
            result.append(
                query.value("nominal_value").toString()); // α标称值(字典value)
            result.append(query.value("r_value").toString()); // R值
            result.append(query.value("dr_dt").toString()); // dR/dt
            result.append(query.value("r_tp").toString()); // R*tp
            result.append(query.value("w_s").toString()); // Ws
            result.append(query.value("dwst_dt").toString()); // dWst/dt
            result.append(query.value("allow_error_upper").toString()); // 允差上限
            result.append(query.value("allow_error_lower").toString()); // 允差下限
            result.append(
                query.value("a_allow_error_upper").toString()); // α允差上限
            result.append(
                query.value("a_allow_error_lower").toString()); // α允差下限
            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());
            result.append(query.value("wave_even_test")
                              .toString()); // 波动性测试和均匀性测试(二等铂)    27
            result.append(
                query.value("insulate_resistance").toString()); // 绝缘电阻(二等铂)
            result.append(query.value("angle_adjust_error")
                              .toString()); // 角度调整误差(二等铂)
            result.append(
                query.value("indication_error").toString()); // 示值误差(二等铂)
            result.append(query.value("allow_error").toString()); // 允差(二等铂)
            resultData.append(result);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }

    // 进行检定项目列头转换
    DataMapList = GetDataReturnItemListByResistanceThermometer(
        itemCatoryName, resultData, DataMapList, headList);
    return DataMapList;
}
QMap<QString, QList<QStringList>>
BaseCommonApi::GetDataReturnItemListByResistanceThermometer(
    QString itemCatoryName, QList<QStringList> ResultData,
    QMap<QString, QList<QStringList>> DataMapList,
    SequentialMap<QString, QStringList>* HeadMapList)
{
    QList<QStringList> VolatilityList;
    QList<QStringList> UniformityList;
    if (itemCatoryName.contains("恒温槽")) {
        for (const QStringList& row : ResultData) {
            QString VerificationItem = NULL;
            bool showUpperLimit = false;
            int num = 0;
            for (int i = 0; i < 16; i++) {
                if (!row.isEmpty() && row[27] == "1") {
                    VerificationItem = "波动性测试";
                    // 添加列头
                    QStringList head;
                    head << "序号"
                         << "下限点"
                         << "上限点"
                         << "单位"
                         << "示值1"
                         << "示值2"
                         << "示值3"
                         << "示值4"
                         << "示值5"
                         << "示值6"
                         << "示值7"
                         << "示值8"
                         << "示值9"
                         << "示值10"
                         << "示值11"
                         << "示值12"
                         << "示值13"
                         << "示值14"
                         << "示值15"
                         << "最大值"
                         << "最小值"
                         << "波动性";
                    HeadMapList->insert(VerificationItem, head);
                    // 添加数据
                    QStringList Data;
                    Data.append(row[0]); // ID
                    Data.append(showUpperLimit ? "" : row[5]); // 下限点
                    Data.append(showUpperLimit ? row[4] : ""); // 上限点
                    Data.append(row[3]); // 单位
                    Data.append(""); // 示值1
                    Data.append(""); // 示值2
                    Data.append(""); // 示值3
                    Data.append(""); // 示值4
                    Data.append(""); // 示值5
                    Data.append(""); // 示值6
                    Data.append(""); // 示值7
                    Data.append(""); // 示值8
                    Data.append(""); // 示值9
                    Data.append(""); // 示值10
                    Data.append(""); // 示值11
                    Data.append(""); // 示值12
                    Data.append(""); // 示值13
                    Data.append(""); // 示值14
                    Data.append(""); // 示值15
                    Data.append(""); // 最大值
                    Data.append(""); // 最小值
                    Data.append(""); // 波动性
                    VolatilityList.append(Data);
                }
                if (!row.isEmpty() && row[27] == "1") {
                    int d_num = 1;
                    d_num = showUpperLimit ? num++ : i;
                    QChar character('A' + d_num);
                    VerificationItem = "均匀性测试";
                    // 添加列头
                    QStringList head;
                    head << "序号"
                         << "下限点"
                         << "上限点"
                         << "单位"
                         << "位置"
                         << "示值ROA1"
                         << "示值RA1"
                         << "RA2"
                         << "ROA2"
                         << "ROA3"
                         << "RA3"
                         << "RA4"
                         << "ROA4"
                         << "ROA均值"
                         << "RA均值"
                         << "上水平面温差"
                         << "下水平面温差"
                         << "水平最大温差"
                         << "工作区域最大温差";
                    HeadMapList->insert(VerificationItem, head);
                    // 添加数据
                    QStringList Data;
                    Data.append(row[0]); // ID
                    Data.append(showUpperLimit ? "" : row[5]); // 下限点
                    Data.append(showUpperLimit ? row[4] : ""); // 上限点
                    Data.append(row[3]); // 单位
                    Data.append(character); // 位置
                    Data.append(""); // 示值ROA1
                    Data.append(""); // 示值RA1
                    Data.append(""); // RA2
                    Data.append(""); // ROA2
                    Data.append(""); // ROA3
                    Data.append(""); // RA3
                    Data.append(""); // RA4
                    Data.append(""); // ROA4
                    Data.append(""); // ROA均值
                    Data.append(""); // RA均值
                    Data.append(""); // 上水平面温差
                    Data.append(""); // 下水平面温差
                    Data.append(""); // 水平最大温差
                    Data.append(""); // 工作区域最大温差
                    UniformityList.append(Data);
                }
                if (i >= 7) {
                    showUpperLimit = true;
                }
            }
        }
    }
    if (VolatilityList.count() != 0) {
        DataMapList.insert("波动性测试", VolatilityList);
    };
    if (UniformityList.count() != 0) {
        DataMapList.insert("均匀性测试", UniformityList);
    };
    return DataMapList;
}

QMap<QString, QList<QStringList>>
GetDataReturnItemListByResistanceThermometerToVolatility(
    QString itemCatoryName, QList<QStringList> ResultData,
    QMap<QString, QList<QStringList>> DataMapList,
    QMap<QString, QStringList>* HeadMapList)
{
    QList<QStringList> UpperList;
    QList<QStringList> LowerList;
    for (const QStringList& row : ResultData)
        for (int i = 0; i < 8; i++) {
            {
                QString VerificationItem = NULL;
                VerificationItem = "波动性测试";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "下限点"
                     << "单位"
                     << "示值1"
                     << "示值2"
                     << "示值3"
                     << "示值4"
                     << "示值5"
                     << "示值6"
                     << "示值7"
                     << "示值8"
                     << "示值9"
                     << "示值10"
                     << "示值11"
                     << "示值12"
                     << "示值13"
                     << "示值14"
                     << "示值15"
                     << "最大值"
                     << "最小值"
                     << "波动性";
                HeadMapList->insert(VerificationItem, head);
                // 添加数据
                QStringList Data;
            }
        }
}

// 结果保存-二等铂电阻
bool BaseCommonApi::InsertDeviceMeasureDataResistanceThermometer(
    DeviceMeasureDataResistanceThermometer thermometer)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_business_device_measure_data_resistance_thermometer ("
        " id, data_id, data_type, data_category, upper_lower_point, location, "
        "unit,"
        " one_value, two_value, three_value, four_value, five_value, six_value, "
        "seven_value,"
        " eight_value, nine_value, ten_value, eleven_value, twelve_value, "
        "thirteen_value,"
        " fourteen_value, fifteen_value, max_value, min_value, volatility,"
        " upper_temperature_different, lower_temperature_different,"
        " max_horizontal_temperature_different, max_work_temperature_different,"
        " calibrated_point, indicating_value_forward_stroke,"
        " indicating_value_reverse_stroke, "
        "measure_indication_value_forward_stroke,"
        " measure_indication_value_reverse_stroke, "
        "indicating_error_forward_stroke,"
        " indicating_error_reverse_stroke, maximum_error, urel, rank_level,"
        " nominal_temperature, item, allow_error_lower, allow_error_upper,"
        " measure_value, create_time, update_time"
        " ) VALUES ("
        " :id, :data_id, :data_type, :data_category, :upper_lower_point, "
        ":location, :unit,"
        " :one_value, :two_value, :three_value, :four_value, :five_value, "
        ":six_value, :seven_value,"
        " :eight_value, :nine_value, :ten_value, :eleven_value, :twelve_value, "
        ":thirteen_value,"
        " :fourteen_value, :fifteen_value, :max_value, :min_value, :volatility,"
        " :upper_temperature_different, :lower_temperature_different,"
        " :max_horizontal_temperature_different, :max_work_temperature_different,"
        " :calibrated_point, :indicating_value_forward_stroke,"
        " :indicating_value_reverse_stroke, "
        ":measure_indication_value_forward_stroke,"
        " :measure_indication_value_reverse_stroke, "
        ":indicating_error_forward_stroke,"
        " :indicating_error_reverse_stroke, :maximum_error, :urel, :rank_level,"
        " :nominal_temperature, :item, :allow_error_lower, :allow_error_upper,"
        " :measure_value, :create_time, :update_time );");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", thermometer.id);
    query.bindValue(":data_id", thermometer.data_id);
    query.bindValue(":data_type", thermometer.data_type);
    query.bindValue(":data_category", thermometer.data_category);
    query.bindValue(":upper_lower_point", thermometer.upper_lower_point);
    query.bindValue(":location", thermometer.location);
    query.bindValue(":unit", thermometer.unit);
    query.bindValue(":one_value", thermometer.one_value);
    query.bindValue(":two_value", thermometer.two_value);
    query.bindValue(":three_value", thermometer.three_value);
    query.bindValue(":four_value", thermometer.four_value);
    query.bindValue(":five_value", thermometer.five_value);
    query.bindValue(":six_value", thermometer.six_value);
    query.bindValue(":seven_value", thermometer.seven_value);
    query.bindValue(":eight_value", thermometer.eight_value);
    query.bindValue(":nine_value", thermometer.nine_value);
    query.bindValue(":ten_value", thermometer.ten_value);
    query.bindValue(":eleven_value", thermometer.eleven_value);
    query.bindValue(":twelve_value", thermometer.twelve_value);
    query.bindValue(":thirteen_value", thermometer.thirteen_value);
    query.bindValue(":fourteen_value", thermometer.fourteen_value);
    query.bindValue(":fifteen_value", thermometer.fifteen_value);
    query.bindValue(":max_value", thermometer.max_value);
    query.bindValue(":min_value", thermometer.min_value);
    query.bindValue(":volatility", thermometer.volatility);
    query.bindValue(":upper_temperature_different",
        thermometer.upper_temperature_different);
    query.bindValue(":lower_temperature_different",
        thermometer.lower_temperature_different);
    query.bindValue(":max_horizontal_temperature_different",
        thermometer.max_horizontal_temperature_different);
    query.bindValue(":max_work_temperature_different",
        thermometer.max_work_temperature_different);
    query.bindValue(":calibrated_point", thermometer.calibrated_point);
    query.bindValue(":indicating_value_forward_stroke",
        thermometer.indicating_value_forward_stroke);
    query.bindValue(":indicating_value_reverse_stroke",
        thermometer.indicating_value_reverse_stroke);
    query.bindValue(":measure_indication_value_forward_stroke",
        thermometer.measure_indication_value_forward_stroke);
    query.bindValue(":measure_indication_value_reverse_stroke",
        thermometer.measure_indication_value_reverse_stroke);
    query.bindValue(":indicating_error_forward_stroke",
        thermometer.indicating_error_forward_stroke);
    query.bindValue(":indicating_error_reverse_stroke",
        thermometer.indicating_error_reverse_stroke);
    query.bindValue(":maximum_error", thermometer.maximum_error);
    query.bindValue(":urel", thermometer.urel);
    query.bindValue(":rank_level", thermometer.rank_level);
    query.bindValue(":nominal_temperature", thermometer.nominal_temperature);
    query.bindValue(":item", thermometer.item);
    query.bindValue(":allow_error_lower", thermometer.allow_error_lower);
    query.bindValue(":allow_error_upper", thermometer.allow_error_upper);
    query.bindValue(":measure_value", thermometer.measure_value);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (isSuccess) {
        QString ssql = "INSERT INTO biz_business_device_measure_data_resistance_thermometer ("
                       " id, data_id, data_type, data_category, upper_lower_point, location, "
                       "unit,"
                       " one_value, two_value, three_value, four_value, five_value, "
                       "six_value, seven_value,"
                       " eight_value, nine_value, ten_value, eleven_value, twelve_value, "
                       "thirteen_value,"
                       " fourteen_value, fifteen_value, max_value, min_value, volatility,"
                       " upper_temperature_different, lower_temperature_different,"
                       " max_horizontal_temperature_different, max_work_temperature_different,"
                       " calibrated_point, indicating_value_forward_stroke,"
                       " indicating_value_reverse_stroke, "
                       "measure_indication_value_forward_stroke,"
                       " measure_indication_value_reverse_stroke, "
                       "indicating_error_forward_stroke,"
                       " indicating_error_reverse_stroke, maximum_error, urel, rank_level,"
                       " nominal_temperature, item, allow_error_lower, allow_error_upper,"
                       " measure_value, create_time, update_time"
                       " ) VALUES ("
                       " :1, :2, :3, :4, :5, :6 :7,"
                       " :8, :9, :10, :11, :12, :13, :14,"
                       " :15, :16, :17, :18, :19, :20,"
                       " :21, :22, :23, :24, :25,"
                       " :26, :27,"
                       " :28, :29,"
                       " :30, :31,"
                       " :32, :33,"
                       " :34, :35,"
                       " :36, :37, :38, :39,"
                       " :40, :41, :42, :43,"
                       " :44, :45, :46 );";
    } else {
        qDebug() << "Insert data failed: " << query.lastError().text();
    }

    return isSuccess;
}
// 【查询】结果表 二等铂电阻
QList<QStringList> BaseCommonApi::getDeviceMeasureDataResistanceThermometer(
    QString data_id, QStringList* HeadList)
{
    // 待实现
}
// 【查询】检定项-频谱分析仪
QMap<QString, QList<QStringList>>
BaseCommonApi::getDeviceMeasureItemDataSpectrumAnalyzer(
    QString itemCatoryName, QString itemId,
    SequentialMap<QString, QStringList>* headList)
{
    QMap<QString, QList<QStringList>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();

    QSqlQuery query;
    query.prepare(
        "SELECT bbdmidsa.*,bbdmii.frequency_display,bbdmii.sweep_width as "
        "sweep_width_demo,bbdmii.middle_frequency_width,"
        " bbdmii.middle_conversion_deviation,bbdmii.reference_level,bbdmii."
        "vertical_scale as vertical_scale_demo,bbdmii.input_frequency,"
        " bbdmii.average_noise_level,bbdmii.harmonic_distortion,bbdmii.input_"
        "attenuation,bbdmii.gain_compression,bbdmii.calibrating_signal "
        " FROM biz_business_device_measure_item_data_spectrum_analyzer bbdmidsa "
        " JOIN biz_business_device_measure_item_info bbdmii ON bbdmii.id = "
        "bbdmidsa.item_id"
        "  WHERE bbdmidsa.item_id =  :itemId "
        " ORDER BY bbdmidsa.create_time ASC");
    query.bindValue(":itemId", itemId);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_id").toString()); // 检定项id
            result.append(
                query.value("data_type")
                    .toString()); // 检定项数据类型(一个检定项中区分两个表格)(字典code)
            result.append(query.value("nominal_value").toString()); // 标称值
            result.append(query.value("unit").toString()); // 标称值单位
            result.append(query.value("sweep_width").toString()); // 扫频宽度     5
            result.append(
                query.value("sweep_width_unit").toString()); // 扫频宽度单位
            result.append(
                query.value("horizontal_resolution").toString()); // 水平分辨率
            result.append(query.value("horizontal_resolution_unit")
                              .toString()); // 水平分辨率单位
            result.append(
                query.value("resolution_bandwidth").toString()); // 分辨率带宽
            result.append(query.value("resolution_bandwidth_unit")
                              .toString()); // 分辨率带宽单位
            result.append(query.value("frequency").toString()); // 频率
            result.append(query.value("frequency_unit").toString()); // 频率单位 12
            result.append(query.value("amplitude").toString()); // 幅度
            result.append(query.value("amplitude_unit").toString()); // 幅度单位
            result.append(
                query.value("frequency_reference_accuracy")
                    .toString()); // 频率基准精度(计算技术指标使用(频率显示))
            result.append(
                query.value("value_one")
                    .toString()); // 数值1(不带%,计算技术指标使用(频率显示))/数值(不带单位,扫频宽度/中频带宽/中频带宽转换偏差参考电平/垂直刻度/输入频响/输入衰减/1dB增益压缩点)
            result.append(
                query.value("value_two")
                    .toString()); // 数值2(不带%,计算技术指标使用(频率显示))/中频带宽
            result.append(
                query.value("value_three")
                    .toString()); // 数值3(不带Hz,计算技术指标使用(频率显示))
            result.append(query.value("value_four")
                              .toString()); // 数值4(计算技术指标使用(频率显示))
            result.append(
                query.value("technical_index")
                    .toString()); // 技术指标(存正负号/存符号(1dB增益压缩点))
            result.append(
                query.value("technical_index_unit").toString()); // 技术指标单位
            result.append(query.value("technical_index_left")
                              .toString()); // 技术指标左(中频带宽)
            result.append(query.value("technical_index_right")
                              .toString()); // 技术指标右(中频带宽)
            result.append(
                query.value("reference_point").toString()); // 是否为参考点(1/0)
            result.append(query.value("standard_attenuator_attenuation")
                              .toString()); // 标准衰减器衰减量
            result.append(query.value("standard_attenuator_attenuation_unit")
                              .toString()); // 标准衰减器衰减量单位
            result.append(
                query.value("signal_source_amplitude").toString()); // 信号源幅度
            result.append(query.value("signal_source_amplitude_unit")
                              .toString()); // 信号源幅度单位
            result.append(query.value("vertical_scale").toString()); // 垂直刻度
            result.append(query.value("start_frequency").toString()); // 起始频率 30
            result.append(
                query.value("start_frequency_unit").toString()); // 起始频率单位
            result.append(query.value("end_frequency").toString()); // 终止频率
            result.append(
                query.value("end_frequency_unit").toString()); // 终止频率单位
            result.append(query.value("corrected_value").toString()); // 修正值
            result.append(
                query.value("corrected_value_unit").toString()); // 修正值单位
            result.append(query.value("upper_index").toString()); // 指标上限
            result.append(
                query.value("upper_index_unit").toString()); // 指标上限单位
            result.append(query.value("input_attenuator").toString()); // 输入衰减器
            result.append(
                query.value("input_attenuator_unit").toString()); // 输入衰减器单位
            result.append(query.value("spectrum_level").toString()); // 参考电平
            result.append(
                query.value("spectrum_level_unit").toString()); // 参考电平单位
            result.append(query.value("rbw_value").toString()); // RBW             42
            result.append(query.value("rbw_value_unit").toString()); // RBW单位
            result.append(query.value("vbw_value").toString()); // VBW
            result.append(query.value("vbw_value_unit").toString()); // VBW单位
            result.append(
                query.value("upper_index_twice").toString()); // 二次谐波失真指标上限
            result.append(query.value("upper_index_twice_unit")
                              .toString()); // 二次谐波失真指标上限单位
            result.append(query.value("upper_index_thrice")
                              .toString()); // 三次谐波失真指标上限
            result.append(query.value("upper_index_thrice_unit")
                              .toString()); // 三次谐波失真指标上限单位
            result.append(query.value("upper_index_quartic")
                              .toString()); // 四次谐波失真指标上限
            result.append(query.value("upper_index_quartic_unit")
                              .toString()); // 四次谐波失真指标上限单位
            result.append(query.value("type_value").toString()); // 类型(校准信号)
            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());
            result.append(query.value("frequency_display")
                              .toString()); // 频率显示(频谱分析仪) 55
            result.append(
                query.value("sweep_width_demo").toString()); // 扫频宽度(频谱分析仪)
            result.append(query.value("middle_frequency_width")
                              .toString()); // 中频宽度(频谱分析仪)
            result.append(query.value("middle_conversion_deviation")
                              .toString()); // 中频带宽转换偏差(频谱分析仪)
            result.append(
                query.value("reference_level").toString()); // 参考电平(频谱分析仪)
            result.append(query.value("vertical_scale_demo")
                              .toString()); // 垂直刻度(频谱分析仪)
            result.append(
                query.value("input_frequency").toString()); // 输入频响(频谱分析仪)
            result.append(query.value("average_noise_level")
                              .toString()); // 平均噪声电平(频谱分析仪)
            result.append(
                query.value("harmonic_distortion")
                    .toString()); // 谐波失真(频谱分析仪)/(低频信号源)/(S参数)
            result.append(
                query.value("input_attenuation").toString()); // 输入衰减(频谱分析仪)
            result.append(query.value("gain_compression")
                              .toString()); // 1dB增益压缩点(频谱分析仪)
            result.append(query.value("calibrating_signal").toString()); // 校准信号
            resultData.append(result);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    // 进行检定项目列头转换
    DataMapList = GetDataReturnItemListBySpectrumAnalyzer(
        itemCatoryName, resultData, DataMapList, headList);
    return DataMapList;
}
QMap<QString, QList<QStringList>>
BaseCommonApi::GetDataReturnItemListBySpectrumAnalyzer(
    QString itemCatoryName, QList<QStringList> ResultData,
    QMap<QString, QList<QStringList>> DataMapList,
    SequentialMap<QString, QStringList>* HeadMapList)
{
    QList<QStringList> FrequencyDisplayList;
    QList<QStringList> SweepWidthList;
    QList<QStringList> IntermediateFrequencyBandwidthList;
    QList<QStringList> ConversionDeviationList;
    QList<QStringList> ReferencelevelList;
    QList<QStringList> VerticalScaleList;
    QList<QStringList> InputFrequencyResponseList;
    QList<QStringList> AverageNoiselevelList;
    QList<QStringList> HarmonicDistortionList;
    QList<QStringList> InputAttenuationList;
    QList<QStringList> GainCompressionPointList;
    QList<QStringList> CalibrationSignalList;

    QStringList FrequencyDisplayHead;
    QStringList SweepWidthHead;
    QStringList IntermediateFrequencyBandwidthHead;
    QStringList ConversionDeviationHead;
    QStringList ReferencelevelHead;
    QStringList VerticalScaleHead;
    QStringList InputFrequencyResponseHead;
    QStringList AverageNoiselevelHead;
    QStringList HarmonicDistortionHead;
    QStringList InputAttenuationHead;
    QStringList GainCompressionPointHead;
    QStringList CalibrationSignalHead;
    if (itemCatoryName.contains("频谱分析仪")) {
        for (const QStringList& row : ResultData) {
            QString VerificationItem = NULL;
            if (!row.isEmpty() && row[55] == "1" && row[2] == "1") {
                VerificationItem = "频率显示";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "标称值"
                     << "扫频宽度"
                     << "幅度"
                     << "分辨力带宽"
                     << "水平分辨率"
                     << "测量值1"
                     << "测量值2"
                     << "测量值3"
                     << "指标下限"
                     << "均值"
                     << "指标上限"
                     << "结论";
                FrequencyDisplayHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(row[2]); // 检定项目
                Data.append(row[3] + row[4]); // 标称值
                Data.append(row[13] + row[14]); // 幅度
                //                Data.append(row[4]);//标称值单位    需要隐藏
                Data.append(row[5] + row[6]); // 扫频宽度
                Data.append(row[9] + row[10]); // 分辨率带宽
                Data.append(row[7] + row[8]); // 水平分辨率
                Data.append(""); // 测量值1
                Data.append(""); // 测量值2
                Data.append(""); // 测量值3
                double index_lower = row[3].toInt() - (row[3].toInt() * row[15].toDouble() + row[16].toDouble() * 0.01 * row[5].toInt() + row[17].toDouble() * 0.01 * row[9].toInt() + row[18].toDouble() + row[19].toDouble() * row[7].toInt());
                Data.append(QString::number(index_lower) + row[4]); // 指标下限
                Data.append(""); // 均值
                double index_upper = row[3].toInt() + (row[3].toInt() * row[15].toDouble() + row[16].toDouble() * 0.01 * row[5].toInt() + row[17].toDouble() * 0.01 * row[9].toInt() + row[18].toDouble() + row[19].toDouble() * row[7].toInt());
                Data.append(QString::number(index_upper) + row[4]); // 指标上限
                Data.append(""); // 结论
                FrequencyDisplayList.append(Data);
            } else if (!row.isEmpty() && row[56] == "1" && row[2] == "2") {
                VerificationItem = "扫频宽度";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "频率"
                     << "幅度"
                     << "标称值"
                     << "水平分辨率"
                     << "fl1"
                     << "fr1"
                     << "fl2"
                     << "fr2"
                     << "fl3"
                     << "fr3"
                     << "指标下限"
                     << "均值"
                     << "指标上限"
                     << "结论";
                SweepWidthHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(row[2]); // 检定项目
                Data.append(row[11] + row[12]); // 频率
                Data.append(row[13] + row[14]); // 幅度
                Data.append(row[3] + row[4]); // 标称值
                //            Data.append(row[4]);//标称值单位  需要隐藏
                Data.append(row[7] + row[8]); // 水平分辨率
                Data.append(""); // fl1
                Data.append(""); // fr1
                Data.append(""); // fl2
                Data.append(""); // fr2
                Data.append(""); // fl3
                Data.append(""); // fr3
                double index_lower = row[3].toInt() - (row[3].toInt() * row[15].toDouble() * 0.01 + row[7].toInt());
                QString index_lowerr_num = formatDoubleWithEffectiveDigits(index_lower, 5);
                Data.append(index_lowerr_num + row[4]); // 指标下限
                Data.append(""); // 均值
                double index_upper = row[3].toInt() + (row[3].toInt() * row[15].toDouble() * 0.01 + row[7].toInt());
                QString index_upper_num = formatDoubleWithEffectiveDigits(index_upper, 5);
                Data.append(index_upper_num + row[4]); // 指标上限
                Data.append(""); // 结论
                SweepWidthList.append(Data);
            } else if (!row.isEmpty() && row[57] == "1" && row[2] == "3") {
                VerificationItem = "中频带宽";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "频率"
                     << "幅度"
                     << "标称值"
                     << "fl1"
                     << "fr1"
                     << "△f1"
                     << "fl2"
                     << "fr2"
                     << "△f2"
                     << "fl3"
                     << "fr3"
                     << "△f3"
                     << "指标下限"
                     << "均值"
                     << "指标上限"
                     << "结论";
                IntermediateFrequencyBandwidthHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(row[2]); // 检定项目
                Data.append(row[11] + row[12]); // 频率
                Data.append(row[13] + row[14]); // 幅度
                Data.append(row[3] + row[4]); // 标称值
                //            Data.append(row[4]);//标称值单位  需要隐藏
                Data.append(""); // fl1
                Data.append(""); // fr1
                Data.append(""); // △f1
                Data.append(""); // fl2
                Data.append(""); // fr2
                Data.append(""); // △f2
                Data.append(""); // fl3
                Data.append(""); // fr3
                Data.append(""); // △f3
                double index_lower = row[3].toDouble() - (row[3].toDouble() * row[15].toDouble() * 0.01);
                QString str_index_lower = formatDoubleWithDecimalPlaces(index_lower, 2);
                Data.append(str_index_lower + row[4]); // 指标下限
                Data.append(""); // 均值
                double index_upper = row[3].toInt() + (row[3].toInt() * row[15].toDouble() * 0.01);
                QString index_upper_num = formatDoubleWithDecimalPlaces(index_upper, 2);
                Data.append(index_upper_num + row[4]); // 指标上限
                Data.append(""); // 结论
                IntermediateFrequencyBandwidthList.append(Data);
            } else if (!row.isEmpty() && row[58] == "1" && row[2] == "4") {
                VerificationItem = "中频带宽转换偏差";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "频率"
                     << "幅度"
                     << "中频带宽"
                     << "测量值"
                     << "指标下限"
                     << "转换偏差"
                     << "指标上限"
                     << "结论";
                ConversionDeviationHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(row[2]); // 检定项目
                Data.append(row[11] + row[12]); // 频率
                Data.append(row[13] + row[14]); // 幅度
                Data.append(row[3] + row[4]); // 中频带宽
                Data.append(""); // 测量值
                QString index_lower = row[20].split("±")[1];
                Data.append("-" + index_lower + row[21]); // 指标下限
                Data.append(""); // 转换偏差
                Data.append("+" + index_lower + row[21]); // 指标上限
                Data.append(""); // 结论
                ConversionDeviationList.append(Data);
            } else if (!row.isEmpty() && row[59] == "1" && row[2] == "5") {
                VerificationItem = "参考电平";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "频率"
                     << "幅度"
                     << "标称值"
                     << "标准衰减器衰减量"
                     << "信号源幅度"
                     << "指标下限"
                     << "测量值"
                     << "指标上限"
                     << "结论";
                ReferencelevelHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(row[2]); // 检定项目
                Data.append(row[11] + row[12]); // 频率
                Data.append(row[13] + row[14]); // 幅度
                Data.append(row[3] + row[4]); // 标称值
                Data.append(row[25] + row[26]); // 标准衰减器衰减量
                Data.append(row[27] + row[28]); // 信号源幅度
                QString index_lower = "/";
                if (row[20].contains("±"))
                    index_lower = row[20].split("±")[1];
                Data.append(
                    QString::number(row[3].toDouble() - index_lower.toDouble()) + row[4]); // 指标下限
                Data.append(""); // 测量值
                Data.append(
                    QString::number(row[3].toDouble() + index_lower.toDouble()) + row[4]); // 指标上限
                Data.append(""); // 结论
                ReferencelevelList.append(Data);
            } else if (!row.isEmpty() && row[60] == "1" && row[2] == "6") {
                VerificationItem = "垂直刻度";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "标称值"
                     << "频率"
                     << "垂直刻度"
                     << "标准衰减器衰减量"
                     << "信号源幅度"
                     << "指标下限"
                     << "测量值"
                     << "指标上限"
                     << "结论";
                VerticalScaleHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(row[2]); // 检定项目
                Data.append(row[11] + row[12]); // 频率
                Data.append(row[3] + row[4]); // 标称值
                Data.append(row[29]); // 垂直刻度
                Data.append(row[25] + row[26]); // 标准衰减器衰减量
                Data.append(row[27] + row[28]); // 信号源幅度
                QString index_lower = "/";
                if (row[20].contains("±"))
                    index_lower = row[20].split("±")[1];
                Data.append(
                    QString::number(row[3].toDouble() - index_lower.toDouble()) + row[4]); // 指标下限
                Data.append(""); // 测量值
                Data.append(
                    QString::number(row[3].toDouble() + index_lower.toDouble()) + row[4]); // 指标上限
                Data.append(""); // 结论
                VerticalScaleList.append(Data);
            } else if (!row.isEmpty() && row[61] == "1" && row[2] == "7") {
                VerificationItem = "输入频响";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "频率"
                     << "测量值1"
                     << "测量值2"
                     << "测量值3"
                     << "均值"
                     << "指标下限"
                     << "输入频响"
                     << "指标上限"
                     << "结论";
                InputFrequencyResponseHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(row[2]); // 检定项目
                Data.append(row[11] + row[12]); // 频率
                Data.append(""); // 测量值1
                Data.append(""); // 测量值2
                Data.append(""); // 测量值3
                Data.append(""); // 均值
                QString index_lower = "/";
                if (row[20].contains("±"))
                    index_lower = row[20].split("±")[1];
                Data.append("-" + index_lower + row[21]); // 指标下限
                Data.append(""); // 输入频响
                Data.append("+" + index_lower + row[21]); // 指标上限
                Data.append(""); // 结论
                InputFrequencyResponseList.append(Data);
            } else if (!row.isEmpty() && row[62] == "1" && row[2] == "8") {
                VerificationItem = "平均噪声电平";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "起始频率"
                     << "终止频率"
                     << "测量值"
                     << "修正值"
                     << "修正后结果"
                     << "指标上限"
                     << "结论";
                AverageNoiselevelHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(row[2]); // 检定项目
                Data.append(row[30] + row[31]); // 起始频率
                Data.append(row[32] + row[33]); // 终止频率
                Data.append(""); // 测量值
                Data.append(row[34] + row[35]); // 修正值
                Data.append(""); // 修正后结果
                Data.append(row[36] + row[37]); // 指标上限
                Data.append(""); // 结论
                AverageNoiselevelList.append(Data);
            } else if (!row.isEmpty() && row[63] == "1" && row[2] == "9") {
                VerificationItem = "谐波失真";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "频率"
                     << "幅度"
                     << "RBW"
                     << "VBW"
                     << "基波幅度"
                     << "二次谐波幅度"
                     << "二次谐波失真"
                     << "三次谐波幅度"
                     << "三次谐波失真"
                     << "四次谐波幅度"
                     << "四次谐波失真"
                     << "二次谐波失真指标上限"
                     << "三次谐波失真指标上限"
                     << "四次谐波失真指标上限"
                     << "结论";
                HarmonicDistortionHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(row[2]); // 检定项目
                Data.append(row[11] + row[12]); // 频率
                Data.append(row[13] + row[14]); // 幅度
                Data.append(row[42] + row[43]); // RBW
                Data.append(row[44] + row[45]); // VBW
                Data.append(""); // 基波幅度
                Data.append(""); // 二次谐波幅度
                Data.append(""); // 二次谐波失真
                Data.append(""); // 三次谐波幅度
                Data.append(""); // 三次谐波失真
                Data.append(""); // 四次谐波幅度
                Data.append(""); // 四次谐波失真
                Data.append(row[46] + row[47]); // 二次谐波失真指标上限
                Data.append(row[48] + row[49]); // 三次谐波失真指标上限
                Data.append(row[50] + row[51]); // 四次谐波失真指标上限
                Data.append(""); // 结论
                HarmonicDistortionList.append(Data);
            } else if (!row.isEmpty() && row[64] == "1" && row[2] == "10 ") {
                VerificationItem = "输入衰减";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "频率"
                     << "幅度"
                     << "测量值1"
                     << "测量值2"
                     << "测量值3"
                     << "均值"
                     << "指标下限"
                     << "输入频响"
                     << "指标上限"
                     << "结论";
                InputAttenuationHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(row[2]); // 检定项目
                Data.append(row[11] + row[12]); // 频率
                Data.append(row[13] + row[14]); // 幅度
                Data.append(""); // 测量值1
                Data.append(""); // 测量值2
                Data.append(""); // 测量值3
                Data.append(""); // 均值
                QString index_lower = "/";
                if (row[20].contains("±"))
                    index_lower = row[20].split("±")[1];
                Data.append("-" + index_lower + row[21]); // 指标下限
                Data.append(""); // 输入频响
                Data.append("+" + index_lower + row[21]); // 指标上限
                Data.append(""); // 结论
                InputAttenuationList.append(Data);
            } else if (!row.isEmpty() && row[65] == "1" && row[2] == "11 ") {
                VerificationItem = "1db增益压缩点";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "频率"
                     << "幅度"
                     << "输入衰减"
                     << "参考电平"
                     << "测量值"
                     << "增益压缩"
                     << "技术指标"
                     << "结论";
                GainCompressionPointHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(row[2]); // 检定项目
                Data.append(row[11] + row[12]); // 频率
                Data.append(row[13] + row[14]); // 幅度
                Data.append(row[38] + row[39]); // 输入衰减
                Data.append(row[40] + row[41]); // 参考电平
                Data.append(""); // 测量值
                Data.append(""); // 增益压缩
                Data.append(row[20] + row[21]); // 技术指标
                Data.append(""); // 结论
                GainCompressionPointList.append(Data);
            } else if (!row.isEmpty() && row[66] == "1" && row[2] == "12 ") {
                VerificationItem = "校准信号";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "类型"
                     << "标称值"
                     << "测量值1"
                     << "测量值2"
                     << "测量值3"
                     << "指标下限"
                     << "测量结果"
                     << "指标上限"
                     << "结论";
                CalibrationSignalHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(row[2]); // 检定项目
                Data.append(row[52]); // 类型
                Data.append(row[3] + row[4]); // 标称值
                Data.append(""); // 测量值1
                Data.append(""); // 测量值2
                Data.append(""); // 测量值3
                QString index_lower = "/";
                if (row[20].contains("±"))
                    index_lower = row[20].split("±")[1];
                Data.append(
                    QString::number(row[3].toDouble() - index_lower.toDouble()) + row[4]); // 指标下限
                Data.append(""); // 测量结果
                Data.append(
                    QString::number(row[3].toDouble() + index_lower.toDouble()) + row[4]); // 指标上限
                Data.append(""); // 结论
                CalibrationSignalList.append(Data);
            }
        }
    }
    if (FrequencyDisplayList.count() != 0) {
        DataMapList.insert("频率显示", FrequencyDisplayList);
        HeadMapList->insert("频率显示", FrequencyDisplayHead);
    };
    if (SweepWidthList.count() != 0) {
        DataMapList.insert("扫频宽度", SweepWidthList);
        HeadMapList->insert("扫频宽度", SweepWidthHead);
    };
    if (IntermediateFrequencyBandwidthList.count() != 0) {
        DataMapList.insert("中频带宽", IntermediateFrequencyBandwidthList);
        HeadMapList->insert("中频带宽", IntermediateFrequencyBandwidthHead);
    };
    if (ConversionDeviationList.count() != 0) {
        DataMapList.insert("中频带宽转换偏差", ConversionDeviationList);
        HeadMapList->insert("中频带宽转换偏差", ConversionDeviationHead);
    };
    if (ReferencelevelList.count() != 0) {
        DataMapList.insert("参考电平", ReferencelevelList);
        HeadMapList->insert("参考电平", ReferencelevelHead);
    };
    if (VerticalScaleList.count() != 0) {
        DataMapList.insert("垂直刻度", VerticalScaleList);
        HeadMapList->insert("垂直刻度", VerticalScaleHead);
    };
    if (InputFrequencyResponseList.count() != 0) {
        DataMapList.insert("输入频响", InputFrequencyResponseList);
        HeadMapList->insert("输入频响", InputFrequencyResponseHead);
    };
    if (AverageNoiselevelList.count() != 0) {
        DataMapList.insert("平均噪声电平", AverageNoiselevelList);
        HeadMapList->insert("平均噪声电平", AverageNoiselevelHead);
    };
    if (HarmonicDistortionList.count() != 0) {
        DataMapList.insert("谐波失真", HarmonicDistortionList);
        HeadMapList->insert("谐波失真", HarmonicDistortionHead);
    };
    if (InputAttenuationList.count() != 0) {
        DataMapList.insert("输入衰减", InputAttenuationList);
        HeadMapList->insert("输入衰减", InputAttenuationHead);
    };
    if (GainCompressionPointList.count() != 0) {
        DataMapList.insert("1db增益压缩点", GainCompressionPointList);
        HeadMapList->insert("1db增益压缩点", GainCompressionPointHead);
    };
    if (CalibrationSignalList.count() != 0) {
        DataMapList.insert("校准信号", CalibrationSignalList);
        HeadMapList->insert("校准信号", CalibrationSignalHead);
    };
    return DataMapList;
}

// 保留5位小数，参数1：值，参数2:保留几位有效数字
QString BaseCommonApi::formatDoubleWithEffectiveDigits(double value,
    int effectiveDigits)
{
    // 转换 double 为 QString 并保留足够的位数来避免精度损失
    QString str = QString::number(
        value, 'g', effectiveDigits + 6); // 'g' 格式表示使用最短的表示形式

    // 移除尾部的零和小数点后的零（如果它们存在的话）
    str.remove(QRegExp("0+$"));
    if (str.endsWith('.')) {
        str.chop(1); // 移除尾部的点
    }

    // 确保至少有一位数字在小数点后面（如果原始数字是小数）
    if (str.contains('.') && str.indexOf('.') == str.length() - 1) {
        str.append('0');
    }

    return str;
}
// 保留小数点后几位
QString BaseCommonApi::formatDoubleWithDecimalPlaces(double value,
    int decimalPlaces)
{
    return QString::number(value, 'f',
        decimalPlaces); // 使用固定点表示法，并指定小数位数
}
// 【结果保存】频谱分析仪
bool BaseCommonApi::InsertDeviceMeasureDataSpectrumAnalyzer(
    DeviceMeasureDataSpectrumAnalyzer analyzer)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_business_device_measure_data_spectrum_analyzer "
        " (id, data_id, data_type, nominal_value, unit, sweep_width, "
        "sweep_width_unit, horizontal_resolution, horizontal_resolution_unit,"
        " resolution_bandwidth, resolution_bandwidth_unit, one_value, two_value, "
        "three_value, four_value, five_value, six_value, seven_value,"
        " eight_value, nine_value, ten_value, upper_index_twice, "
        "upper_index_twice_unit, upper_index_thrice, upper_index_thrice_unit, "
        " upper_index_quartic, upper_index_quartic_unit, lower_index, "
        "lower_index_unit, average_value, upper_index, upper_index_unit, "
        " conclusion, convert_deviation, lower_convert_deviation, "
        "upper_convert_deviation, standard_attenuator_attenuation, "
        " standard_attenuator_attenuation_unit, signal_source_amplitude, "
        "signal_source_amplitude_unit, vertical_scale, frequency, "
        " frequency_unit, input_frequency, start_frequency, "
        "start_frequency_unit, end_frequency, end_frequency_unit, "
        "revised_result, "
        " input_attenuator, input_attenuator_unit, "
        "input_attenuator_conversion_effect, spectrum_level, "
        "spectrum_level_unit, gain_compression, "
        " exceed_mark, exceed_mark_one, exceed_mark_two, measure_result, "
        "measure_result_unit, technical_index, technical_index_left, "
        " technical_index_right, corrected_value, type_value, create_time, "
        "update_time ) VALUES ( :id, :data_id, :data_type, :nominal_value, "
        " :unit, :sweep_width, :sweep_width_unit, :horizontal_resolution, "
        ":horizontal_resolution_unit, :resolution_bandwidth, "
        ":resolution_bandwidth_unit,"
        " :one_value, :two_value, :three_value, :four_value, :five_value, "
        ":six_value, :seven_value, :eight_value, :nine_value, :ten_value, "
        ":upper_index_twice,"
        " :upper_index_twice_unit, :upper_index_thrice, "
        ":upper_index_thrice_unit, :upper_index_quartic, "
        ":upper_index_quartic_unit, :lower_index, "
        " :lower_index_unit, :average_value, :upper_index, :upper_index_unit, "
        ":conclusion, :convert_deviation, :lower_convert_deviation,"
        " :upper_convert_deviation, :standard_attenuator_attenuation, "
        ":standard_attenuator_attenuation_unit, :signal_source_amplitude, "
        " :signal_source_amplitude_unit, :vertical_scale, :frequency, "
        ":frequency_unit, :input_frequency, :start_frequency, "
        ":start_frequency_unit, "
        " :end_frequency, :end_frequency_unit, :revised_result, "
        ":input_attenuator, :input_attenuator_unit, "
        ":input_attenuator_conversion_effect, "
        " :spectrum_level, :spectrum_level_unit, :gain_compression, "
        ":exceed_mark, :exceed_mark_one, :exceed_mark_two, :measure_result, "
        " :measure_result_unit, :technical_index, :technical_index_left, "
        ":technical_index_right, :corrected_value, :type_value, :create_time, "
        " :update_time );");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", analyzer.id);
    query.bindValue(":data_id", analyzer.data_id);
    query.bindValue(":data_type", analyzer.data_type);
    query.bindValue(":nominal_value", analyzer.nominal_value);
    query.bindValue(":unit", analyzer.unit);
    query.bindValue(":sweep_width", analyzer.sweep_width);
    query.bindValue(":sweep_width_unit", analyzer.sweep_width_unit);
    query.bindValue(":horizontal_resolution", analyzer.horizontal_resolution);
    query.bindValue(":horizontal_resolution_unit",
        analyzer.horizontal_resolution_unit);
    query.bindValue(":resolution_bandwidth", analyzer.resolution_bandwidth);
    query.bindValue(":resolution_bandwidth_unit",
        analyzer.resolution_bandwidth_unit);
    query.bindValue(":one_value", analyzer.one_value);
    query.bindValue(":two_value", analyzer.two_value);
    query.bindValue(":three_value", analyzer.three_value);
    query.bindValue(":four_value", analyzer.four_value);
    query.bindValue(":five_value", analyzer.five_value);
    query.bindValue(":six_value", analyzer.six_value);
    query.bindValue(":seven_value", analyzer.seven_value);
    query.bindValue(":eight_value", analyzer.eight_value);
    query.bindValue(":nine_value", analyzer.nine_value);
    query.bindValue(":ten_value", analyzer.ten_value);
    query.bindValue(":upper_index_twice", analyzer.upper_index_twice);
    query.bindValue(":upper_index_twice_unit", analyzer.upper_index_twice_unit);
    query.bindValue(":upper_index_thrice", analyzer.upper_index_thrice);
    query.bindValue(":upper_index_thrice_unit", analyzer.upper_index_thrice_unit);
    query.bindValue(":upper_index_quartic", analyzer.upper_index_quartic);
    query.bindValue(":upper_index_quartic_unit",
        analyzer.upper_index_quartic_unit);
    query.bindValue(":lower_index", analyzer.lower_index);
    query.bindValue(":lower_index_unit", analyzer.lower_index_unit);
    query.bindValue(":average_value", analyzer.average_value);
    query.bindValue(":upper_index", analyzer.upper_index);
    query.bindValue(":upper_index_unit", analyzer.upper_index_unit);
    query.bindValue(":conclusion", analyzer.conclusion);
    query.bindValue(":convert_deviation", analyzer.convert_deviation);
    query.bindValue(":lower_convert_deviation", analyzer.lower_convert_deviation);
    query.bindValue(":upper_convert_deviation", analyzer.upper_convert_deviation);
    query.bindValue(":standard_attenuator_attenuation",
        analyzer.standard_attenuator_attenuation);
    query.bindValue(":standard_attenuator_attenuation_unit",
        analyzer.standard_attenuator_attenuation_unit);
    query.bindValue(":signal_source_amplitude", analyzer.signal_source_amplitude);
    query.bindValue(":signal_source_amplitude_unit",
        analyzer.signal_source_amplitude_unit);
    query.bindValue(":vertical_scale", analyzer.vertical_scale);
    query.bindValue(":frequency", analyzer.frequency);
    query.bindValue(":frequency_unit", analyzer.frequency_unit);
    query.bindValue(":input_frequency", analyzer.input_frequency);
    query.bindValue(":start_frequency", analyzer.start_frequency);
    query.bindValue(":start_frequency_unit", analyzer.start_frequency_unit);
    query.bindValue(":end_frequency", analyzer.end_frequency);
    query.bindValue(":end_frequency_unit", analyzer.end_frequency_unit);
    query.bindValue(":revised_result", analyzer.revised_result);
    query.bindValue(":input_attenuator", analyzer.input_attenuator);
    query.bindValue(":input_attenuator_unit", analyzer.input_attenuator_unit);
    query.bindValue(":input_attenuator_conversion_effect",
        analyzer.input_attenuator_conversion_effect);
    query.bindValue(":spectrum_level", analyzer.spectrum_level);
    query.bindValue(":spectrum_level_unit", analyzer.spectrum_level_unit);
    query.bindValue(":gain_compression", analyzer.gain_compression);
    query.bindValue(":exceed_mark", analyzer.exceed_mark);
    query.bindValue(":exceed_mark_one", analyzer.exceed_mark_one);
    query.bindValue(":exceed_mark_two", analyzer.exceed_mark_two);
    query.bindValue(":measure_result", analyzer.measure_result);
    query.bindValue(":measure_result_unit", analyzer.measure_result_unit);
    query.bindValue(":technical_index", analyzer.technical_index);
    query.bindValue(":technical_index_left", analyzer.technical_index_left);
    query.bindValue(":technical_index_right", analyzer.technical_index_right);
    query.bindValue(":corrected_value", analyzer.corrected_value);
    query.bindValue(":type_value", analyzer.type_value);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (isSuccess) {
        QString ssql = "INSERT INTO biz_business_device_measure_data_spectrum_analyzer "
                       " (id, data_id, data_type, nominal_value, unit, sweep_width, "
                       "sweep_width_unit, horizontal_resolution, horizontal_resolution_unit,"
                       " resolution_bandwidth, resolution_bandwidth_unit, one_value, "
                       "two_value, three_value, four_value, five_value, six_value, "
                       "seven_value,"
                       " eight_value, nine_value, ten_value, upper_index_twice, "
                       "upper_index_twice_unit, upper_index_thrice, upper_index_thrice_unit, "
                       " upper_index_quartic, upper_index_quartic_unit, lower_index, "
                       "lower_index_unit, average_value, upper_index, upper_index_unit, "
                       " conclusion, convert_deviation, lower_convert_deviation, "
                       "upper_convert_deviation, standard_attenuator_attenuation, "
                       " standard_attenuator_attenuation_unit, signal_source_amplitude, "
                       "signal_source_amplitude_unit, vertical_scale, frequency, "
                       " frequency_unit, input_frequency, start_frequency, "
                       "start_frequency_unit, end_frequency, end_frequency_unit, "
                       "revised_result, "
                       " input_attenuator, input_attenuator_unit, "
                       "input_attenuator_conversion_effect, spectrum_level, "
                       "spectrum_level_unit, gain_compression, "
                       " exceed_mark, exceed_mark_one, exceed_mark_two, measure_result, "
                       "measure_result_unit, technical_index, technical_index_left, "
                       " technical_index_right, corrected_value, type_value, create_time, "
                       "update_time ) VALUES ( :1, :2, :3, :4, "
                       " :5, :6, :7, :8, :9, :10, :11,"
                       " :12, :13, :14, :15, :16, :17, :18, :19, :20, :21, :22,"
                       " :23, :24, :25, :26, :27, :28, "
                       " :29, :30, :31, :32, :33, :34, :35,"
                       " :36, :37, :38, :39, "
                       " :40, :41, :42, :43, :44, :45, :46, "
                       " :47, :48, :49, :50, :51, :52, "
                       " :53, :54, :55, :56, :57, :58, :59, "
                       " :60, :61, :62, :63, :64, :65, :66, "
                       " :67 );";
        saveLogToSql(ssql, &analyzer);
    } else {
        qDebug() << "Insert data failed: " << query.lastError().text();
    }
    return isSuccess;
}

// 【查询】角色 根据分类id
QList<DeviceRole> BaseCommonApi::getDeviceRoleBydeviceId(QString cateory_id)
{
    QList<DeviceRole> result;
    QSqlQuery query;
    query.prepare("SELECT * FROM zd_device_role WHERE cateory_id = :cateory_id ");
    query.bindValue(":cateory_id", cateory_id);
    bool isSuccess = query.exec();
    if (isSuccess) {
        while (query.next()) {
            DeviceRole role;
            role.id = query.value("id").toString();
            role.belong_vername = query.value("belong_vername").toString();
            role.belong_checkname = query.value("belong_checkname").toString();
            role.cateory_id = query.value("cateory_id").toString();
            role.device_id = query.value("device_id").toString();
            role.device_type = query.value("device_type").toString();
            role.role = query.value("role").toString();
            role.create_time = query.value("create_time").toDateTime();
            role.update_time = query.value("update_time").toDateTime();
            result.append(role);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
}
// 【新增】角色
bool BaseCommonApi::InsertDeviceRole(DeviceRole role)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO zd_device_role "
        "(id,belong_vername,belong_checkname,cateory_id,device_id,device_type,"
        "role,create_time,update_time) "
        " VALUES(:id,:belong_vername,:belong_checkname,:cateory_id,:device_id,:"
        "device_type,:role,:create_time,:update_time)");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", role.id);
    query.bindValue(":belong_vername", role.belong_vername);
    query.bindValue(":belong_checkname", role.belong_checkname);
    query.bindValue(":cateory_id", role.cateory_id);
    query.bindValue(":device_id", role.device_id);
    query.bindValue(":device_type", role.device_type);
    query.bindValue(":role", role.role);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << "Insert data failed: " << query.lastError().text();
    }
    return isSuccess;
}
// 【更新】角色
bool BaseCommonApi::UpdateDeviceRole(DeviceRole role)
{
    QSqlQuery query;
    query.prepare(
        "UPDATE zd_device_role "
        " SET belong_vername = :belong_vername,belong_checkname = "
        ":belong_checkname,cateory_id = :cateory_id,"
        " device_id=:device_id,device_type=:device_type, "
        "role=:role,update_time=:update_time "
        "  WHERE id = :id");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", role.id);
    query.bindValue(":belong_vername", role.belong_vername);
    query.bindValue(":belong_checkname", role.belong_checkname);
    query.bindValue(":cateory_id", role.cateory_id);
    query.bindValue(":device_id", role.device_id);
    query.bindValue(":device_type", role.device_type);
    query.bindValue(":role", role.role);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << "Insert data failed: " << query.lastError().text();
    }
    return isSuccess;
}
// 检定项-信号发生器
QMap<QString, QList<QStringList>>
BaseCommonApi::getDeviceMeasureItemDataSignalGenerator(
    QString itemCatoryName, QString itemId,
    SequentialMap<QString, QStringList>* headList)
{
    QMap<QString, QList<QStringList>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();

    QSqlQuery query;
    query.prepare(
        "SELECT "
        "bbdmidsg.*,bbdmii.frequency,bbdmii.power,bbdmii.amplitude_modulation_"
        "depth,bbdmii.frequency_modulation_deviation,"
        " bbdmii.remain_amplitude_modulation,bbdmii.remain_frequency_modulation,"
        "bbdmii.harmonic,bbdmii.phase_modulation_phase_deviation,"
        " bbdmii.no_harmonic,bbdmii.power AS POW,bbdmii.frequency AS FREQ FROM "
        "biz_business_device_measure_item_data_signal_generator bbdmidsg"
        " JOIN biz_business_device_measure_item_info bbdmii ON bbdmii.id = "
        "bbdmidsg.item_id"
        " WHERE bbdmidsg.item_id =  :itemId"
        " ORDER BY bbdmidsg.create_time ASC");
    query.bindValue(":itemId", itemId);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_id").toString()); // 检定项id
            result.append(
                query.value("data_type")
                    .toString()); // 检定项数据类型(一个检定项中区分两个表格)(字典code)
            result.append(query.value("nominal_value").toString()); // 标称值
            result.append(query.value("unit").toString()); // 标称值单位
            result.append(query.value("amplitude").toString()); // 幅度
            result.append(query.value("amplitude_unit").toString()); // 幅度单位
            result.append(query.value("frequency").toString()); // 频率
            result.append(query.value("frequency_unit").toString()); // 频率单位
            result.append(query.value("power").toString()); // 功率
            result.append(query.value("power_unit").toString()); // 功率单位
            result.append(
                query.value("technical_index_symbol").toString()); // 技术指标符号
            result.append(query.value("technical_index").toString()); // 技术指标 12
            result.append(
                query.value("technical_index_unit").toString()); // 技术指标单位
            result.append(query.value("urel").toString()); // U(k=2)
            result.append(query.value("high_pass_filter").toString()); // 高通滤波
            result.append(
                query.value("high_pass_filter_unit").toString()); // 高通滤波单位  16
            result.append(query.value("lower_pass_filter").toString()); // 低通滤波
            result.append(
                query.value("lower_pass_filter_unit").toString()); // 低通滤波单位
            result.append(query.value("modulation_rate").toString()); // 调制速率
            result.append(
                query.value("modulation_rate_unit").toString()); // 调制速率单位
            result.append(query.value("upper_index_symbol")
                              .toString()); // 指标上限符号/THD上限符号
            result.append(query.value("upper_index").toString()); // 指标上限/THD上限
            result.append(query.value("upper_index_unit")
                              .toString()); // 指标上限单位/THD上限单位
            result.append(query.value("harmonic_number").toString()); // 谐波次数
            result.append(query.value("rbw_value").toString()); // RBW            25
            result.append(query.value("rbw_value_unit").toString()); // RBW单位
            result.append(query.value("vbw_value").toString()); // VBW
            result.append(query.value("vbw_value_unit").toString()); // VBW单位
            result.append(query.value("offset_frequency").toString()); // 偏移频率
            result.append(
                query.value("offset_frequency_unit").toString()); // 偏移频率单位
            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());
            result.append(query.value("FREQ").toString()); // 频率 33
            result.append(query.value("POW").toString()); // 功率     34
            result.append(
                query.value("amplitude_modulation_depth").toString()); // 调幅深度
            result.append(query.value("frequency_modulation_deviation")
                              .toString()); // 调频频偏
            result.append(
                query.value("remain_amplitude_modulation").toString()); // 剩余调幅
            result.append(
                query.value("remain_frequency_modulation").toString()); // 剩余调频
            result.append(query.value("harmonic").toString()); // 谐波
            result.append(query.value("phase_modulation_phase_deviation")
                              .toString()); // 调相相偏
            result.append(query.value("no_harmonic").toString()); // 非谐波
            resultData.append(result);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    // 进行检定项目列头转换
    DataMapList = GetDataReturnItemListBySignalGenerator(
        itemCatoryName, resultData, DataMapList, headList);
    return DataMapList;
}
QMap<QString, QList<QStringList>>
BaseCommonApi::GetDataReturnItemListBySignalGenerator(
    QString itemCatoryName, QList<QStringList> ResultData,
    QMap<QString, QList<QStringList>> DataMapList,
    SequentialMap<QString, QStringList>* HeadMapList)
{
    QList<QStringList> FrequencyList;
    QList<QStringList> PowerList;
    QList<QStringList> AmplitudeModulationDepthList;
    QList<QStringList> FMFrequencyOffsetList;
    QList<QStringList> RemainingAmplitudeModulationList;
    QList<QStringList> ResidualFMList;
    QList<QStringList> HarmonicList;
    QList<QStringList> PhaseModulationPhaseDeviationList;
    QList<QStringList> NonHarmonicList;

    QStringList FrequencyHead;
    QStringList PowerHead;
    QStringList AmplitudeModulationDepthHead;
    QStringList FMFrequencyOffsetHead;
    QStringList RemainingAmplitudeModulationHead;
    QStringList ResidualFMHead;
    QStringList HarmonicHead;
    QStringList PhaseModulationPhaseDeviationHead;
    QStringList NonHarmonicHead;
    if (itemCatoryName.contains("信号发生器")) {
        for (const QStringList& row : ResultData) {
            QString VerificationItem = NULL;
            if (!row.isEmpty() && row[33] == "1" && row[2] == "1") {
                VerificationItem = "频率";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "标称值"
                     << "指标下限"
                     << "测量值"
                     << "指标上限"
                     << "相对误差"
                     << "U(k=2)";
                FrequencyHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[3] + row[4]); // 标称值
                QString temp_low = QString::number(row[3].toDouble() - row[12].toDouble());
                Data.append(temp_low + row[4]); // 指标下限
                Data.append(""); // 测量值
                QString temp_hight = QString::number(row[3].toDouble() + row[12].toDouble());
                Data.append(temp_hight + row[4]); // 指标上限
                Data.append(""); // 相对误差
                Data.append(row[14]); // U(k=2)
                FrequencyList.append(Data);
            } else if (!row.isEmpty() && row[34] == "1" && row[2] == "2") {
                VerificationItem = "功率";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "频率"
                     << "标称值"
                     << "指标下限"
                     << "测量值"
                     << "指标上限"
                     << "U(k=2)";
                PowerHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[7] + row[8]); // 频率
                Data.append(row[3] + row[4]); // 标称值
                QString temp_low = QString::number(row[3].toDouble() - row[12].toDouble());
                Data.append(temp_low + row[4]); // 指标下限
                Data.append(""); // 测量值
                QString temp_hight = QString::number(row[3].toDouble() + row[12].toDouble());
                Data.append(temp_hight + row[4]); // 指标上限
                Data.append(row[14]); // U(k=2)
                PowerList.append(Data);
            } else if (!row.isEmpty() && row[35] == "1" && row[2] == "3") {
                VerificationItem = "调幅深度";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "频率"
                     << "高通滤波"
                     << "低通滤波"
                     << "调制速率"
                     << "标称值"
                     << "指标下限"
                     << "测量值"
                     << "指标上限"
                     << "U(k=2)"
                     << "THD"
                     << "THD上限";
                AmplitudeModulationDepthHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[7] + row[8]); // 频率
                Data.append(row[15] + row[16]); // 高通滤波
                Data.append(row[17] + row[18]); // 低通滤波
                Data.append(row[19] + row[20]); // 调制速率
                QString temp_hight = QString::number(row[3].toDouble() + row[12].toDouble());
                Data.append(row[3] + row[4]); // 标称值
                QString temp_low = QString::number(row[3].toDouble() - row[12].toDouble());
                Data.append(temp_low + row[4]); // 指标下限
                Data.append(""); // 测量值
                Data.append(temp_hight + row[4]); // 指标上限
                Data.append(row[14]); // U(k=2)
                Data.append(""); // THD
                Data.append(row[21] + row[22] + row[23]); // THD上限
                AmplitudeModulationDepthList.append(Data);
            } else if (!row.isEmpty() && row[36] == "1" && row[2] == "4") {
                VerificationItem = "调频频偏";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "频率"
                     << "高通滤波"
                     << "低通滤波"
                     << "调制速率"
                     << "标称值"
                     << "指标下限"
                     << "测量值"
                     << "指标上限"
                     << "U(k=2)"
                     << "THD"
                     << "THD上限";
                FMFrequencyOffsetHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[7] + row[8]); // 频率
                Data.append(row[15] + row[16]); // 高通滤波
                Data.append(row[17] + row[18]); // 低通滤波
                Data.append(row[19] + row[20]); // 调制速率
                QString temp_hight = formatDoubleWithEffectiveDigits(
                    row[3].toDouble() + row[12].toDouble(), 4);
                Data.append(row[3] + row[4]); // 标称值
                QString temp_low = formatDoubleWithEffectiveDigits(
                    row[3].toDouble() - row[12].toDouble(), 4);
                Data.append(temp_low + row[4]); // 指标下限
                Data.append(""); // 测量值
                Data.append(temp_hight + row[4]); // 指标上限
                Data.append(row[14]); // U(k=2)
                Data.append(""); // THD
                Data.append(row[21] + row[22] + row[23]); // THD上限
                FMFrequencyOffsetList.append(Data);
            } else if (!row.isEmpty() && row[37] == "1" && row[2] == "5") {
                VerificationItem = "剩余调幅";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "频率"
                     << "高通滤波"
                     << "低通滤波"
                     << "剩余调幅"
                     << "指标上限"
                     << "U(k=2)";
                RemainingAmplitudeModulationHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[7] + row[8]); // 频率
                Data.append(row[15] + row[16]); // 高通滤波
                Data.append(row[17] + row[18]); // 低通滤波
                Data.append(""); // 剩余调幅
                Data.append(row[21] + row[22] + row[23]); // 指标上限
                Data.append(row[14]); // U(k=2)
                RemainingAmplitudeModulationList.append(Data);
            } else if (!row.isEmpty() && row[38] == "1" && row[2] == "6") {
                VerificationItem = "剩余调频";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "频率"
                     << "高通滤波"
                     << "低通滤波"
                     << "剩余调频"
                     << "指标上限"
                     << "U(k=2)";
                ResidualFMHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[7] + row[8]); // 频率
                Data.append(row[15] + row[16]); // 高通滤波
                Data.append(row[17] + row[18]); // 低通滤波
                Data.append(""); // 剩余调频
                Data.append(row[21] + row[22] + row[23]); // 指标上限
                Data.append(row[14]); // U(k=2)
                ResidualFMList.append(Data);
            } else if (!row.isEmpty() && row[39] == "1" && row[2] == "7") {
                VerificationItem = "谐波";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "频率"
                     << "幅度"
                     << "谐波次数"
                     << "基波幅度"
                     << "谐波幅度"
                     << "测量值"
                     << "指标上限"
                     << "U(k=2)";
                HarmonicHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[7] + row[8]); // 频率
                Data.append(row[5] + row[6]); // 幅度
                Data.append(row[24]); // 谐波次数
                Data.append(""); // 基波幅度
                Data.append(""); // 谐波幅度
                Data.append(""); // 测量值
                Data.append(row[21] + row[22] + row[23]); // 指标上限
                Data.append(row[14]); // U(k=2)
                HarmonicList.append(Data);
            } else if (!row.isEmpty() && row[40] == "1" && row[2] == "8") {
                VerificationItem = "调相相偏";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "频率"
                     << "高通滤波"
                     << "低通滤波"
                     << "调制速率"
                     << "标称值"
                     << "指标下限"
                     << "测量值"
                     << "指标上限"
                     << "U(k=2)"
                     << "THD"
                     << "THD上限";
                PhaseModulationPhaseDeviationHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[7] + row[8]); // 频率
                Data.append(row[15] + row[16]); // 高通滤波
                Data.append(row[17] + row[18]); // 低通滤波
                Data.append(row[19] + row[20]); // 调制速率
                Data.append(row[3] + row[4]); // 标称值
                QString temp_low = formatDoubleWithEffectiveDigits(
                    row[3].toDouble() - row[12].toDouble(), 4);
                Data.append(temp_low + row[4]); // 指标下限
                Data.append(""); // 测量值
                QString temp_hight = formatDoubleWithEffectiveDigits(
                    row[3].toDouble() + row[12].toDouble(), 4);
                Data.append(temp_hight + row[4]); // 指标上限
                Data.append(row[14]); // U(k=2)
                Data.append(""); // THD
                Data.append(row[21] + row[22] + row[23]); // THD上限
                PhaseModulationPhaseDeviationList.append(Data);
            } else if (!row.isEmpty() && row[41] == "1" && row[2] == "9") {
                VerificationItem = "非谐波";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "频率"
                     << "功率"
                     << "偏移频率"
                     << "测量值"
                     << "指标上限"
                     << "U(k=2)";
                NonHarmonicHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[7] + row[8]); // 频率
                Data.append(row[15] + row[16]); // 功率
                Data.append(row[29] + row[30]); // 偏移频率
                Data.append(""); // 测量值
                Data.append(row[21] + row[22] + row[23]); // 指标上限
                Data.append(row[14]); // U(k=2)
                NonHarmonicList.append(Data);
            }
        }
    }
    if (FrequencyList.count() != 0) {
        DataMapList.insert("频率", FrequencyList);
        HeadMapList->insert("频率", FrequencyHead);
    };
    if (PowerList.count() != 0) {
        DataMapList.insert("功率", PowerList);
        HeadMapList->insert("功率", PowerHead);
    };
    if (AmplitudeModulationDepthList.count() != 0) {
        DataMapList.insert("调幅深度", AmplitudeModulationDepthList);
        HeadMapList->insert("调幅深度", AmplitudeModulationDepthHead);
    };
    if (FMFrequencyOffsetList.count() != 0) {
        DataMapList.insert("调频频偏", FMFrequencyOffsetList);
        HeadMapList->insert("调频频偏", FMFrequencyOffsetHead);
    };
    if (RemainingAmplitudeModulationList.count() != 0) {
        DataMapList.insert("剩余调幅", RemainingAmplitudeModulationList);
        HeadMapList->insert("剩余调幅", RemainingAmplitudeModulationHead);
    };
    if (ResidualFMList.count() != 0) {
        DataMapList.insert("剩余调频", ResidualFMList);
        HeadMapList->insert("剩余调频", ResidualFMHead);
    };
    if (HarmonicList.count() != 0) {
        DataMapList.insert("谐波", HarmonicList);
        HeadMapList->insert("谐波", HarmonicHead);
    };
    if (PhaseModulationPhaseDeviationList.count() != 0) {
        DataMapList.insert("调相相偏", PhaseModulationPhaseDeviationList);
        HeadMapList->insert("调相相偏", PhaseModulationPhaseDeviationHead);
    };
    if (NonHarmonicList.count() != 0) {
        DataMapList.insert("非谐波", NonHarmonicList);
        HeadMapList->insert("非谐波", NonHarmonicHead);
    };
    return DataMapList;
}
// 【保存】信号发生器
bool BaseCommonApi::InsertDeviceMeasureDataSignalGenerator(
    DeviceMeasureDataSignalGenerator generator)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_business_device_measure_data_signal_generator "
        "( id, data_id, data_type, nominal_value, unit, upper_index, upper_thd, "
        "lower_index, measure_value, measure_value_unit,"
        " relative_error, urel, frequency, frequency_unit, power, power_unit, "
        "amplitude, amplitude_unit, high_pass_filter, "
        "high_pass_filter_unit, lower_pass_filter, lower_pass_filter_unit, "
        "modulation_rate, modulation_rate_unit, thd, upper_index_symbol, "
        "upper_index_unit, remain_amplitude_modulation,harmonic_number, "
        "fundamental_amplitude, fundamental_amplitude_unit, "
        "harmonic_amplitude, harmonic_amplitude_unit, offset_frequency, "
        "offset_frequency_unit, exceed_mark, exceed_mark_one, "
        "create_time, update_time ) VALUES ( :id, :data_id, :data_type, "
        ":nominal_value, :unit, :upper_index, :upper_thd, :lower_index, "
        ":measure_value, :measure_value_unit, :relative_error, :urel, "
        ":frequency, :frequency_unit, :power, :power_unit, :amplitude,"
        " :amplitude_unit, :high_pass_filter, :high_pass_filter_unit, "
        ":lower_pass_filter, :lower_pass_filter_unit, :modulation_rate, "
        ":modulation_rate_unit,"
        " :thd, :upper_index_symbol, :upper_index_unit, "
        ":remain_amplitude_modulation, :harmonic_number, :fundamental_amplitude,"
        " :fundamental_amplitude_unit, :harmonic_amplitude, "
        ":harmonic_amplitude_unit, :offset_frequency, :offset_frequency_unit,"
        " :exceed_mark, :exceed_mark_one, :create_time, :update_time );");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", generator.id);
    query.bindValue(":data_id", generator.data_id);
    query.bindValue(":data_type", generator.data_type);
    query.bindValue(":nominal_value", generator.nominal_value);
    query.bindValue(":unit", generator.unit);
    query.bindValue(":upper_index", generator.upper_index);
    query.bindValue(":upper_thd", generator.upper_thd);
    query.bindValue(":lower_index", generator.lower_index);
    query.bindValue(":measure_value", generator.measure_value);
    query.bindValue(":measure_value_unit", generator.measure_value_unit);
    query.bindValue(":relative_error", generator.relative_error);
    query.bindValue(":urel", generator.urel);
    query.bindValue(":frequency", generator.frequency);
    query.bindValue(":frequency_unit", generator.frequency_unit);
    query.bindValue(":power", generator.power);
    query.bindValue(":power_unit", generator.power_unit);
    query.bindValue(":amplitude", generator.amplitude);
    query.bindValue(":amplitude_unit", generator.amplitude_unit);
    query.bindValue(":high_pass_filter", generator.high_pass_filter);
    query.bindValue(":high_pass_filter_unit", generator.high_pass_filter_unit);
    query.bindValue(":lower_pass_filter", generator.lower_pass_filter);
    query.bindValue(":lower_pass_filter_unit", generator.lower_pass_filter_unit);
    query.bindValue(":modulation_rate", generator.modulation_rate);
    query.bindValue(":modulation_rate_unit", generator.modulation_rate_unit);
    query.bindValue(":thd", generator.thd);
    query.bindValue(":upper_index_symbol", generator.upper_index_symbol);
    query.bindValue(":upper_index_unit", generator.upper_index_unit);
    query.bindValue(":remain_amplitude_modulation",
        generator.remain_amplitude_modulation);
    query.bindValue(":harmonic_number", generator.harmonic_number);
    query.bindValue(":fundamental_amplitude", generator.fundamental_amplitude);
    query.bindValue(":fundamental_amplitude_unit",
        generator.fundamental_amplitude_unit);
    query.bindValue(":harmonic_amplitude", generator.harmonic_amplitude);
    query.bindValue(":harmonic_amplitude_unit",
        generator.harmonic_amplitude_unit);
    query.bindValue(":offset_frequency", generator.offset_frequency);
    query.bindValue(":offset_frequency_unit", generator.offset_frequency_unit);
    query.bindValue(":exceed_mark", generator.exceed_mark);
    query.bindValue(":exceed_mark_one", generator.exceed_mark_one);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (isSuccess) {
        QString ssql = "INSERT INTO biz_business_device_measure_data_signal_generator "
                       "( id, data_id, data_type, nominal_value, unit, upper_index, "
                       "upper_thd, lower_index, measure_value, measure_value_unit,"
                       " relative_error, urel, frequency, frequency_unit, power, power_unit, "
                       "amplitude, amplitude_unit, high_pass_filter, "
                       "high_pass_filter_unit, lower_pass_filter, lower_pass_filter_unit, "
                       "modulation_rate, modulation_rate_unit, thd, upper_index_symbol, "
                       "upper_index_unit, remain_amplitude_modulation,harmonic_number, "
                       "fundamental_amplitude, fundamental_amplitude_unit, "
                       "harmonic_amplitude, harmonic_amplitude_unit, offset_frequency, "
                       "offset_frequency_unit, exceed_mark, exceed_mark_one, "
                       "create_time, update_time ) VALUES ( :1, :2, :3, :4, :5, :6, :7, :8, "
                       ":9, :10, :11, :12, :13, :14, :15, :16, :17,"
                       " :18, :19, :20, :21, :22, :23, :24,"
                       " :25, :26, :27, :28, :29, :30,"
                       " :31, :32, :33, :34, :35,"
                       " :36, :37, :38, :39 );";
        saveLogToSql(ssql, &generator);
    } else {
        qDebug() << "Insert data failed: " << query.lastError().text();
    }
    return isSuccess;
}

// 新建检定程序中查询所有的标准装置
QList<StandardInfo> BaseCommonApi::getStandardInfo()
{
    QList<StandardInfo> result;
    QSqlQuery query;
    bool isSuccess = query.exec(
        "SELECT besi.id, besi.standard_cert_no, besi.standard_name, "
        "besi.dept_name, besi.manage_status, "
        " GROUP_CONCAT(bestir.measure_item SEPARATOR ';') AS merged_measure_items"
        " FROM biz_equipment_standard_info besi"
        " LEFT JOIN biz_equipment_standard_technology_index_relation bestir ON "
        "besi.standard_no = bestir.standard_no"
        " GROUP BY besi.id;");
    if (isSuccess) {
        while (query.next()) {
            StandardInfo info;
            info.id = query.value("id").toString();
            info.standard_cert_no = query.value("standard_cert_no").toString();
            info.standard_name = query.value("standard_name").toString();
            info.dept_name = query.value("dept_name").toString();
            info.manage_status = query.value("manage_status").toString();
            info.measure_item = query.value("merged_measure_items").toString();
            result.append(info);
        }
    } else {
        qDebug() << "select data failed: " << query.lastError().text();
    }
    return result;
}

// 根据标准装置id查询标准设备
QList<BizDevice> BaseCommonApi::getStandardInfoListByStandardId(
    QString standardId)
{
    QList<BizDevice> result;
    QSqlQuery query;
    query.prepare(QString("SELECT bei.id ,bei.equipment_name ,bei.model "
                          ",bei.manufacture_no ,bei.manufacturer,bei.type,"
                          " GROUP_CONCAT(bett.measure_param SEPARATOR ';') AS "
                          "measure_params FROM biz_equipment_info bei"
                          " JOIN biz_equipment_standard_equipment_relation beser "
                          "ON beser.equipment_id = bei.id"
                          " LEFT JOIN biz_equipment_technical_target bett ON "
                          "bett.equipment_no = bei.equipment_no"
                          " WHERE beser.standard_id = %1"
                          " GROUP BY bei.id ")
                      .arg(standardId));
    // query.bindValue(":standardId",standardId);
    bool isSuccess = query.exec();
    if (isSuccess) {
        while (query.next()) {
            BizDevice device;
            device.id = query.value("id").toString();
            device.equipment_name = query.value("equipment_name").toString();
            device.model = query.value("model").toString();
            device.manufacture_no = query.value("manufacture_no").toString();
            device.manufacturer = query.value("manufacturer").toString();
            device.type = query.value("type").toString();
            device.measure_param = query.value("measure_params").toString();
            result.append(device);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
}
QMap<QString, QList<QStringList>> BaseCommonApi::getDeviceMicrowaveAttenuation(
    QString itemCatoryName, QString itemId,
    SequentialMap<QString, QStringList>* headList)
{
    QMap<QString, QList<QStringList>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();

    QSqlQuery query;
    query.prepare(
        "SELECT bbdmidama.*,bbdmii.attenuation_amount  FROM "
        "biz_business_device_measure_item_data_microwave_attenuation bbdmidama"
        " JOIN biz_business_device_measure_item_info bbdmii ON bbdmii.id = "
        "bbdmidama.item_id"
        " WHERE bbdmidama.item_id = :itemId ORDER BY bbdmidama.create_time ASC");
    // 将参数绑定到占位符
    query.bindValue(":itemId", itemId);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_id").toString());
            result.append(query.value("data_type").toString()); // 检定项目ID
            result.append(
                query.value("attenuation_amount").toString()); // 检定项目状态
            result.append(query.value("frequency").toString()); // 频率
            result.append(query.value("frequency_unit").toString()); // 频率单位
            result.append(query.value("nominal_value").toString()); // 标称值
            result.append(
                query.value("nominal_value_unit").toString()); // 标称值单位 7
            result.append(query.value("composition").toString()); // 组合方式
            result.append(query.value("urel").toString()); // U(k=2)
            result.append(
                query.value("technical_index_symbol").toString()); // 技术指标前符号
            result.append(query.value("technical_index").toString()); // 技术指标
            result.append(
                query.value("technical_index_unit").toString()); // 技术指标单位
            result.append(query.value("create_time").toString()); // 创建时间
            result.append(query.value("update_time").toString()); // 更新时间
            resultData.append(result);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    DataMapList = GetDataReturnItemListByMicrowaveAttenuation(
        itemCatoryName, resultData, DataMapList, headList);
    return DataMapList;
}
QMap<QString, QList<QStringList>>
BaseCommonApi::GetDataReturnItemListByMicrowaveAttenuation(
    QString itemCatoryName, QList<QStringList> ResultData,
    QMap<QString, QList<QStringList>> DataMapList,
    SequentialMap<QString, QStringList>* HeadMapList)
{
    QList<QStringList> PadValueList;
    for (const QStringList& row : ResultData) {
        QString VerificationItem = NULL;
        if (!row.isEmpty() && row[2] == "1" && row[3] == "1") {
            VerificationItem = "衰减量";
            // 添加列头
            QStringList head;
            head << "序号"
                 << "检定项目"
                 << "频率"
                 << "标称值"
                 << "组合方式"
                 << "直通测量值"
                 << "加入衰减测量值"
                 << "指标下限"
                 << "衰减量"
                 << "指标上限"
                 << "U(k=2)"
                 << "结论";
            HeadMapList->insert(VerificationItem, head);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(VerificationItem); // 检定项目
            Data.append(row[4] + row[5]); // 频率
            Data.append(row[6] + row[7]); // 标称值
            Data.append(row[8]); // 组合方式
            Data.append(""); // 直通测量值
            Data.append(""); // 加入衰减测量值
            Data.append(QString::number(row[6].toDouble() - row[11].toDouble()) + row[7]); // 指标下限
            Data.append(""); // 衰减量
            Data.append(QString::number(row[6].toDouble() + row[11].toDouble()) + row[7]); // 指标上限
            Data.append(""); // U(k=2)
            Data.append(row[9]); // 结论
            PadValueList.append(Data);
        }
    }
    if (PadValueList.count() != 0) {
        DataMapList.insert("衰减量", PadValueList);
    };
    return DataMapList;
}
// 【结果保存】 微波衰减器
bool BaseCommonApi::InsertDeviceMicrowaveAttenuation(
    MicrowaveAttenuationCalibration scope)
{
    QSqlQuery query;
    query.prepare(
        " INSERT INTO biz_business_device_measure_data_microwave_attenuation "
        " (id,data_id,data_type,frequency,frequency_unit,nominal_value, "
        " nominal_value_unit,composition,direct_measure_value,add_measure_value, "
        " attenuation_amount,lower_index,upper_index,urel,conclusion,exceed_mark,"
        " "
        " create_time,update_time)VALUES( "
        " :id,:data_id,:data_type,:frequency,:frequency_unit,:nominal_value, "
        " :nominal_value_unit,:composition,:direct_measure_value,:add_measure_"
        "value, "
        " :attenuation_amount,:lower_index,:upper_index,:urel,:conclusion,:"
        "exceed_mark,"
        " :create_time,:update_time "
        " )");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", scope.id);
    query.bindValue(":data_id", scope.data_id);
    query.bindValue(":data_type", scope.data_type);
    query.bindValue(":frequency", scope.frequency);
    query.bindValue(":frequency_unit", scope.frequency_unit);
    query.bindValue(":nominal_value", scope.nominal_value);
    query.bindValue(":nominal_value_unit", scope.nominal_value_unit);
    query.bindValue(":composition", scope.composition);
    query.bindValue(":direct_measure_value", scope.direct_measure_value);
    query.bindValue(":add_measure_value", scope.add_measure_value);
    query.bindValue(":attenuation_amount", scope.attenuation_amount);
    query.bindValue(":lower_index", scope.lower_index);
    query.bindValue(":upper_index", scope.upper_index);
    query.bindValue(":urel", scope.urel);
    query.bindValue(":exceed_mark", scope.exceed_mark);
    query.bindValue(":conclusion", scope.conclusion);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << "Insert data failed: " << query.lastError().text();
    }
    return isSuccess;
}
QMap<QString, QList<QStringList>>
BaseCommonApi::getDeviceMeasureItemDataLFSignalGenerator(
    QString itemCatoryName, QString itemId,
    SequentialMap<QString, QStringList>* headList)
{
    QMap<QString, QList<QStringList>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();

    QSqlQuery query;
    query.prepare(
        "SELECT *,bbdmii.frequency as freq  FROM "
        "biz_business_device_measure_item_data_low_frequency_signal bbdmidlfs"
        " JOIN biz_business_device_measure_item_info bbdmii ON bbdmii.id = "
        "bbdmidlfs.item_id"
        " WHERE bbdmidlfs.item_id = :itemId ORDER BY bbdmidlfs.create_time ASC");
    // 将参数绑定到占位符
    query.bindValue(":itemId", itemId);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_id").toString());
            result.append(query.value("data_type").toString()); // 检定项目ID
            result.append(query.value("freq").toString()); // 频率状态 3
            result.append(
                query.value("harmonic_distortion").toString()); // 谐波失真状态 4
            result.append(query.value("rise_time").toString()); // 上升时间状态 5
            result.append(query.value("ac_voltage").toString()); // 交流电压状态 6
            result.append(query.value("dc_voltage").toString()); // 直流电流状态 7
            result.append(
                query.value("sinusoid_flatness").toString()); // 正弦信号平坦度状态 8
            result.append(query.value("frequency").toString()); // 频率
            result.append(query.value("frequency_unit").toString()); // 频率单位
            result.append(query.value("nominal_value").toString()); // 标称值
            result.append(query.value("unit").toString()); // 标称值单位 12
            result.append(query.value("amplitude").toString()); // 幅度
            result.append(query.value("amplitude_unit").toString()); // 幅度单位
            result.append(
                query.value("technical_index_symbol").toString()); // 技术指标前符号
            result.append(query.value("technical_index").toString()); // 技术指标
            result.append(
                query.value("technical_index_unit").toString()); // 技术指标单位  17
            result.append(query.value("ac_frequency").toString()); // 交流频率
            result.append(
                query.value("ac_frequency_unit").toString()); // 交流频率单位
            result.append(query.value("valid_digit").toString()); // 有效位数
            result.append(
                query.value("datum_point").toString()); // 是否为基准点(1/0)  21
            result.append(query.value("voltage").toString()); // 电压
            result.append(query.value("voltage_unit").toString()); // 电压单位
            result.append(query.value("value_one")
                              .toString()); // 数值1(不带%,计算技术指标使用)
            result.append(
                query.value("value_two").toString()); // 数值2(计算技术指标使用) 25
            result.append(query.value("create_time").toString()); // 创建时间
            result.append(query.value("update_time").toString()); // 更新时间
            resultData.append(result);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    DataMapList = GetDataReturnItemListByLFSignalGenerator(
        itemCatoryName, resultData, DataMapList, headList);
    return DataMapList;
}
QMap<QString, QList<QStringList>>
BaseCommonApi::GetDataReturnItemListByLFSignalGenerator(
    QString itemCatoryName, QList<QStringList> ResultData,
    QMap<QString, QList<QStringList>> DataMapList,
    SequentialMap<QString, QStringList>* HeadMapList)
{
    QList<QStringList> freqList;
    QList<QStringList> ac_voltageList;
    QList<QStringList> dc_voltageList;
    QList<QStringList> rise_timeList;
    QList<QStringList> sinusoid_flatnessList;
    QList<QStringList> harmonic_distortionList;

    QStringList freqHead;
    QStringList ac_voltageHead;
    QStringList dc_voltageHead;
    QStringList rise_timeHead;
    QStringList sinusoid_flatnessHead;
    QStringList harmonic_distortionHead;
    for (const QStringList& row : ResultData) {
        QString VerificationItem = NULL;
        // state   data type
        if (!row.isEmpty() && row[3] == "1" && row[2] == "1") {
            VerificationItem = "频率";
            // 添加列头
            QStringList head;
            head << "序号"
                 << "检定项目"
                 << "标称值"
                 << "幅度"
                 << "指标下限"
                 << "测量值"
                 << "指标上限"
                 << "结论"
                 << "有效位数";
            freqHead = head;
            // HeadMapList->insert(VerificationItem, head);
            //  添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(VerificationItem); // 检定项目
            Data.append(row[11] + row[12]); // 标称值
            Data.append(row[13] + row[14]); // 幅度
            Data.append(QString::number(row[11].toDouble() - (row[11].toDouble() * row[16].toDouble())) + row[12]); // 指标下限
            Data.append(""); // 测量值
            Data.append(QString::number(row[11].toDouble() + (row[11].toDouble() * row[16].toDouble())) + row[12]); // 指标上限
            Data.append(""); // 结论
            Data.append(row[20]); // 有效位数
            freqList.append(Data);
        } else if (!row.isEmpty() && row[6] == "1" && row[2] == "2") {
            VerificationItem = "交流电压";
            // 添加列头
            QStringList head;
            head << "序号"
                 << "检定项目"
                 << "标称值"
                 << "幅度"
                 << "交流频率"
                 << "指标下限"
                 << "测量值"
                 << "指标上限"
                 << "结论";
            ac_voltageHead = head;
            // HeadMapList->insert(VerificationItem, head);
            //  添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(VerificationItem); // 检定项目
            Data.append(row[11] + row[12]); // 标称值
            Data.append(row[13] + row[14]); // 幅度
            Data.append(row[18] + row[19]); // 交流频率
            Data.append(QString::number(row[11].toDouble() - (row[11].toDouble() * row[16].toDouble())) + row[12]); // 指标下限
            Data.append(""); // 测量值
            Data.append(QString::number(row[11].toDouble() + (row[11].toDouble() * row[16].toDouble())) + row[12]); // 指标上限
            Data.append(""); // 结论
            ac_voltageList.append(Data);
        } else if (!row.isEmpty() && row[7] == "1" && row[2] == "3") {
            VerificationItem = "直流电压";
            // 添加列头
            QStringList head;
            head << "序号"
                 << "检定项目"
                 << "标称值"
                 << "幅度"
                 << "指标下限"
                 << "测量值"
                 << "指标上限"
                 << "结论";
            dc_voltageHead = head;
            // HeadMapList->insert(VerificationItem, head);
            //  添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(VerificationItem); // 检定项目
            Data.append(row[11] + row[12]); // 标称值
            Data.append(row[13] + row[14]); // 幅度
            Data.append(QString::number(row[11].toDouble() - row[16].toDouble()) + row[12]); // 指标下限
            Data.append(""); // 测量值
            Data.append(QString::number(row[11].toDouble() + row[16].toDouble()) + row[12]); // 指标上限
            Data.append(""); // 结论
            dc_voltageList.append(Data);
        } else if (!row.isEmpty() && row[5] == "1" && row[2] == "4") {
            VerificationItem = "上升时间";
            // 添加列头
            QStringList head;
            head << "序号"
                 << "检定项目"
                 << "频率"
                 << "幅度"
                 << "上升时间"
                 << "技术指标"
                 << "结论";
            rise_timeHead = head;
            // HeadMapList->insert(VerificationItem, head);
            //  添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(VerificationItem); // 检定项目
            Data.append(row[9] + row[10]); // 频率
            Data.append(row[13] + row[14]); // 幅度
            Data.append(""); // 上升时间
            Data.append(row[15] + row[16] + row[17]); // 技术指标
            Data.append(""); // 结论
            rise_timeList.append(Data);
        } else if (!row.isEmpty() && row[8] == "1" && row[2] == "5") {
            VerificationItem = "正弦信号平坦度";
            // 添加列头
            QStringList head;
            head << "序号"
                 << "检定项目"
                 << "电压"
                 << "频率点"
                 << "测量值"
                 << "平坦度"
                 << "技术指标"
                 << "结论"
                 << "基准点";
            sinusoid_flatnessHead = head;
            // HeadMapList->insert(VerificationItem, head);
            //  添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(VerificationItem); // 检定项目
            Data.append(row[22] + row[23]); // 电压
            Data.append(row[9] + row[10]); // 频率点
            Data.append(""); // 测量值
            Data.append(""); // 平坦度
            Data.append(row[15] + row[16] + row[17]); // 技术指标
            Data.append(""); // 结论
            Data.append(row[21]); // 基准点
            sinusoid_flatnessList.append(Data);
        } else if (!row.isEmpty() && row[8] == "1" && row[2] == "5") {
            VerificationItem = "总谐波失真";
            // 添加列头
            QStringList head;
            head << "序号"
                 << "检定项目"
                 << "频率"
                 << "测量值"
                 << "技术指标"
                 << "结论";
            harmonic_distortionHead = head;
            // HeadMapList->insert(VerificationItem, head);
            //  添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(VerificationItem); // 检定项目
            Data.append(row[9] + row[10]); // 频率点
            Data.append(""); // 测量值
            Data.append(row[15] + row[16] + row[17]); // 技术指标
            Data.append(""); // 结论
            harmonic_distortionList.append(Data);
        }
    }
    if (freqList.count() != 0) {
        DataMapList.insert("频率", freqList);
        HeadMapList->insert("频率", freqHead);
    };
    if (ac_voltageList.count() != 0) {
        DataMapList.insert("交流电压", ac_voltageList);
        HeadMapList->insert("交流电压", ac_voltageHead);
    };
    if (dc_voltageList.count() != 0) {
        DataMapList.insert("直流电压", dc_voltageList);
        HeadMapList->insert("直流电压", dc_voltageHead);
    };
    if (rise_timeList.count() != 0) {
        DataMapList.insert("上升时间", rise_timeList);
        HeadMapList->insert("上升时间", rise_timeHead);
    };
    if (sinusoid_flatnessList.count() != 0) {
        DataMapList.insert("正弦信号平坦度", sinusoid_flatnessList);
        HeadMapList->insert("正弦信号平坦度", sinusoid_flatnessHead);
    };
    if (harmonic_distortionList.count() != 0) {
        DataMapList.insert("总谐波失真", harmonic_distortionList);
        HeadMapList->insert("总谐波失真", harmonic_distortionHead);
    };
    return DataMapList;
}
bool BaseCommonApi::InsertDeviceLFSignalGenerator(
    LFSignalGeneratoCalibration scope)
{
    QSqlQuery query;
    query.prepare(
        " INSERT INTO "
        "biz_business_device_measure_data_low_frequency_signal(id,data_id,data_"
        "type,"
        "  "
        "nominal_value,unit,upper_index,lower_index,measure_value,measure_value_"
        "unit,conclusion,"
        "  "
        "convert_deviation,frequency,frequency_unit,voltage,voltage_unit,rise_"
        "time,datum_point,"
        "  "
        "ac_frequency,ac_frequency_unit,amplitude,amplitude_unit,flatness,"
        "technical_index_symbol,"
        "  "
        "technical_index,technical_index_unit,exceed_mark,create_time,update_"
        "time)VALUES("
        "  :id,:data_id,:data_type,"
        "  "
        ":nominal_value,:unit,:upper_index,:lower_index,:measure_value,:measure_"
        "value_unit,:conclusion,"
        "  "
        ":convert_deviation,:frequency,:frequency_unit,:voltage,:voltage_unit,:"
        "rise_time,:datum_point,"
        "  "
        ":ac_frequency,:ac_frequency_unit,:amplitude,:amplitude_unit,:flatness,:"
        "technical_index_symbol,"
        "  "
        ":technical_index,:technical_index_unit,:exceed_mark,:create_time,:"
        "update_time"
        "  )");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", scope.id);
    query.bindValue(":data_id", scope.data_id);
    query.bindValue(":data_type", scope.data_type);
    query.bindValue(":nominal_value", scope.nominal_value);
    query.bindValue(":unit", scope.unit);
    query.bindValue(":lower_index", scope.lower_index);
    query.bindValue(":upper_index", scope.upper_index);
    query.bindValue(":measure_value", scope.measure_value);
    query.bindValue(":measure_value_unit", scope.measure_value_unit);
    query.bindValue(":conclusion", scope.conclusion);
    query.bindValue(":convert_deviation", scope.convert_deviation);
    query.bindValue(":frequency", scope.frequency);
    query.bindValue(":frequency_unit", scope.frequency_unit);
    query.bindValue(":voltage", scope.voltage);
    query.bindValue(":voltage_unit", scope.voltage_unit);
    query.bindValue(":rise_time", scope.rise_time);
    query.bindValue(":datum_point", scope.datum_point);
    query.bindValue(":ac_frequency", scope.ac_frequency);
    query.bindValue(":ac_frequency_unit", scope.ac_frequency_unit);
    query.bindValue(":amplitude", scope.amplitude);
    query.bindValue(":amplitude_unit", scope.amplitude_unit);
    query.bindValue(":flatness", scope.flatness);
    query.bindValue(":technical_index_symbol", scope.technical_index_symbol);
    query.bindValue(":technical_index", scope.technical_index);
    query.bindValue(":technical_index_unit", scope.technical_index_unit);
    query.bindValue(":exceed_mark", scope.exceed_mark);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (isSuccess) {
    } else {
        qDebug() << "Insert data failed: " << query.lastError().text();
    }

    return isSuccess;
}
// 检定项-小功率装置
QMap<QString, QList<QStringList>> BaseCommonApi::getDeviceSmallPower(
    QString itemCatoryName, QString itemId,
    SequentialMap<QString, QStringList>* headList)
{
    QMap<QString, QList<QStringList>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();

    QSqlQuery query;
    query.prepare(
        "SELECT bbdmidlp.*,bbdmii.calibration_factor  FROM "
        "biz_business_device_measure_item_data_low_power bbdmidlp"
        " JOIN biz_business_device_measure_item_info bbdmii ON bbdmii.id = "
        "bbdmidlp.item_id"
        " WHERE bbdmidlp.item_id = :itemId ORDER BY bbdmidlp.create_time ASC");
    // 将参数绑定到占位符
    query.bindValue(":itemId", itemId);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_id").toString());
            result.append(query.value("data_type").toString()); // 检定项目ID
            result.append(query.value("calibration_factor").toString()); // 启动状态
            result.append(query.value("frequency").toString()); // 频率
            result.append(query.value("frequency_unit").toString()); // 频率单位  5
            result.append(query.value("kc").toString()); // Kc/%
            result.append(query.value("standard_reflection_coefficient")
                              .toString()); // 标准反射系数ГGe
            result.append(query.value("device_reflection_coefficient")
                              .toString()); // 被检设备反射系数Гu  8
            result.append(query.value("urel").toString()); // U(k=2)
            result.append(query.value("create_time").toString()); // 创建时间
            result.append(query.value("update_time").toString()); // 更新时间
            resultData.append(result);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    DataMapList = GetDataReturnItemListBySmallPower(
        itemCatoryName, resultData, DataMapList, headList);
    return DataMapList;
}
QMap<QString, QList<QStringList>>
BaseCommonApi::GetDataReturnItemListBySmallPower(
    QString itemCatoryName, QList<QStringList> ResultData,
    QMap<QString, QList<QStringList>> DataMapList,
    SequentialMap<QString, QStringList>* HeadMapList)
{
    QList<QStringList> calibrationFactorList;

    for (const QStringList& row : ResultData) {
        QString VerificationItem = NULL;
        // state   data type
        if (!row.isEmpty() && row[3] == "1" && row[2] == "1") {
            VerificationItem = "校准因子";
            // 添加列头
            QStringList head;
            head << "序号"
                 << "检定项目"
                 << "频率"
                 << "Pcu/mW"
                 << "Kc/%"
                 << "被测探头示数Pbu/mW"
                 << "标准反射系数ГGe"
                 << "被检设备反射系数Гu"
                 << "被测探头校准因子Kb/%"
                 << "U(k=2)";
            HeadMapList->insert(VerificationItem, head);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(VerificationItem); // 检定项目
            Data.append(row[4] + row[5]); // 频率
            Data.append(""); // 幅度
            Data.append(row[6]); // Kc/%
            Data.append(""); // 被测探头示数Pbu/mW
            Data.append(row[7]); // 标准反射系数ГGe
            Data.append(row[8]); // 被检设备反射系数Гu
            Data.append(""); // 被测探头校准因子Kb/%
            Data.append(row[9]); // U(k=2)
            calibrationFactorList.append(Data);
        }
    }
    if (calibrationFactorList.count() != 0) {
        DataMapList.insert("校准因子", calibrationFactorList);
    };
    return DataMapList;
}
bool BaseCommonApi::InsertDeviceSmallPower(SmallPowerCalibration scope)
{
    QSqlQuery query;
    query.prepare(
        " INSERT INTO "
        "biz_business_device_measure_data_low_power(id,data_id,data_type,"
        " frequency,frequency_unit,pcu,kc,probe_pbu,standard_reflection_"
        "coefficient,"
        " device_reflection_coefficient,probe_calibration_factor,urel,exceed_"
        "mark,"
        " create_time,update_time)VALUES("
        " :id,:data_id,:data_type,"
        " :frequency,:frequency_unit,:pcu,:kc,:probe_pbu,:standard_reflection_"
        "coefficient,"
        " :device_reflection_coefficient,:probe_calibration_factor,:urel,:exceed_"
        "mark,"
        " :create_time,:update_time"
        " )");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", scope.id);
    query.bindValue(":data_id", scope.data_id);
    query.bindValue(":data_type", scope.data_type);
    query.bindValue(":frequency", scope.frequency);
    query.bindValue(":frequency_unit", scope.frequency_unit);
    query.bindValue(":pcu", scope.pcu);
    query.bindValue(":kc", scope.kc);
    query.bindValue(":probe_pbu", scope.probe_pbu);
    query.bindValue(":standard_reflection_coefficient",
        scope.standard_reflection_coefficient);
    query.bindValue(":device_reflection_coefficient",
        scope.device_reflection_coefficient);
    query.bindValue(":probe_calibration_factor", scope.probe_calibration_factor);
    query.bindValue(":urel", scope.urel);
    query.bindValue(":exceed_mark", scope.exceed_mark);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (isSuccess) {
    } else {
        qDebug() << "Insert data failed: " << query.lastError().text();
    }

    return isSuccess;
}

// 【查询】检定项-s参数
QMap<QString, QList<QStringList>> BaseCommonApi::getDeviceMeasureItemDataSParam(QString itemCatoryName, QString itemId, SequentialMap<QString, QStringList>* headList)
{
    QMap<QString, QList<QStringList>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();

    QSqlQuery query;
    query.prepare(
        " SELECT bbdmidsp.*,bbdmii.vswr as vswrVerification,bbdmii.insertion_loss,bbdmii.isolation_degree,bbdmii.gain_flatness,bbdmii.coupling_direction,bbdmii.amplitude_consistency,bbdmii.phase_shift,"
        " bbdmii.phase_consistency,bbdmii.group_delay,bbdmii.filter_character,bbdmii.point_output_power_level,bbdmii.power_flatness,bbdmii.spurious_suppression,bbdmii.phase_noise,"
        " bbdmii.transfer_amplitude,bbdmii.transfer_phase_shift FROM biz_business_device_measure_item_data_s_param bbdmidsp"
        " JOIN biz_business_device_measure_item_info bbdmii ON bbdmidsp.item_id = bbdmii.id WHERE bbdmidsp.item_id = :item_id ORDER BY bbdmidsp.create_time ASC");
    // 将参数绑定到占位符
    query.bindValue(":itemId", itemId);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_id").toString()); // 检定项id
            result.append(query.value("data_type").toString()); // 检定项数据类型(一个检定项中区分两个表格)(字典code)
            result.append(query.value("type_value").toString()); // 类型
            result.append(query.value("display_format").toString()); // 显示格式
            result.append(query.value("if_bandwidth").toString()); // 中频带宽  5
            result.append(query.value("if_bandwidth_unit").toString()); // 中频带宽单位
            result.append(query.value("frequency").toString()); // 频率
            result.append(query.value("frequency_unit").toString()); // 频率单位
            result.append(query.value("test_number").toString()); // 测试次数
            result.append(query.value("urel").toString()); // U(k=2)/增益U(k=2)/耦合性U(k=2)/3dB带宽U(k=2)/输出功率U(k=2)   10
            result.append(query.value("flatness_urel").toString()); // 增益平坦度U(k=2)/方向性U(k=2)/60dB带宽U(k=2)/输出功率平坦度U(k=2)
            result.append(query.value("min_loss_urel").toString()); // 通带最小损耗U(k=2)
            result.append(query.value("max_loss_urel").toString()); // 阻带最大损耗U(k=2)
            result.append(query.value("upper_frequency").toString()); // 上边频频率
            result.append(query.value("upper_frequency_unit").toString()); // 上边频频率单位
            result.append(query.value("lower_frequency").toString()); // 下边频频率
            result.append(query.value("lower_frequency_unit").toString()); // 下边频频率单位
            result.append(query.value("output_power").toString()); // 输出功率
            result.append(query.value("output_power_unit").toString()); // 输出功率单位
            result.append(query.value("input_power").toString()); // 输入功率     20
            result.append(query.value("input_power_unit").toString()); // 输入功率单位
            result.append(query.value("fundamental_frequency").toString()); // 基波频率
            result.append(query.value("fundamental_frequency_unit").toString()); // 基波频率单位
            result.append(query.value("spectrum_level").toString()); // 参考电平
            result.append(query.value("spectrum_level_unit").toString()); // 参考电平单位
            result.append(query.value("intermediate_frequency").toString()); // 中频频率
            result.append(query.value("intermediate_frequency_unit").toString()); // 中频频率单位
            result.append(query.value("span").toString()); // 跨度
            result.append(query.value("resolution_bandwidth").toString()); // 分辨带宽
            result.append(query.value("resolution_bandwidth_unit").toString()); // 分辨带宽单位   30
            result.append(query.value("fundamental_amplitude").toString()); // 基波幅度
            result.append(query.value("fundamental_amplitude_unit").toString()); // 基波幅度单位
            result.append(query.value("harmonic_number").toString()); // 谐波次数
            result.append(query.value("rbw_value").toString()); // RBW
            result.append(query.value("rbw_value_unit").toString()); // RBW单位
            result.append(query.value("vbw_value").toString()); // VBW
            result.append(query.value("vbw_value_unit").toString()); // VBW单位
            result.append(query.value("sideband_frequency").toString()); // 边带频率
            result.append(query.value("sideband_frequency_unit").toString()); // 边带频率单位
            result.append(query.value("random_noise_correct_value").toString()); // 随机噪声修正值   40
            result.append(query.value("random_noise_correct_value_unit").toString()); // 随机噪声修正值单位
            result.append(query.value("vsmr").toString()); // VSMR
            result.append(query.value("standard_value").toString()); // 标准值
            result.append(query.value("standard_value_unit").toString()); // 标准值单位
            result.append(query.value("technical_index_symbol").toString()); // 技术指标前符号
            result.append(query.value("technical_index").toString()); // 技术指标
            result.append(query.value("technical_index_unit").toString()); // 技术指标单位
            result.append(query.value("amplitude").toString()); // 幅度
            result.append(query.value("amplitude_unit").toString()); // 幅度单位

            result.append(query.value("vswrVerification").toString()); // 电压驻波比(S参数-微波器件/矢量网络分析仪) 1/0    50
            result.append(query.value("insertion_loss").toString()); // 插入损耗(S参数) 1/0
            result.append(query.value("isolation_degree").toString()); // 隔离度(S参数) 1/0
            result.append(query.value("gain_flatness").toString()); // 增益、增益平坦度(S参数) 1/0
            result.append(query.value("coupling_direction").toString()); // 耦合度、方向性(S参数) 1/0
            result.append(query.value("amplitude_consistency").toString()); // 幅度一致性(S参数) 1/0
            result.append(query.value("phase_shift").toString()); // 相移(S参数) 1/0
            result.append(query.value("phase_consistency").toString()); // 相位一致性(S参数) 1/0
            result.append(query.value("group_delay").toString()); // 群时延(S参数) 1/0
            result.append(query.value("filter_character").toString()); // 滤波器特性(S参数) 1/0
            result.append(query.value("point_output_power_level").toString()); // 1dB压缩点输出功率电平(S参数) 1/0    60
            result.append(query.value("power_flatness").toString()); // 输出功率、功率平坦度(S参数) 1/0
            result.append(query.value("spurious_suppression").toString()); // 杂散抑制(S参数) 1/0
            result.append(query.value("phase_noise").toString()); // 相位噪声(S参数) 1/0
            result.append(query.value("transfer_amplitude").toString()); // 传输幅度(S参数-矢量网络分析仪) 1/0
            result.append(query.value("transfer_phase_shift").toString()); // 传输相移(S参数-矢量网络分析仪) 1/0
            result.append(query.value("create_time").toString()); // 创建时间
            result.append(query.value("update_time").toString()); // 更新时间
            resultData.append(result);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    DataMapList = GetDataReturnItemListBySParam(
        itemCatoryName, resultData, DataMapList, headList);
    return DataMapList;
}
QMap<QString, QList<QStringList>> BaseCommonApi::GetDataReturnItemListBySParam(QString itemCatoryName, QList<QStringList> ResultData, QMap<QString, QList<QStringList>> DataMapList, SequentialMap<QString, QStringList>* HeadMapList)
{
    QList<QStringList> VoltageStandingWaveRatioList;
    QList<QStringList> TransmissionAmplitudeList;
    QList<QStringList> TransmissionPhaseShiftList;
    if (itemCatoryName.contains("矢量网络分析仪")) {
        for (const QStringList& row : ResultData) {
            QString VerificationItem = NULL;
            if (!row.isEmpty() && row[2] == "1" && row[50] == "1") {
                VerificationItem = "电压驻波比";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "类型"
                     << "VSWR"
                     << "频率"
                     << "频率单位"
                     << "测量次数及测量值1"
                     << "测量次数及测量值2"
                     << "测量次数及测量值3"
                     << "测量次数及测量值4"
                     << "测量次数及测量值5"
                     << "测量次数及测量值6"
                     << "平均值"
                     << "标准值"
                     << "标准值单位"
                     << "技术指标前符号"
                     << "技术指标"
                     << "技术指标单位"
                     << "误差"
                     << "U(k=2)";
                HeadMapList->insert(VerificationItem, head);
                // 添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[3]); // 类型
                Data.append(row[42]); // VSWR
                Data.append(row[7] + row[8]); // 频率
                Data.append(row[8]); // 频率单位
                Data.append(""); // 测量次数及测量值1
                Data.append(""); // 测量次数及测量值2
                Data.append(""); // 测量次数及测量值3
                Data.append(""); // 4
                Data.append(""); // 5
                Data.append(""); // 6
                Data.append(""); // 平均值
                Data.append(row[43] + row[44]); // 标准值
                Data.append(row[44]); // 标准值单位
                Data.append(row[45]); // 技术指标前符号
                Data.append(row[45] + row[46] + row[47]); // 技术指标
                Data.append(row[47]); // 技术指标单位
                Data.append(""); // 误差
                Data.append(row[10]); // U(k=2)
                VoltageStandingWaveRatioList.append(Data);
            } else if (!row.isEmpty() && row[2] == "2" && row[64] == "1") {
                VerificationItem = "传输幅度";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "类型"
                     << "频率"
                     << "频率单位"
                     << "幅度"
                     << "幅度单位"
                     << "测量次数及测量值1"
                     << "测量次数及测量值2"
                     << "测量次数及测量值3"
                     << "测量次数及测量值4"
                     << "测量次数及测量值5"
                     << "测量次数及测量值6"
                     << "平均值"
                     << "标准值"
                     << "标准值单位"
                     << "技术指标前符号"
                     << "技术指标"
                     << "技术指标单位"
                     << "误差"
                     << "U(k=2)";
                HeadMapList->insert(VerificationItem, head);
                // 添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[3]); // 类型
                Data.append(row[7] + row[8]); // 频率
                Data.append(row[8]); // 频率单位
                Data.append(row[48] + row[49]); // 幅度
                Data.append(row[49]); // 幅度单位
                Data.append(""); // 测量次数及测量值1
                Data.append(""); // 测量次数及测量值2
                Data.append(""); // 测量次数及测量值3
                Data.append(""); // 4
                Data.append(""); // 5
                Data.append(""); // 6
                Data.append(""); // 平均值
                Data.append(row[43] + row[44]); // 标准值
                Data.append(row[44]); // 标准值单位
                Data.append(row[45]); // 技术指标前符号
                Data.append(row[45] + row[46] + row[47]); // 技术指标
                Data.append(row[47]); // 技术指标单位
                Data.append(""); // 误差
                Data.append(row[10]); // U(k=2)
                TransmissionAmplitudeList.append(Data);
            } else if (!row.isEmpty() && row[2] == "2" && row[64] == "1") {
                VerificationItem = "传输幅度";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "类型"
                     << "频率"
                     << "频率单位"
                     << "幅度"
                     << "幅度单位"
                     << "测量次数及测量值1"
                     << "测量次数及测量值2"
                     << "测量次数及测量值3"
                     << "测量次数及测量值4"
                     << "测量次数及测量值5"
                     << "测量次数及测量值6"
                     << "平均值"
                     << "标准值"
                     << "标准值单位"
                     << "技术指标前符号"
                     << "技术指标"
                     << "技术指标单位"
                     << "误差"
                     << "U(k=2)";
                HeadMapList->insert(VerificationItem, head);
                // 添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[3]); // 类型
                Data.append(row[7] + row[8]); // 频率
                Data.append(row[8]); // 频率单位
                Data.append(row[48] + row[49]); // 幅度
                Data.append(row[49]); // 幅度单位
                Data.append(""); // 测量次数及测量值1
                Data.append(""); // 测量次数及测量值2
                Data.append(""); // 测量次数及测量值3
                Data.append(""); // 4
                Data.append(""); // 5
                Data.append(""); // 6
                Data.append(""); // 平均值
                Data.append(row[43] + row[44]); // 标准值
                Data.append(row[44]); // 标准值单位
                Data.append(row[45]); // 技术指标前符号
                Data.append(row[45] + row[46] + row[47]); // 技术指标
                Data.append(row[47]); // 技术指标单位
                Data.append(""); // 误差
                Data.append(row[10]); // U(k=2)
                TransmissionAmplitudeList.append(Data);
            } else if (!row.isEmpty() && row[2] == "3" && row[65] == "1") {
                VerificationItem = "传输相移";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "类型"
                     << "频率"
                     << "频率单位"
                     << "测量次数及测量值1"
                     << "测量次数及测量值2"
                     << "测量次数及测量值3"
                     << "测量次数及测量值4"
                     << "测量次数及测量值5"
                     << "测量次数及测量值6"
                     << "平均值"
                     << "标准值"
                     << "标准值单位"
                     << "技术指标前符号"
                     << "技术指标"
                     << "技术指标单位"
                     << "误差"
                     << "U(k=2)";
                HeadMapList->insert(VerificationItem, head);
                // 添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[3]); // 类型
                Data.append(row[7] + row[8]); // 频率
                Data.append(row[8]); // 频率单位
                Data.append(""); // 测量次数及测量值1
                Data.append(""); // 测量次数及测量值2
                Data.append(""); // 测量次数及测量值3
                Data.append(""); // 4
                Data.append(""); // 5
                Data.append(""); // 6
                Data.append(""); // 平均值
                Data.append(row[43] + row[44]); // 标准值
                Data.append(row[44]); // 标准值单位
                Data.append(row[45]); // 技术指标前符号
                Data.append(row[45] + row[46] + row[47]); // 技术指标
                Data.append(row[47]); // 技术指标单位
                Data.append(""); // 误差
                Data.append(row[10]); // U(k=2)
                TransmissionPhaseShiftList.append(Data);
            }
        }
    }
    if (VoltageStandingWaveRatioList.count() != 0) {
        DataMapList.insert("电压驻波比", VoltageStandingWaveRatioList);
    };
    if (TransmissionAmplitudeList.count() != 0) {
        DataMapList.insert("传输幅度", TransmissionAmplitudeList);
    };
    if (TransmissionPhaseShiftList.count() != 0) {
        DataMapList.insert("传输相移", TransmissionPhaseShiftList);
    };
    return DataMapList;
}
// 【保存】检定项-s参数
bool BaseCommonApi::InsertDeviceMeasureItemDataSParam(DeviceMeasureDataSParam sParam)
{
    QSqlQuery query;
    query.prepare("INSERT INTO biz_business_device_measure_data_s_param ("
                  " id,  data_id, data_type,  frequency,  frequency_unit, output_power, output_power_unit, test_value_one,  other_value_one, "
                  " test_value_two, other_value_two, test_value_three, other_value_three, test_value_four, other_value_four,  test_value_five, "
                  " other_value_five,  test_value_six, other_value_six, test_value_seven, other_value_seven,  test_value_eight, other_value_eight, "
                  " test_value_nine, other_value_nine, test_value_ten, other_value_ten, group_delay_unit, urel, gain_flatness, flatness_urel, "
                  " average_value, directivity_average_value,  upper_frequency, upper_frequency_unit,  lower_frequency, lower_frequency_unit, "
                  " passband_upper_frequency, passband_lower_frequency, passband_width, passband_upper_frequency_two, "
                  " passband_lower_frequency_two,  passband_width_two,  min_loss, max_loss,  min_loss_urel,  max_loss_urel, "
                  " output_power_flatness, spectrum_level, spectrum_level_unit,  intermediate_frequency, intermediate_frequency_unit, "
                  " resolution_bandwidth,  resolution_bandwidth_unit,  fundamental_frequency, fundamental_frequency_unit, "
                  " mixed_frequency, mixed_frequency_unit,  fundamental_amplitude,  fundamental_amplitude_unit,  mixed_amplitude, "
                  " mixed_amplitude_unit,  spurious_suppression,  amplitude, amplitude_unit,  harmonic_number, harmonic_amplitude, "
                  " harmonic_amplitude_unit, harmonic_distortion, random_noise_correct_value,  random_noise_correct_value_unit, "
                  " sideband_frequency, sideband_frequency_unit,  carrier_level, sideband_level,  ssb_hase_noise, type_value, "
                  " vswr,  standard_value, standard_value_value, technical_index_symbol,  technical_index,  technical_index_unit, "
                  " error_value, exceed_mark, create_time,  update_time"
                  " )  VALUES (  :id,  :data_id, :data_type, :frequency, :frequency_unit,  :output_power, :output_power_unit, "
                  " :test_value_one,  :other_value_one,  :test_value_two,  :other_value_two,  :test_value_three,  :other_value_three, "
                  " :test_value_four,  :other_value_four,  :test_value_five,  :other_value_five,  :test_value_six, :other_value_six, "
                  " :test_value_seven, :other_value_seven,  :test_value_eight, :other_value_eight, :test_value_nine, :other_value_nine, "
                  " :test_value_ten,  :other_value_ten, :group_delay_unit, :urel, :gain_flatness, :flatness_urel,  :average_value, "
                  " :directivity_average_value,  :upper_frequency, :upper_frequency_unit,  :lower_frequency,  :lower_frequency_unit, "
                  " :passband_upper_frequency,  :passband_lower_frequency, :passband_width, :passband_upper_frequency_two, "
                  " :passband_lower_frequency_two,  :passband_width_two,  :min_loss,  :max_loss,  :min_loss_urel,  :max_loss_urel, "
                  " :output_power_flatness,  :spectrum_level,  :spectrum_level_unit,  :intermediate_frequency,  :intermediate_frequency_unit, "
                  " :resolution_bandwidth,  :resolution_bandwidth_unit,  :fundamental_frequency, :fundamental_frequency_unit, "
                  " :mixed_frequency, :mixed_frequency_unit,  :fundamental_amplitude,  :fundamental_amplitude_unit, "
                  " :mixed_amplitude, :mixed_amplitude_unit, :spurious_suppression, :amplitude,  :amplitude_unit,  :harmonic_number, "
                  " :harmonic_amplitude,  :harmonic_amplitude_unit, :harmonic_distortion, :random_noise_correct_value, "
                  " :random_noise_correct_value_unit,  :sideband_frequency,  :sideband_frequency_unit,  :carrier_level, "
                  " :sideband_level,  :ssb_hase_noise,  :type_value,  :vswr,  :standard_value,  :standard_value_value,  :technical_index_symbol, "
                  " :technical_index,  :technical_index_unit,  :error_value,  :exceed_mark,  :create_time,  :update_time);");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", sParam.id);
    query.bindValue(":data_id", sParam.data_id);
    query.bindValue(":data_type", sParam.data_type);
    query.bindValue(":frequency", sParam.frequency);
    query.bindValue(":frequency_unit", sParam.frequency_unit);
    query.bindValue(":output_power", sParam.output_power);
    query.bindValue(":output_power_unit", sParam.output_power_unit);
    query.bindValue(":test_value_one", sParam.test_value_one);
    query.bindValue(":other_value_one", sParam.other_value_one);
    query.bindValue(":test_value_two", sParam.test_value_two);
    query.bindValue(":other_value_two", sParam.other_value_two);
    query.bindValue(":test_value_three", sParam.test_value_three);
    query.bindValue(":other_value_three", sParam.other_value_three);
    query.bindValue(":test_value_four", sParam.test_value_four);
    query.bindValue(":other_value_four", sParam.other_value_four);
    query.bindValue(":test_value_five", sParam.test_value_five);
    query.bindValue(":other_value_five", sParam.other_value_five);
    query.bindValue(":test_value_six", sParam.test_value_six);
    query.bindValue(":other_value_six", sParam.other_value_six);
    query.bindValue(":test_value_seven", sParam.test_value_seven);
    query.bindValue(":other_value_seven", sParam.other_value_seven);
    query.bindValue(":test_value_eight", sParam.test_value_eight);
    query.bindValue(":other_value_eight", sParam.other_value_eight);
    query.bindValue(":test_value_nine", sParam.test_value_nine);
    query.bindValue(":other_value_nine", sParam.other_value_nine);
    query.bindValue(":test_value_ten", sParam.test_value_ten);
    query.bindValue(":other_value_ten", sParam.other_value_ten);
    query.bindValue(":group_delay_unit", sParam.group_delay_unit);
    query.bindValue(":urel", sParam.urel);
    query.bindValue(":gain_flatness", sParam.gain_flatness);
    query.bindValue(":flatness_urel", sParam.flatness_urel);
    query.bindValue(":average_value", sParam.average_value);
    query.bindValue(":directivity_average_value", sParam.directivity_average_value);
    query.bindValue(":upper_frequency", sParam.upper_frequency);
    query.bindValue(":upper_frequency_unit", sParam.upper_frequency_unit);
    query.bindValue(":lower_frequency", sParam.lower_frequency);
    query.bindValue(":lower_frequency_unit", sParam.lower_frequency_unit);
    query.bindValue(":passband_upper_frequency", sParam.passband_upper_frequency);
    query.bindValue(":passband_lower_frequency", sParam.passband_lower_frequency);
    query.bindValue(":passband_width", sParam.passband_width);
    query.bindValue(":passband_upper_frequency_two", sParam.passband_upper_frequency_two);
    query.bindValue(":passband_lower_frequency_two", sParam.passband_lower_frequency_two);
    query.bindValue(":passband_width_two", sParam.passband_width_two);
    query.bindValue(":min_loss", sParam.min_loss);
    query.bindValue(":max_loss", sParam.max_loss);
    query.bindValue(":min_loss_urel", sParam.min_loss_urel);
    query.bindValue(":max_loss_urel", sParam.max_loss_urel);
    query.bindValue(":output_power_flatness", sParam.output_power_flatness);
    query.bindValue(":spectrum_level", sParam.spectrum_level);
    query.bindValue(":spectrum_level_unit", sParam.spectrum_level_unit);
    query.bindValue(":intermediate_frequency", sParam.intermediate_frequency);
    query.bindValue(":intermediate_frequency_unit", sParam.intermediate_frequency_unit);
    query.bindValue(":resolution_bandwidth", sParam.resolution_bandwidth);
    query.bindValue(":resolution_bandwidth_unit", sParam.resolution_bandwidth_unit);
    query.bindValue(":fundamental_frequency", sParam.fundamental_frequency);
    query.bindValue(":fundamental_frequency_unit", sParam.fundamental_frequency_unit);
    query.bindValue(":mixed_frequency", sParam.mixed_frequency);
    query.bindValue(":mixed_frequency_unit", sParam.mixed_frequency_unit);
    query.bindValue(":fundamental_amplitude", sParam.fundamental_amplitude);
    query.bindValue(":fundamental_amplitude_unit", sParam.fundamental_amplitude_unit);
    query.bindValue(":mixed_amplitude", sParam.mixed_amplitude);
    query.bindValue(":mixed_amplitude_unit", sParam.mixed_amplitude_unit);
    query.bindValue(":spurious_suppression", sParam.spurious_suppression);
    query.bindValue(":amplitude", sParam.amplitude);
    query.bindValue(":amplitude_unit", sParam.amplitude_unit);
    query.bindValue(":harmonic_number", sParam.harmonic_number);
    query.bindValue(":harmonic_amplitude", sParam.harmonic_amplitude);
    query.bindValue(":harmonic_amplitude_unit", sParam.harmonic_amplitude_unit);
    query.bindValue(":harmonic_distortion", sParam.harmonic_distortion);
    query.bindValue(":random_noise_correct_value", sParam.random_noise_correct_value);
    query.bindValue(":random_noise_correct_value_unit", sParam.random_noise_correct_value_unit);
    query.bindValue(":sideband_frequency", sParam.sideband_frequency);
    query.bindValue(":sideband_frequency_unit", sParam.sideband_frequency_unit);
    query.bindValue(":carrier_level", sParam.carrier_level);
    query.bindValue(":sideband_level", sParam.sideband_level);
    query.bindValue(":ssb_hase_noise", sParam.ssb_hase_noise);
    query.bindValue(":type_value", sParam.type_value);
    query.bindValue(":vswr", sParam.vswr);
    query.bindValue(":standard_value", sParam.standard_value);
    query.bindValue(":standard_value_value", sParam.standard_value_value);
    query.bindValue(":technical_index_symbol", sParam.technical_index_symbol);
    query.bindValue(":technical_index", sParam.technical_index);
    query.bindValue(":technical_index_unit", sParam.technical_index_unit);
    query.bindValue(":error_value", sParam.error_value);
    query.bindValue(":exceed_mark", sParam.exceed_mark);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << query.lastError().text();
    }
    return isSuccess;
}

// 核查程序管理-核查设置-多功能校准源  参数设备id
// 同信息管理-标准设备管理-核查项及核查点
QMap<QString, QMap<QString, QList<QStringList>>>
BaseCommonApi::getStandardCheckItemDataCalibrator(
    QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList)
{
    QMap<QString, QMap<QString, QList<QStringList>>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();
    QSqlQuery query;
    query.prepare(
        "SELECT * FROM biz_equipment_standard_check_item_data_calibrator "
        " WHERE equipment_id = :equipment_id ORDER BY create_time ASC ");
    query.bindValue(":equipment_id", equipment_id);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_category_id")
                              .toString()); // 核查项分类id(能够确定是哪个标准装置)
            result.append(query.value("equipment_id").toString()); // 配套设备id
            result.append(
                query.value("check_type").toString()); // 核查类型(直接存字典value)
            result.append(
                query.value("test_type").toString()); // 核查点类型(直接存字典value)
            result.append(
                query.value("params").toString()); // 核查项目(直接存字典value)
            result.append(query.value("unit").toString()); // 单位(直接存字典value)
            result.append(query.value("frequency").toString()); // 频率
            result.append(query.value("resolution").toString()); // 分辨力
            result.append(query.value("check_point").toString()); // 核查点
            result.append(query.value("frequency_unit")
                              .toString()); // 频率单位(直接存字典value)
            result.append(query.value("cycle_number").toString()); // 循环次数
            result.append(query.value("urel").toString()); // Urel
            result.append(query.value("thoroughfare")
                              .toString()); // 通道(直接存字典value,多个分号分割)
            result.append(query.value("tfer").toString()); // TFER(直接存字典value)
            result.append(query.value("dccp").toString()); // DCCP(直接存字典value)
            result.append(query.value("flit").toString()); // FLIT(直接存字典value)
            result.append(query.value("remark").toString()); // 核查项备注
            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());
            resultData.append(result);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    DataMapList = GetDataReturnItemListByCheckItemDataCalibrator(
        resultData, DataMapList, headList);

    return DataMapList;
};
// 【查询】核查项-多功能校准源
QMap<QString, QMap<QString, QList<QStringList>>>
BaseCommonApi::GetDataReturnItemListByCheckItemDataCalibrator(
    QList<QStringList> ResultData,
    QMap<QString, QMap<QString, QList<QStringList>>> DataMapList,
    QMap<QString, QMap<QString, QStringList>>* headList)
{
    QString nowDate = QDate::currentDate().toString("yyyy年MM月dd日");
    CheckProgramManage programInfo = BaseCommonApi::getCheckProgramManageById(loginNowVerifyProgramId);
    QList<DataCalibratorStability> data = BaseCommonApi::GetEquipmentStandardCheckInfoByids(programInfo.standard_id, loginNowVerifyEquimentId, programInfo.equipment_type, 1);
    QMap<QString, QMap<QString, QList<QStringList>>> result;
    QMap<QString, QList<QStringList>> resultMap;
    QList<QStringList> DCVList;
    QList<QStringList> ACVList;
    QList<QStringList> DCIList;
    QList<QStringList> ACIList;
    QList<QStringList> RList;
    QList<QStringList> DCVStableList;
    QList<QStringList> ACVStableList;
    QList<QStringList> DCIStableList;
    QList<QStringList> ACIStableList;
    QList<QStringList> RStableList;
    QMap<QString, QStringList> headerMap;
    QStringList historyDate;
    QStringList DCVDate;
    QStringList ACVDate;
    QStringList DCIDate;
    QStringList ACIDate;
    QStringList RDate;
    for (int i = 0; i < data.size(); ++i) {
        if (i < data.size()) {
            //            historyDate << data[i].date.toString("yyyy年MM月dd日");
            if (data[i].params == "DCV") {
                DCVDate << data[i].date.toString("yyyy年MM月dd日");
            } else if (data[i].params == "ACV") {
                ACVDate << data[i].date.toString("yyyy年MM月dd日");
            } else if (data[i].params == "DCI") {
                DCIDate << data[i].date.toString("yyyy年MM月dd日");
            } else if (data[i].params == "ACI") {
                ACIDate << data[i].date.toString("yyyy年MM月dd日");
            } else if (data[i].params == "R") {
                RDate << data[i].date.toString("yyyy年MM月dd日");
            } else {
                DCVDate << "";
                ACVDate << "";
                DCIDate << "";
                ACIDate << "";
                RDate << "";
            }
        } else {
            DCVDate << "";
            ACVDate << "";
            DCIDate << "";
            ACIDate << "";
            RDate << "";
        }
    }
    QStringList DCVvalues;
    QStringList ACVvalues;
    QStringList DCIvalues;
    QStringList ACIvalues;
    QStringList Rvalues;
    for (int i = 0; i < data.size(); ++i) {
        if (i < data.size()) {
            if (data[i].params == "DCV") {
                DCVvalues << data[i].value;
            } else if (data[i].params == "ACV") {
                ACVvalues << data[i].value;
            } else if (data[i].params == "DCI") {
                DCIvalues << data[i].value;
            } else if (data[i].params == "ACI") {
                ACIvalues << data[i].value;
            } else if (data[i].params == "R") {
                Rvalues << data[i].value;
            }
        } else {
            DCVvalues << "";
            ACVvalues << "";
            DCIvalues << "";
            ACIvalues << "";
            Rvalues << "";
        }
    }
    for (const QStringList& row : ResultData) {
        QString VerificationItem = NULL;
        if (!row.isEmpty() && row[5] == "DCV" && row[3].contains("重复性")) {
            VerificationItem = row[5];
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "单位"
                 << "核查类型"
                 << "核查点类型"
                 << "频率"
                 << "频率单位"
                 << "示值1"
                 << "示值2"
                 << "示值3"
                 << "示值4"
                 << "示值5"
                 << "示值6"
                 << "算数平均值"
                 << "标准偏差S(X)"
                 << "相对重复性";
            headerMap.insert("重复性", head);
            headList->insert(row[5], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(VerificationItem); // 检定项目
            Data.append(row[9]); // 核查点
            Data.append(row[6]); // 单位
            Data.append(row[3]); // 核查类型
            Data.append(row[4]); // 核查点类型
            Data.append(row[7]); // 频率
            Data.append(row[10]); // 频率单位
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); // 算数平均值
            Data.append(""); // 标准偏差S(X)
            Data.append(""); // 相对重复性
            DCVList.append(Data);
            resultMap.insert("重复性", DCVList);
            //                 resultMap.insert()
        } else if (!row.isEmpty() && row[5] == "ACV" && row[3].contains("重复性")) {
            VerificationItem = row[5];
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "单位"
                 << "核查类型"
                 << "核查点类型"
                 << "频率"
                 << "频率单位"
                 << "示值1"
                 << "示值2"
                 << "示值3"
                 << "示值4"
                 << "示值5"
                 << "示值6"
                 << "算数平均值"
                 << "标准偏差S(X)"
                 << "相对重复性";
            headerMap.insert("重复性", head);
            headList->insert(row[5], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(VerificationItem); // 检定项目
            Data.append(row[9]); // 核查点
            Data.append(row[6]); // 单位
            Data.append(row[3]); // 核查类型
            Data.append(row[4]); // 核查点类型
            Data.append(row[7]); // 频率
            Data.append(row[10]); // 频率单位
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); // 算数平均值
            Data.append(""); // 标准偏差S(X)
            Data.append(""); // 相对重复性
            ACVList.append(Data);
            resultMap.insert("重复性", ACVList);
            result.insert(row[5], resultMap);
        } else if (!row.isEmpty() && row[5] == "DCI" && row[3].contains("重复性")) {
            VerificationItem = row[5];
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "单位"
                 << "核查类型"
                 << "核查点类型"
                 << "频率"
                 << "频率单位"
                 << "示值1"
                 << "示值2"
                 << "示值3"
                 << "示值4"
                 << "示值5"
                 << "示值6"
                 << "算数平均值"
                 << "标准偏差S(X)"
                 << "相对重复性";
            headerMap.insert("重复性", head);
            headList->insert(row[5], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(VerificationItem); // 检定项目
            Data.append(row[9]); // 核查点
            Data.append(row[6]); // 单位
            Data.append(row[3]); // 核查类型
            Data.append(row[4]); // 核查点类型
            Data.append(row[7]); // 频率
            Data.append(row[10]); // 频率单位
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); // 算数平均值
            Data.append(""); // 标准偏差S(X)
            Data.append(""); // 相对重复性
            DCIList.append(Data);
            resultMap.insert("重复性", DCIList);
            result.insert(row[5], resultMap);
        } else if (!row.isEmpty() && row[5] == "ACI" && row[3].contains("重复性")) {
            VerificationItem = row[5];
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "单位"
                 << "核查类型"
                 << "核查点类型"
                 << "频率"
                 << "频率单位"
                 << "示值1"
                 << "示值2"
                 << "示值3"
                 << "示值4"
                 << "示值5"
                 << "示值6"
                 << "算数平均值"
                 << "标准偏差S(X)"
                 << "相对重复性";
            headerMap.insert("重复性", head);
            headList->insert(row[5], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(VerificationItem); // 检定项目
            Data.append(row[9]); // 核查点
            Data.append(row[6]); // 单位
            Data.append(row[3]); // 核查类型
            Data.append(row[4]); // 核查点类型
            Data.append(row[7]); // 频率
            Data.append(row[10]); // 频率单位
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); // 算数平均值
            Data.append(""); // 标准偏差S(X)
            Data.append(""); // 相对重复性
            ACIList.append(Data);
            resultMap.insert("重复性", ACIList);
            result.insert(row[5], resultMap);
        } else if (!row.isEmpty() && row[5] == "R" && row[3].contains("重复性")) {
            VerificationItem = row[5];
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "单位"
                 << "核查类型"
                 << "核查点类型"
                 << "频率"
                 << "频率单位"
                 << "示值1"
                 << "示值2"
                 << "示值3"
                 << "示值4"
                 << "示值5"
                 << "示值6"
                 << "算数平均值"
                 << "标准偏差S(X)"
                 << "相对重复性";
            headerMap.insert("重复性", head);
            headList->insert(row[5], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(VerificationItem); // 检定项目
            Data.append(row[9]); // 核查点
            Data.append(row[6]); // 单位
            Data.append(row[3]); // 核查类型
            Data.append(row[4]); // 核查点类型
            Data.append(row[7]); //(频率)
            Data.append(row[10]); // 频率单位
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); // 算数平均值
            Data.append(""); // 标准偏差S(X)
            Data.append(""); // 相对重复性
            RList.append(Data);
            resultMap.insert("重复性", RList);
            result.insert(row[5], resultMap);
        }
        if (!row.isEmpty() && row[5] == "DCV" && row[3].contains("稳定性")) {
            VerificationItem = row[5];
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "单位"
                 << "核查类型"
                 << "核查点类型"
                 << "频率"
                 << "频率单位" << DCVDate << nowDate
                 << "算数平均值"
                 << "标准偏差S(X)"
                 << "相对重复性"
                 << "Urel"
                 << "相对重复性是否小于相对扩展不确定度Urel";
            headerMap.insert("稳定性", head);
            headList->insert(row[5], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(VerificationItem); // 检定项目
            Data.append(row[9]); // 核查点
            Data.append(row[6]); // 单位
            Data.append(row[3]); // 核查类型
            Data.append(row[4]); // 核查点类型
            Data.append(row[7]); // 单位(频率)
            Data.append(row[10]); // 频率单位
            Data << DCVvalues;
            Data.append(""); //
            Data.append(""); // 算数平均值
            Data.append(""); // 标准偏差S(X)
            Data.append(""); // 相对重复性
            Data.append(row[12]); // Urel
            Data.append(""); // 相对重复性是否小于相对扩展不确定度Urel
            DCVStableList.append(Data);
            resultMap.insert("稳定性", DCVStableList);
            result.insert(row[5], resultMap);
        } else if (!row.isEmpty() && row[5] == "ACV" && row[3].contains("稳定性")) {
            VerificationItem = row[5];
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "单位"
                 << "核查类型"
                 << "核查点类型"
                 << "单位(频率)"
                 << "频率单位" << ACVDate << nowDate
                 << "算数平均值"
                 << "标准偏差S(X)"
                 << "相对重复性"
                 << "Urel"
                 << "相对重复性是否小于相对扩展不确定度Urel";
            headerMap.insert("稳定性", head);
            headList->insert(row[5], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(VerificationItem); // 检定项目
            Data.append(row[9]); // 核查点
            Data.append(row[6]); // 单位
            Data.append(row[3]); // 核查类型
            Data.append(row[4]); // 核查点类型
            Data.append(row[7]); //(频率)
            Data.append(row[10]); // 频率单位
            Data << ACVvalues;
            Data.append(""); //
            Data.append(""); // 算数平均值
            Data.append(""); // 标准偏差S(X)
            Data.append(""); // 相对重复性
            Data.append(row[12]); // Urel
            Data.append(""); // 相对重复性是否小于相对扩展不确定度Urel
            ACVStableList.append(Data);
            resultMap.insert("稳定性", ACVStableList);
            result.insert(row[5], resultMap);
        } else if (!row.isEmpty() && row[5] == "DCI" && row[3].contains("稳定性")) {
            VerificationItem = row[5];
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "单位"
                 << "核查类型"
                 << "核查点类型"
                 << "频率"
                 << "频率单位" << DCIDate << nowDate
                 << "算数平均值"
                 << "标准偏差S(X)"
                 << "相对重复性"
                 << "Urel"
                 << "相对重复性是否小于相对扩展不确定度Urel";
            headerMap.insert("稳定性", head);
            headList->insert(row[5], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(VerificationItem); // 检定项目
            Data.append(row[9]); // 核查点
            Data.append(row[6]); // 单位
            Data.append(row[3]); // 核查类型
            Data.append(row[4]); // 核查点类型
            Data.append(row[7]); //(频率)
            Data.append(row[10]); // 频率单位
            Data << DCIvalues;
            Data.append(""); //
            Data.append(""); // 算数平均值
            Data.append(""); // 标准偏差S(X)
            Data.append(""); // 相对重复性
            Data.append(row[12]); // Urel
            Data.append(""); // 相对重复性是否小于相对扩展不确定度Urel
            DCIStableList.append(Data);
            resultMap.insert("稳定性", DCIStableList);
            result.insert(row[5], resultMap);
        } else if (!row.isEmpty() && row[5] == "ACI" && row[3].contains("稳定性")) {
            VerificationItem = row[5];
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "单位"
                 << "核查类型"
                 << "核查点类型"
                 << "频率"
                 << "频率单位" << ACIDate << nowDate
                 << "算数平均值"
                 << "标准偏差S(X)"
                 << "相对重复性"
                 << "Urel"
                 << "相对重复性是否小于相对扩展不确定度Urel";
            headerMap.insert("稳定性", head);
            headList->insert(row[5], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(VerificationItem); // 检定项目
            Data.append(row[9]); // 核查点
            Data.append(row[6]); // 单位
            Data.append(row[3]); // 核查类型
            Data.append(row[4]); // 核查点类型
            Data.append(row[7]); //(频率)
            Data.append(row[10]); // 频率单位
            Data << ACIvalues;
            Data.append(""); //
            Data.append(""); // 算数平均值
            Data.append(""); // 标准偏差S(X)
            Data.append(""); // 相对重复性
            Data.append(row[12]); // Urel
            Data.append(""); // 相对重复性是否小于相对扩展不确定度Urel
            ACIStableList.append(Data);
            resultMap.insert("稳定性", ACIStableList);
            result.insert(row[5], resultMap);
        } else if (!row.isEmpty() && row[5] == "R" && row[3].contains("稳定性")) {
            VerificationItem = row[5];
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "单位"
                 << "核查类型"
                 << "核查点类型"
                 << "频率"
                 << "频率单位" << RDate << nowDate
                 << "算数平均值"
                 << "标准偏差S(X)"
                 << "相对重复性"
                 << "Urel"
                 << "相对重复性是否小于相对扩展不确定度Urel";
            headerMap.insert("稳定性", head);
            headList->insert(row[5], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(VerificationItem); // 检定项目
            Data.append(row[9]); // 核查点
            Data.append(row[6]); // 单位
            Data.append(row[3]); // 核查类型
            Data.append(row[4]); // 核查点类型
            Data.append(row[7]); //(频率)
            Data.append(row[10]); // 频率单位
            Data << Rvalues;
            Data.append(""); //
            Data.append(""); // 算数平均值
            Data.append(""); // 标准偏差S(X)
            Data.append(""); // 相对重复性
            Data.append(row[12]); // Urel
            Data.append(""); // 相对重复性是否小于相对扩展不确定度Urel
            RStableList.append(Data);
            resultMap.insert("稳定性", RStableList);
            result.insert(row[5], resultMap);
        }
    }
    return result;
}
// 【保存】核查数据-多功能校准源
bool BaseCommonApi::InsertStandardCheckDataCalibrator(
    StandardCheckDataCalibrator calibrator)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_equipment_standard_check_data_calibrator ( id, data_id, "
        "check_type, test_type, params, unit, "
        " frequency, frequency_unit, check_point, test_value_one_date, "
        "test_value_one, test_value_two_date, test_value_two, "
        " test_value_three_date, test_value_three, test_value_four_date, "
        "test_value_four, test_value_five_date, test_value_five,"
        " test_value_six_date, test_value_six, average_value, "
        "standard_deviation, relative_repeatability, urel, less_than, qualified, "
        " create_time, update_time ) VALUES ( :id, :data_id, :check_type, "
        ":test_type, :params, :unit, :frequency, :frequency_unit, "
        " :check_point, :test_value_one_date, :test_value_one, "
        ":test_value_two_date, :test_value_two, :test_value_three_date, "
        ":test_value_three, "
        " :test_value_four_date, :test_value_four, :test_value_five_date, "
        ":test_value_five, :test_value_six_date, :test_value_six, "
        ":average_value, "
        " :standard_deviation, :relative_repeatability, :urel, :less_than, "
        ":qualified, :create_time, :update_time );");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", calibrator.id);
    query.bindValue(":data_id", calibrator.data_id);
    query.bindValue(":check_type", calibrator.check_type);
    query.bindValue(":test_type", calibrator.test_type);
    query.bindValue(":params", calibrator.params);
    query.bindValue(":unit", calibrator.unit);
    query.bindValue(":frequency", calibrator.frequency);
    query.bindValue(":frequency_unit", calibrator.frequency_unit);
    query.bindValue(":check_point", calibrator.check_point);
    query.bindValue(":test_value_one_date", calibrator.test_value_one_date);
    query.bindValue(":test_value_one", calibrator.test_value_one);
    query.bindValue(":test_value_two_date", calibrator.test_value_two_date);
    query.bindValue(":test_value_two", calibrator.test_value_two);
    query.bindValue(":test_value_three_date", calibrator.test_value_three_date);
    query.bindValue(":test_value_three", calibrator.test_value_three);
    query.bindValue(":test_value_four_date", calibrator.test_value_four_date);
    query.bindValue(":test_value_four", calibrator.test_value_four);
    query.bindValue(":test_value_five_date", calibrator.test_value_five_date);
    query.bindValue(":test_value_five", calibrator.test_value_five);
    query.bindValue(":test_value_six_date", calibrator.test_value_six_date);
    query.bindValue(":test_value_six", calibrator.test_value_six);
    query.bindValue(":average_value", calibrator.average_value);
    query.bindValue(":standard_deviation", calibrator.standard_deviation);
    query.bindValue(":relative_repeatability", calibrator.relative_repeatability);
    query.bindValue(":urel", calibrator.urel);
    query.bindValue(":less_than", calibrator.less_than);
    query.bindValue(":qualified", calibrator.qualified);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << query.lastError().text();
    }
    return isSuccess;
}
// 【保存】核查基本信息
bool BaseCommonApi::InsertStandardCheckInfo(StandardCheckInfo info)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_equipment_standard_check_info ( id, data_no, "
        "check_date, check_address, temperature, humidity, "
        " check_accord, stability_examine, standard_equipment_id, equipment_id, "
        "item_category_id, conclusion, remark, create_user_id, "
        " create_user_name, create_time, update_time ) VALUES ( :id, :data_no, "
        ":check_date, :check_address, :temperature, :humidity, "
        " :check_accord, :stability_examine, :standard_equipment_id, "
        ":equipment_id, :item_category_id, :conclusion, :remark, :create_user_id,"
        " :create_user_name, :create_time, :update_time );");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", info.id);
    query.bindValue(":data_no", info.data_no);
    query.bindValue(":check_date", info.check_date);
    query.bindValue(":check_address", info.check_address);
    query.bindValue(":temperature", info.temperature);
    query.bindValue(":humidity", info.humidity);
    query.bindValue(":check_accord", info.check_accord);
    query.bindValue(":stability_examine", info.stability_examine);
    query.bindValue(":standard_equipment_id", info.standard_equipment_id);
    query.bindValue(":equipment_id", info.equipment_id);
    query.bindValue(":item_category_id", info.item_category_id);
    query.bindValue(":conclusion", info.conclusion);
    query.bindValue(":remark", info.remark);
    query.bindValue(":create_user_id", info.create_user_id);
    query.bindValue(":create_user_name", info.create_user_name);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << query.lastError().text();
    }
    return isSuccess;
}
// 核查程序-标准装置-分类
QList<StandardCheckItemCategory> BaseCommonApi::getStandardCheckItemCategory(
    QString item)
{
    QList<StandardCheckItemCategory> result;
    QSqlQuery query;
    if (item == "")
        query.prepare("SELECT * FROM biz_equipment_standard_check_item_category");
    else {
        query.prepare(
            "SELECT * FROM biz_equipment_standard_check_item_category WHERE "
            "belong_standard_equipment = :item ");
    }
    query.bindValue(":item", item);
    if (query.exec()) {
        while (query.next()) {
            StandardCheckItemCategory category;
            category.id = query.value("id").toString();
            category.category_no = query.value("category_no").toString();
            category.category_name = query.value("category_name").toString();
            category.equipment_type = query.value("equipment_type").toString();
            category.belong_standard_equipment = query.value("belong_standard_equipment").toString();
            category.belong_standard_equipment_name = query.value("belong_standard_equipment_name").toString();
            result.append(category);
        }
    } else {
        qDebug() << query.lastError().text();
    }
    return result;
}
// 字段更新
bool BaseCommonApi::UpdateMeasureItemInfo(QString item_id)
{
    QSqlQuery query;
    query.prepare(
        "UPDATE biz_business_device_measure_item_info "
        " SET data_sync=1,sync_time=:sync_time"
        " WHERE id= :item_id");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":sync_time", currentDateTime);
    query.bindValue(":item_id", item_id);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << query.lastError().text();
    }
    return isSuccess;
}

bool BaseCommonApi::UpdateStandardEquipmentRelation(QString equipment_id,
    QString standard_id)
{
    QSqlQuery query;
    query.prepare(
        "UPDATE biz_equipment_standard_equipment_relation"
        " SET data_sync=1,sync_time=:sync_time"
        " WHERE equipment_id = :equipment_id AND standard_id =:standard_id;");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":sync_time", currentDateTime);
    query.bindValue(":equipment_id", equipment_id);
    query.bindValue(":standard_id", standard_id);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << query.lastError().text();
    }
    return isSuccess;
}
// QString BaseCommonApi::getStandardItemInfo()
//{
//     QSqlQuery query;
//     QString result;
//     query.prepare("SELECT item_id FROM biz_business_device_measure_info ORDER
//     BY create_time desc LIMIT 1"); if(query.exec()){
//         result = query.value("item_id").toString();
//     }
//     return result;
// }
// 【查询】核查项-二等铂电阻温度计
QMap<QString, QMap<QString, QList<QStringList>>>
BaseCommonApi::getStandardCheckItemDataResistanceThermometer(
    QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList)
{
    QMap<QString, QMap<QString, QList<QStringList>>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();
    QSqlQuery query;
    query.prepare(
        "SELECT * FROM "
        "biz_equipment_standard_check_item_data_resistance_thermometer "
        " WHERE equipment_id = :equipment_id ORDER BY create_time ASC");
    query.bindValue(":equipment_id", equipment_id);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_category_id")
                              .toString()); // 核查项分类id(能够确定是哪个标准装置)
            result.append(query.value("equipment_id").toString()); // 配套设备id
            result.append(
                query.value("params").toString()); // 核查项目(直接存字典value)
            result.append(
                query.value("check_point").toString()); // 温度点/水三相点/确认点
            result.append(query.value("unit").toString()); // 单位(直接存字典value)
            result.append(query.value("cycle_number").toString()); // 循环次数
            result.append(query.value("urel").toString()); // Urel
            result.append(
                query.value("check_type").toString()); // 核查类型(直接存字典value)
            result.append(query.value("allow_value").toString()); // 允许值
            result.append(query.value("maximum_error")
                              .toString()); // 最大允许误差/最大允许误差绝对值
            result.append(query.value("remark").toString()); // 核查项备注
            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());
            resultData.append(result);
        }
    } else {
        qDebug() << query.lastError().text();
    }
    DataMapList = GetDataReturnItemListByCheckItemDataResistanceThermometer(
        resultData, DataMapList, headList);
    return DataMapList;
}
QMap<QString, QMap<QString, QList<QStringList>>>
BaseCommonApi::GetDataReturnItemListByCheckItemDataResistanceThermometer(
    QList<QStringList> ResultData,
    QMap<QString, QMap<QString, QList<QStringList>>> DataMapList,
    QMap<QString, QMap<QString, QStringList>>* headList)
{
    QMap<QString, QMap<QString, QList<QStringList>>> result;
    QMap<QString, QList<QStringList>> resultMap;
    QList<QStringList> TemperatureList;
    QList<QStringList> RTPList;
    QList<QStringList> InsulationResistanceList;
    QList<QStringList> ThermometerList;
    QList<QStringList> TemperatureStableList;
    QList<QStringList> RTPStableList;
    QList<QStringList> InsulationResistanceStableList;
    QList<QStringList> ThermometerStableList;
    QMap<QString, QStringList> headerMap;
    CheckProgramManage programInfo = BaseCommonApi::getCheckProgramManageById(loginNowVerifyProgramId);
    QList<DataCalibratorStability> data = BaseCommonApi::GetEquipmentStandardCheckInfoByids(programInfo.standard_id, loginNowVerifyEquimentId, programInfo.equipment_type, 5);
    QStringList historyDate;
    QStringList TemperatureDate;
    QStringList RTPDate;
    QString NowDate = QDateTime::currentDateTime().toString("yyyy-MM-DD HH:mm:ss");
    for (int i = 0; i < data.size(); ++i) {
        if (i < data.size()) {
            if (data[i].params == "温度值") {
                TemperatureDate << data[i].date.toString("yyyy年MM月dd日");
            } else if (data[i].params == "RTP值") {
                RTPDate << data[i].date.toString("yyyy年MM月dd日");
            } else {
                TemperatureDate << "";
                RTPDate << "";
            }
        } else {
            historyDate << "";
        }
    }
    QStringList TemperatureValues;
    QStringList RTPvalues;
    QStringList InsulationResistancevalues;
    QStringList ThermometerIvalues;
    for (int i = 0; i < 5; ++i) {
        if (i < data.size()) {
            if (data[i].params == "温度值") {
                TemperatureValues << data[i].value;
            } else if (data[i].params == "RTP值") {
                RTPvalues << data[i].value;
            } else {
                TemperatureValues << "";
                RTPvalues << "";
            }
        } else {
            TemperatureValues << "";
            RTPvalues << "";
        }
    }
    for (const QStringList& row : ResultData) {
        QString VerificationItem = NULL;
        if (!row.isEmpty() && row[3] == "温度值" && row[8].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "温度点"
                 << "单位"
                 << "核查标准示值1"
                 << "测量标准示值1"
                 << "差值1"
                 << "核查标准示值2"
                 << "测量标准示值2"
                 << "差值2"
                 << "核查标准示值3"
                 << "测量标准示值3"
                 << "差值3"
                 << "核查标准示值4"
                 << "测量标准示值4"
                 << "差值4"
                 << "核查标准示值5"
                 << "测量标准示值5"
                 << "差值5"
                 << "核查标准示值6"
                 << "测量标准示值6"
                 << "差值6"
                 << "平均值(差值)"
                 << "重复性S(x)";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4]); // 温度点
            Data.append(row[5]); // 单位
            Data.append(row[4]); // 核查标准示值1
            Data.append(""); // 测量标准示值1
            Data.append(""); // 差值1
            Data.append(""); // 核查标准示值2
            Data.append(""); // 测量标准示值2
            Data.append(""); // 差值2
            Data.append(""); // 测量标准示值3
            Data.append(""); // 测量标准示值3
            Data.append(""); // 差值3
            Data.append(""); // 核查标准示值4
            Data.append(""); // 测量标准示值4
            Data.append(""); // 差值4
            Data.append(""); // 核查标准示值5
            Data.append(""); // 测量标准示值5
            Data.append(""); // 差值5
            Data.append(""); // 核查标准示值6
            Data.append(""); // 测量标准示值6
            Data.append(""); // 差值6
            Data.append(""); // 平均值(差值)
            Data.append(""); // 重复性S(x)
            TemperatureList.append(Data);
            resultMap.insert("重复性", TemperatureList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "RTP值" && row[8].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "水三相点"
                 << "单位"
                 << "被校示值(Ω)1"
                 << "被校示值(Ω)2"
                 << "被校示值(Ω)3"
                 << "被校示值(Ω)4"
                 << "被校示值(Ω)5"
                 << "被校示值(Ω)6"
                 << "平均值";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4]); // 水三相点
            Data.append(row[5]); // 单位
            Data.append(""); // 被校示值(Ω)1
            Data.append(""); // 被校示值(Ω)2
            Data.append(""); // 被校示值(Ω)3
            Data.append(""); // 被校示值(Ω)4
            Data.append(""); // 被校示值(Ω)5
            Data.append(""); // 被校示值(Ω)6
            Data.append(""); // 平均值
            RTPList.append(Data);
            resultMap.insert("重复性", RTPList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "绝缘电阻表校准") {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "单位"
                 << "确认点"
                 << "测量值"
                 << "示值误差"
                 << "最大允许误差";
            headerMap.insert(row[3], head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[5]); // 单位
            Data.append(row[4]); // 确认点
            Data.append(""); // 测量值
            Data.append(""); // 示值误差
            Data.append(row[10]); // 最大允许误差
            InsulationResistanceList.append(Data);
            resultMap.insert(row[3], InsulationResistanceList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "测温仪电阻核查") {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "单位"
                 << "通道"
                 << "测温仪示值1"
                 << "测温仪示值2"
                 << "测温仪示值3"
                 << "测温仪示值4"
                 << "测温仪示值5"
                 << "测温仪示值6"
                 << "平均值"
                 << "最大偏差"
                 << "最大允许误差绝对值|MPE|";
            headerMap.insert(row[3], head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4]); // 核查点
            Data.append(row[5]); // 单位
            Data.append(""); // 通道
            Data.append(""); // 测温仪示值1
            Data.append(""); // 测温仪示值2
            Data.append(""); // 测温仪示值3
            Data.append(""); // 测温仪示值4
            Data.append(""); // 测温仪示值5
            Data.append(""); // 测温仪示值6
            Data.append(""); // 平均值
            Data.append(""); // 最大偏差
            Data.append(row[10]); // 最大允许误差绝对值|MPE|
            ThermometerList.append(Data);
            resultMap.insert(row[3], ThermometerList);
            result.insert(row[3], resultMap);
        }
        if (!row.isEmpty() && row[3] == "温度值" && row[8].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "温度点"
                 << "单位"
                 << TemperatureDate
                 << NowDate
                 << "平均值"
                 << "Sm"
                 << "扩展不确定度U"
                 << "是否Sm≤U";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4]); // 温度点
            Data.append(row[5]); // 单位
            Data << TemperatureValues;
            Data.append(""); // 测量差值6
            Data.append(""); // 平均值
            Data.append(""); // Sm
            Data.append(row[7]); // 扩展不确定度U
            Data.append(""); // 是否Sm≤U
            TemperatureStableList.append(Data);
            resultMap.insert("稳定性", TemperatureStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "RTP值" && row[8].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "水三相点"
                 << "单位"
                 << RTPDate
                 << NowDate
                 << "测量极差值"
                 << "允许值"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4]); // 水三相点
            Data.append(row[5]); // 单位
            Data << RTPvalues;
            Data.append(""); // 被校示值(Ω)6
            Data.append(""); // 测量极差值
            Data.append(""); // 允许值
            Data.append(""); // 结论
            RTPStableList.append(Data);
            resultMap.insert("稳定性", RTPStableList);
            result.insert(row[3], resultMap);
        } /*else if (!row.isEmpty() && row[3] == "绝缘电阻表校准") {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "单位"
                 << "确认点"
                 << "测量值"
                 << "示值误差"
                 << "最大允许误差";
            headerMap.insert(row[3], head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[5]); // 单位
            Data.append(row[4]); // 确认点
            Data.append(""); // 测量值
            Data.append(""); // 示值误差
            Data.append(row[10]); // 最大允许误差
            InsulationResistanceList.append(Data);
            resultMap.insert(row[3], InsulationResistanceList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "测温仪电阻核查") {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "单位"
                 << "通道"
                 << "测温仪示值1"
                 << "测温仪示值2"
                 << "测温仪示值3"
                 << "测温仪示值4"
                 << "测温仪示值5"
                 << "测温仪示值6"
                 << "平均值"
                 << "最大偏差"
                 << "最大允许误差绝对值|MPE|";
            headerMap.insert(row[3], head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4]); // 核查点
            Data.append(row[5]); // 单位
            Data.append(""); // 通道
            Data.append(""); // 测温仪示值1
            Data.append(""); // 测温仪示值2
            Data.append(""); // 测温仪示值3
            Data.append(""); // 测温仪示值4
            Data.append(""); // 测温仪示值5
            Data.append(""); // 测温仪示值6
            Data.append(""); // 平均值
            Data.append(""); // 最大偏差
            Data.append(row[10]); // 最大允许误差绝对值|MPE|
            ThermometerList.append(Data);
            resultMap.insert(row[3], ThermometerList);
            result.insert(row[3], resultMap);
        }*/
    }
    return result;
}
// 【保存】核查项-二等铂电阻温度计
bool BaseCommonApi::InsertStandardCheckItemDataResistanceThermometer(
    StandardCheckDataResistanceThermometer resistanceThermometer)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_equipment_standard_check_data_resistance_thermometer ("
        " id,  data_id, check_type, this_item_check,  params,  check_point,  "
        "unit,  thoroughfare, test_value_one_date, "
        " test_value_one,  measure_value_one,  different_value_one,  "
        "test_value_two_date,  test_value_two,  measure_value_two, "
        " different_value_two,  test_value_three_date, test_value_three,  "
        "measure_value_three,  different_value_three, "
        " test_value_four_date,  test_value_four, measure_value_four,  "
        "different_value_four,  test_value_five_date, "
        " test_value_five,  measure_value_five,  different_value_five,  "
        "test_value_six_date,  test_value_six,  measure_value_six, "
        " different_value_six,  average_value,  standard_deviation, "
        "relative_repeatability,  urel,  less_than,  measure_value, "
        " allow_value,  illustration,  indicating_error,  maximum_error, "
        "maximum_deviation,  conclusion,   create_time,  update_time"
        " )  VALUES ( :id,  :data_id,  :check_type,  :this_item_check,  :params, "
        " :check_point,  :unit,  :thoroughfare,  :test_value_one_date, "
        " :test_value_one, :measure_value_one,  :different_value_one,   "
        ":test_value_two_date, :test_value_two,  :measure_value_two, "
        " :different_value_two,  :test_value_three_date,  :test_value_three,  "
        ":measure_value_three,  :different_value_three, "
        " :test_value_four_date,  :test_value_four,  :measure_value_four,  "
        ":different_value_four,  :test_value_five_date, "
        " :test_value_five,  :measure_value_five,  :different_value_five,  "
        ":test_value_six_date,  :test_value_six,  :measure_value_six, "
        " :different_value_six,  :average_value,  :standard_deviation,  "
        ":relative_repeatability, :urel,  :less_than,  :measure_value, "
        " :allow_value,  :illustration,  :indicating_error,  :maximum_error,  "
        ":maximum_deviation,  :conclusion,  :create_time,  :update_time"
        " );");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", resistanceThermometer.id);
    query.bindValue(":data_id", resistanceThermometer.data_id);
    query.bindValue(":check_type", resistanceThermometer.check_type);
    query.bindValue(":this_item_check", resistanceThermometer.this_item_check);
    query.bindValue(":params", resistanceThermometer.params);
    query.bindValue(":check_point", resistanceThermometer.check_point);
    query.bindValue(":unit", resistanceThermometer.unit);
    query.bindValue(":thoroughfare", resistanceThermometer.thoroughfare);
    query.bindValue(":test_value_one_date",
        resistanceThermometer.test_value_one_date);
    query.bindValue(":test_value_one", resistanceThermometer.test_value_one);
    query.bindValue(":measure_value_one",
        resistanceThermometer.measure_value_one);
    query.bindValue(":different_value_one",
        resistanceThermometer.different_value_one);
    query.bindValue(":test_value_two_date",
        resistanceThermometer.test_value_two_date);
    query.bindValue(":test_value_two", resistanceThermometer.test_value_two);
    query.bindValue(":measure_value_two",
        resistanceThermometer.measure_value_two);
    query.bindValue(":different_value_two",
        resistanceThermometer.different_value_two);
    query.bindValue(":test_value_three_date",
        resistanceThermometer.test_value_three_date);
    query.bindValue(":test_value_three", resistanceThermometer.test_value_three);
    query.bindValue(":measure_value_three",
        resistanceThermometer.measure_value_three);
    query.bindValue(":different_value_three",
        resistanceThermometer.different_value_three);
    query.bindValue(":test_value_four_date",
        resistanceThermometer.test_value_four_date);
    query.bindValue(":test_value_four", resistanceThermometer.test_value_four);
    query.bindValue(":measure_value_four",
        resistanceThermometer.measure_value_four);
    query.bindValue(":different_value_four",
        resistanceThermometer.different_value_four);
    query.bindValue(":test_value_five_date",
        resistanceThermometer.test_value_five_date);
    query.bindValue(":test_value_five", resistanceThermometer.test_value_five);
    query.bindValue(":measure_value_five",
        resistanceThermometer.measure_value_five);
    query.bindValue(":different_value_five",
        resistanceThermometer.different_value_five);
    query.bindValue(":test_value_six_date",
        resistanceThermometer.test_value_six_date);
    query.bindValue(":test_value_six", resistanceThermometer.test_value_six);
    query.bindValue(":measure_value_six",
        resistanceThermometer.measure_value_six);
    query.bindValue(":different_value_six",
        resistanceThermometer.different_value_six);
    query.bindValue(":average_value", resistanceThermometer.average_value);
    query.bindValue(":standard_deviation",
        resistanceThermometer.standard_deviation);
    query.bindValue(":relative_repeatability",
        resistanceThermometer.relative_repeatability);
    query.bindValue(":urel", resistanceThermometer.urel);
    query.bindValue(":less_than", resistanceThermometer.less_than);
    query.bindValue(":measure_value", resistanceThermometer.measure_value);
    query.bindValue(":allow_value", resistanceThermometer.allow_value);
    query.bindValue(":illustration", resistanceThermometer.illustration);
    query.bindValue(":indicating_error", resistanceThermometer.indicating_error);
    query.bindValue(":maximum_error", resistanceThermometer.maximum_error);
    query.bindValue(":maximum_deviation",
        resistanceThermometer.maximum_deviation);
    query.bindValue(":conclusion", resistanceThermometer.conclusion);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << query.lastError().text();
    }
    return isSuccess;
}

// 【查询】核查项-直流稳压电源
QMap<QString, QMap<QString, QList<QStringList>>>
BaseCommonApi::getStandardCheckItemDataDcPower(
    QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList)
{
    QMap<QString, QMap<QString, QList<QStringList>>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();
    QSqlQuery query;
    query.prepare(
        "SELECT *	FROM biz_equipment_standard_check_item_data_dc_power  "
        " WHERE equipment_id = :equipment_id ORDER BY create_time ASC");
    query.bindValue(":equipment_id", equipment_id);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_category_id")
                              .toString()); // 核查项分类id(能够确定是哪个标准装置)
            result.append(query.value("equipment_id").toString()); // 配套设备id
            result.append(
                query.value("params").toString()); // 核查项目(直接存字典value)
            result.append(query.value("check_point").toString()); // 核查点
            result.append(query.value("unit").toString()); // 单位(直接存字典value) 5
            result.append(query.value("resolution").toString()); // 分辨力
            result.append(query.value("cycle_number").toString()); // 循环次数
            result.append(query.value("urel").toString()); // Urel
            result.append(
                query.value("check_type").toString()); // 核查类型(直接存字典value)
            result.append(
                query.value("datum_point").toString()); // 是否为基准点(1/0)
            result.append(query.value("resistance_value").toString()); // 电阻值/频率
            result.append(query.value("resistance_value_unit")
                              .toString()); // 电阻值单位/频率单位(直接存字典value)
            result.append(query.value("remark").toString()); // 核查项备注
            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());
            resultData.append(result);
        }
    } else {
        qDebug() << query.lastError().text();
    }
    DataMapList = GetDataReturnItemListByCheckItemDataDcPower(
        resultData, DataMapList, headList);
    return DataMapList;
}
QMap<QString, QMap<QString, QList<QStringList>>>
BaseCommonApi::GetDataReturnItemListByCheckItemDataDcPower(
    QList<QStringList> ResultData,
    QMap<QString, QMap<QString, QList<QStringList>>> DataMapList,
    QMap<QString, QMap<QString, QStringList>>* headList)
{

    QString NowDate = QDate::currentDate().toString("yyyy年MM月dd日");
    QMap<QString, QMap<QString, QList<QStringList>>> result;
    QMap<QString, QList<QStringList>> resultMap;
    QList<QStringList> DCVoltageList;
    QList<QStringList> PowerAndLoadEffectsList;
    QList<QStringList> DirectMeasureList;
    QList<QStringList> IndirectMeasureList;
    QList<QStringList> RippleVoltageList;
    QList<QStringList> DCVoltageStableList;
    QList<QStringList> PowerAndLoadEffectsStableList;
    QList<QStringList> DirectMeasureStableList;
    QList<QStringList> IndirectMeasureStableList;
    QList<QStringList> RippleVoltageStableList;
    QMap<QString, QStringList> headerMap;
    CheckProgramManage programInfo = BaseCommonApi::getCheckProgramManageById(loginNowVerifyProgramId);
    QList<DataCalibratorStability> data = BaseCommonApi::GetEquipmentStandardCheckInfoByids(programInfo.standard_id, loginNowVerifyEquimentId, programInfo.equipment_type, 2);
    QStringList historyDate;
    QStringList DCVoltageDate;
    QStringList PowerAndLoadEffectsDate;
    QStringList DirectMeasureDate;
    QStringList IndirectMeasureDate;
    QStringList RippleVoltageDate;
    for (int i = 0; i < data.size(); ++i) {
        //        if (i < data.size()) {
        //            historyDate << data[i].date.toString("yyyy年MM月dd日");
        //        } else {
        //            historyDate << "";
        //        }
        if (i < data.size()) {
            if (data[i].params == "直流电压") {
                DCVoltageDate << data[i].date.toString("yyyy年MM月dd日");
            } else if (data[i].params == "电源效应和负载效应") {
                PowerAndLoadEffectsDate << data[i].date.toString("yyyy年MM月dd日");
            } else if (data[i].params == "直流电流(直接测量)") {
                DirectMeasureDate << data[i].date.toString("yyyy年MM月dd日");
            } else if (data[i].params == "直流电流(间接测量)") {
                IndirectMeasureDate << data[i].date.toString("yyyy年MM月dd日");
            } else if (data[i].params == "纹波电压") {
                RippleVoltageDate << data[i].date.toString("yyyy年MM月dd日");
            } else {
                DCVoltageDate << "";
                PowerAndLoadEffectsDate << "";
                DirectMeasureDate << "";
                IndirectMeasureDate << "";
                RippleVoltageDate << "";
            }
        } else {
            DCVoltageDate << "";
            PowerAndLoadEffectsDate << "";
            DirectMeasureDate << "";
            IndirectMeasureDate << "";
            RippleVoltageDate << "";
        }
    }
    QStringList DCVoltageValues;
    QStringList PowerAndLoadEffectsValues;
    QStringList DirectMeasureValues;
    QStringList IndirectMeasureValues;
    QStringList RippleVoltageValues;
    for (int i = 0; i < data.size(); ++i) {
        if (i < data.size()) {
            if (data[i].params == "直流电压") {
                DCVoltageValues << data[i].value;
            } else if (data[i].params == "电源效应和负载效应") {
                PowerAndLoadEffectsValues << data[i].value;
            } else if (data[i].params == "直流电流(直接测量)") {
                DirectMeasureValues << data[i].value;
            } else if (data[i].params == "直流电流(间接测量)") {
                IndirectMeasureValues << data[i].value;
            } else if (data[i].params == "纹波电压") {
                RippleVoltageValues << data[i].value;
            } else {
                DCVoltageValues << "";
                PowerAndLoadEffectsValues << "";
                DirectMeasureValues << "";
                IndirectMeasureValues << "";
                RippleVoltageValues << "";
            }
        } else {
            DCVoltageValues << "";
            PowerAndLoadEffectsValues << "";
            DirectMeasureValues << "";
            IndirectMeasureValues << "";
            RippleVoltageValues << "";
        }
    }
    for (const QStringList& row : ResultData) {
        if (!row.isEmpty() && row[3] == "直流电压" && row[9].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "核查类型"
                 << "单位"
                 << "分辨力"
                 << "示值1"
                 << "示值2"
                 << "示值3"
                 << "示值4"
                 << "示值5"
                 << "示值6"
                 << "平均值"
                 << "标准偏差"
                 << "相对标准偏差";
            headerMap.insert("重复性", head);
            headList->insert("直流电压", headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append("直流电压"); // 核查项目
            Data.append(row[4]); // 核查点
            Data.append(row[9]); // 核查类型
            Data.append(row[5]); // 单位
            Data.append(row[6]); // 分辨力
            Data.append(""); // 示值1
            Data.append(""); // 示值2
            Data.append(""); // 示值3
            Data.append(""); // 示值4
            Data.append(""); // 示值5
            Data.append(""); // 示值6
            Data.append(""); // 平均值
            Data.append(""); // 标准偏差
            Data.append(""); // 相对标准偏差
            DCVoltageList.append(Data);
            resultMap.insert("重复性", DCVoltageList);
            result.insert("直流电压", resultMap);
        } else if (!row.isEmpty() && row[3] == "电源效应和负载效应" && row[9].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "核查类型"
                 << "单位"
                 << "分辨力"
                 << "示值1"
                 << "示值2"
                 << "示值3"
                 << "示值4"
                 << "示值5"
                 << "示值6"
                 << "平均值"
                 << "标准偏差"
                 << "相对标准偏差";
            headerMap.insert("重复性", head);
            headList->insert("电源效应和负载效应", headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append("电源效应和负载效应"); // 核查项目
            Data.append(row[4]); // 核查点
            Data.append(row[9]); // 核查类型
            Data.append(row[5]); // 单位
            Data.append(row[6]); // 分辨力
            Data.append(""); // 示值1
            Data.append(""); // 示值2
            Data.append(""); // 示值3
            Data.append(""); // 示值4
            Data.append(""); // 示值5
            Data.append(""); // 示值6
            Data.append(""); // 平均值
            Data.append(""); // 标准偏差
            Data.append(""); // 相对标准偏差
            PowerAndLoadEffectsList.append(Data);
            resultMap.insert("重复性", PowerAndLoadEffectsList);
            result.insert("电源效应和负载效应", resultMap);
        } else if (!row.isEmpty() && row[3] == "直流电流(直接测量)" && row[9].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "核查类型"
                 << "单位"
                 << "分辨力"
                 << "示值1"
                 << "示值2"
                 << "示值3"
                 << "示值4"
                 << "示值5"
                 << "示值6"
                 << "平均值"
                 << "标准偏差"
                 << "相对标准偏差";
            headerMap.insert("重复性", head);
            headList->insert("直流电流(直接测量)", headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append("直流电流(直接测量)"); // 核查项目
            Data.append(row[4]); // 核查点
            Data.append(row[9]); // 核查类型
            Data.append(row[5]); // 单位
            Data.append(row[6]); // 分辨力
            Data.append(""); // 示值1
            Data.append(""); // 示值2
            Data.append(""); // 示值3
            Data.append(""); // 示值4
            Data.append(""); // 示值5
            Data.append(""); // 示值6
            Data.append(""); // 平均值
            Data.append(""); // 标准偏差
            Data.append(""); // 相对标准偏差
            DirectMeasureList.append(Data);
            resultMap.insert("重复性", DirectMeasureList);
            result.insert("直流电流(直接测量)", resultMap);
        } else if (!row.isEmpty() && row[3] == "直流电流(间接测量)" && row[9].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "核查类型"
                 << "电阻值"
                 << "核查参数"
                 << "单位"
                 << "分辨力"
                 << "示值1"
                 << "示值2"
                 << "示值3"
                 << "示值4"
                 << "示值5"
                 << "示值6"
                 << "平均值"
                 << "标准偏差"
                 << "相对标准偏差";
            headerMap.insert("重复性", head);
            headList->insert("直流电流(间接测量)", headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append("直流电流(间接测量)"); // 核查项目
            Data.append(row[4]); // 核查点
            Data.append(row[9]); // 核查类型
            Data.append(row[11] + row[12]); // 电阻值
            Data.append(""); // 核查参数
            Data.append(row[5]); // 单位
            Data.append(row[6]); // 分辨力
            Data.append(""); // 示值1
            Data.append(""); // 示值2
            Data.append(""); // 示值3
            Data.append(""); // 示值4
            Data.append(""); // 示值5
            Data.append(""); // 示值6
            Data.append(""); // 平均值
            Data.append(""); // 标准偏差
            Data.append(""); // 相对标准偏差
            IndirectMeasureList.append(Data);
            resultMap.insert("重复性", IndirectMeasureList);
            result.insert("直流电流(间接测量)", resultMap);
        } else if (!row.isEmpty() && row[3] == "纹波电压" && row[9].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "核查类型"
                 << "单位"
                 << "分辨力"
                 << "示值1"
                 << "示值2"
                 << "示值3"
                 << "示值4"
                 << "示值5"
                 << "示值6"
                 << "平均值"
                 << "标准偏差"
                 << "相对标准偏差";
            headerMap.insert("重复性", head);
            headList->insert("纹波电压", headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append("纹波电压"); // 核查项目
            Data.append(row[4]); // 核查点
            Data.append(row[9]); // 核查类型
            Data.append(row[5]); // 单位
            Data.append(row[6]); // 分辨力
            Data.append(""); // 示值1
            Data.append(""); // 示值2
            Data.append(""); // 示值3
            Data.append(""); // 示值4
            Data.append(""); // 示值5
            Data.append(""); // 示值6
            Data.append(""); // 平均值
            Data.append(""); // 标准偏差
            Data.append(""); // 相对标准偏差
            RippleVoltageList.append(Data);
            resultMap.insert("重复性", RippleVoltageList);
            result.insert("纹波电压", resultMap);
        }
        if (!row.isEmpty() && row[3] == "直流电压" && row[9].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "核查类型"
                 << "单位" << DCVoltageDate << NowDate
                 << "平均值"
                 << "标准偏差"
                 << "相对标准偏差"
                 << "相对扩展不确定度Urel"
                 << "相对标准偏差是否小于相对扩展不确定度Urel";
            headerMap.insert("稳定性", head);
            headList->insert("直流电压", headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append("直流电压"); // 核查项目
            Data.append(row[4]); // 核查点
            Data.append(row[9]); // 核查类型
            Data.append(row[5]); // 单位
            Data << DCVoltageValues;
            Data.append(""); // 示值6
            Data.append(""); // 平均值
            Data.append(""); // 标准偏差
            Data.append(""); // 相对标准偏差
            Data.append(""); // 相对扩展不确定度Urel
            Data.append(""); // 相对标准偏差是否小于相对扩展不确定度Urel
            DCVoltageStableList.append(Data);
            resultMap.insert("稳定性", DCVoltageStableList);
            result.insert("直流电压", resultMap);
        } else if (!row.isEmpty() && row[3] == "电源效应和负载效应" && row[9].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "核查类型"
                 << "单位" << PowerAndLoadEffectsDate << NowDate
                 << "平均值"
                 << "标准偏差"
                 << "相对标准偏差"
                 << "相对扩展不确定度Urel"
                 << "相对标准偏差是否小于相对扩展不确定度Urel";
            headerMap.insert("稳定性", head);
            headList->insert("电源效应和负载效应", headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append("电源效应和负载效应"); // 核查项目
            Data.append(row[4]); // 核查点
            Data.append(row[9]); // 核查类型
            Data.append(row[5]); // 单位
            Data << PowerAndLoadEffectsValues;
            Data.append(""); // 示值6
            Data.append(""); // 平均值
            Data.append(""); // 标准偏差
            Data.append(""); // 相对标准偏差
            Data.append(""); // 相对扩展不确定度Urel
            Data.append(""); // 相对标准偏差是否小于相对扩展不确定度Urel
            PowerAndLoadEffectsStableList.append(Data);
            resultMap.insert("稳定性", PowerAndLoadEffectsStableList);
            result.insert("电源效应和负载效应", resultMap);
        } else if (!row.isEmpty() && row[3] == "直流电流(直接测量)" && row[9].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "核查类型"
                 << "单位" << DirectMeasureDate << NowDate
                 << "平均值"
                 << "标准偏差"
                 << "相对标准偏差"
                 << "相对扩展不确定度Urel"
                 << "相对标准偏差是否小于相对扩展不确定度Urel";
            headerMap.insert("稳定性", head);
            headList->insert("直流电流(直接测量)", headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append("直流电流(直接测量)"); // 核查项目
            Data.append(row[4]); // 核查点
            Data.append(row[9]); // 核查类型
            Data.append(row[5]); // 单位
            Data << DirectMeasureValues;
            Data.append(""); // 示值6
            Data.append(""); // 平均值
            Data.append(""); // 标准偏差
            Data.append(""); // 相对标准偏差
            Data.append(""); // 相对扩展不确定度Urel
            Data.append(""); // 相对标准偏差是否小于相对扩展不确定度Urel
            DirectMeasureStableList.append(Data);
            resultMap.insert("稳定性", DirectMeasureStableList);
            result.insert("直流电流(直接测量)", resultMap);
        } else if (!row.isEmpty() && row[3] == "直流电流(间接测量)" && row[9].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "核查类型"
                 << "电阻值"
                 << "核查参数"
                 << "单位" << IndirectMeasureDate << NowDate
                 << "平均值"
                 << "标准偏差"
                 << "相对标准偏差"
                 << "相对扩展不确定度Urel"
                 << "相对标准偏差是否小于相对扩展不确定度Urel";
            headerMap.insert("稳定性", head);
            headList->insert("直流电流(间接测量)", headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append("直流电流(间接测量))"); // 核查项目
            Data.append(row[4]); // 核查点
            Data.append(row[9]); // 核查类型
            Data.append(row[11] + row[12]); // 电阻值
            Data.append(""); // 核查参数
            Data.append(row[5]); // 单位
            Data << IndirectMeasureValues;
            Data.append(""); // 示值6
            Data.append(""); // 平均值
            Data.append(""); // 标准偏差
            Data.append(""); // 相对标准偏差
            Data.append(""); // 相对扩展不确定度Urel
            Data.append(""); // 相对标准偏差是否小于相对扩展不确定度Urel
            IndirectMeasureStableList.append(Data);
            resultMap.insert("稳定性", IndirectMeasureStableList);
            result.insert("直流电流(间接测量)", resultMap);
        } else if (!row.isEmpty() && row[3] == "纹波电压" && row[9].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "核查类型"
                 << "单位"
                 << "频率" << RippleVoltageDate << NowDate
                 << "平均值"
                 << "标准偏差"
                 << "相对标准偏差"
                 << "相对扩展不确定度Urel"
                 << "相对标准偏差是否小于相对扩展不确定度Urel";
            headerMap.insert("稳定性", head);
            headList->insert("纹波电压", headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append("纹波电压"); // 核查项目
            Data.append(row[4]); // 核查点
            Data.append(row[9]); // 核查类型
            Data.append(row[5]); // 单位
            Data.append(row[11]); // 频率
            Data << RippleVoltageValues;
            Data.append(""); // 示值6
            Data.append(""); // 平均值
            Data.append(""); // 标准偏差
            Data.append(""); // 相对标准偏差
            Data.append(""); // 相对扩展不确定度Urel
            Data.append(""); // 相对标准偏差是否小于相对扩展不确定度Urel
            RippleVoltageStableList.append(Data);
            resultMap.insert("稳定性", RippleVoltageStableList);
            result.insert("纹波电压", resultMap);
        }
    }
    return result;
}
// 【保存】核查数据-直流稳压电源
bool BaseCommonApi::InsertStandardCheckItemDataDcPower(
    StandardCheckDataDcPower power)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_equipment_standard_check_data_dc_power ("
        " id, data_id, check_type, params, check_param, check_point, unit,"
        " average_value, test_value_one_date, test_value_one, "
        "test_value_two_date,"
        " test_value_two, test_value_three_date, test_value_three, "
        "test_value_four_date,"
        " test_value_four, test_value_five_date, test_value_five, "
        "test_value_six_date,"
        " test_value_six, test_value_seven, test_value_eight, test_value_nine,"
        " test_value_ten, standard_deviation, relative_repeatability, urel, "
        "less_than,"
        " qualified, create_time, update_time"
        " )"
        " VALUES ("
        " :id, :data_id, :check_type, :params, :check_param, :check_point, :unit,"
        " :average_value, :test_value_one_date, :test_value_one, "
        ":test_value_two_date,"
        " :test_value_two, :test_value_three_date, :test_value_three, "
        ":test_value_four_date,"
        " :test_value_four, :test_value_five_date, :test_value_five, "
        ":test_value_six_date,"
        " :test_value_six, :test_value_seven, :test_value_eight, "
        ":test_value_nine,"
        " :test_value_ten, :standard_deviation, :relative_repeatability, :urel, "
        ":less_than,"
        " :qualified, :create_time, :update_time"
        " );");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", power.id);
    query.bindValue(":data_id", power.data_id);
    query.bindValue(":check_type", power.check_type);
    query.bindValue(":params", power.params);
    query.bindValue(":check_param", power.check_param);
    query.bindValue(":check_point", power.check_point);
    query.bindValue(":unit", power.unit);
    query.bindValue(":average_value", power.average_value);
    query.bindValue(":test_value_one_date", power.test_value_one_date);
    query.bindValue(":test_value_one", power.test_value_one);
    query.bindValue(":test_value_two_date", power.test_value_two_date);
    query.bindValue(":test_value_two", power.test_value_two);
    query.bindValue(":test_value_three_date", power.test_value_three_date);
    query.bindValue(":test_value_three", power.test_value_three);
    query.bindValue(":test_value_four_date", power.test_value_four_date);
    query.bindValue(":test_value_four", power.test_value_four);
    query.bindValue(":test_value_five_date", power.test_value_five_date);
    query.bindValue(":test_value_five", power.test_value_five);
    query.bindValue(":test_value_six_date", power.test_value_six_date);
    query.bindValue(":test_value_six", power.test_value_six);
    query.bindValue(":test_value_seven", power.test_value_seven);
    query.bindValue(":test_value_eight", power.test_value_eight);
    query.bindValue(":test_value_nine", power.test_value_nine);
    query.bindValue(":test_value_ten", power.test_value_ten);
    query.bindValue(":standard_deviation", power.standard_deviation);
    query.bindValue(":relative_repeatability", power.relative_repeatability);
    query.bindValue(":urel", power.urel);
    query.bindValue(":less_than", power.less_than);
    query.bindValue(":qualified", power.qualified);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << query.lastError().text();
    }
    return isSuccess;
}

QMap<QString, QMap<QString, QList<QStringList>>>
BaseCommonApi::getStandardCheckItemDataSignalGenerator(
    QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList)
{
    QMap<QString, QMap<QString, QList<QStringList>>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();
    QSqlQuery query;
    query.prepare(
        "SELECT *	FROM biz_equipment_standard_check_item_data_signal_generator  "
        " WHERE equipment_id = :equipment_id ORDER BY create_time ASC");
    query.bindValue(":equipment_id", equipment_id);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_category_id")
                              .toString()); // 核查项分类id(能够确定是哪个标准装置)
            result.append(query.value("equipment_id").toString()); // 配套设备id
            result.append(
                query.value("params").toString()); // 核查项目(直接存字典value)
            result.append(
                query.value("check_point").toString()); // 频率点(字典value)
            result.append(query.value("unit").toString()); // 频率点单位(字典value)
            result.append(query.value("urel").toString()); // U(k=2)
            result.append(query.value("amplitude").toString()); // 幅度
            result.append(
                query.value("amplitude_unit").toString()); // 幅度单位(字典value)
            result.append(query.value("power").toString()); // 功率(字典value)
            result.append(
                query.value("power_unit").toString()); // 功率单位(字典value)   10
            result.append(
                query.value("relative_level").toString()); // 相对电平(字典value)
            result.append(query.value("relative_level_unit")
                              .toString()); // 相对电平单位(字典value)
            result.append(query.value("cycle_number").toString()); // 循环次数
            result.append(
                query.value("check_type").toString()); // 核查类型(字典value)
            result.append(query.value("modulation").toString()); // 调幅度    15
            result.append(query.value("high_pass_filter").toString()); // 高通滤波
            result.append(
                query.value("high_pass_filter_unit").toString()); // 高通滤波单位
            result.append(query.value("lower_pass_filter").toString()); // 低通滤波
            result.append(
                query.value("lower_pass_filter_unit").toString()); // 低通滤波单位
            result.append(query.value("modulation_rate").toString()); // 调制速率
            result.append(
                query.value("modulation_rate_unit").toString()); // 调制速率单位
            result.append(query.value("frequency_deviation").toString()); // 调频频偏
            result.append(
                query.value("frequency_deviation_unit").toString()); // 调频频偏单位
            result.append(query.value("phase_deviation").toString()); // 调相相偏
            result.append(
                query.value("phase_deviation_unit").toString()); // 调相相偏单位
            result.append(query.value("rbw_value").toString()); // RBW   26
            result.append(query.value("rbw_value_unit").toString()); // RBW单位
            result.append(query.value("vbw_value").toString()); // VBW
            result.append(query.value("vbw_value_unit").toString()); // VBW单位
            result.append(query.value("remark").toString()); // 核查项备注
            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());
            resultData.append(result);
        }
    } else {
        qDebug() << query.lastError().text();
    }
    DataMapList = GetDataReturnItemListByCheckItemDataSignalGenerator(
        resultData, DataMapList, headList);
    return DataMapList;
}
QMap<QString, QMap<QString, QList<QStringList>>>
BaseCommonApi::GetDataReturnItemListByCheckItemDataSignalGenerator(
    QList<QStringList> ResultData,
    QMap<QString, QMap<QString, QList<QStringList>>> DataMapList,
    QMap<QString, QMap<QString, QStringList>>* headList)
{
    QString NowDate = QDate::currentDate().toString("yyyy年MM月dd日");
    QMap<QString, QMap<QString, QList<QStringList>>> result;
    QMap<QString, QList<QStringList>> resultMap;
    QList<QStringList> FrequencyList;
    QList<QStringList> PowerList;
    QList<QStringList> RelativeLevelList;
    QList<QStringList> RangeList;
    QList<QStringList> FrequencyOffsetList;
    QList<QStringList> HarmonicList;
    QList<QStringList> PhaseDeviationList;
    QList<QStringList> FrequencyStableList;
    QList<QStringList> PowerStableList;
    QList<QStringList> RelativeLevelStableList;
    QList<QStringList> RangeStableList;
    QList<QStringList> FrequencyOffsetStableList;
    QList<QStringList> HarmonicStableList;
    QList<QStringList> PhaseDeviationStableList;
    QMap<QString, QStringList> headerMap;

    CheckProgramManage programInfo = BaseCommonApi::getCheckProgramManageById(loginNowVerifyProgramId);
    QList<DataCalibratorStability> data = BaseCommonApi::GetEquipmentStandardCheckInfoByids(programInfo.standard_id, loginNowVerifyEquimentId, programInfo.equipment_type, 10);
    //    QStringList historyDate;
    QStringList FrequencyDate;
    QStringList PowerDate;
    QStringList RelativeLevelDate;
    QStringList RangeDate;
    QStringList FrequencyOffsetDate;
    QStringList HarmonicDate;
    QStringList PhaseDeviationDate;
    for (int i = 0; i < data.size(); ++i) {
        //        if (i < data.size()) {
        //            historyDate << data[i].date.toString("yyyy年MM月dd日");
        //        } else {
        //            historyDate << "";
        //        }
        if (i < data.size()) {
            if (data[i].params == "频率") {
                FrequencyDate << data[i].date.toString("yyyy年MM月dd日");
            } else if (data[i].params == "功率") {
                PowerDate << data[i].date.toString("yyyy年MM月dd日");
            } else if (data[i].params == "相对电平") {
                RelativeLevelDate << data[i].date.toString("yyyy年MM月dd日");
            } else if (data[i].params == "调幅度") {
                RangeDate << data[i].date.toString("yyyy年MM月dd日");
            } else if (data[i].params == "调频频偏") {
                FrequencyOffsetDate << data[i].date.toString("yyyy年MM月dd日");
            } else if (data[i].params == "谐波") {
                HarmonicDate << data[i].date.toString("yyyy年MM月dd日");
            } else if (data[i].params == "调相相偏") {
                PhaseDeviationDate << data[i].date.toString("yyyy年MM月dd日");
            } else {
                FrequencyDate << "";
                PowerDate << "";
                RelativeLevelDate << "";
                RangeDate << "";
                FrequencyOffsetDate << "";
                HarmonicDate << "";
                PhaseDeviationDate << "";
            }
        } else {
            FrequencyDate << "";
            PowerDate << "";
            RelativeLevelDate << "";
            RangeDate << "";
            FrequencyOffsetDate << "";
            HarmonicDate << "";
            PhaseDeviationDate << "";
        }
    }
    QStringList FrequencyValues;
    QStringList PowerValues;
    QStringList RelativeLevelValues;
    QStringList RangeValues;
    QStringList FrequencyOffsetValues;
    QStringList HarmonicValues;
    QStringList PhaseDeviationValues;
    for (int i = 0; i < data.size(); ++i) {
        if (i < data.size()) {
            if (data[i].params == "频率") {
                FrequencyValues << data[i].value;
            } else if (data[i].params == "功率") {
                PowerValues << data[i].value;
            } else if (data[i].params == "相对电平") {
                RelativeLevelValues << data[i].value;
            } else if (data[i].params == "调幅度") {
                RangeValues << data[i].value;
            } else if (data[i].params == "调频频偏") {
                FrequencyOffsetValues << data[i].value;
            } else if (data[i].params == "谐波") {
                HarmonicValues << data[i].value;
            } else if (data[i].params == "调相相偏") {
                PhaseDeviationValues << data[i].value;
            } else {
                FrequencyValues << "";
                PowerValues << "";
                RelativeLevelValues << "";
                RangeValues << "";
                FrequencyOffsetValues << "";
                HarmonicValues << "";
                PhaseDeviationValues << "";
            }
        } else {
            FrequencyValues << "";
            PowerValues << "";
            RelativeLevelValues << "";
            RangeValues << "";
            FrequencyOffsetValues << "";
            HarmonicValues << "";
            PhaseDeviationValues << "";
        }
    }

    for (const QStringList& row : ResultData) {
        QString VerificationItem = NULL;
        if (!row.isEmpty() && row[3] == "频率" && row[14].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率点"
                 << "幅度"
                 << "核查类型"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "测量值7"
                 << "测量值8"
                 << "测量值9"
                 << "测量值10"
                 << "平均值x"
                 << "重复性S(x)/x";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 频率点
            Data.append(row[7] + row[8]); // 幅度
            Data.append(row[14]);
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 测量值7
            Data.append(""); // 测量值8
            Data.append(""); // 测量值9
            Data.append(""); // 测量值10
            Data.append(""); // 平均值x
            Data.append(""); // 重复性S(x)/x
            FrequencyList.append(Data);
            resultMap.insert("重复性", FrequencyList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "功率" && row[14].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率点"
                 << "功率"
                 << "核查类型"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "测量值7"
                 << "测量值8"
                 << "测量值9"
                 << "测量值10"
                 << "平均值x"
                 << "重复性S(x)/x";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 频率点
            Data.append(row[9] + row[10]); // 功率
            Data.append(row[14]);
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 测量值7
            Data.append(""); // 测量值8
            Data.append(""); // 测量值9
            Data.append(""); // 测量值10
            Data.append(""); // 平均值x
            Data.append(""); // 重复性S(x)/x
            PowerList.append(Data);
            resultMap.insert("重复性", PowerList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "相对电平" && row[14].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率点"
                 << "相对电平"
                 << "核查类型"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "测量值7"
                 << "测量值8"
                 << "测量值9"
                 << "测量值10"
                 << "平均值x"
                 << "重复性S(x)/x";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 频率点
            Data.append(row[11] + row[12]); // 相对电平
            Data.append(row[14]);
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 测量值7
            Data.append(""); // 测量值8
            Data.append(""); // 测量值9
            Data.append(""); // 测量值10
            Data.append(""); // 平均值x
            Data.append(""); // 重复性S(x)/x
            RelativeLevelList.append(Data);
            resultMap.insert("重复性", RelativeLevelList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "调幅度" && row[14].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率点"
                 << "调幅度"
                 << "幅度"
                 << "高通滤波"
                 << "低通滤波"
                 << "调制速率"
                 << "核查类型"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "测量值7"
                 << "测量值8"
                 << "测量值9"
                 << "测量值10"
                 << "平均值x"
                 << "重复性S(x)/x";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 频率点
            Data.append(row[15]); // 调幅度
            Data.append(row[7] + row[8]); // 幅度
            Data.append(row[16] + row[17]); // 高通滤波
            Data.append(row[18] + row[19]); // 低通滤波
            Data.append(row[20] + row[21]); // 调制速率
            Data.append(row[14]);
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 测量值7
            Data.append(""); // 测量值8
            Data.append(""); // 测量值9
            Data.append(""); // 测量值10
            Data.append(""); // 平均值x
            Data.append(""); // 重复性S(x)/x
            RangeList.append(Data);
            resultMap.insert("重复性", RangeList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "调频频偏" && row[14].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率点"
                 << "调频频偏"
                 << "幅度"
                 << "高通滤波"
                 << "低通滤波"
                 << "调制速率"
                 << "核查类型"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "测量值7"
                 << "测量值8"
                 << "测量值9"
                 << "测量值10"
                 << "平均值x"
                 << "重复性S(x)/x";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 频率点
            Data.append(row[22] + row[23]); // 调频频偏
            Data.append(row[7] + row[8]); // 幅度
            Data.append(row[16] + row[17]); // 高通滤波
            Data.append(row[18] + row[19]); // 低通滤波
            Data.append(row[20] + row[21]); // 调制速率
            Data.append(row[14]);
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 测量值7
            Data.append(""); // 测量值8
            Data.append(""); // 测量值9
            Data.append(""); // 测量值10
            Data.append(""); // 平均值x
            Data.append(""); // 重复性S(x)/x
            FrequencyOffsetList.append(Data);
            resultMap.insert("重复性", FrequencyOffsetList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "谐波" && row[14].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率点"
                 << "幅度"
                 << "RBW"
                 << "VBW"
                 << "核查类型"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "测量值7"
                 << "测量值8"
                 << "测量值9"
                 << "测量值10"
                 << "平均值x"
                 << "重复性S(x)/x";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 频率点
            Data.append(row[7] + row[8]); // 幅度
            Data.append(row[26] + row[27]); // RBW
            Data.append(row[28] + row[29]); // VBW
            Data.append(row[14]);
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 测量值7
            Data.append(""); // 测量值8
            Data.append(""); // 测量值9
            Data.append(""); // 测量值10
            Data.append(""); // 算数平均值
            Data.append(""); // 标准偏差S(X)
            Data.append(""); // 相对重复性
            HarmonicList.append(Data);
            resultMap.insert("重复性", HarmonicList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "调相相偏" && row[14].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率点"
                 << "调相相偏"
                 << "幅度"
                 << "高通滤波"
                 << "低通滤波"
                 << "调制速率"
                 << "核查类型"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "测量值7"
                 << "测量值8"
                 << "测量值9"
                 << "测量值10"
                 << "平均值x"
                 << "重复性S(x)/x";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 频率点
            Data.append(row[24] + row[25]); // 调相相偏
            Data.append(row[7] + row[8]); // 幅度
            Data.append(row[16] + row[17]); // 高通滤波
            Data.append(row[18] + row[19]); // 低通滤波
            Data.append(row[20] + row[21]); // 调制速率
            Data.append(row[14]);
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 测量值7
            Data.append(""); // 测量值8
            Data.append(""); // 测量值9
            Data.append(""); // 测量值10
            Data.append(""); // 平均值x
            Data.append(""); // 重复性S(x)/x
            PhaseDeviationList.append(Data);
            resultMap.insert("重复性", PhaseDeviationList);
            result.insert(row[3], resultMap);
        }
        if (!row.isEmpty() && row[3] == "频率" && row[14].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率点"
                 << "核查类型"
                 << FrequencyDate << NowDate << "平均值"
                 << "稳定性"
                 << "U(k=2)"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 频率点
            Data.append(row[14]);
            Data << FrequencyValues;
            Data.append(""); // 测量值6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(""); // U(k=2)
            Data.append(""); // 结论
            FrequencyStableList.append(Data);
            resultMap.insert("稳定性", FrequencyStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "功率" && row[14].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率点"
                 << "功率"
                 << "核查类型"
                 << PowerDate << NowDate << "平均值"
                 << "稳定性"
                 << "U(k=2)"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 频率点
            Data.append(row[9] + row[10]); // 功率
            Data.append(row[14]);
            Data << PowerValues;
            Data.append(""); // 测量值6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(""); // U(k=2)
            Data.append(""); // 结论
            PowerStableList.append(Data);
            resultMap.insert("稳定性", PowerStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "相对电平" && row[14].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率点"
                 << "相对电平"
                 << "核查类型"
                 << RelativeLevelDate << NowDate << "平均值"
                 << "稳定性"
                 << "U(k=2)"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 频率点
            Data.append(row[11] + row[12]); // 相对电平
            Data.append(row[14]);
            Data << RelativeLevelValues;
            Data.append(""); // 测量值6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(""); // U(k=2)
            Data.append(""); // 结论
            RelativeLevelStableList.append(Data);
            resultMap.insert("稳定性", RelativeLevelStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "调幅度" && row[14].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率点"
                 << "调幅度"
                 << "核查类型"
                 << RangeDate << NowDate << "平均值"
                 << "稳定性"
                 << "U(k=2)"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 频率点
            Data.append(row[15]); // 调幅度
            Data.append(row[14]);
            Data << RangeValues;
            Data.append(""); // 测量值6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(""); // U(k=2)
            Data.append(""); // 结论
            RangeStableList.append(Data);
            resultMap.insert("稳定性", RangeStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "调频频偏" && row[14].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率点"
                 << "调频频偏"
                 << "核查类型"
                 << FrequencyOffsetDate << NowDate << "平均值"
                 << "稳定性"
                 << "U(k=2)"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 频率点
            Data.append(row[22] + row[23]); // 调频频偏
            Data.append(row[14]);
            Data << FrequencyOffsetValues;
            Data.append(""); // 测量值6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(""); // U(k=2)
            Data.append(""); // 结论
            FrequencyOffsetStableList.append(Data);
            resultMap.insert("稳定性", FrequencyOffsetStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "谐波" && row[14].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率点"
                 << "核查类型"
                 << HarmonicDate << NowDate << "平均值"
                 << "稳定性"
                 << "U(k=2)"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 频率点
            Data.append(row[14]);
            Data << HarmonicValues;
            Data.append(""); // 测量值6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(""); // U(k=2)
            Data.append(""); // 结论
            HarmonicStableList.append(Data);
            resultMap.insert("稳定性", HarmonicStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "调相相偏" && row[14].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率点"
                 << "调相相偏"
                 << "核查类型"
                 << PhaseDeviationDate << NowDate << "平均值"
                 << "稳定性"
                 << "U(k=2)"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 频率点
            Data.append(row[24] + row[25]); // 调相相偏
            Data.append(row[14]);
            Data << PhaseDeviationValues;
            Data.append(""); // 测量值6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(""); // U(k=2)
            Data.append(""); // 结论
            PhaseDeviationStableList.append(Data);
            resultMap.insert("稳定性", PhaseDeviationStableList);
            result.insert(row[3], resultMap);
        }
    }
    return result;
}
// 【保存】核查数据-信号发生器
bool BaseCommonApi::InsertStandardCheckItemDataSignalGenerator(
    StandardCheckItemDataSignalGenerator signalGenerator)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_equipment_standard_check_data_signal_generator ("
        " id, data_id,  check_type, params, check_point, unit,  power,  "
        "power_unit,  relative_level,  relative_level_unit, "
        " modulation,  frequency_deviation,  frequency_deviation_unit,  "
        "phase_deviation, phase_deviation_unit, "
        " average_value,  test_value_one_date,  test_value_one,   "
        "test_value_two_date,  test_value_two, "
        " test_value_three_date, test_value_three,   test_value_four_date,  "
        "test_value_four,   test_value_five_date,"
        " test_value_five,   test_value_six_date,  test_value_six,  "
        "test_value_seven_date,  test_value_seven, "
        " test_value_eight_date,  test_value_eight, test_value_nine_date,   "
        "test_value_nine,   test_value_ten_date, "
        " test_value_ten,  standard_deviation,   urel,  less_than,  stability,  "
        "qualified,  create_time,  update_time"
        " ) VALUES ( :id,  :data_id,  :check_type,  :params,  :check_point,   "
        ":unit, :power,  :power_unit, :relative_level, "
        " :relative_level_unit,  :modulation,  :frequency_deviation,  "
        ":frequency_deviation_unit,  :phase_deviation, "
        " :phase_deviation_unit,  :average_value,  :test_value_one_date,   "
        ":test_value_one,  :test_value_two_date, "
        " :test_value_two,  :test_value_three_date,  :test_value_three,  "
        ":test_value_four_date,  :test_value_four, "
        " :test_value_five_date,  :test_value_five,  :test_value_six_date,   "
        ":test_value_six,   :test_value_seven_date, "
        " :test_value_seven,  :test_value_eight_date,  :test_value_eight,  "
        ":test_value_nine_date,  :test_value_nine, "
        " :test_value_ten_date,  :test_value_ten,  :standard_deviation, :urel,  "
        ":less_than,  :stability,  :qualified, "
        " :create_time,  :update_time"
        " );");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", signalGenerator.id);
    query.bindValue(":data_id", signalGenerator.data_id);
    query.bindValue(":check_type", signalGenerator.check_type);
    query.bindValue(":params", signalGenerator.params);
    query.bindValue(":check_point", signalGenerator.check_point);
    query.bindValue(":unit", signalGenerator.unit);
    query.bindValue(":power", signalGenerator.power);
    query.bindValue(":power_unit", signalGenerator.power_unit);
    query.bindValue(":relative_level", signalGenerator.relative_level);
    query.bindValue(":relative_level_unit", signalGenerator.relative_level_unit);
    query.bindValue(":modulation", signalGenerator.modulation);
    query.bindValue(":frequency_deviation", signalGenerator.frequency_deviation);
    query.bindValue(":frequency_deviation_unit",
        signalGenerator.frequency_deviation_unit);
    query.bindValue(":phase_deviation", signalGenerator.phase_deviation);
    query.bindValue(":phase_deviation_unit",
        signalGenerator.phase_deviation_unit);
    query.bindValue(":average_value", signalGenerator.average_value);
    query.bindValue(":test_value_one_date", signalGenerator.test_value_one_date);
    query.bindValue(":test_value_one", signalGenerator.test_value_one);
    query.bindValue(":test_value_two_date", signalGenerator.test_value_two_date);
    query.bindValue(":test_value_two", signalGenerator.test_value_two);
    query.bindValue(":test_value_three_date",
        signalGenerator.test_value_three_date);
    query.bindValue(":test_value_three", signalGenerator.test_value_three);
    query.bindValue(":test_value_four_date",
        signalGenerator.test_value_four_date);
    query.bindValue(":test_value_four", signalGenerator.test_value_four);
    query.bindValue(":test_value_five_date",
        signalGenerator.test_value_five_date);
    query.bindValue(":test_value_five", signalGenerator.test_value_five);
    query.bindValue(":test_value_six_date", signalGenerator.test_value_six_date);
    query.bindValue(":test_value_six", signalGenerator.test_value_six);
    query.bindValue(":test_value_seven_date",
        signalGenerator.test_value_seven_date);
    query.bindValue(":test_value_seven", signalGenerator.test_value_seven);
    query.bindValue(":test_value_eight_date",
        signalGenerator.test_value_eight_date);
    query.bindValue(":test_value_eight", signalGenerator.test_value_eight);
    query.bindValue(":test_value_nine_date",
        signalGenerator.test_value_nine_date);
    query.bindValue(":test_value_nine", signalGenerator.test_value_nine);
    query.bindValue(":test_value_ten_date", signalGenerator.test_value_ten_date);
    query.bindValue(":test_value_ten", signalGenerator.test_value_ten);
    query.bindValue(":standard_deviation", signalGenerator.standard_deviation);
    query.bindValue(":urel", signalGenerator.urel);
    query.bindValue(":less_than", signalGenerator.less_than);
    query.bindValue(":stability", signalGenerator.stability);
    query.bindValue(":qualified", signalGenerator.qualified);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << query.lastError().text();
    }
    return isSuccess;
}
// 【查询】核查项-0.02压力
QMap<QString, QMap<QString, QList<QStringList>>>
BaseCommonApi::getStandardCheckItemDataPistonGauge(
    QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList)
{
    QMap<QString, QMap<QString, QList<QStringList>>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();
    QSqlQuery query;
    query.prepare(
        "SELECT *	FROM biz_equipment_standard_check_item_data_piston_gauge  "
        " WHERE equipment_id = :equipment_id ORDER BY create_time ASC");
    query.bindValue(":equipment_id", equipment_id);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_category_id")
                              .toString()); // 核查项分类id(能够确定是哪个标准装置)
            result.append(query.value("equipment_id").toString()); // 配套设备id
            result.append(
                query.value("check_item").toString()); // 核查项目(直接存字典value)
            result.append(query.value("unit").toString()); // 单位(直接存字典value)
            result.append(
                query.value("check_point").toString()); // 核查点(直接存字典value)
            result.append(
                query.value("standard_indication").toString()); // 核查标准示值
            result.append(query.value("maximum_error_absolute")
                              .toString()); // 最大允许误差绝对值(直接存字典value) 7
            result.append(query.value("resolution").toString()); // 分辨力
            result.append(query.value("cycle_number")
                              .toString()); // 循环次数（正行程和反行程）
            result.append(query.value("remark").toString()); // 核查项备注
            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());
            resultData.append(result);
        }
    } else {
        qDebug() << query.lastError().text();
    }
    DataMapList = GetDataReturnItemListByCheckItemDataPistonGauge(
        resultData, DataMapList, headList);
    return DataMapList;
}

QMap<QString, QMap<QString, QList<QStringList>>>
BaseCommonApi::GetDataReturnItemListByCheckItemDataPistonGauge(
    QList<QStringList> ResultData,
    QMap<QString, QMap<QString, QList<QStringList>>> DataMapList,
    QMap<QString, QMap<QString, QStringList>>* headList)
{
    QMap<QString, QMap<QString, QList<QStringList>>> result;
    QMap<QString, QList<QStringList>> resultMap;
    QList<QStringList> PressureValueList;

    QMap<QString, QStringList> headerMap;
    for (const QStringList& row : ResultData) {
        if (!row.isEmpty() && row[3] == "3") {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "单位"
                 << "核查点"
                 << "第一次正行程"
                 << "第一次反行程"
                 << "第二次正行程"
                 << "第二次反行程"
                 << "第三次正行程"
                 << "第三次反行程"
                 << "均值"
                 << "最大示值误差绝对值"
                 << "最大回程误差绝对值"
                 << "最大允许误差绝对值";
            headerMap.insert("压力值", head);
            headList->insert("压力值", headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[4]); // 单位
            Data.append(row[5]); // 核查点
            Data.append(""); // 第一次正行程
            Data.append(""); // 第一次反行程
            Data.append(""); // 第二次正行程
            Data.append(""); // 第二次反行程
            Data.append(""); // 第三次正行程
            Data.append(""); // 第三次反行程
            Data.append(""); // 均值
            Data.append(""); // 最大示值误差绝对值
            Data.append(""); // 最大回程误差绝对值
            Data.append(row[7]); // 最大允许误差绝对值
            PressureValueList.append(Data);
            resultMap.insert("压力值", PressureValueList);
            result.insert("压力值", resultMap);
        }
        else if(row[3] == "1" ||row[3] == "2"){
            QStringList head;
            head  <<"序号"<<"核查项目"<<"核查点"<<"单位"<<"第一次核查标准示值"<<"第一次测量标准示值"
                 <<"第二次核查标准示值"<<"第二次测量标准示值"<<"第三次核查标准示值"<<"第三次测量标准示值"
                   <<"第四次核查标准示值"<<"第四次测量标准示值"<<"第五次核查标准示值"<<"第五次测量标准示值"
                     <<"第六次核查标准示值"<<"第六次测量标准示值"
                 <<"测量平均值"<<"核查平均值"<<"最大示值误差"<<"最大允许误差";
            headerMap.insert("电信号",head);
            headList->insert("电信号",headerMap);
            //添加数据
            QStringList Data;
            Data.append(row[0]);  //ID
            Data.append(row[3]); // 核查项目
            Data.append(row[5]);//核查点
            Data.append(row[4]);//单位
            Data.append(row[6]);//第一次核查标准示值
            Data.append("");//第一次测量标准示值
            Data.append(row[6]);//第二次核查标准示值
            Data.append("");//第二次测量标准示值
            Data.append(row[6]);//第三次核查标准示值
            Data.append("");//第三次测量标准示值
            Data.append(row[6]);//第四次核查标准示值
            Data.append("");//第四次测量标准示值
            Data.append(row[6]);//第五次核查标准示值
            Data.append("");//第五次测量标准示值
            Data.append(row[6]);//第六次核查标准示值
            Data.append("");//第六次测量标准示值
            Data.append(row[6]);//核查平均值
            Data.append("");//测量平均值
            Data.append("");//最大示值误差
            Data.append(row[7]);//最大允许误差
            PressureValueList.append(Data);
            resultMap.insert("电信号",PressureValueList);
            result.insert("电信号",resultMap);
        }
    }
    return result;
}
// 【保存】核查项-0.02压力
bool BaseCommonApi::InsertStandardCheckItemDataPistonGauge(
    StandardCheckItemDataPistonGauge pistonGauge)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_equipment_standard_check_data_piston_gauge ("
        " id, data_id, unit, check_point, first_forward_stroke, "
        "first_reverse_stroke,  second_forward_stroke,  second_reverse_stroke,"
        " third_forward_stroke,  third_reverse_stroke,  average_value,  "
        "average_average_value,  max_indicating_error, "
        " max_return_error,  params,  test_value_one,  measure_value_one,  "
        "test_value_two,  measure_value_two, "
        " test_value_three,  measure_value_three, test_value_four,  "
        "measure_value_four,  test_value_five,  measure_value_five, "
        " test_value_six, measure_value_six, maximum_error, create_time, "
        "update_time"
        " ) VALUES ( :id,  :data_id,  :unit,  :check_point,  "
        ":first_forward_stroke,  :first_reverse_stroke,  :second_forward_stroke, "
        " :second_reverse_stroke, :third_forward_stroke, :third_reverse_stroke,  "
        ":average_value,  :average_average_value, "
        " :max_indicating_error,  :max_return_error,  :params,  :test_value_one, "
        " :measure_value_one,  :test_value_two, "
        " :measure_value_two,  :test_value_three,  :measure_value_three,  "
        ":test_value_four,  :measure_value_four, "
        " :test_value_five, :measure_value_five, :test_value_six, "
        ":measure_value_six, :maximum_error, :create_time, "
        " :update_time);");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", pistonGauge.id);
    query.bindValue(":data_id", pistonGauge.data_id);
    query.bindValue(":unit", pistonGauge.unit);
    query.bindValue(":check_point", pistonGauge.check_point);
    query.bindValue(":first_forward_stroke", pistonGauge.first_forward_stroke);
    query.bindValue(":first_reverse_stroke", pistonGauge.first_reverse_stroke);
    query.bindValue(":second_forward_stroke", pistonGauge.second_forward_stroke);
    query.bindValue(":second_reverse_stroke", pistonGauge.second_reverse_stroke);
    query.bindValue(":third_forward_stroke", pistonGauge.third_forward_stroke);
    query.bindValue(":third_reverse_stroke", pistonGauge.third_reverse_stroke);
    query.bindValue(":average_value", pistonGauge.average_value);
    query.bindValue(":average_average_value", pistonGauge.average_average_value);
    query.bindValue(":max_indicating_error", pistonGauge.max_indicating_error);
    query.bindValue(":max_return_error", pistonGauge.max_return_error);
    query.bindValue(":params", pistonGauge.params);
    query.bindValue(":test_value_one", pistonGauge.test_value_one);
    query.bindValue(":measure_value_one", pistonGauge.measure_value_one);
    query.bindValue(":test_value_two", pistonGauge.test_value_two);
    query.bindValue(":measure_value_two", pistonGauge.measure_value_two);
    query.bindValue(":test_value_three", pistonGauge.test_value_three);
    query.bindValue(":measure_value_three", pistonGauge.measure_value_three);
    query.bindValue(":test_value_four", pistonGauge.test_value_four);
    query.bindValue(":measure_value_four", pistonGauge.measure_value_four);
    query.bindValue(":test_value_five", pistonGauge.test_value_five);
    query.bindValue(":measure_value_five", pistonGauge.measure_value_five);
    query.bindValue(":test_value_six", pistonGauge.test_value_six);
    query.bindValue(":measure_value_six", pistonGauge.measure_value_six);
    query.bindValue(":maximum_error", pistonGauge.maximum_error);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << query.lastError().text();
    }
    return isSuccess;
}
// 【查询】核查项-频谱分析仪
QMap<QString, QMap<QString, QList<QStringList>>>
BaseCommonApi::getStandardCheckItemDataSpectrumAnalyzer(
    QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList)
{
    QMap<QString, QMap<QString, QList<QStringList>>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();
    QSqlQuery query;
    query.prepare(
        "SELECT *	FROM biz_equipment_standard_check_item_data_spectrum_analyzer  "
        " WHERE equipment_id = :equipment_id  ORDER BY create_time ASC");
    query.bindValue(":equipment_id", equipment_id);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_category_id")
                              .toString()); // 核查项分类id(能够确定是哪个标准装置)
            result.append(query.value("equipment_id").toString()); // 配套设备id
            result.append(
                query.value("params").toString()); // 核查项目(直接存字典value)
            result.append(
                query.value("check_point").toString()); // 频率点(字典value)
            result.append(query.value("unit").toString()); // 频率点单位(字典value) 5
            result.append(query.value("urel").toString()); // U(k=2)
            result.append(query.value("amplitude").toString()); // 幅度
            result.append(
                query.value("amplitude_unit").toString()); // 幅度单位(字典value)
            result.append(query.value("power").toString()); // 功率(字典value)
            result.append(
                query.value("power_unit").toString()); // 功率单位(字典value)  10
            result.append(
                query.value("relative_level").toString()); // 相对电平(字典value)
            result.append(query.value("relative_level_unit")
                              .toString()); // 相对电平单位(字典value)
            result.append(query.value("cycle_number").toString()); // 循环次数
            result.append(
                query.value("check_type").toString()); // 核查类型(字典value)
            result.append(
                query.value("composition").toString()); // 组合方式(字典value)
            result.append(query.value("remark").toString()); // 核查项备注
            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());
            resultData.append(result);
        }
    } else {
        qDebug() << query.lastError().text();
    }
    DataMapList = GetDataReturnItemListByCheckItemDataSpectrumAnalyzer(
        resultData, DataMapList, headList);
    return DataMapList;
}
QMap<QString, QMap<QString, QList<QStringList>>>
BaseCommonApi::GetDataReturnItemListByCheckItemDataSpectrumAnalyzer(
    QList<QStringList> ResultData,
    QMap<QString, QMap<QString, QList<QStringList>>> DataMapList,
    QMap<QString, QMap<QString, QStringList>>* headList)
{
    QMap<QString, QMap<QString, QList<QStringList>>> result;
    QMap<QString, QList<QStringList>> resultMap;
    QList<QStringList> FrequencyList;
    QList<QStringList> PowerList;
    QList<QStringList> RelativeLevelList;
    QList<QStringList> FrequencyStableList;
    QList<QStringList> PowerStableList;
    QList<QStringList> RelativeLevelStableList;
    QMap<QString, QStringList> headerMap;

    CheckProgramManage programInfo = BaseCommonApi::getCheckProgramManageById(loginNowVerifyProgramId);
    QList<DataCalibratorStability> data = BaseCommonApi::GetEquipmentStandardCheckInfoByids(programInfo.standard_id, loginNowVerifyEquimentId, programInfo.equipment_type, 9);
    QString NowDate = QDate::currentDate().toString("yyyy年MM月dd");
    QStringList FrequencyDate;
    QStringList PowerDate;
    QStringList RelativeLevelDate;
    for (int i = 0; i < data.size(); ++i) {
        //        if (i < data.size()) {
        //            historyDate << data[i].date.toString("yyyy年MM月dd日");
        //        } else {
        //            historyDate << "";
        //        }
        if (i < data.size()) {
            if (data[i].params == "频率") {
                FrequencyDate << data[i].date.toString("yyyy年MM月dd日");
            } else if (data[i].params == "功率") {
                PowerDate << data[i].date.toString("yyyy年MM月dd日");
            } else if (data[i].params == "相对电平") {
                RelativeLevelDate << data[i].date.toString("yyyy年MM月dd日");
            } else {
                FrequencyDate << "";
                PowerDate << "";
                RelativeLevelDate << "";
            }
        } else {
            FrequencyDate << "";
            PowerDate << "";
            RelativeLevelDate << "";
        }
    }
    QStringList FrequencyValues;
    QStringList PowerValues;
    QStringList RelativeLevelValues;
    for (int i = 0; i < data.size(); ++i) {
        if (i < data.size()) {
            if (data[i].params == "频率") {
                FrequencyValues << data[i].value;
            } else if (data[i].params == "功率") {
                PowerValues << data[i].value;
            } else if (data[i].params == "相对电平") {
                RelativeLevelValues << data[i].value;
            } else {
                FrequencyValues << "";
                PowerValues << "";
                RelativeLevelValues << "";
            }
        } else {
            FrequencyValues << "";
            PowerValues << "";
            RelativeLevelValues << "";
        }
    }

    for (const QStringList& row : ResultData) {
        if (!row.isEmpty() && row[3] == "频率" && row[14].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率点"
                 << "幅度"
                 << "幅度单位"
                 << "频率点单位"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "测量值7"
                 << "测量值8"
                 << "测量值9"
                 << "测量值10"
                 << "平均值x"
                 << "重复性S(x)/x";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 频率点
            Data.append(row[5]); // 频率点单位
            Data.append(row[7] + row[8]); // 幅度
            Data.append(row[8]); // 幅度单位
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 测量值7
            Data.append(""); // 测量值8
            Data.append(""); // 测量值9
            Data.append(""); // 测量值10
            Data.append(""); // 平均值x
            Data.append(""); // 重复性S(x)/x
            FrequencyList.append(Data);
            resultMap.insert("重复性", FrequencyList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "功率" && row[14].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率点"
                 << "频率点单位"
                 << "功率"
                 << "功率单位"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "测量值7"
                 << "测量值8"
                 << "测量值9"
                 << "测量值10"
                 << "平均值x"
                 << "重复性S(x)/x";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 频率点
            Data.append(row[5]); // 频率点单位
            Data.append(row[9] + row[10]); // 功率
            Data.append(row[10]); // 功率单位
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 测量值7
            Data.append(""); // 测量值8
            Data.append(""); // 测量值9
            Data.append(""); // 测量值10
            Data.append(""); // 平均值x
            Data.append(""); // 重复性S(x)/x
            PowerList.append(Data);
            resultMap.insert("重复性", PowerList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "相对电平" && row[14].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "组合方式"
                 << "频率点"
                 << "频率点单位"
                 << "相对电平"
                 << "相对电平单位"
                 << "幅度"
                 << "幅度单位"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "测量值7"
                 << "测量值8"
                 << "测量值9"
                 << "测量值10"
                 << "平均值x"
                 << "重复性S(x)/x";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[15]); // 组合方式
            Data.append(row[4] + row[5]); // 频率点
            Data.append(row[5]); // 频率点单位
            Data.append(row[11] + row[12]); // 相对电平
            Data.append(row[12]); // 相对电平单位
            Data.append(row[7] + row[8]); // 幅度
            Data.append(row[8]); // 幅度单位
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 测量值7
            Data.append(""); // 测量值8
            Data.append(""); // 测量值9
            Data.append(""); // 测量值10
            Data.append(""); // 平均值x
            Data.append(""); // 重复性S(x)/x
            RelativeLevelList.append(Data);
            resultMap.insert("重复性", RelativeLevelList);
            result.insert(row[3], resultMap);
        }
        if (!row.isEmpty() && row[3] == "频率" && row[14].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率点"
                 << "频率点单位"
                 << FrequencyDate << NowDate
                 << "平均值"
                 << "稳定性"
                 << "U(k=2)"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 频率点
            Data.append(row[5]); // 频率点单位
            Data << FrequencyValues;
            Data.append(""); // 测量值6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[6]); // U(k=2)
            Data.append(""); // 结论
            FrequencyStableList.append(Data);
            resultMap.insert("稳定性", FrequencyList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "功率" && row[14].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率点"
                 << "频率点单位"
                 << "功率"
                 << "功率单位"
                 << PowerDate << NowDate
                 << "平均值"
                 << "稳定性"
                 << "U(k=2)"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 频率点
            Data.append(row[5]); // 频率点单位
            Data.append(row[9] + row[10]); // 功率
            Data.append(row[10]); // 功率单位
            Data << PowerValues;
            Data.append(""); // 测量值6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[6]); // U(k=2)
            Data.append(""); // 结论
            PowerStableList.append(Data);
            resultMap.insert("稳定性", PowerStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "相对电平" && row[14].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率点"
                 << "频率点单位"
                 << "相对电平"
                 << "相对电平单位"
                 << RelativeLevelDate << NowDate
                 << "平均值"
                 << "稳定性"
                 << "U(k=2)"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 频率点
            Data.append(row[5]); // 频率点单位
            Data.append(row[11] + row[12]); // 相对电平
            Data.append(row[12]); // 相对电平单位
            Data << RelativeLevelValues;
            Data.append(""); // 测量值6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[6]); // U(k=2)
            Data.append(""); // 结论
            RelativeLevelStableList.append(Data);
            resultMap.insert("稳定性", RelativeLevelStableList);
            result.insert(row[3], resultMap);
        }
    }
    return result;
}
//   【保存】核查项-频谱分析仪
bool BaseCommonApi::InsertStandardCheckItemDataSpectrumAnalyzer(
    StandardCheckItemDataSpectrumAnalyzer spectrumAnalyzer)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_equipment_standard_check_data_spectrum_analyzer ("
        " id, data_id,  check_type, params, check_point,  unit,  power, "
        "power_unit,  relative_level,  relative_level_unit, "
        " composition, average_value,  test_value_one_date,  test_value_one, "
        "test_value_two_date, test_value_two, "
        "  test_value_three_date, test_value_three, test_value_four_date,  "
        "test_value_four,  test_value_five_date, "
        " test_value_five, test_value_six_date, test_value_six,  "
        "test_value_seven_date, test_value_seven, test_value_eight_date, "
        " test_value_eight, test_value_nine_date,  test_value_nine, "
        "test_value_ten_date,  test_value_ten, standard_deviation, "
        " urel,  less_than, stability, qualified, create_time,  update_time"
        " ) VALUES (:id,  :data_id,  :check_type,  :params,  :check_point, "
        ":unit,  :power,  :power_unit,  :relative_level, "
        " :relative_level_unit,  :composition,  :average_value, "
        ":test_value_one_date, :test_value_one,  :test_value_two_date, "
        " :test_value_two,  :test_value_three_date, :test_value_three,  "
        ":test_value_four_date,  :test_value_four, "
        " :test_value_five_date,  :test_value_five,  :test_value_six_date,  "
        ":test_value_six,  :test_value_seven_date, "
        " :test_value_seven, :test_value_eight_date, :test_value_eight, "
        ":test_value_nine_date,  :test_value_nine, "
        " :test_value_ten_date, :test_value_ten,  :standard_deviation, :urel, "
        " :less_than, :stability, :qualified,  :create_time, :update_time );");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", spectrumAnalyzer.id);
    query.bindValue(":data_id", spectrumAnalyzer.data_id);
    query.bindValue(":check_type", spectrumAnalyzer.check_type);
    query.bindValue(":params", spectrumAnalyzer.params);
    query.bindValue(":check_point", spectrumAnalyzer.check_point);
    query.bindValue(":unit", spectrumAnalyzer.unit);
    query.bindValue(":power", spectrumAnalyzer.power);
    query.bindValue(":power_unit", spectrumAnalyzer.power_unit);
    query.bindValue(":relative_level", spectrumAnalyzer.relative_level);
    query.bindValue(":relative_level_unit", spectrumAnalyzer.relative_level_unit);
    query.bindValue(":composition", spectrumAnalyzer.composition);
    query.bindValue(":average_value", spectrumAnalyzer.average_value);
    query.bindValue(":test_value_one_date", spectrumAnalyzer.test_value_one_date);
    query.bindValue(":test_value_one", spectrumAnalyzer.test_value_one);
    query.bindValue(":test_value_two_date", spectrumAnalyzer.test_value_two_date);
    query.bindValue(":test_value_two", spectrumAnalyzer.test_value_two);
    query.bindValue(":test_value_three_date",
        spectrumAnalyzer.test_value_three_date);
    query.bindValue(":test_value_three", spectrumAnalyzer.test_value_three);
    query.bindValue(":test_value_four_date",
        spectrumAnalyzer.test_value_four_date);
    query.bindValue(":test_value_four", spectrumAnalyzer.test_value_four);
    query.bindValue(":test_value_five_date",
        spectrumAnalyzer.test_value_five_date);
    query.bindValue(":test_value_five", spectrumAnalyzer.test_value_five);
    query.bindValue(":test_value_six_date", spectrumAnalyzer.test_value_six_date);
    query.bindValue(":test_value_six", spectrumAnalyzer.test_value_six);
    query.bindValue(":test_value_seven_date",
        spectrumAnalyzer.test_value_seven_date);
    query.bindValue(":test_value_seven", spectrumAnalyzer.test_value_seven);
    query.bindValue(":test_value_eight_date",
        spectrumAnalyzer.test_value_eight_date);
    query.bindValue(":test_value_eight", spectrumAnalyzer.test_value_eight);
    query.bindValue(":test_value_nine_date",
        spectrumAnalyzer.test_value_nine_date);
    query.bindValue(":test_value_nine", spectrumAnalyzer.test_value_nine);
    query.bindValue(":test_value_ten_date", spectrumAnalyzer.test_value_ten_date);
    query.bindValue(":test_value_ten", spectrumAnalyzer.test_value_ten);
    query.bindValue(":standard_deviation", spectrumAnalyzer.standard_deviation);
    query.bindValue(":urel", spectrumAnalyzer.urel);
    query.bindValue(":less_than", spectrumAnalyzer.less_than);
    query.bindValue(":stability", spectrumAnalyzer.stability);
    query.bindValue(":qualified", spectrumAnalyzer.qualified);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << query.lastError().text();
    }
    return isSuccess;
}

// 【查询】核查项-示波器
QMap<QString, QMap<QString, QList<QStringList>>>
BaseCommonApi::getStandardCheckItemDataOscilloscope(
    QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList)
{
    QMap<QString, QMap<QString, QList<QStringList>>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();
    QSqlQuery query;
    query.prepare(
        "SELECT *	FROM biz_equipment_standard_check_item_data_oscilloscope  "
        " WHERE equipment_id = :equipment_id ORDER BY create_time ASC");
    query.bindValue(":equipment_id", equipment_id);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_category_id")
                              .toString()); // 核查项分类id(能够确定是哪个标准装置)
            result.append(query.value("equipment_id").toString()); // 配套设备id
            result.append(
                query.value("params").toString()); // 核查项目(直接存字典value)
            result.append(query.value("check_point")
                              .toString()); // 核查点/快沿(上升时间)(字典value)
            result.append(
                query.value("unit")
                    .toString()); // 核查点单位/快沿单位(上升时间)(字典value)
            result.append(query.value("cycle_number").toString()); // 循环次数
            result.append(
                query.value("check_type").toString()); // 核查类型(字典value)
            result.append(query.value("maximum_error_absolute")
                              .toString()); // 最大允许误差的绝对值
            result.append(query.value("maximum_error_absolute_unit")
                              .toString()); // 最大允许误差的绝对值单位
            result.append(query.value("type_value").toString()); // 类型  10
            result.append(query.value("reference_point").toString()); // 是否为参考点
            result.append(query.value("manufacture_no").toString()); // 探头 出厂编号
            result.append(query.value("equipment_name").toString()); // 探头设备名称
            result.append(query.value("model").toString()); // 探头规格型号
            result.append(
                query.value("technical_index_symbol").toString()); // 技术指标前符号
            result.append(query.value("technical_index").toString()); // 技术指标
            result.append(
                query.value("technical_index_unit").toString()); // 技术指标单位
            result.append(query.value("remark").toString()); // 核查项备注
            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());
            resultData.append(result);
        }
    } else {
        qDebug() << query.lastError().text();
    }
    DataMapList = GetDataReturnItemListByCheckItemDataOscilloscope(
        resultData, DataMapList, headList);
    return DataMapList;
}
QMap<QString, QMap<QString, QList<QStringList>>>
BaseCommonApi::GetDataReturnItemListByCheckItemDataOscilloscope(
    QList<QStringList> ResultData,
    QMap<QString, QMap<QString, QList<QStringList>>> DataMapList,
    QMap<QString, QMap<QString, QStringList>>* headList)
{
    QMap<QString, QMap<QString, QList<QStringList>>> result;
    QMap<QString, QList<QStringList>> resultMap;
    QList<QStringList> TimeScaleList;
    QList<QStringList> SquareWaveVoltageFiftyList;
    QList<QStringList> SquareWaveVoltageOneList;
    QList<QStringList> DCVoltageFiftyList;
    QList<QStringList> DCVoltageOneList;
    QList<QStringList> InputImpedanceList;
    QList<QStringList> VoltageFlatnessList;
    QList<QStringList> RiseTimeList;

    QList<QStringList> TimeScaleStableList;
    QList<QStringList> SquareWaveVoltageFiftyStableList;
    QList<QStringList> SquareWaveVoltageOneStableList;
    QList<QStringList> DCVoltageFiftyStableList;
    QList<QStringList> DCVoltageOneStableList;
    QList<QStringList> InputImpedanceStableList;
    QList<QStringList> VoltageFlatnessStableList;
    QList<QStringList> RiseTimeStableList;
    QMap<QString, QStringList> headerMap;
    for (const QStringList& row : ResultData) {
        if (!row.isEmpty() && row[3] == "时标" && row[7].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "测量值*X1"
                 << "测量值*X2"
                 << "测量值*X3"
                 << "测量值*X4"
                 << "测量值*X5"
                 << "测量值*X6"
                 << "测量值*X7"
                 << "测量值*X8"
                 << "测量值*X9"
                 << "测量值*X10"
                 << "平均值x"
                 << "重复性S(x)";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 核查点
            Data.append(row[5]); // 核查点单位
            Data.append(""); // 测量值*X1
            Data.append(""); // 测量值*X2
            Data.append(""); // 测量值*X3
            Data.append(""); // 测量值*X4
            Data.append(""); // 测量值*X5
            Data.append(""); // 测量值*X6
            Data.append(""); // 测量值*X7
            Data.append(""); // 测量值*X8
            Data.append(""); // 测量值*X9
            Data.append(""); // 测量值*X10
            Data.append(""); // 平均值
            Data.append(""); // 重复性S(x)
            TimeScaleList.append(Data);
            resultMap.insert("重复性", TimeScaleList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "方波电压(50Ω)" && row[7].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "测量值*X1"
                 << "测量值*X2"
                 << "测量值*X3"
                 << "测量值*X4"
                 << "测量值*X5"
                 << "测量值*X6"
                 << "测量值*X7"
                 << "测量值*X8"
                 << "测量值*X9"
                 << "测量值*X10"
                 << "平均值x"
                 << "重复性S(x)";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 核查点
            Data.append(row[5]); // 核查点单位
            Data.append(""); // 测量值*X1
            Data.append(""); // 测量值*X2
            Data.append(""); // 测量值*X3
            Data.append(""); // 测量值*X4
            Data.append(""); // 测量值*X5
            Data.append(""); // 测量值*X6
            Data.append(""); // 测量值*X7
            Data.append(""); // 测量值*X8
            Data.append(""); // 测量值*X9
            Data.append(""); // 测量值*X10
            Data.append(""); // 平均值
            Data.append(""); // 重复性S(x)
            SquareWaveVoltageFiftyList.append(Data);
            resultMap.insert("重复性", SquareWaveVoltageFiftyList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "方波电压(1MΩ)" && row[7].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "测量值*X1"
                 << "测量值*X2"
                 << "测量值*X3"
                 << "测量值*X4"
                 << "测量值*X5"
                 << "测量值*X6"
                 << "测量值*X7"
                 << "测量值*X8"
                 << "测量值*X9"
                 << "测量值*X10"
                 << "平均值x"
                 << "重复性S(x)";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 核查点
            Data.append(row[5]); // 核查点单位
            Data.append(""); // 测量值*X1
            Data.append(""); // 测量值*X2
            Data.append(""); // 测量值*X3
            Data.append(""); // 测量值*X4
            Data.append(""); // 测量值*X5
            Data.append(""); // 测量值*X6
            Data.append(""); // 测量值*X7
            Data.append(""); // 测量值*X8
            Data.append(""); // 测量值*X9
            Data.append(""); // 测量值*X10
            Data.append(""); // 平均值
            Data.append(""); // 重复性S(x)
            SquareWaveVoltageOneList.append(Data);
            resultMap.insert("重复性", SquareWaveVoltageOneList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "直流电压(50Ω)" && row[7].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "测量值*X1"
                 << "测量值*X2"
                 << "测量值*X3"
                 << "测量值*X4"
                 << "测量值*X5"
                 << "测量值*X6"
                 << "测量值*X7"
                 << "测量值*X8"
                 << "测量值*X9"
                 << "测量值*X10"
                 << "平均值x"
                 << "重复性S(x)";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 核查点
            Data.append(row[5]); // 核查点单位
            Data.append(""); // 测量值*X1
            Data.append(""); // 测量值*X2
            Data.append(""); // 测量值*X3
            Data.append(""); // 测量值*X4
            Data.append(""); // 测量值*X5
            Data.append(""); // 测量值*X6
            Data.append(""); // 测量值*X7
            Data.append(""); // 测量值*X8
            Data.append(""); // 测量值*X9
            Data.append(""); // 测量值*X10
            Data.append(""); // 平均值
            Data.append(""); // 重复性S(x)
            DCVoltageFiftyList.append(Data);
            resultMap.insert("重复性", DCVoltageFiftyList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "直流电压(1MΩ)" && row[7].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "测量值*X1"
                 << "测量值*X2"
                 << "测量值*X3"
                 << "测量值*X4"
                 << "测量值*X5"
                 << "测量值*X6"
                 << "测量值*X7"
                 << "测量值*X8"
                 << "测量值*X9"
                 << "测量值*X10"
                 << "平均值x"
                 << "重复性S(x)";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 核查点
            Data.append(row[5]); // 核查点单位
            Data.append(""); // 测量值*X1
            Data.append(""); // 测量值*X2
            Data.append(""); // 测量值*X3
            Data.append(""); // 测量值*X4
            Data.append(""); // 测量值*X5
            Data.append(""); // 测量值*X6
            Data.append(""); // 测量值*X7
            Data.append(""); // 测量值*X8
            Data.append(""); // 测量值*X9
            Data.append(""); // 测量值*X10
            Data.append(""); // 平均值
            Data.append(""); // 重复性S(x)
            DCVoltageOneList.append(Data);
            resultMap.insert("重复性", DCVoltageOneList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "输入阻抗" && row[7].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "测量值*X1"
                 << "测量值*X2"
                 << "测量值*X3"
                 << "测量值*X4"
                 << "测量值*X5"
                 << "测量值*X6"
                 << "测量值*X7"
                 << "测量值*X8"
                 << "测量值*X9"
                 << "测量值*X10"
                 << "平均值x"
                 << "重复性S(x)";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 核查点
            Data.append(row[5]); // 核查点单位
            Data.append(""); // 测量值*X1
            Data.append(""); // 测量值*X2
            Data.append(""); // 测量值*X3
            Data.append(""); // 测量值*X4
            Data.append(""); // 测量值*X5
            Data.append(""); // 测量值*X6
            Data.append(""); // 测量值*X7
            Data.append(""); // 测量值*X8
            Data.append(""); // 测量值*X9
            Data.append(""); // 测量值*X10
            Data.append(""); // 平均值
            Data.append(""); // 重复性S(x)
            InputImpedanceList.append(Data);
            resultMap.insert("重复性", InputImpedanceList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "稳幅正弦信号电压平坦度" && row[7].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "类型"
                 << "核查点"
                 << "测量值单位"
                 << "转换为电压值单位"
                 << "测量值X1"
                 << "转换为电压值X1"
                 << "平坦度/%X1"
                 << "测量值X2"
                 << "转换为电压值X2"
                 << "平坦度/%X2"
                 << "测量值X3"
                 << "转换为电压值X3"
                 << "平坦度/%X3"
                 << "测量值X4"
                 << "转换为电压值X4"
                 << "平坦度/%X4"
                 << "测量值X5"
                 << "转换为电压值X5"
                 << "平坦度/%X5"
                 << "测量值X6"
                 << "转换为电压值X6"
                 << "平坦度/%X6"
                 << "测量值X7"
                 << "转换为电压值X7"
                 << "平坦度/%X7"
                 << "测量值X8"
                 << "转换为电压值X8"
                 << "平坦度/%X8"
                 << "测量值X9"
                 << "转换为电压值X9"
                 << "平坦度/%X9"
                 << "测量值X10"
                 << "转换为电压值X10"
                 << "平坦度/%X10"
                 << "平均值x"
                 << "重复性S(x)";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[10]); // 类型
            Data.append(row[4] + row[5]); // 核查点
            Data.append(row[5]); // 核查点单位
            Data.append(""); // 测量值单位
            Data.append(""); // 转换为电压值单位
            Data.append(""); // 测量值*X1
            Data.append("");
            Data.append("");
            Data.append(""); // 测量值*X2
            Data.append("");
            Data.append("");
            Data.append(""); // 测量值*X3
            Data.append("");
            Data.append("");
            Data.append(""); // 测量值*X4
            Data.append("");
            Data.append("");
            Data.append(""); // 测量值*X5
            Data.append("");
            Data.append("");
            Data.append(""); // 测量值*X6
            Data.append("");
            Data.append("");
            Data.append(""); // 测量值*X7
            Data.append("");
            Data.append("");
            Data.append(""); // 测量值*X8
            Data.append("");
            Data.append("");
            Data.append(""); // 测量值*X9
            Data.append("");
            Data.append("");
            Data.append(""); // 测量值*X10
            Data.append("");
            Data.append("");
            Data.append(""); // 平均值
            Data.append(""); // 重复性S(x)
            VoltageFlatnessList.append(Data);
            resultMap.insert("重复性", VoltageFlatnessList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "上升时间" && row[7].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "快沿"
                 << "快沿单位"
                 << "探头出厂编号"
                 << "探头设备名称"
                 << "探头规格型号"
                 << "测量值";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 快沿
            Data.append(row[5]); // 快沿单位
            Data.append(row[12]); // 探头出厂编号
            Data.append(row[13]); // 探头设备名称
            Data.append(row[14]); // 探头规格型号
            Data.append(""); // 测量值
            RiseTimeList.append(Data);
            resultMap.insert("重复性", RiseTimeList);
            result.insert(row[3], resultMap);
        }
        if (!row.isEmpty() && row[3] == "时标" && row[7].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "核查点单位"
                 << "核查点均值1"
                 << "核查点均值2"
                 << "核查点均值3"
                 << "核查点均值4"
                 << "核查点均值5"
                 << "核查点均值6"
                 << "平均值"
                 << "稳定性"
                 << "最大允许误差的绝对值"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 核查点
            Data.append(row[5]); // 核查点单位
            Data.append(""); // 核查点均值1
            Data.append(""); // 核查点均值2
            Data.append(""); // 核查点均值3
            Data.append(""); // 核查点均值4
            Data.append(""); // 核查点均值5
            Data.append(""); // 核查点均值6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[8] + row[9]); // 最大允许误差的绝对值
            Data.append(""); // 结论
            TimeScaleStableList.append(Data);
            resultMap.insert("稳定性", TimeScaleStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "方波电压(50Ω)" && row[7].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "核查点单位"
                 << "核查点均值1"
                 << "核查点均值2"
                 << "核查点均值3"
                 << "核查点均值4"
                 << "核查点均值5"
                 << "核查点均值6"
                 << "平均值"
                 << "稳定性"
                 << "最大允许误差的绝对值"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 核查点
            Data.append(row[5]); // 核查点单位
            Data.append(""); // 核查点均值1
            Data.append(""); // 核查点均值2
            Data.append(""); // 核查点均值3
            Data.append(""); // 核查点均值4
            Data.append(""); // 核查点均值5
            Data.append(""); // 核查点均值6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[8] + row[9]); // 最大允许误差的绝对值
            Data.append(""); // 结论
            SquareWaveVoltageFiftyStableList.append(Data);
            resultMap.insert("稳定性", SquareWaveVoltageFiftyStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "方波电压(1MΩ)" && row[7].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "核查点单位"
                 << "核查点均值1"
                 << "核查点均值2"
                 << "核查点均值3"
                 << "核查点均值4"
                 << "核查点均值5"
                 << "核查点均值6"
                 << "平均值"
                 << "稳定性"
                 << "最大允许误差的绝对值"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 核查点
            Data.append(row[5]); // 核查点单位
            Data.append(""); // 核查点均值1
            Data.append(""); // 核查点均值2
            Data.append(""); // 核查点均值3
            Data.append(""); // 核查点均值4
            Data.append(""); // 核查点均值5
            Data.append(""); // 核查点均值6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[8] + row[9]); // 最大允许误差的绝对值
            Data.append(""); // 结论
            SquareWaveVoltageOneStableList.append(Data);
            resultMap.insert("稳定性", SquareWaveVoltageOneStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "直流电压(50Ω)" && row[7].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "核查点单位"
                 << "核查点均值1"
                 << "核查点均值2"
                 << "核查点均值3"
                 << "核查点均值4"
                 << "核查点均值5"
                 << "核查点均值6"
                 << "平均值"
                 << "稳定性"
                 << "最大允许误差的绝对值"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 核查点
            Data.append(row[5]); // 核查点单位
            Data.append(""); // 核查点均值1
            Data.append(""); // 核查点均值2
            Data.append(""); // 核查点均值3
            Data.append(""); // 核查点均值4
            Data.append(""); // 核查点均值5
            Data.append(""); // 核查点均值6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[8] + row[9]); // 最大允许误差的绝对值
            Data.append(""); // 结论
            DCVoltageFiftyStableList.append(Data);
            resultMap.insert("稳定性", DCVoltageFiftyStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "直流电压(1MΩ)" && row[7].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "核查点单位"
                 << "核查点均值1"
                 << "核查点均值2"
                 << "核查点均值3"
                 << "核查点均值4"
                 << "核查点均值5"
                 << "核查点均值6"
                 << "平均值"
                 << "稳定性"
                 << "最大允许误差的绝对值"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 核查点
            Data.append(row[5]); // 核查点单位
            Data.append(""); // 核查点均值1
            Data.append(""); // 核查点均值2
            Data.append(""); // 核查点均值3
            Data.append(""); // 核查点均值4
            Data.append(""); // 核查点均值5
            Data.append(""); // 核查点均值6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[8] + row[9]); // 最大允许误差的绝对值
            Data.append(""); // 结论
            DCVoltageOneStableList.append(Data);
            resultMap.insert("稳定性", DCVoltageOneStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "输入阻抗" && row[7].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查点"
                 << "核查点单位"
                 << "核查点均值1"
                 << "核查点均值2"
                 << "核查点均值3"
                 << "核查点均值4"
                 << "核查点均值5"
                 << "核查点均值6"
                 << "平均值"
                 << "稳定性"
                 << "最大允许误差的绝对值"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 核查点
            Data.append(row[5]); // 核查点单位
            Data.append(""); // 核查点均值1
            Data.append(""); // 核查点均值2
            Data.append(""); // 核查点均值3
            Data.append(""); // 核查点均值4
            Data.append(""); // 核查点均值5
            Data.append(""); // 核查点均值6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[8] + row[9]); // 最大允许误差的绝对值
            Data.append(""); // 结论
            InputImpedanceStableList.append(Data);
            resultMap.insert("稳定性", InputImpedanceStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "稳幅正弦信号电压平坦度" && row[7].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "类型"
                 << "核查点"
                 << "核查点单位"
                 << "核查点均值1"
                 << "核查点均值2"
                 << "核查点均值3"
                 << "核查点均值4"
                 << "核查点均值5"
                 << "核查点均值6"
                 << "平均值"
                 << "稳定性"
                 << "最大允许误差的绝对值"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[10]); // 类型
            Data.append(row[4] + row[5]); // 核查点
            Data.append(row[5]); // 核查点单位
            Data.append(""); // 核查点均值1
            Data.append(""); // 核查点均值2
            Data.append(""); // 核查点均值3
            Data.append(""); // 核查点均值4
            Data.append(""); // 核查点均值5
            Data.append(""); // 核查点均值6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[8] + row[9]); // 最大允许误差的绝对值
            Data.append(""); // 结论
            VoltageFlatnessStableList.append(Data);
            resultMap.insert("稳定性", VoltageFlatnessStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "上升时间" && row[7].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "快沿"
                 << "快沿单位"
                 << "探头出厂编号"
                 << "探头设备名称"
                 << "探头规格型号"
                 << "测量值"
                 << "技术指标前符号"
                 << "技术指标"
                 << "技术指标单位"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 快沿
            Data.append(row[5]); // 快沿单位
            Data.append(row[12]); // 探头出厂编号
            Data.append(row[13]); // 探头设备名称
            Data.append(row[14]); // 探头规格型号
            Data.append(""); // 测量值
            Data.append(row[15]); // 技术指标前符号
            Data.append(row[16]); // 技术指标
            Data.append(row[17]); // 技术指标单位
            Data.append(""); // 结论
            RiseTimeStableList.append(Data);
            resultMap.insert("稳定性", RiseTimeStableList);
            result.insert(row[3], resultMap);
        }
    }
    return result;
}
//   【保存】核查项-示波器
bool BaseCommonApi::InsertStandardCheckItemDataOscilloscope(
    StandardCheckDataOscilloscope oscilloscope)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_equipment_standard_check_data_oscilloscope ("
        " id,data_id,check_type,params, check_point,unit,type_value, "
        "test_value_unit,convert_voltage_unit,"
        " test_value_one_date,test_value_one,convert_voltage_one,flatness_one,"
        "test_value_two_date, test_value_two,"
        " convert_voltage_two, flatness_two, test_value_three_date, "
        "test_value_three, convert_voltage_three,"
        " flatness_three, test_value_four_date, test_value_four, "
        "convert_voltage_four,flatness_four, test_value_five_date,"
        " test_value_five,convert_voltage_five, flatness_five, "
        "test_value_six_date,test_value_six,convert_voltage_six,"
        " flatness_six, "
        "test_value_seven_date,test_value_seven,convert_voltage_seven, "
        "flatness_seven, test_value_eight_date,"
        " test_value_eight,convert_voltage_eight, "
        "flatness_eight,test_value_nine_date,test_value_nine,convert_voltage_"
        "nine,"
        " flatness_nine, test_value_ten_date, "
        "test_value_ten,convert_voltage_ten,flatness_ten,average_value,"
        "manufacture_no,"
        " equipment_name, "
        "model,standard_deviation,maximum_error_absolute,maximum_error_absolute_"
        "unit,"
        " conclusion,technical_index_symbol,technical_index, "
        "technical_index_unit, reference_point,stability,"
        " qualified, create_time, update_time"
        " ) VALUES ( :id, :data_id, :check_type, :params, :check_point, :unit, "
        ":type_value, :test_value_unit, :convert_voltage_unit,"
        " :test_value_one_date, :test_value_one, "
        ":convert_voltage_one,:flatness_one,:test_value_two_date, "
        ":test_value_two,"
        ":convert_voltage_two,:flatness_two,:test_value_three_date, "
        ":test_value_three, :convert_voltage_three, :flatness_three,"
        " :test_value_four_date, :test_value_four, :convert_voltage_four, "
        ":flatness_four, :test_value_five_date, :test_value_five,"
        " :convert_voltage_five, :flatness_five, :test_value_six_date, "
        ":test_value_six, :convert_voltage_six, :flatness_six,"
        " :test_value_seven_date, :test_value_seven,:convert_voltage_seven, "
        ":flatness_seven, :test_value_eight_date,"
        " :test_value_eight, "
        ":convert_voltage_eight,:flatness_eight,:test_value_nine_date,:test_"
        "value_nine, :convert_voltage_nine,"
        " :flatness_nine, :test_value_ten_date, :test_value_ten, "
        ":convert_voltage_ten, :flatness_ten, :average_value,"
        " :manufacture_no, :equipment_name, :model,:standard_deviation, "
        ":maximum_error_absolute,"
        " :maximum_error_absolute_unit, :conclusion, :technical_index_symbol, "
        ":technical_index, :technical_index_unit,"
        " :reference_point,  :stability, :qualified, :create_time, :update_time"
        " );");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", oscilloscope.id);
    query.bindValue(":data_id", oscilloscope.data_id);
    query.bindValue(":check_type", oscilloscope.check_type);
    query.bindValue(":params", oscilloscope.params);
    query.bindValue(":check_point", oscilloscope.check_point);
    query.bindValue(":unit", oscilloscope.unit);
    query.bindValue(":type_value", oscilloscope.type_value);
    query.bindValue(":test_value_unit", oscilloscope.test_value_unit);
    query.bindValue(":convert_voltage_unit", oscilloscope.convert_voltage_unit);
    query.bindValue(":test_value_one_date", oscilloscope.test_value_one_date);
    query.bindValue(":test_value_one", oscilloscope.test_value_one);
    query.bindValue(":convert_voltage_one", oscilloscope.convert_voltage_one);
    query.bindValue(":flatness_one", oscilloscope.flatness_one);
    query.bindValue(":test_value_two_date", oscilloscope.test_value_two_date);
    query.bindValue(":test_value_two", oscilloscope.test_value_two);
    query.bindValue(":convert_voltage_two", oscilloscope.convert_voltage_two);
    query.bindValue(":flatness_two", oscilloscope.flatness_two);
    query.bindValue(":test_value_three_date", oscilloscope.test_value_three_date);
    query.bindValue(":test_value_three", oscilloscope.test_value_three);
    query.bindValue(":convert_voltage_three", oscilloscope.convert_voltage_three);
    query.bindValue(":flatness_three", oscilloscope.flatness_three);
    query.bindValue(":test_value_four_date", oscilloscope.test_value_four_date);
    query.bindValue(":test_value_four", oscilloscope.test_value_four);
    query.bindValue(":convert_voltage_four", oscilloscope.convert_voltage_four);
    query.bindValue(":flatness_four", oscilloscope.flatness_four);
    query.bindValue(":test_value_five_date", oscilloscope.test_value_five_date);
    query.bindValue(":test_value_five", oscilloscope.test_value_five);
    query.bindValue(":convert_voltage_five", oscilloscope.convert_voltage_five);
    query.bindValue(":flatness_five", oscilloscope.flatness_five);
    query.bindValue(":test_value_six_date", oscilloscope.test_value_six_date);
    query.bindValue(":test_value_six", oscilloscope.test_value_six);
    query.bindValue(":convert_voltage_six", oscilloscope.convert_voltage_six);
    query.bindValue(":flatness_six", oscilloscope.flatness_six);
    query.bindValue(":test_value_seven_date", oscilloscope.test_value_seven_date);
    query.bindValue(":test_value_seven", oscilloscope.test_value_seven);
    query.bindValue(":convert_voltage_seven", oscilloscope.convert_voltage_seven);
    query.bindValue(":flatness_seven", oscilloscope.flatness_seven);
    query.bindValue(":test_value_eight_date", oscilloscope.test_value_eight_date);
    query.bindValue(":test_value_eight", oscilloscope.test_value_eight);
    query.bindValue(":convert_voltage_eight", oscilloscope.convert_voltage_eight);
    query.bindValue(":flatness_eight", oscilloscope.flatness_eight);
    query.bindValue(":test_value_nine_date", oscilloscope.test_value_nine_date);
    query.bindValue(":test_value_nine", oscilloscope.test_value_nine);
    query.bindValue(":convert_voltage_nine", oscilloscope.convert_voltage_nine);
    query.bindValue(":flatness_nine", oscilloscope.flatness_nine);
    query.bindValue(":test_value_ten_date", oscilloscope.test_value_ten_date);
    query.bindValue(":test_value_ten", oscilloscope.test_value_ten);
    query.bindValue(":convert_voltage_ten", oscilloscope.convert_voltage_ten);
    query.bindValue(":flatness_ten", oscilloscope.flatness_ten);
    query.bindValue(":average_value", oscilloscope.average_value);
    query.bindValue(":manufacture_no", oscilloscope.manufacture_no);
    query.bindValue(":equipment_name", oscilloscope.equipment_name);
    query.bindValue(":model", oscilloscope.model);
    query.bindValue(":standard_deviation", oscilloscope.standard_deviation);
    query.bindValue(":maximum_error_absolute",
        oscilloscope.maximum_error_absolute);
    query.bindValue(":maximum_error_absolute_unit",
        oscilloscope.maximum_error_absolute_unit);
    query.bindValue(":conclusion", oscilloscope.conclusion);
    query.bindValue(":technical_index_symbol",
        oscilloscope.technical_index_symbol);
    query.bindValue(":technical_index", oscilloscope.technical_index);
    query.bindValue(":technical_index_unit", oscilloscope.technical_index_unit);
    query.bindValue(":reference_point", oscilloscope.reference_point);
    query.bindValue(":stability", oscilloscope.stability);
    query.bindValue(":qualified", oscilloscope.qualified);
    query.bindValue(":create_time", oscilloscope.create_time);
    query.bindValue(":update_time", oscilloscope.update_time);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << query.lastError().text();
    }
    return isSuccess;
}
// QString belongStandardEquipment(QString id){
//     QSqlQuery query;
//     QString result;
//     query.prepare("SELECT belong_standard_equipment FROM
//     zd_verification_program_manage WHERE id =:id ");
//     query.bindValue(":id",id);
//     if(query.exec()){
//         while (query.next()) {
//             result = query.value("belong_standard_equipment").toString();
//         }
//     }
//     return result;
// }

// QMap<QString,QMap<QString, QList<QStringList>>>
// SearchBelongStandardEquipmentById(QString id,QString
// equipment_id,QMap<QString,QMap<QString, QStringList>> *headList)
//{
//     QMap<QString,QMap<QString, QList<QStringList>>> DataMap;
//     QString StandardId = belongStandardEquipment(id);
//     switch (StandardId.toInt()) {
//     case 1:
//         DataMap =
//         BaseCommonApi::getStandardCheckItemDataCalibrator(equipment_id,headList);
//         break;
//     case 2:
//         DataMap =
//         BaseCommonApi::getStandardCheckItemDataDcPower(equipment_id,headList);
//         break;
//     case 4:
//         DataMap =
//         BaseCommonApi::getStandardCheckItemDataPistonGauge(equipment_id,headList);
//         break;
//     case 5:
//         DataMap =
//         BaseCommonApi::getStandardCheckItemDataResistanceThermometer(equipment_id,headList);
//         break;
//     }
// }

// 【查询】核查项-失真度
QMap<QString, QMap<QString, QList<QStringList>>>
BaseCommonApi::getStandardCheckItemDataDistortionFactor(
    QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList)
{
    QMap<QString, QMap<QString, QList<QStringList>>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();
    QSqlQuery query;
    query.prepare(
        "SELECT *	FROM biz_equipment_standard_check_item_data_distortion_factor  "
        " WHERE equipment_id = :equipment_id ORDER BY create_time ASC");
    query.bindValue(":equipment_id", equipment_id);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_category_id")
                              .toString()); // 核查项分类id(能够确定是哪个标准装置)
            result.append(query.value("equipment_id").toString()); // 配套设备id
            result.append(
                query.value("params").toString()); // 核查项目(直接存字典value)
            result.append(query.value("frequency").toString()); // 频率
            result.append(query.value("frequency_unit").toString()); // 频率单位  5
            result.append(query.value("nominal_value").toString()); // 标称值/%
            result.append(query.value("urel").toString()); // U(k=2)
            result.append(query.value("cycle_number").toString()); // 循环次数
            result.append(
                query.value("check_type").toString()); // 核查类型(字典value)
            result.append(query.value("remark").toString()); // 核查项备注
            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());
            resultData.append(result);
        }
    } else {
        qDebug() << query.lastError().text();
    }
    DataMapList = GetDataReturnItemListByCheckItemDataDistortionFactor(
        resultData, DataMapList, headList);
    return DataMapList;
}
QMap<QString, QMap<QString, QList<QStringList>>>
BaseCommonApi::GetDataReturnItemListByCheckItemDataDistortionFactor(
    QList<QStringList> ResultData,
    QMap<QString, QMap<QString, QList<QStringList>>> DataMapList,
    QMap<QString, QMap<QString, QStringList>>* headList)
{
    QMap<QString, QMap<QString, QList<QStringList>>> result;
    QMap<QString, QList<QStringList>> resultMap;
    QList<QStringList> DistortionList;
    QList<QStringList> DistortionStableList;
    QMap<QString, QStringList> headerMap;
    for (const QStringList& row : ResultData) {
        if (!row.isEmpty() && row[3] == "失真度" && row[9].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率"
                 << "频率单位"
                 << "标称值/%"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "测量值7"
                 << "测量值8"
                 << "测量值9"
                 << "测量值10"
                 << "平均值x"
                 << "重复性S(x)";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 频率
            Data.append(row[5]); // 频率点单位
            Data.append(row[6]); // 标称值/%
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 测量值7
            Data.append(""); // 测量值8
            Data.append(""); // 测量值9
            Data.append(""); // 测量值10
            Data.append(""); // 平均值x
            Data.append(""); // 重复性S(x)
            DistortionList.append(Data);
            resultMap.insert("重复性", DistortionList);
            result.insert(row[3], resultMap);
        }
        if (!row.isEmpty() && row[3] == "失真度" && row[9].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率"
                 << "频率单位"
                 << "标称值/%"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "平均值"
                 << "稳定性"
                 << "U(k=2)"
                 << "结论"
                 << "备注";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 频率
            Data.append(row[5]); // 频率点单位
            Data.append(row[6]); // 标称值/%
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[7]); // U(k=2)
            Data.append(""); // 结论
            Data.append(row[10]);
            DistortionStableList.append(Data);
            resultMap.insert("稳定性", DistortionStableList);
            result.insert(row[3], resultMap);
        }
    }
    return result;
}
//   【保存】核查项-失真度
bool BaseCommonApi::InsertStandardCheckItemDataDistortionFactor(
    StandardCheckDataDistortionFactor distortionFactor)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_equipment_standard_check_data_distortion_factor ("
        " id,data_id, check_type,params, frequency, "
        "frequency_unit,nominal_value,average_value, test_value_one_date,"
        " test_value_one, test_value_two_date, "
        "test_value_two,test_value_three_date,test_value_three,test_value_four_"
        "date,"
        " test_value_four, "
        "test_value_five_date,test_value_five,test_value_six_date, "
        "test_value_six, test_value_seven_date,"
        " test_value_seven,test_value_eight_date, test_value_eight, "
        "test_value_nine_date, test_value_nine,test_value_ten_date,"
        " test_value_ten,standard_deviation,urel,less_than, "
        "stability,qualified,create_time, update_time )"
        " VALUES ( :id, :data_id, :check_type, :params, :frequency, "
        ":frequency_unit, :nominal_value, :average_value, :test_value_one_date,"
        " :test_value_one, :test_value_two_date, "
        ":test_value_two,:test_value_three_date,:test_value_three, "
        ":test_value_four_date,"
        " :test_value_four, :test_value_five_date, :test_value_five, "
        ":test_value_six_date, :test_value_six, :test_value_seven_date,"
        " :test_value_seven, :test_value_eight_date, :test_value_eight, "
        ":test_value_nine_date, :test_value_nine, :test_value_ten_date,"
        " :test_value_ten, :standard_deviation, :urel, :less_than, :stability, "
        ":qualified, :create_time, :update_time );");
    query.bindValue(":id", distortionFactor.id);
    query.bindValue(":data_id", distortionFactor.data_id);
    query.bindValue(":check_type", distortionFactor.check_type);
    query.bindValue(":params", distortionFactor.params);
    query.bindValue(":frequency", distortionFactor.frequency);
    query.bindValue(":frequency_unit", distortionFactor.frequency_unit);
    query.bindValue(":nominal_value", distortionFactor.nominal_value);
    query.bindValue(":average_value", distortionFactor.average_value);
    query.bindValue(":test_value_one_date", distortionFactor.test_value_one_date);
    query.bindValue(":test_value_one", distortionFactor.test_value_one);
    query.bindValue(":test_value_two_date", distortionFactor.test_value_two_date);
    query.bindValue(":test_value_two", distortionFactor.test_value_two);
    query.bindValue(":test_value_three_date",
        distortionFactor.test_value_three_date);
    query.bindValue(":test_value_three", distortionFactor.test_value_three);
    query.bindValue(":test_value_four_date",
        distortionFactor.test_value_four_date);
    query.bindValue(":test_value_four", distortionFactor.test_value_four);
    query.bindValue(":test_value_five_date",
        distortionFactor.test_value_five_date);
    query.bindValue(":test_value_five", distortionFactor.test_value_five);
    query.bindValue(":test_value_six_date", distortionFactor.test_value_six_date);
    query.bindValue(":test_value_six", distortionFactor.test_value_six);
    query.bindValue(":test_value_seven_date",
        distortionFactor.test_value_seven_date);
    query.bindValue(":test_value_seven", distortionFactor.test_value_seven);
    query.bindValue(":test_value_eight_date",
        distortionFactor.test_value_eight_date);
    query.bindValue(":test_value_eight", distortionFactor.test_value_eight);
    query.bindValue(":test_value_nine_date",
        distortionFactor.test_value_nine_date);
    query.bindValue(":test_value_nine", distortionFactor.test_value_nine);
    query.bindValue(":test_value_ten_date", distortionFactor.test_value_ten_date);
    query.bindValue(":test_value_ten", distortionFactor.test_value_ten);
    query.bindValue(":standard_deviation", distortionFactor.standard_deviation);
    query.bindValue(":urel", distortionFactor.urel);
    query.bindValue(":less_than", distortionFactor.less_than);
    query.bindValue(":stability", distortionFactor.stability);
    query.bindValue(":qualified", distortionFactor.qualified);
    query.bindValue(":create_time", distortionFactor.create_time);
    query.bindValue(":update_time", distortionFactor.update_time);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << query.lastError().text();
    }
    return isSuccess;
}

// 稳定性-核查基本信息表-查询id：参数
QList<DataCalibratorStability> BaseCommonApi::GetEquipmentStandardCheckInfoByids(
    QString standard_equipment_id, QString equipment_id,
    QString item_category_id, int num)
{
    QSqlQuery query;
    QStringList resultList;
    QList<DataCalibratorStability> resultMap;
    QList<DataCalibratorStability> result;
    query.prepare(
        "SELECT * FROM biz_equipment_standard_check_info WHERE stability_examine "
        "= 1 AND "
        "standard_equipment_id = :standard_equipment_id AND equipment_id = "
        ":equipment_id AND "
        "item_category_id = :item_category_id  AND conclusion = '合格' ORDER BY "
        "update_time ASC LIMIT 5");
    query.bindValue(":standard_equipment_id", standard_equipment_id);
    query.bindValue(":equipment_id", equipment_id);
    query.bindValue(":item_category_id", item_category_id);
    if (query.exec()) {
        while (query.next()) {
            QString data_id = query.value("id").toString();
            QDate check_date = query.value("check_date").toDate();
            switch (num) {
            case 1:
                resultMap = GetStabilityHeaderAndDataByDataId(data_id);
                break;
            case 2:
                resultMap = GetStabilityHeaderAndDataDcPowerByDataId(data_id);
                break;
            case 5:
                resultMap = GetStabilityHeaderAndDataThermometerByDataId(data_id);
                break;
            case 9:
                resultMap = GetStabilityHeaderAndDataSpectrumAnalyzerByDataId(data_id);
            case 10:
                resultMap = GetStabilityHeaderAndDataSignalGeneratorByDataId(data_id);
                break;
            }
            for (DataCalibratorStability stab : resultMap) {
                if (stab.date == check_date)
                    result.append(stab);
            }
            //            resultList.append(data_id);
        }
    }
    //    if (result.size() < 5) {
    //        for (int i = 0; i < 5 - result.size(); i++) {
    //            DataCalibratorStability stab;
    //            result.append(stab);
    //        }
    //    }
    return result;
}
// 获取多功能稳定性的头和数据
// QList<DataCalibratorStability> BaseCommonApi::GetStabilityHeaderAndDataByDataId(QString data_id)
//{
//    QSqlQuery query;
//    QList<DataCalibratorStability> result;
//    query.prepare(
//        "WITH RankedData AS ( "
//        " SELECT *,  ROW_NUMBER() OVER (PARTITION BY params ORDER BY create_time ASC) as rn "
//        " FROM biz_equipment_standard_check_data_calibrator WHERE data_id = :data_id  ) "
//        " SELECT *  FROM RankedData  WHERE rn <= 5;	");
//    query.bindValue(":data_id", data_id);
//    if (query.exec()) {
//        while (query.next()) {
//            DataCalibratorStability sta;
//            sta.date = query.value("test_value_six_date").toDate();
//            sta.value = query.value("test_value_six").toString();
//            sta.params = query.value("params").toString();
//            result.append(sta);
//        }
//    }
//    return result;
//}

QList<DataCalibratorStability> BaseCommonApi::GetStabilityHeaderAndDataByDataId(QString data_id)
{
    QSqlQuery query;
    QList<DataCalibratorStability> result;

    // 获取所有不同的params
    query.prepare("SELECT DISTINCT params FROM biz_equipment_standard_check_data_calibrator WHERE data_id = :data_id;");
    query.bindValue(":data_id", data_id);
    query.exec();
    QSet<QString> distinctParams;
    while (query.next()) {
        distinctParams.insert(query.value(0).toString());
    }

    foreach (const QString& param, distinctParams) {
        // 对于每个params，获取最多5条记录，不足的用空对象填充
        query.prepare(
            "WITH RankedData AS ( "
            " SELECT *, ROW_NUMBER() OVER (PARTITION BY params ORDER BY create_time ASC) as rn "
            " FROM biz_equipment_standard_check_data_calibrator "
            " WHERE data_id = :data_id AND params = :param ) "
            " SELECT params, test_value_six_date, test_value_six "
            " FROM RankedData "
            " WHERE rn <= 5;");
        query.bindValue(":data_id", data_id);
        query.bindValue(":param", param);
        if (query.exec()) {
            int count = 0;
            while (query.next() && count < 5) {
                DataCalibratorStability sta;
                sta.date = query.value("test_value_six_date").toDate();
                sta.value = query.value("test_value_six").toString();
                sta.params = param;
                result.append(sta);
                ++count;
            }

            // 如果当前params的实际记录数少于5，则添加空对象填充
            for (int i = count; i < 5; ++i) {
                DataCalibratorStability emptySta;
                emptySta.params = ""; // 设置params，其他字段保持默认或空值
                result.append(emptySta);
            }
        }
    }

    return result;
}

// 获取二等铂电阻的头和数据
QList<DataCalibratorStability> BaseCommonApi::GetStabilityHeaderAndDataThermometerByDataId(QString data_id)
{
    QSqlQuery query;
    QList<DataCalibratorStability> result;
    query.prepare(
        "SELECT * FROM "
        "biz_equipment_standard_check_data_resistance_thermometer WHERE data_id = :data_id ORDER BY create_time ASC LIMIT 5");
    query.bindValue(":data_id", data_id);
    if (query.exec()) {
        while (query.next()) {
            DataCalibratorStability sta;
            sta.date = query.value("test_value_six_date").toDate();
            sta.value = query.value("test_value_six").toString();
            sta.params = query.value("params").toString();
            result.append(sta);
        }
    }
    return result;
}
// 获取直流稳压稳定性的头和数据
QList<DataCalibratorStability> BaseCommonApi::GetStabilityHeaderAndDataDcPowerByDataId(QString data_id)
{
    QSqlQuery query;
    QList<DataCalibratorStability> result;
    query.prepare(
        "WITH RankedData AS ( "
        " SELECT *,  ROW_NUMBER() OVER (PARTITION BY params ORDER BY create_time ASC) as rn "
        " FROM biz_equipment_standard_check_data_dc_power WHERE data_id = :data_id  ) "
        " SELECT *  FROM RankedData  WHERE rn <= 5;	");
    query.bindValue(":data_id", data_id);
    if (query.exec()) {
        while (query.next()) {
            DataCalibratorStability sta;
            sta.date = query.value("test_value_six_date").toDate();
            sta.value = query.value("test_value_six").toString();
            sta.params = query.value("params").toString();
            result.append(sta);
        }
    }
    return result;
}
// 获取信号发生器稳定性的头和数据
//  信号发生器-稳定性
QList<DataCalibratorStability> BaseCommonApi::GetStabilityHeaderAndDataSignalGeneratorByDataId(QString data_id)
{
    QSqlQuery query;
    QList<DataCalibratorStability> result;
    query.prepare(
        "WITH RankedData AS ( "
        " SELECT *,  ROW_NUMBER() OVER (PARTITION BY params ORDER BY create_time ASC) as rn "
        " FROM biz_equipment_standard_check_data_signal_generator WHERE data_id = :data_id  ) "
        " SELECT *  FROM RankedData  WHERE rn <= 5;	");
    query.bindValue(":data_id", data_id);
    if (query.exec()) {
        while (query.next()) {
            DataCalibratorStability sta;
            sta.date = query.value("test_value_six_date").toDate();
            sta.value = query.value("test_value_six").toString();
            sta.params = query.value("params").toString();
            result.append(sta);
        }
    }
    return result;
}
// 频谱分析仪-稳定性
QList<DataCalibratorStability> BaseCommonApi::GetStabilityHeaderAndDataSpectrumAnalyzerByDataId(QString data_id)
{
    QSqlQuery query;
    QList<DataCalibratorStability> result;
    query.prepare(
        "WITH RankedData AS ( "
        " SELECT *,  ROW_NUMBER() OVER (PARTITION BY params ORDER BY create_time ASC) as rn "
        " FROM biz_equipment_standard_check_data_spectrum_analyzer WHERE data_id = :data_id  ) "
        " SELECT *  FROM RankedData  WHERE rn <= 5;	");
    query.bindValue(":data_id", data_id);
    if (query.exec()) {
        while (query.next()) {
            DataCalibratorStability sta;
            sta.date = query.value("test_value_six_date").toDate();
            sta.value = query.value("test_value_six").toString();
            sta.params = query.value("params").toString();
            result.append(sta);
        }
    }
    return result;
}

// 【查询】核查项-微波衰减标准装置
QMap<QString, QMap<QString, QList<QStringList>>>
BaseCommonApi::getStandardCheckItemDataMicrowaveAttenuation(
    QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList)
{
    QMap<QString, QMap<QString, QList<QStringList>>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();
    QSqlQuery query;
    query.prepare(
        "SELECT *	FROM "
        "biz_equipment_standard_check_item_data_microwave_attenuation  "
        " WHERE equipment_id = :equipment_id ORDER BY create_time ASC");
    query.bindValue(":equipment_id", equipment_id);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_category_id")
                              .toString()); // 核查项分类id(能够确定是哪个标准装置)
            result.append(query.value("equipment_id").toString()); // 配套设备id
            result.append(
                query.value("params").toString()); // 核查项目(直接存字典value)
            result.append(query.value("frequency").toString()); // 频率
            result.append(query.value("frequency_unit").toString()); // 频率单位
            result.append(query.value("gear_type").toString()); // 档位类型
            result.append(query.value("attenuation_amount").toString()); // 衰减量 7
            result.append(
                query.value("attenuation_amount_unit").toString()); // 衰减量单位
            result.append(query.value("composition").toString()); // 组合方式
            result.append(query.value("amplitude").toString()); // 幅度
            result.append(query.value("amplitude_unit").toString()); // 幅度单位
            result.append(query.value("urel").toString()); // U(k=2)
            result.append(query.value("cycle_number").toString()); // 循环次数
            result.append(
                query.value("check_type").toString()); // 核查类型(字典value)
            result.append(query.value("remark").toString()); // 核查项备注
            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());
            resultData.append(result);
        }
    } else {
        qDebug() << query.lastError().text();
    }
    DataMapList = GetDataReturnItemListByCheckItemDataMicrowaveAttenuation(
        resultData, DataMapList, headList);
    return DataMapList;
}
QMap<QString, QMap<QString, QList<QStringList>>>
BaseCommonApi::GetDataReturnItemListByCheckItemDataMicrowaveAttenuation(
    QList<QStringList> ResultData,
    QMap<QString, QMap<QString, QList<QStringList>>> DataMapList,
    QMap<QString, QMap<QString, QStringList>>* headList)
{
    QMap<QString, QMap<QString, QList<QStringList>>> result;
    QMap<QString, QList<QStringList>> resultMap;
    QList<QStringList> MicrowaveAttenuationList;
    QList<QStringList> MicrowaveAttenuationStableList;
    QMap<QString, QStringList> headerMap;
    for (const QStringList& row : ResultData) {
        if (!row.isEmpty() && row[3] == "微波衰减" && row[14].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "档位类型"
                 << "频率点"
                 << "频率点单位"
                 << "衰减量"
                 << "衰减量单位"
                 << "组合方式"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "测量值7"
                 << "测量值8"
                 << "测量值9"
                 << "测量值10"
                 << "平均值x"
                 << "重复性S(x)";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[6]); // 档位类型
            Data.append(row[4] + row[5]); // 频率点
            Data.append(row[5]); // 频率点单位
            Data.append(row[7]); // 衰减量
            Data.append(row[8]); // 衰减量单位
            Data.append(row[9]); // 组合方式
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 测量值7
            Data.append(""); // 测量值8
            Data.append(""); // 测量值9
            Data.append(""); // 测量值10
            Data.append(""); // 平均值x
            Data.append(""); // 重复性S(x)
            MicrowaveAttenuationList.append(Data);
            resultMap.insert("重复性", MicrowaveAttenuationList);
            result.insert(row[3], resultMap);
        }
        if (!row.isEmpty() && row[3] == "微波衰减" && row[14].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "档位类型"
                 << "频率点"
                 << "频率点单位"
                 << "衰减量"
                 << "衰减量单位"
                 << "组合方式"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "平均值"
                 << "稳定性"
                 << "U(k=2)"
                 << "结论"
                 << "备注";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[6]); // 档位类型
            Data.append(row[4] + row[5]); // 频率点
            Data.append(row[5]); // 频率点单位
            Data.append(row[7]); // 衰减量
            Data.append(row[8]); // 衰减量单位
            Data.append(row[9]); // 组合方式
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 测量值7
            Data.append(""); // 测量值8
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[12]); // U(k=2)
            Data.append(""); // 结论
            Data.append(row[15]); // 备注
            MicrowaveAttenuationStableList.append(Data);
            resultMap.insert("稳定性", MicrowaveAttenuationStableList);
            result.insert(row[3], resultMap);
        }
    }
    return result;
}
//   【保存】核查项-微波衰减
bool BaseCommonApi::InsertStandardCheckItemDataMicrowaveAttenuation(
    StandardCheckItemDataMicrowaveAttenuation microwaveAttenuation)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO biz_equipment_standard_check_data_microwave_attenuation ("
        " id,data_id,check_type,params,frequency,frequency_unit, "
        "attenuation_amount, attenuation_amount_unit, composition,gear_type, "
        "average_value,test_value_one_date,"
        " test_value_one, test_value_two_date, "
        "test_value_two,test_value_three_date, test_value_three, "
        "test_value_four_date,test_value_four, test_value_five_date,"
        " test_value_five, test_value_six_date, "
        "test_value_six,test_value_seven_date,   test_value_seven, "
        "test_value_eight_date,test_value_eight,test_value_nine_date,"
        " test_value_nine, test_value_ten_date, test_value_ten, "
        "standard_deviation,urel, less_than, stability, qualified,create_time, "
        "update_time ) VALUES ("
        " :id, :data_id, :check_type,  :params, :frequency, :frequency_unit, "
        ":attenuation_amount, :attenuation_amount_unit,:composition, :gear_type, "
        ":average_value,"
        " :test_value_one_date, :test_value_one,:test_value_two_date, "
        ":test_value_two,:test_value_three_date,  :test_value_three, "
        ":test_value_four_date, :test_value_four,"
        " :test_value_five_date,:test_value_five, :test_value_six_date, "
        ":test_value_six, :test_value_seven_date, :test_value_seven, "
        ":test_value_eight_date, :test_value_eight,"
        " :test_value_nine_date,:test_value_nine, "
        ":test_value_ten_date,:test_value_ten, :standard_deviation, "
        ":urel,:less_than, :stability,:qualified, :create_time, :update_time );");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", microwaveAttenuation.id);
    query.bindValue(":data_id", microwaveAttenuation.data_id);
    query.bindValue(":check_type", microwaveAttenuation.check_type);
    query.bindValue(":params", microwaveAttenuation.params);
    query.bindValue(":frequency", microwaveAttenuation.frequency);
    query.bindValue(":frequency_unit", microwaveAttenuation.frequency_unit);
    query.bindValue(":attenuation_amount",
        microwaveAttenuation.attenuation_amount);
    query.bindValue(":attenuation_amount_unit",
        microwaveAttenuation.attenuation_amount_unit);
    query.bindValue(":composition", microwaveAttenuation.composition);
    query.bindValue(":gear_type", microwaveAttenuation.gear_type);
    query.bindValue(":average_value", microwaveAttenuation.average_value);
    query.bindValue(":test_value_one_date",
        microwaveAttenuation.test_value_one_date);
    query.bindValue(":test_value_one", microwaveAttenuation.test_value_one);
    query.bindValue(":test_value_two_date",
        microwaveAttenuation.test_value_two_date);
    query.bindValue(":test_value_two", microwaveAttenuation.test_value_two);
    query.bindValue(":test_value_three_date",
        microwaveAttenuation.test_value_three_date);
    query.bindValue(":test_value_three", microwaveAttenuation.test_value_three);
    query.bindValue(":test_value_four_date",
        microwaveAttenuation.test_value_four_date);
    query.bindValue(":test_value_four", microwaveAttenuation.test_value_four);
    query.bindValue(":test_value_five_date",
        microwaveAttenuation.test_value_five_date);
    query.bindValue(":test_value_five", microwaveAttenuation.test_value_five);
    query.bindValue(":test_value_six_date",
        microwaveAttenuation.test_value_six_date);
    query.bindValue(":test_value_six", microwaveAttenuation.test_value_six);
    query.bindValue(":test_value_seven_date",
        microwaveAttenuation.test_value_seven_date);
    query.bindValue(":test_value_seven", microwaveAttenuation.test_value_seven);
    query.bindValue(":test_value_eight_date",
        microwaveAttenuation.test_value_eight_date);
    query.bindValue(":test_value_eight", microwaveAttenuation.test_value_eight);
    query.bindValue(":test_value_nine_date",
        microwaveAttenuation.test_value_nine_date);
    query.bindValue(":test_value_nine", microwaveAttenuation.test_value_nine);
    query.bindValue(":test_value_ten_date",
        microwaveAttenuation.test_value_ten_date);
    query.bindValue(":test_value_ten", microwaveAttenuation.test_value_ten);
    query.bindValue(":standard_deviation",
        microwaveAttenuation.standard_deviation);
    query.bindValue(":urel", microwaveAttenuation.urel);
    query.bindValue(":less_than", microwaveAttenuation.less_than);
    query.bindValue(":stability", microwaveAttenuation.stability);
    query.bindValue(":qualified", microwaveAttenuation.qualified);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << query.lastError().text();
    }
    return isSuccess;
}
// 【查询】核查项-低频信号源检定装置
QMap<QString, QMap<QString, QList<QStringList>>>
BaseCommonApi::getStandardCheckItemDataLowFrequencySignal(
    QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList)
{
    QMap<QString, QMap<QString, QList<QStringList>>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();
    QSqlQuery query;
    query.prepare(
        "SELECT *	FROM "
        "biz_equipment_standard_check_item_data_low_frequency_signal  "
        " WHERE equipment_id = :equipment_id ORDER BY create_time ASC");
    query.bindValue(":equipment_id", equipment_id);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_category_id").toString()); // 核查项分类id(能够确定是哪个标准装置)
            result.append(query.value("equipment_id").toString()); // 配套设备id
            result.append(query.value("params").toString()); // 核查项目(直接存字典value)
            result.append(query.value("nominal_value").toString()); // 标称值
            result.append(query.value("unit").toString()); // 标称值单位   5
            result.append(query.value("impedance").toString()); // 阻抗
            result.append(query.value("impedance_unit").toString()); // 阻抗单位   7
            result.append(query.value("amplitude").toString()); // 幅度
            result.append(query.value("amplitude_unit").toString()); // 幅度单位
            result.append(query.value("frequency").toString()); // 频率
            result.append(query.value("frequency_unit").toString()); // 频率单位
            result.append(query.value("voltage").toString()); // 电压
            result.append(query.value("voltage_unit").toString()); // 电压单位
            result.append(query.value("gate_time").toString()); // 闸门时间
            result.append(query.value("gate_time_unit").toString()); // 闸门时间单位
            result.append(query.value("valid_digit").toString()); // 有效位数
            result.append(query.value("cycle_number").toString()); // 循环次数
            result.append(query.value("check_type").toString()); // 核查类型(字典value)  18
            result.append(query.value("urel").toString()); // U(k=2)
            result.append(query.value("reference_point").toString()); // 是否为参考点(1/0)
            result.append(query.value("remark").toString()); // 核查项备注
            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());
            resultData.append(result);
        }
    } else {
        qDebug() << query.lastError().text();
    }
    DataMapList = GetDataReturnItemListByCheckItemDataLowFrequencySignal(
        resultData, DataMapList, headList);
    return DataMapList;
}
QMap<QString, QMap<QString, QList<QStringList>>>
BaseCommonApi::GetDataReturnItemListByCheckItemDataLowFrequencySignal(
    QList<QStringList> ResultData,
    QMap<QString, QMap<QString, QList<QStringList>>> DataMapList,
    QMap<QString, QMap<QString, QStringList>>* headList)
{
    QMap<QString, QMap<QString, QList<QStringList>>> result;
    QMap<QString, QList<QStringList>> resultMap;
    QList<QStringList> FrequencyList;
    QList<QStringList> ACVList;
    QList<QStringList> DCVList;
    QList<QStringList> RiseAndFallTimeList;
    QList<QStringList> FlatnessList;
    QList<QStringList> DistortionList;
    QList<QStringList> FrequencyStableList;
    QList<QStringList> ACVStableList;
    QList<QStringList> DCVStableList;
    QList<QStringList> RiseAndFallTimeStableList;
    QList<QStringList> FlatnessStableList;
    QList<QStringList> DistortionStableList;
    QMap<QString, QStringList> headerMap;
    for (const QStringList& row : ResultData) {
        if (!row.isEmpty() && row[3] == "频率" && row[18].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "标称值"
                 << "标称值单位"
                 << "幅度"
                 << "幅度单位"
                 << "闸门时间"
                 << "闸门时间单位"
                 << "有效位数"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "测量值7"
                 << "测量值8"
                 << "测量值9"
                 << "测量值10"
                 << "平均值x"
                 << "重复性S(x)/x";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 标称值
            Data.append(row[5]); // 标称值单位
            Data.append(row[8] + row[9]); // 幅度
            Data.append(row[9]); // 幅度单位
            Data.append(row[14] + row[15]); // 闸门时间
            Data.append(row[15]); // 闸门时间单位
            Data.append(row[16]); // 有效位数
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 测量值7
            Data.append(""); // 测量值8
            Data.append(""); // 测量值9
            Data.append(""); // 测量值10
            Data.append(""); // 平均值x
            Data.append(""); // 重复性S(x)
            FrequencyList.append(Data);
            resultMap.insert("重复性", FrequencyList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "交流电压" && row[18].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率"
                 << "频率单位"
                 << "标称值"
                 << "标称值单位"
                 << "阻抗"
                 << "阻抗单位"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "测量值7"
                 << "测量值8"
                 << "测量值9"
                 << "测量值10"
                 << "平均值x"
                 << "重复性S(x)/x";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[10]); // 频率
            Data.append(row[11]); // 频率单位
            Data.append(row[4] + row[5]); // 标称值
            Data.append(row[5]); // 标称值单位
            Data.append(row[6] + row[7]); // 阻抗
            Data.append(row[7]); // 阻抗单位
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 测量值7
            Data.append(""); // 测量值8
            Data.append(""); // 测量值9
            Data.append(""); // 测量值10
            Data.append(""); // 平均值x
            Data.append(""); // 重复性S(x)
            ACVList.append(Data);
            resultMap.insert("重复性", ACVList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "直流电压" && row[18].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "标称值"
                 << "标称值单位"
                 << "阻抗"
                 << "阻抗单位"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "测量值7"
                 << "测量值8"
                 << "测量值9"
                 << "测量值10"
                 << "平均值x"
                 << "重复性S(x)/x";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[10] + row[11]); // 频率
            Data.append(row[11]); // 频率单位
            Data.append(row[4] + row[5]); // 标称值
            Data.append(row[5]); // 标称值单位
            Data.append(row[6] + row[7]); // 阻抗
            Data.append(row[7]); // 阻抗单位
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 测量值7
            Data.append(""); // 测量值8
            Data.append(""); // 测量值9
            Data.append(""); // 测量值10
            Data.append(""); // 平均值x
            Data.append(""); // 重复性S(x)
            DCVList.append(Data);
            resultMap.insert("重复性", DCVList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && (row[3] == "上升时间" || row[3] == "下降时间") && row[18].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "单位"
                 << "频率"
                 << "频率单位"
                 << "幅度"
                 << "幅度单位"
                 << "阻抗"
                 << "阻抗单位"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "测量值7"
                 << "测量值8"
                 << "测量值9"
                 << "测量值10"
                 << "平均值x"
                 << "重复性S(x)/x";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append("ns"); // 单位
            Data.append(row[10]); // 频率
            Data.append(row[11]); // 频率单位
            Data.append(row[8] + row[9]); // 幅度
            Data.append(row[9]); // 幅度单位
            Data.append(row[6] + row[7]); // 阻抗
            Data.append(row[7]); // 阻抗单位
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 测量值7
            Data.append(""); // 测量值8
            Data.append(""); // 测量值9
            Data.append(""); // 测量值10
            Data.append(""); // 平均值x
            Data.append(""); // 重复性S(x)
            RiseAndFallTimeList.append(Data);
            resultMap.insert("重复性", RiseAndFallTimeList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "平坦度" && row[18].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "幅度"
                 << "幅度单位"
                 << "标称值"
                 << "标称值单位"
                 << "是否为参考点"
                 << "测量值1"
                 << "平坦度参考点1"
                 << "测量值2"
                 << "平坦度参考点2"
                 << "测量值3"
                 << "平坦度参考点3"
                 << "测量值4"
                 << "平坦度参考点4"
                 << "测量值5"
                 << "平坦度参考点5"
                 << "测量值6"
                 << "平坦度参考点6"
                 << "测量值7"
                 << "平坦度参考点7"
                 << "测量值8"
                 << "平坦度参考点8"
                 << "测量值9"
                 << "平坦度参考点9"
                 << "平均值测量值"
                 << "平均值平坦度"
                 << "重复性S(x)/x";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[8] + row[9]); // 幅度
            Data.append(row[9]); // 幅度单位
            Data.append(row[4] + row[5]); // 标称值
            Data.append(row[5]); // 标称值单位
            Data.append(row[20]); // 是否为参考点
            Data.append(""); // 测量值1
            Data.append(""); // 平坦度参考点1
            Data.append(""); // 测量值2
            Data.append(""); // 平坦度参考点2
            Data.append(""); // 测量值3
            Data.append(""); // 平坦度参考点3
            Data.append(""); // 测量值4
            Data.append(""); // 平坦度参考点4
            Data.append(""); // 测量值5
            Data.append(""); // 平坦度参考点5
            Data.append(""); // 测量值6
            Data.append(""); // 平坦度参考点6
            Data.append(""); // 测量值7
            Data.append(""); // 平坦度参考点7
            Data.append(""); // 测量值8
            Data.append(""); // 平坦度参考点8
            Data.append(""); // 测量值9
            Data.append(""); // 平坦度参考点9
            Data.append(""); // 平均值测量值
            Data.append(""); // 平均值平坦度
            Data.append(""); // 重复性S(x)
            FlatnessList.append(Data);
            resultMap.insert("重复性", FlatnessList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "失真" && row[18].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率"
                 << "频率单位"
                 << "电压"
                 << "电压单位"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "测量值7"
                 << "测量值8"
                 << "测量值9"
                 << "测量值10"
                 << "平均值x"
                 << "重复性S(x)/x";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[10] + row[11]); // 频率
            Data.append(row[11]); // 频率单位
            Data.append(row[12] + row[13]); // 电压
            Data.append(row[13]); // 电压单位
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 测量值7
            Data.append(""); // 测量值8
            Data.append(""); // 测量值9
            Data.append(""); // 测量值10
            Data.append(""); // 平均值x
            Data.append(""); // 重复性S(x)
            DistortionList.append(Data);
            resultMap.insert("重复性", DistortionList);
            result.insert(row[3], resultMap);
        }
        if (!row.isEmpty() && row[3] == "频率" && row[18].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "标称值"
                 << "标称值单位"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "平均值"
                 << "稳定性"
                 << "U(k=2)"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 标称值
            Data.append(row[5]); // 标称值单位
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 测量值7
            Data.append(""); // 测量值8
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[19]); // U(k=2)
            Data.append(""); // 结论
            FrequencyStableList.append(Data);
            resultMap.insert("稳定性", FrequencyStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "交流电压" && row[18].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率"
                 << "频率单位"
                 << "标称值"
                 << "标称值单位"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "平均值"
                 << "稳定性"
                 << "U(k=2)"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[10]); // 频率
            Data.append(row[11]); // 频率单位
            Data.append(row[4] + row[5]); // 标称值
            Data.append(row[5]); // 标称值单位
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[19]); // U(k=2)
            Data.append(""); // 结论
            ACVStableList.append(Data);
            resultMap.insert("稳定性", ACVStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "直流电压" && row[18].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "标称值"
                 << "标称值单位"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "平均值"
                 << "稳定性"
                 << "U(k=2)"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 标称值
            Data.append(row[5]); // 标称值单位
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[19]); // U(k=2)
            Data.append(""); // 结论
            DCVStableList.append(Data);
            resultMap.insert("稳定性", DCVStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && (row[3] == "上升时间" || row[3] == "下降时间") && row[18].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "单位"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "平均值"
                 << "稳定性"
                 << "U(k=2)"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append("ns"); // 单位
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[19]); // U(k=2)
            Data.append(""); // 结论
            RiseAndFallTimeStableList.append(Data);
            resultMap.insert("稳定性", RiseAndFallTimeStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "平坦度" && row[18].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "幅度"
                 << "幅度单位"
                 << "标称值"
                 << "标称值单位"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "平均值"
                 << "稳定性"
                 << "U(k=2)"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[8] + row[9]); // 幅度
            Data.append(row[9]); // 幅度单位
            Data.append(row[4] + row[5]); // 标称值
            Data.append(row[5]); // 标称值单位
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[19]); // U(k=2)
            Data.append(""); // 结论
            FlatnessStableList.append(Data);
            resultMap.insert("稳定性", FlatnessStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "失真" && row[18].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率"
                 << "频率单位"
                 << "电压"
                 << "电压单位"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "平均值"
                 << "稳定性"
                 << "U(k=2)"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[10] + row[11]); // 频率
            Data.append(row[11]); // 频率单位
            Data.append(row[12] + row[13]); // 电压
            Data.append(row[13]); // 电压单位
            Data.append(""); // 测量值1
            Data.append(""); // 测量值2
            Data.append(""); // 测量值3
            Data.append(""); // 测量值4
            Data.append(""); // 测量值5
            Data.append(""); // 测量值6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[19]); // U(k=2)
            Data.append(""); // 结论
            DistortionStableList.append(Data);
            resultMap.insert("稳定性", DistortionStableList);
            result.insert(row[3], resultMap);
        }
    }
    return result;
}
//   【保存】核查项-低频信号源检定装置
bool BaseCommonApi::InsertStandardCheckItemDataLowFrequencySignal(StandardCheckDataLowFrequencySignal lowFrequencySignal)
{
    QSqlQuery query;
    query.prepare("INSERT INTO biz_equipment_standard_check_data_low_frequency_signal ("
                  " id,data_id, check_type, params, nominal_value, unit, frequency, frequency_unit,"
                  " amplitude, amplitude_unit, voltage, voltage_unit, average_value, average_value_two,"
                  " test_value_one_date, test_value_one, flatness_value_one, test_value_two_date,"
                  " test_value_two, flatness_value_two, test_value_three_date, test_value_three,"
                  " flatness_value_three, test_value_four_date, test_value_four, flatness_value_four,"
                  " test_value_five_date, test_value_five, flatness_value_five, test_value_six_date,"
                  " test_value_six, flatness_value_six, test_value_seven_date, test_value_seven,"
                  " flatness_value_seven, test_value_eight_date, test_value_eight, flatness_value_eight,"
                  " test_value_nine_date, test_value_nine, flatness_value_nine, test_value_ten_date,"
                  " test_value_ten, flatness_value_ten, standard_deviation, urel, less_than, stability,"
                  " qualified, create_time, update_time)"
                  " VALUES("
                  " : id, : data_id, : check_type, : params, : nominal_value, : unit, : frequency, : frequency_unit, : amplitude, : amplitude_unit,"
                  " : voltage, : voltage_unit, : average_value, : average_value_two, : test_value_one_date, : test_value_one, : flatness_value_one,"
                  ": test_value_two_date, : test_value_two, : flatness_value_two, : test_value_three_date, : test_value_three, : flatness_value_three,"
                  " : test_value_four_date, : test_value_four, : flatness_value_four, : test_value_five_date, : test_value_five, : flatness_value_five, "
                  ": test_value_six_date, : test_value_six, : flatness_value_six, : test_value_seven_date, : test_value_seven, : flatness_value_seven,"
                  " : test_value_eight_date, : test_value_eight, : flatness_value_eight, : test_value_nine_date, : test_value_nine, : flatness_value_nine, "
                  ": test_value_ten_date, : test_value_ten, : flatness_value_ten, : standard_deviation, : urel, : less_than, : stability, : qualified, "
                  ": create_time, : update_time); ");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", lowFrequencySignal.id);
    query.bindValue(":data_id", lowFrequencySignal.data_id);
    query.bindValue(":check_type", lowFrequencySignal.check_type);
    query.bindValue(":params", lowFrequencySignal.params);
    query.bindValue(":nominal_value", lowFrequencySignal.nominal_value);
    query.bindValue(":unit", lowFrequencySignal.unit);
    query.bindValue(":frequency", lowFrequencySignal.frequency);
    query.bindValue(":frequency_unit", lowFrequencySignal.frequency_unit);
    query.bindValue(":amplitude", lowFrequencySignal.amplitude);
    query.bindValue(":amplitude_unit", lowFrequencySignal.amplitude_unit);
    query.bindValue(":voltage", lowFrequencySignal.voltage);
    query.bindValue(":voltage_unit", lowFrequencySignal.voltage_unit);
    query.bindValue(":average_value", lowFrequencySignal.average_value);
    query.bindValue(":average_value_two", lowFrequencySignal.average_value_two);
    query.bindValue(":test_value_one_date", lowFrequencySignal.test_value_one_date);
    query.bindValue(":test_value_one", lowFrequencySignal.test_value_one);
    query.bindValue(":flatness_value_one", lowFrequencySignal.flatness_value_one);
    query.bindValue(":test_value_two_date", lowFrequencySignal.test_value_two_date);
    query.bindValue(":test_value_two", lowFrequencySignal.test_value_two);
    query.bindValue(":flatness_value_two", lowFrequencySignal.flatness_value_two);
    query.bindValue(":test_value_three_date", lowFrequencySignal.test_value_three_date);
    query.bindValue(":test_value_three", lowFrequencySignal.test_value_three);
    query.bindValue(":flatness_value_three", lowFrequencySignal.flatness_value_three);
    query.bindValue(":test_value_four_date", lowFrequencySignal.test_value_four_date);
    query.bindValue(":test_value_four", lowFrequencySignal.test_value_four);
    query.bindValue(":flatness_value_four", lowFrequencySignal.flatness_value_four);
    query.bindValue(":test_value_five_date", lowFrequencySignal.test_value_five_date);
    query.bindValue(":test_value_five", lowFrequencySignal.test_value_five);
    query.bindValue(":flatness_value_five", lowFrequencySignal.flatness_value_five);
    query.bindValue(":test_value_six_date", lowFrequencySignal.test_value_six_date);
    query.bindValue(":test_value_six", lowFrequencySignal.test_value_six);
    query.bindValue(":flatness_value_six", lowFrequencySignal.flatness_value_six);
    query.bindValue(":test_value_seven_date", lowFrequencySignal.test_value_seven_date);
    query.bindValue(":test_value_seven", lowFrequencySignal.test_value_seven);
    query.bindValue(":flatness_value_seven", lowFrequencySignal.flatness_value_seven);
    query.bindValue(":test_value_eight_date", lowFrequencySignal.test_value_eight_date);
    query.bindValue(":test_value_eight", lowFrequencySignal.test_value_eight);
    query.bindValue(":flatness_value_eight", lowFrequencySignal.flatness_value_eight);
    query.bindValue(":test_value_nine_date", lowFrequencySignal.test_value_nine_date);
    query.bindValue(":test_value_nine", lowFrequencySignal.test_value_nine);
    query.bindValue(":flatness_value_nine", lowFrequencySignal.flatness_value_nine);
    query.bindValue(":test_value_ten_date", lowFrequencySignal.test_value_ten_date);
    query.bindValue(":test_value_ten", lowFrequencySignal.test_value_ten);
    query.bindValue(":flatness_value_ten", lowFrequencySignal.flatness_value_ten);
    query.bindValue(":standard_deviation", lowFrequencySignal.standard_deviation);
    query.bindValue(":urel", lowFrequencySignal.urel);
    query.bindValue(":less_than", lowFrequencySignal.less_than);
    query.bindValue(":stability", lowFrequencySignal.stability);
    query.bindValue(":qualified", lowFrequencySignal.qualified);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (!isSuccess)
        qDebug() << query.lastError().text();
    return isSuccess;
}

// 【查询】核查项-小功率
QMap<QString, QMap<QString, QList<QStringList>>> BaseCommonApi::getStandardCheckItemDataLowerPower(QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList)
{
    QMap<QString, QMap<QString, QList<QStringList>>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();
    QSqlQuery query;
    query.prepare(
        "SELECT *	FROM "
        "biz_equipment_standard_check_item_data_lower_power  "
        " WHERE equipment_id = :equipment_id ORDER BY create_time ASC");
    query.bindValue(":equipment_id", equipment_id);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_category_id").toString()); // 核查项分类id(能够确定是哪个标准装置)
            result.append(query.value("equipment_id").toString()); // 配套设备id
            result.append(query.value("params").toString()); // 核查项目(直接存字典value)
            result.append(query.value("frequency").toString()); // 频率
            result.append(query.value("frequency_unit").toString()); // 频率单位   5
            result.append(query.value("kc").toString()); // Kc/%
            result.append(query.value("urel").toString()); // U(k=2)   7
            result.append(query.value("cycle_number").toString()); // 循环次数
            result.append(query.value("check_type").toString()); // 核查类型(字典value)
            result.append(query.value("remark").toString()); // 核查项备注
            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());
            resultData.append(result);
        }
    } else {
        qDebug() << query.lastError().text();
    }
    DataMapList = GetDataReturnItemListByCheckItemDataLowerPower(
        resultData, DataMapList, headList);
    return DataMapList;
}
QMap<QString, QMap<QString, QList<QStringList>>> BaseCommonApi::GetDataReturnItemListByCheckItemDataLowerPower(QList<QStringList> ResultData, QMap<QString, QMap<QString, QList<QStringList>>> DataMapList, QMap<QString, QMap<QString, QStringList>>* headList)
{
    QMap<QString, QMap<QString, QList<QStringList>>> result;
    QMap<QString, QList<QStringList>> resultMap;
    QList<QStringList> CalibrationFactorList;
    QList<QStringList> CalibrationFactorStableList;
    QMap<QString, QStringList> headerMap;
    for (const QStringList& row : ResultData) {
        if (!row.isEmpty() && row[3] == "校准因子" && row[9].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "频率"
                 << "频率单位"
                 << "Kc/%"
                 << "第1次Pcu/mW"
                 << "第1次示数Pbu/mW"
                 << "第1次校准因子Ku/%"
                 << "第2次Pcu/mW"
                 << "第2次示数Pbu/mW"
                 << "第2次校准因子Ku/%"
                 << "第3次Pcu/mW"
                 << "第3次示数Pbu/mW"
                 << "第3次校准因子Ku/%"
                 << "第4次Pcu/mW"
                 << "第4次示数Pbu/mW"
                 << "第4次校准因子Ku/%"
                 << "第5次Pcu/mW"
                 << "第5次示数Pbu/mW"
                 << "第5次校准因子Ku/%"
                 << "第6次Pcu/mW"
                 << "第6次示数Pbu/mW"
                 << "第6次校准因子Ku/%"
                 << "第7次Pcu/mW"
                 << "第7次示数Pbu/mW"
                 << "第7次校准因子Ku/%"
                 << "第8次Pcu/mW"
                 << "第8次示数Pbu/mW"
                 << "第8次校准因子Ku/%"
                 << "第9次Pcu/mW"
                 << "第9次示数Pbu/mW"
                 << "第9次校准因子Ku/%"
                 << "第10次Pcu/mW"
                 << "第10次示数Pbu/mW"
                 << "第10次校准因子Ku/%"
                 << "平均值(校准因子Ku/%)"
                 << "重复性S(x)";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[4] + row[5]); // 频率
            Data.append(row[5]); // 频率单位
            Data.append(row[6]); // Kc/%
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); // 平均值(校准因子Ku/%)
            Data.append(""); // 重复性S(x)
            CalibrationFactorList.append(Data);
            resultMap.insert("重复性", CalibrationFactorList);
            result.insert(row[3], resultMap);
        }
        if (!row.isEmpty() && row[3] == "校准因子" && row[9].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "频率"
                 << "频率单位"
                 << "校准因子Ku/%（平均值）1"
                 << "校准因子Ku/%（平均值）2"
                 << "校准因子Ku/%（平均值）3"
                 << "校准因子Ku/%（平均值）4"
                 << "校准因子Ku/%（平均值）5"
                 << "校准因子Ku/%（平均值）6"
                 << "平均值"
                 << "稳定性"
                 << "U(k=2)"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[4] + row[5]); // 频率
            Data.append(row[5]); // 频率单位
            Data.append(""); // 校准因子Ku/%（平均值）1
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); //
            Data.append(""); // 平均值
            Data.append(""); // 测量值8
            Data.append(""); // 稳定性
            Data.append(""); // U(k=2)
            Data.append(row[12]); // 结论
            CalibrationFactorStableList.append(Data);
            resultMap.insert("重复性", CalibrationFactorStableList);
            result.insert(row[3], resultMap);
        }
    }
    return result;
}
//   【保存】核查项-小功率
bool BaseCommonApi::InsertStandardCheckItemDataLowFrequencySignal(StandardCheckDataLowPower lowPower)
{
    QSqlQuery query;
    query.prepare("INSERT INTO biz_equipment_standard_check_data_low_power ("
                  " id,  data_id, check_type, params,frequency,frequency_unit, average_value, test_value_one_date, pcu_value_one, "
                  " pbu_value_one,  ku_value_one,test_value_two_date, pcu_value_two, pbu_value_two, ku_value_two, test_value_three_date, "
                  " pcu_value_three, pbu_value_three, ku_value_three,  test_value_four_date,  pcu_value_four, pbu_value_four,  ku_value_four, "
                  " test_value_five_date, pcu_value_five,  pbu_value_five,  ku_value_five,  test_value_six_date, pcu_value_six, pbu_value_six, "
                  " ku_value_six, test_value_seven_date, pcu_value_seven, pbu_value_seven, ku_value_seven, test_value_eight_date, "
                  " pcu_value_eight, pbu_value_eight, ku_value_eight, test_value_nine_date, pcu_value_nine, pbu_value_nine, "
                  " ku_value_nine, test_value_ten_date, pcu_value_ten, pbu_value_ten,ku_value_ten, standard_deviation, urel,  less_than, "
                  " stability,  qualified, create_time,  update_time ) "
                  " VALUES ( :id, :data_id,  :check_type, :params,  :frequency,  :frequency_unit, :average_value,  :test_value_one_date, "
                  " :pcu_value_one,  :pbu_value_one,  :ku_value_one, :test_value_two_date,  :pcu_value_two,  :pbu_value_two,  :ku_value_two, "
                  " :test_value_three_date,  :pcu_value_three,  :pbu_value_three,  :ku_value_three,  :test_value_four_date,  :pcu_value_four, "
                  " :pbu_value_four,  :ku_value_four,  :test_value_five_date,  :pcu_value_five,  :pbu_value_five,  :ku_value_five,  :test_value_six_date, "
                  " :pcu_value_six, :pbu_value_six, :ku_value_six, :test_value_seven_date, :pcu_value_seven,  :pbu_value_seven,  :ku_value_seven, "
                  " :test_value_eight_date,  :pcu_value_eight,  :pbu_value_eight, :ku_value_eight,  :test_value_nine_date,  :pcu_value_nine, "
                  " :pbu_value_nine,  :ku_value_nine, :test_value_ten_date, :pcu_value_ten,  :pbu_value_ten,  :ku_value_ten, "
                  " :standard_deviation, :urel,  :less_than,  :stability,  :qualified,  :create_time,  :update_time );");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", lowPower.id);
    query.bindValue(":data_id", lowPower.data_id);
    query.bindValue(":check_type", lowPower.check_type);
    query.bindValue(":params", lowPower.params);
    query.bindValue(":frequency", lowPower.frequency);
    query.bindValue(":frequency_unit", lowPower.frequency_unit);
    query.bindValue(":average_value", lowPower.average_value);
    query.bindValue(":test_value_one_date", lowPower.test_value_one_date);
    query.bindValue(":pcu_value_one", lowPower.pcu_value_one);
    query.bindValue(":pbu_value_one", lowPower.pbu_value_one);
    query.bindValue(":ku_value_one", lowPower.ku_value_one);
    query.bindValue(":test_value_two_date", lowPower.test_value_two_date);
    query.bindValue(":pcu_value_two", lowPower.pcu_value_two);
    query.bindValue(":pbu_value_two", lowPower.pbu_value_two);
    query.bindValue(":ku_value_two", lowPower.ku_value_two);
    query.bindValue(":test_value_three_date", lowPower.test_value_three_date);
    query.bindValue(":pcu_value_three", lowPower.pcu_value_three);
    query.bindValue(":pbu_value_three", lowPower.pbu_value_three);
    query.bindValue(":ku_value_three", lowPower.ku_value_three);
    query.bindValue(":test_value_four_date", lowPower.test_value_four_date);
    query.bindValue(":pcu_value_four", lowPower.pcu_value_four);
    query.bindValue(":pbu_value_four", lowPower.pbu_value_four);
    query.bindValue(":ku_value_four", lowPower.ku_value_four);
    query.bindValue(":test_value_five_date", lowPower.test_value_five_date);
    query.bindValue(":pcu_value_five", lowPower.pcu_value_five);
    query.bindValue(":pbu_value_five", lowPower.pbu_value_five);
    query.bindValue(":ku_value_five", lowPower.ku_value_five);
    query.bindValue(":test_value_six_date", lowPower.test_value_six_date);
    query.bindValue(":pcu_value_six", lowPower.pcu_value_six);
    query.bindValue(":pbu_value_six", lowPower.pbu_value_six);
    query.bindValue(":ku_value_six", lowPower.ku_value_six);
    query.bindValue(":test_value_seven_date", lowPower.test_value_seven_date);
    query.bindValue(":pcu_value_seven", lowPower.pcu_value_seven);
    query.bindValue(":pbu_value_seven", lowPower.pbu_value_seven);
    query.bindValue(":ku_value_seven", lowPower.ku_value_seven);
    query.bindValue(":test_value_eight_date", lowPower.test_value_eight_date);
    query.bindValue(":pcu_value_eight", lowPower.pcu_value_eight);
    query.bindValue(":pbu_value_eight", lowPower.pbu_value_eight);
    query.bindValue(":ku_value_eight", lowPower.ku_value_eight);
    query.bindValue(":test_value_nine_date", lowPower.test_value_nine_date);
    query.bindValue(":pcu_value_nine", lowPower.pcu_value_nine);
    query.bindValue(":pbu_value_nine", lowPower.pbu_value_nine);
    query.bindValue(":ku_value_nine", lowPower.ku_value_nine);
    query.bindValue(":test_value_ten_date", lowPower.test_value_ten_date);
    query.bindValue(":pcu_value_ten", lowPower.pcu_value_ten);
    query.bindValue(":pbu_value_ten", lowPower.pbu_value_ten);
    query.bindValue(":ku_value_ten", lowPower.ku_value_ten);
    query.bindValue(":standard_deviation", lowPower.standard_deviation);
    query.bindValue(":urel", lowPower.urel);
    query.bindValue(":less_than", lowPower.less_than);
    query.bindValue(":stability", lowPower.stability);
    query.bindValue(":qualified", lowPower.qualified);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << query.lastError().text();
    }
    return isSuccess;
}

// 【查询】核查项-s参数
QMap<QString, QMap<QString, QList<QStringList>>> BaseCommonApi::getStandardCheckItemDataSParam(QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList)
{
    QMap<QString, QMap<QString, QList<QStringList>>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();
    QSqlQuery query;
    query.prepare(
        "SELECT *	FROM "
        "biz_equipment_standard_check_item_data_s_param  "
        " WHERE equipment_id = :equipment_id ORDER BY create_time ASC");
    query.bindValue(":equipment_id", equipment_id);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_category_id").toString()); // 核查项分类id(能够确定是哪个标准装置)
            result.append(query.value("equipment_id").toString()); // 配套设备id
            result.append(query.value("params").toString()); // 核查项目(直接存字典value)
            result.append(query.value("type_value").toString()); // 类型
            result.append(query.value("vswr").toString()); // VSWR   5
            result.append(query.value("frequency").toString()); // 频率
            result.append(query.value("frequency_unit").toString()); // 频率单位  7
            result.append(query.value("amplitude").toString()); // 幅度
            result.append(query.value("amplitude_unit").toString()); // 幅度单位
            result.append(query.value("phase").toString()); // 相位
            result.append(query.value("phase_unit").toString()); // 相位单位
            result.append(query.value("urel").toString()); // U(k=2)
            result.append(query.value("cycle_number").toString()); // 循环次数
            result.append(query.value("check_type").toString()); // 核查类型(字典value)
            result.append(query.value("remark").toString()); // 核查项备注
            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());
            resultData.append(result);
        }
    } else {
        qDebug() << query.lastError().text();
    }
    DataMapList = GetDataReturnItemListByCheckItemDataSParam(
        resultData, DataMapList, headList);
    return DataMapList;
}
QMap<QString, QMap<QString, QList<QStringList>>> BaseCommonApi::GetDataReturnItemListByCheckItemDataSParam(QList<QStringList> ResultData, QMap<QString, QMap<QString, QList<QStringList>>> DataMapList, QMap<QString, QMap<QString, QStringList>>* headList)
{
    QMap<QString, QMap<QString, QList<QStringList>>> result;
    QMap<QString, QList<QStringList>> resultMap;
    QList<QStringList> MismatchMeasurementList;
    QList<QStringList> AttenuationMeasurementList;
    QList<QStringList> PhaseShiftMeasurementList;

    QList<QStringList> MismatchMeasurementStableList;
    QList<QStringList> AttenuationMeasurementStableList;
    QList<QStringList> PhaseShiftMeasurementStableList;
    QMap<QString, QStringList> headerMap;
    for (const QStringList& row : ResultData) {
        if (!row.isEmpty() && row[3] == "失配测量" && row[14].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "类型"
                 << "VSMR"
                 << "频率"
                 << "频率单位"
                 << "测量次数及测量值1"
                 << "测量次数及测量值2"
                 << "测量次数及测量值3"
                 << "测量次数及测量值4"
                 << "测量次数及测量值5"
                 << "测量次数及测量值6"
                 << "测量次数及测量值7"
                 << "测量次数及测量值8"
                 << "测量次数及测量值9"
                 << "测量次数及测量值10"
                 << "平均值"
                 << "重复性S(x)";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4]); // 类型
            Data.append(row[5]); // VSMR
            Data.append(row[6] + row[7]); // 频率
            Data.append(row[7]); // 频率单位
            Data.append(""); // 1
            Data.append(""); // 2
            Data.append(""); // 3
            Data.append(""); // 4
            Data.append(""); // 5
            Data.append(""); // 6
            Data.append(""); // 7
            Data.append(""); // 8
            Data.append(""); // 9
            Data.append(""); // 10
            Data.append(""); // 平均值
            Data.append(""); // 重复性S(x)
            MismatchMeasurementList.append(Data);
            resultMap.insert("重复性", MismatchMeasurementList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "衰减测量" && row[14].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "类型"
                 << "频率"
                 << "频率单位"
                 << "幅度"
                 << "幅度单位"
                 << "测量次数及测量值1"
                 << "测量次数及测量值2"
                 << "测量次数及测量值3"
                 << "测量次数及测量值4"
                 << "测量次数及测量值5"
                 << "测量次数及测量值6"
                 << "测量次数及测量值7"
                 << "测量次数及测量值8"
                 << "测量次数及测量值9"
                 << "测量次数及测量值10"
                 << "平均值"
                 << "重复性S(x)";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4]); // 类型
            Data.append(row[6] + row[7]); // 频率
            Data.append(row[7]); // 频率单位
            Data.append(row[8] + row[9]); // 幅度
            Data.append(row[9]); // 幅度单位
            Data.append(""); // 1
            Data.append(""); // 2
            Data.append(""); // 3
            Data.append(""); // 4
            Data.append(""); // 5
            Data.append(""); // 6
            Data.append(""); // 7
            Data.append(""); // 8
            Data.append(""); // 9
            Data.append(""); // 10
            Data.append(""); // 平均值
            Data.append(""); // 重复性S(x)
            AttenuationMeasurementList.append(Data);
            resultMap.insert("重复性", AttenuationMeasurementList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "相移测量" && row[14].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "类型"
                 << "频率"
                 << "频率单位"
                 << "相位"
                 << "相位单位"
                 << "测量次数及测量值1"
                 << "测量次数及测量值2"
                 << "测量次数及测量值3"
                 << "测量次数及测量值4"
                 << "测量次数及测量值5"
                 << "测量次数及测量值6"
                 << "测量次数及测量值7"
                 << "测量次数及测量值8"
                 << "测量次数及测量值9"
                 << "测量次数及测量值10"
                 << "平均值"
                 << "重复性S(x)";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4]); // 类型
            Data.append(row[6] + row[7]); // 频率
            Data.append(row[7]); // 频率单位
            Data.append(row[10] + row[11]); // 相位
            Data.append(row[11]); // 相位单位
            Data.append(""); // 1
            Data.append(""); // 2
            Data.append(""); // 3
            Data.append(""); // 4
            Data.append(""); // 5
            Data.append(""); // 6
            Data.append(""); // 7
            Data.append(""); // 8
            Data.append(""); // 9
            Data.append(""); // 10
            Data.append(""); // 平均值
            Data.append(""); // 重复性S(x)
            PhaseShiftMeasurementList.append(Data);
            resultMap.insert("重复性", PhaseShiftMeasurementList);
            result.insert(row[3], resultMap);
        }
        if (!row.isEmpty() && row[3] == "失配测量" && row[14].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "类型"
                 << "VSMR"
                 << "频率"
                 << "频率单位"
                 << "测量值1"
                 << "测量值2"
                 << "测量值3"
                 << "测量值4"
                 << "测量值5"
                 << "测量值6"
                 << "平均值"
                 << "稳定性"
                 << "U(k=2)"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4]); // 类型
            Data.append(row[5]); // VSMR
            Data.append(row[6] + row[7]); // 频率
            Data.append(row[7]); // 频率单位
            Data.append(""); // 1
            Data.append(""); // 2
            Data.append(""); // 3
            Data.append(""); // 4
            Data.append(""); // 5
            Data.append(""); // 6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[12]); // U(k=2)
            Data.append(""); // 结论
            MismatchMeasurementStableList.append(Data);
            resultMap.insert("稳定性", MismatchMeasurementStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "衰减测量" && row[14].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "类型"
                 << "频率"
                 << "频率单位"
                 << "幅度"
                 << "幅度单位"
                 << "测量次数及测量值1"
                 << "测量次数及测量值2"
                 << "测量次数及测量值3"
                 << "测量次数及测量值4"
                 << "测量次数及测量值5"
                 << "测量次数及测量值6"
                 << "测量次数及测量值7"
                 << "测量次数及测量值8"
                 << "平均值"
                 << "稳定性"
                 << "U(k=2)"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4]); // 类型
            Data.append(row[6] + row[7]); // 频率
            Data.append(row[7]); // 频率单位
            Data.append(row[8] + row[9]); // 幅度
            Data.append(row[9]); // 幅度单位
            Data.append(""); // 1
            Data.append(""); // 2
            Data.append(""); // 3
            Data.append(""); // 4
            Data.append(""); // 5
            Data.append(""); // 6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[12]); // U(k=2)
            Data.append(""); // 结论
            AttenuationMeasurementStableList.append(Data);
            resultMap.insert("稳定性", AttenuationMeasurementStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "相移测量" && row[14].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "类型"
                 << "频率"
                 << "频率单位"
                 << "相位"
                 << "相位单位"
                 << "测量次数及测量值1"
                 << "测量次数及测量值2"
                 << "测量次数及测量值3"
                 << "测量次数及测量值4"
                 << "测量次数及测量值5"
                 << "测量次数及测量值6"
                 << "平均值"
                 << "稳定性"
                 << "U(k=2)"
                 << "结论"
                 << "备注";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4]); // 类型
            Data.append(row[6] + row[7]); // 频率
            Data.append(row[7]); // 频率单位
            Data.append(row[10] + row[11]); // 相位
            Data.append(row[11]); // 相位单位
            Data.append(""); // 1
            Data.append(""); // 2
            Data.append(""); // 3
            Data.append(""); // 4
            Data.append(""); // 5
            Data.append(""); // 6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[12]); // U(k=2)
            Data.append(""); // 结论
            Data.append(row[15]); // 备注
            PhaseShiftMeasurementStableList.append(Data);
            resultMap.insert("稳定性", PhaseShiftMeasurementStableList);
            result.insert(row[3], resultMap);
        }
    }
    return result;
}
//   【保存】核查项-s参数
bool BaseCommonApi::InsertStandardCheckItemDataSParam(StandardCheckDataSParam sParam)
{
    QSqlQuery query;
    query.prepare("INSERT INTO biz_equipment_standard_check_data_s_param ("
                  " id,data_id,check_type,params,frequency,frequency_unit,amplitude,amplitude_unit,phase, phase_unit,type_value, vsmr,average_value,"
                  " test_value_one_date,test_value_one, test_value_two_date,test_value_two,test_value_three_date,test_value_three, test_value_four_date,"
                  " test_value_four,test_value_five_date,test_value_five, test_value_six_date,test_value_six,test_value_seven_date, test_value_seven, test_value_eight_date,"
                  " test_value_eight,test_value_nine_date,test_value_nine,test_value_ten_date, test_value_ten,standard_deviation,urel, less_than, stability,qualified,create_time,"
                  " update_time) VALUES( : id, : data_id, : check_type,  : params, : frequency,: frequency_unit, : amplitude,: amplitude_unit, : phase, : phase_unit, : type_value,"
                  " : vsmr, : average_value, : test_value_one_date, : test_value_one, : test_value_two_date,: test_value_two,: test_value_three_date, : test_value_three,"
                  " : test_value_four_date, : test_value_four, : test_value_five_date, : test_value_five, : test_value_six_date,: test_value_six, : test_value_seven_date,"
                  " : test_value_seven, : test_value_eight_date, : test_value_eight, : test_value_nine_date, : test_value_nine, : test_value_ten_date,: test_value_ten,"
                  " : standard_deviation, : urel, : less_than, : stability, : qualified, : create_time, : update_time); ");
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":id", sParam.id);
    query.bindValue(":data_id", sParam.data_id);
    query.bindValue(":check_type", sParam.check_type);
    query.bindValue(":params", sParam.params);
    query.bindValue(":frequency", sParam.frequency);
    query.bindValue(":frequency_unit", sParam.frequency_unit);
    query.bindValue(":amplitude", sParam.amplitude);
    query.bindValue(":amplitude_unit", sParam.amplitude_unit);
    query.bindValue(":phase", sParam.phase);
    query.bindValue(":phase_unit", sParam.phase_unit);
    query.bindValue(":type_value", sParam.type_value);
    query.bindValue(":vsmr", sParam.vsmr);
    query.bindValue(":average_value", sParam.average_value);
    query.bindValue(":test_value_one_date", sParam.test_value_one_date);
    query.bindValue(":test_value_one", sParam.test_value_one);
    query.bindValue(":test_value_two_date", sParam.test_value_two_date);
    query.bindValue(":test_value_two", sParam.test_value_two);
    query.bindValue(":test_value_three_date", sParam.test_value_three_date);
    query.bindValue(":test_value_three", sParam.test_value_three);
    query.bindValue(":test_value_four_date", sParam.test_value_four_date);
    query.bindValue(":test_value_four", sParam.test_value_four);
    query.bindValue(":test_value_five_date", sParam.test_value_five_date);
    query.bindValue(":test_value_five", sParam.test_value_five);
    query.bindValue(":test_value_six_date", sParam.test_value_six_date);
    query.bindValue(":test_value_six", sParam.test_value_six);
    query.bindValue(":test_value_seven_date", sParam.test_value_seven_date);
    query.bindValue(":test_value_seven", sParam.test_value_seven);
    query.bindValue(":test_value_eight_date", sParam.test_value_eight_date);
    query.bindValue(":test_value_eight", sParam.test_value_eight);
    query.bindValue(":test_value_nine_date", sParam.test_value_nine_date);
    query.bindValue(":test_value_nine", sParam.test_value_nine);
    query.bindValue(":test_value_ten_date", sParam.test_value_ten_date);
    query.bindValue(":test_value_ten", sParam.test_value_ten);
    query.bindValue(":standard_deviation", sParam.standard_deviation);
    query.bindValue(":urel", sParam.urel);
    query.bindValue(":less_than", sParam.less_than);
    query.bindValue(":stability", sParam.stability);
    query.bindValue(":qualified", sParam.qualified);
    query.bindValue(":create_time", currentDateTime);
    query.bindValue(":update_time", currentDateTime);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << query.lastError().text();
    }
    return isSuccess;
}
// 【查询】核查项-铯原子
QMap<QString, QMap<QString, QList<QStringList>>> BaseCommonApi::getStandardCheckItemDataCesiumAtom(QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList)
{
    QMap<QString, QMap<QString, QList<QStringList>>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();
    QSqlQuery query;
    query.prepare(
        "SELECT *	FROM "
        "biz_equipment_standard_check_item_data_cesium_atom  "
        " WHERE equipment_id = :equipment_id ORDER BY create_time ASC");
    query.bindValue(":equipment_id", equipment_id);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_category_id").toString()); // 核查项分类id(能够确定是哪个标准装置)
            result.append(query.value("equipment_id").toString()); // 配套设备id
            result.append(query.value("params").toString()); // 核查项目(字典value)
            result.append(query.value("check_point").toString()); // 频率点
            result.append(query.value("unit").toString()); // 频率点单位   5
            result.append(query.value("impedance").toString()); // 阻抗
            result.append(query.value("impedance_unit").toString()); // 阻抗单位  7
            result.append(query.value("electrical_level").toString()); // 电平
            result.append(query.value("electrical_level_unit").toString()); // 电平单位
            result.append(query.value("amplitude").toString()); // 幅度
            result.append(query.value("amplitude_unit").toString()); // 幅度单位
            result.append(query.value("check_method").toString()); // 核查方法
            result.append(query.value("cycle_number").toString()); // 循环次数
            result.append(query.value("check_type").toString()); // 核查类型(字典value)
            result.append(query.value("urel").toString()); // U(k=2)
            result.append(query.value("remark").toString()); // 核查项备注
            result.append(query.value("create_time").toString());
            result.append(query.value("update_time").toString());
            resultData.append(result);
        }
    } else {
        qDebug() << query.lastError().text();
    }
    DataMapList = GetDataReturnItemListByCheckItemDataCesiumAtom(
        resultData, DataMapList, headList);
    return DataMapList;
}
QMap<QString, QMap<QString, QList<QStringList>>> BaseCommonApi::GetDataReturnItemListByCheckItemDataCesiumAtom(QList<QStringList> ResultData, QMap<QString, QMap<QString, QList<QStringList>>> DataMapList, QMap<QString, QMap<QString, QStringList>>* headList)
{
    QMap<QString, QMap<QString, QList<QStringList>>> result;
    QMap<QString, QList<QStringList>> resultMap;
    QList<QStringList> FrequencyStandardList;
    QList<QStringList> FunctionSignalGeneratorList;
    QList<QStringList> SignalSourceVerificationList;

    QList<QStringList> FrequencyStandardStableList;
    QList<QStringList> FunctionSignalGeneratorStableList;
    QList<QStringList> SignalSourceVerificationStableList;
    QMap<QString, QStringList> headerMap;
    for (const QStringList& row : ResultData) {
        if (!row.isEmpty() && row[3] == "铯原子频标核查" && row[14].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查方法"
                 << "频率点"
                 << "频率点单位"
                 << "*测量值X1"
                 << "*测量值X2"
                 << "*测量值X3"
                 << "*测量值X4"
                 << "*测量值X5"
                 << "*测量值X6"
                 << "*测量值X7"
                 << "*测量值X8"
                 << "*测量值X9"
                 << "*测量值X10"
                 << "平均值"
                 << "重复性S(x)";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[12]); // 核查方法
            Data.append(row[4] + row[5]); // 频率
            Data.append(row[5]); // 频率单位
            Data.append(""); // 1
            Data.append(""); // 2
            Data.append(""); // 3
            Data.append(""); // 4
            Data.append(""); // 5
            Data.append(""); // 6
            Data.append(""); // 7
            Data.append(""); // 8
            Data.append(""); // 9
            Data.append(""); // 10
            Data.append(""); // 平均值
            Data.append(""); // 重复性S(x)
            FrequencyStandardList.append(Data);
            resultMap.insert("重复性", FrequencyStandardList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "函数信号发生器核查" && row[14].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查方法"
                 << "阻抗"
                 << "阻抗单位"
                 << "频率点"
                 << "频率点单位"
                 << "电平"
                 << "电平单位"
                 << "*测量值X1"
                 << "*测量值X2"
                 << "*测量值X3"
                 << "*测量值X4"
                 << "*测量值X5"
                 << "*测量值X6"
                 << "*测量值X7"
                 << "*测量值X8"
                 << "*测量值X9"
                 << "*测量值X10"
                 << "平均值"
                 << "重复性S(x)";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[12]); // 核查方法
            Data.append(row[6] + row[7]); // 阻抗
            Data.append(row[7]); // 阻抗单位
            Data.append(row[4] + row[5]); // 频率
            Data.append(row[5]); // 频率单位
            Data.append(row[8] + row[9]); // 电平
            Data.append(row[9]); // 电平单位
            Data.append(""); // 1
            Data.append(""); // 2
            Data.append(""); // 3
            Data.append(""); // 4
            Data.append(""); // 5
            Data.append(""); // 6
            Data.append(""); // 7
            Data.append(""); // 8
            Data.append(""); // 9
            Data.append(""); // 10
            Data.append(""); // 平均值
            Data.append(""); // 重复性S(x)
            FunctionSignalGeneratorList.append(Data);
            resultMap.insert("重复性", FunctionSignalGeneratorList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "信号源核查" && row[14].contains("重复性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率点"
                 << "频率点单位"
                 << "幅度"
                 << "幅度单位"
                 << "*测量值X1"
                 << "*测量值X2"
                 << "*测量值X3"
                 << "*测量值X4"
                 << "*测量值X5"
                 << "*测量值X6"
                 << "*测量值X7"
                 << "*测量值X8"
                 << "*测量值X9"
                 << "*测量值X10"
                 << "平均值"
                 << "重复性S(x)";
            headerMap.insert("重复性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 频率
            Data.append(row[5]); // 频率单位
            Data.append(row[10] + row[11]); // 幅度
            Data.append(row[11]); // 幅度单位
            Data.append(""); // 1
            Data.append(""); // 2
            Data.append(""); // 3
            Data.append(""); // 4
            Data.append(""); // 5
            Data.append(""); // 6
            Data.append(""); // 7
            Data.append(""); // 8
            Data.append(""); // 9
            Data.append(""); // 10
            Data.append(""); // 平均值
            Data.append(""); // 重复性S(x)
            FunctionSignalGeneratorList.append(Data);
            resultMap.insert("重复性", FunctionSignalGeneratorList);
            result.insert(row[3], resultMap);
        }
        if (!row.isEmpty() && row[3] == "铯原子频标核查" && row[14].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查方法"
                 << "频率点"
                 << "频率点单位"
                 << "*测量值X1"
                 << "*测量值X2"
                 << "*测量值X3"
                 << "*测量值X4"
                 << "*测量值X5"
                 << "*测量值X6"
                 << "*平均值"
                 << "*稳定性"
                 << "U(k=2)"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[12]); // 核查方法
            Data.append(row[4] + row[5]); // 频率
            Data.append(row[5]); // 频率单位
            Data.append(""); // 1
            Data.append(""); // 2
            Data.append(""); // 3
            Data.append(""); // 4
            Data.append(""); // 5
            Data.append(""); // 6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[15]); // U(k=2)
            Data.append(""); // 重复性S(x)
            FrequencyStandardStableList.append(Data);
            resultMap.insert("稳定性", FrequencyStandardStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "函数信号发生器核查" && row[14].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "核查方法"
                 << "频率点"
                 << "频率点单位"
                 << "电平"
                 << "电平单位"
                 << "*测量值X1"
                 << "*测量值X2"
                 << "*测量值X3"
                 << "*测量值X4"
                 << "*测量值X5"
                 << "*测量值X6"
                 << "*平均值"
                 << "*稳定性"
                 << "U(k=2)"
                 << "结论";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[12]); // 核查方法
            Data.append(row[4] + row[5]); // 频率
            Data.append(row[5]); // 频率单位
            Data.append(row[8] + row[9]); // 电平
            Data.append(row[9]); // 电平单位
            Data.append(""); // 1
            Data.append(""); // 2
            Data.append(""); // 3
            Data.append(""); // 4
            Data.append(""); // 5
            Data.append(""); // 6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[15]); // U(k=2)
            Data.append(""); // 重复性S(x)
            FunctionSignalGeneratorStableList.append(Data);
            resultMap.insert("稳定性", FunctionSignalGeneratorStableList);
            result.insert(row[3], resultMap);
        } else if (!row.isEmpty() && row[3] == "信号源核查" && row[14].contains("稳定性")) {
            // 添加列头
            QStringList head;
            head << "序号"
                 << "核查项目"
                 << "频率点"
                 << "频率点单位"
                 << "幅度"
                 << "幅度单位"
                 << "*测量值X1"
                 << "*测量值X2"
                 << "*测量值X3"
                 << "*测量值X4"
                 << "*测量值X5"
                 << "*测量值X6"
                 << "*平均值"
                 << "*稳定性"
                 << "U(k=2)"
                 << "结论"
                 << "备注";
            headerMap.insert("稳定性", head);
            headList->insert(row[3], headerMap);
            // 添加数据
            QStringList Data;
            Data.append(row[0]); // ID
            Data.append(row[3]); // 核查项目
            Data.append(row[4] + row[5]); // 频率
            Data.append(row[5]); // 频率单位
            Data.append(row[10] + row[11]); // 幅度
            Data.append(row[11]); // 幅度单位
            Data.append(""); // 1
            Data.append(""); // 2
            Data.append(""); // 3
            Data.append(""); // 4
            Data.append(""); // 5
            Data.append(""); // 6
            Data.append(""); // 平均值
            Data.append(""); // 稳定性
            Data.append(row[15]); // U(k=2)
            Data.append(""); // 重复性S(x)
            Data.append(row[16]); // 备注
            FunctionSignalGeneratorStableList.append(Data);
            resultMap.insert("稳定性", FunctionSignalGeneratorStableList);
            result.insert(row[3], resultMap);
        }
    }
    return result;
}
//   【保存】核查项-铯原子
bool BaseCommonApi::InsertStandardCheckDataCesiumAtom(StandardCheckDataCesiumAtom cesiumAtom)
{
    QSqlQuery query;
    query.prepare("INSERT INTO biz_equipment_standard_check_data_cesium_atom ("
                  " id,data_id,check_type,params,check_point,unit,electrical_level,electrical_level_unit,amplitude,amplitude_unit,check_method,test_value_one_date,"
                  " test_value_one, test_value_two_date,test_value_two, test_value_three_date,test_value_three,test_value_four_date, test_value_four,test_value_five_date,"
                  " test_value_five, test_value_six_date, test_value_six, test_value_seven_date,test_value_seven, test_value_eight_date,test_value_eight,"
                  " test_value_nine_date,test_value_nine,test_value_ten_date,test_value_ten, average_value,standard_deviation,urel, conclusion,stability,qualified,"
                  " create_time,update_time ) VALUES ( :id, :data_id, :check_type, :params, :check_point, :unit, :electrical_level, :electrical_level_unit, :amplitude,"
                  " :amplitude_unit, :check_method, :test_value_one_date,:test_value_one, :test_value_two_date, :test_value_two, :test_value_three_date,"
                  " :test_value_three, :test_value_four_date,:test_value_four, :test_value_five_date,:test_value_five, :test_value_six_date, :test_value_six,"
                  " :test_value_seven_date, :test_value_seven, :test_value_eight_date, :test_value_eight, :test_value_nine_date, :test_value_nine, :test_value_ten_date,"
                  " :test_value_ten, :average_value, :standard_deviation, :urel,:conclusion, :stability, :qualified, :create_time,:update_time);");
    query.bindValue(":id", cesiumAtom.id);
    query.bindValue(":data_id", cesiumAtom.data_id);
    query.bindValue(":check_type", cesiumAtom.check_type);
    query.bindValue(":params", cesiumAtom.params);
    query.bindValue(":check_point", cesiumAtom.check_point);
    query.bindValue(":unit", cesiumAtom.unit);
    query.bindValue(":electrical_level", cesiumAtom.electrical_level);
    query.bindValue(":electrical_level_unit", cesiumAtom.electrical_level_unit);
    query.bindValue(":amplitude", cesiumAtom.amplitude);
    query.bindValue(":amplitude_unit", cesiumAtom.amplitude_unit);
    query.bindValue(":check_method", cesiumAtom.check_method);
    query.bindValue(":test_value_one_date", cesiumAtom.test_value_one_date);
    query.bindValue(":test_value_one", cesiumAtom.test_value_one);
    query.bindValue(":test_value_two_date", cesiumAtom.test_value_two_date);
    query.bindValue(":test_value_two", cesiumAtom.test_value_two);
    query.bindValue(":test_value_three_date", cesiumAtom.test_value_three_date);
    query.bindValue(":test_value_three", cesiumAtom.test_value_three);
    query.bindValue(":test_value_four_date", cesiumAtom.test_value_four_date);
    query.bindValue(":test_value_four", cesiumAtom.test_value_four);
    query.bindValue(":test_value_five_date", cesiumAtom.test_value_five_date);
    query.bindValue(":test_value_five", cesiumAtom.test_value_five);
    query.bindValue(":test_value_six_date", cesiumAtom.test_value_six_date);
    query.bindValue(":test_value_six", cesiumAtom.test_value_six);
    query.bindValue(":test_value_seven_date", cesiumAtom.test_value_seven_date);
    query.bindValue(":test_value_seven", cesiumAtom.test_value_seven);
    query.bindValue(":test_value_eight_date", cesiumAtom.test_value_eight_date);
    query.bindValue(":test_value_eight", cesiumAtom.test_value_eight);
    query.bindValue(":test_value_nine_date", cesiumAtom.test_value_nine_date);
    query.bindValue(":test_value_nine", cesiumAtom.test_value_nine);
    query.bindValue(":test_value_ten_date", cesiumAtom.test_value_ten_date);
    query.bindValue(":test_value_ten", cesiumAtom.test_value_ten);
    query.bindValue(":average_value", cesiumAtom.average_value);
    query.bindValue(":standard_deviation", cesiumAtom.standard_deviation);
    query.bindValue(":urel", cesiumAtom.urel);
    query.bindValue(":conclusion", cesiumAtom.conclusion);
    query.bindValue(":stability", cesiumAtom.stability);
    query.bindValue(":qualified", cesiumAtom.qualified);
    query.bindValue(":create_time", cesiumAtom.create_time);
    query.bindValue(":update_time", cesiumAtom.update_time);
    bool isSuccess = query.exec();
    if (!isSuccess) {
        qDebug() << query.lastError().text();
    }
    return isSuccess;
}

// 【查询】检定项-铯原子频率
QMap<QString, QList<QStringList>>
BaseCommonApi::getDeviceCesiumAtomicFrequency(
    QString itemCatoryName, QString itemId,
    SequentialMap<QString, QStringList>* headList)
{
    QMap<QString, QList<QStringList>> DataMapList;
    QList<QStringList> resultData;
    QStringList result;
    resultData.clear();
    QSqlQuery query;
    query.prepare(
        " SELECT *,bbdmii.band_width as BW  FROM "
        " biz_business_device_measure_item_data_cesium_atom bbdmidca"
        " JOIN biz_business_device_measure_item_info bbdmii ON bbdmii.id = "
        " bbdmidca.item_id "
        " WHERE bbdmidca.item_id = :itemId  ORDER BY bbdmidca.create_time ASC");
    // 将参数绑定到占位符
    query.bindValue(":itemId", itemId);
    if (query.exec()) {
        while (query.next()) {
            result.clear();
            result.append(query.value("id").toString());
            result.append(query.value("item_id").toString());
            result.append(query.value("data_type").toString()); // 检定项目ID

            result.append(query.value("crystal_oscillator_startup_character").toString()); // 晶振开机特性 3
            result.append(query.value("crystal_oscillator_stability").toString()); // 晶振1s频率稳定度
            result.append(query.value("crystal_oscillator_deviation").toString()); // 晶振相对频率偏差
            result.append(query.value("crystal_oscillator_reproducibility").toString()); // 晶振频率复现性 6
            result.append(query.value("crystal_oscillator_daily_aging_rate").toString()); // 晶振日老化率
            result.append(query.value("frequency_measure_range").toString()); // 频率测量范围、输入灵敏度
            result.append(query.value("frequency_measure_error").toString()); // 频率测量误差
            result.append(query.value("periodic_measure_range").toString()); // 周期测量范围、输入灵敏度 10
            result.append(query.value("periodic_measure_error").toString()); // 周期测量误差
            result.append(query.value("time_interval_measure_range").toString()); // 时间间隔测量范围及测量误差
            result.append(query.value("output_signal").toString()); // 输出信号  13
            result.append(query.value("warm_up").toString()); // 开机特性
            result.append(query.value("frequency_reproducibility").toString()); // 频率复现性  15
            result.append(query.value("relative_frequency_deviation").toString()); // 相对频率偏差
            result.append(query.value("frequency_stability").toString()); // 频率稳定度
            result.append(query.value("daily_frequency_drift_rate").toString()); // 日频率漂移率
            result.append(query.value("frequency_signal_output_amplitude").toString()); // 频率信号输出幅度 19
            result.append(query.value("second_pulse_output").toString()); // 秒脉冲输出
            result.append(query.value("daily_aging_rate").toString()); // 日老化率 21

            result.append(query.value("gate_time").toString()); // 闸门时间
            result.append(query.value("gate_time_unit").toString()); // 闸门时间单位
            result.append(query.value("multiply_frequency").toString()); // 倍增次数
            result.append(query.value("group_number").toString()); // 组数         25
            result.append(query.value("measure_interval").toString()); // 测量间隔
            result.append(query.value("measure_interval_unit").toString()); // 测量间隔单位
            result.append(query.value("technical_index_symbol").toString()); // 技术指标前符号/输入灵敏度技术指标前符号/上升时间技术指标符号
            result.append(query.value("technical_index").toString()); // 技术指标/输入灵敏度技术指标/上升时间技术指标
            result.append(query.value("technical_index_unit").toString()); // 技术指标单位/输入灵敏度技术指标单位/上升时间技术指标单位  30
            result.append(query.value("other_technical_index_symbol").toString()); // 最大输入电平技术指标前符号
            result.append(query.value("other_technical_index").toString()); // 最大输入电平技术指标
            result.append(query.value("other_technical_index_unit").toString()); // 最大输入电平技术指标单位
            result.append(query.value("amplitude_technical_index_lower").toString()); // 幅度技术指标下限
            result.append(query.value("amplitude_technical_index_upper").toString()); // 幅度技术指标上限        35
            result.append(query.value("amplitude_technical_index_unit").toString()); // 幅度技术指标单位
            result.append(query.value("pulse_width_technical_index_lower").toString()); // 脉冲宽度技术指标下限
            result.append(query.value("pulse_width_technical_index_upper").toString()); // 脉冲宽度技术指标上限
            result.append(query.value("pulse_width_technical_index_unit").toString()); // 脉冲宽度技术指标单位
            result.append(query.value("urel").toString()); // 不确定度                                       40
            result.append(query.value("vertical_deflection_gear").toString()); // 低电平测量时垂直偏转档位
            result.append(query.value("vertical_deflection_gear_unit").toString()); // 低电平测量时垂直偏转档位单位  42
            result.append(query.value("t_one").toString()); // T1
            result.append(query.value("t_one_unit").toString()); // T1单位
            result.append(query.value("t_two").toString()); // T2单位      45
            result.append(query.value("t_two_unit").toString()); // T3
            result.append(query.value("t_three").toString()); // T3单位
            result.append(query.value("t_three_unit").toString()); // 被测信号频率
            result.append(query.value("tested_signal_frequency").toString()); // 被测信号频率单位 49
            result.append(query.value("tested_signal_frequency_unit").toString()); // 取样时间
            result.append(query.value("sample_time").toString()); // 取样时间
            result.append(query.value("sample_time_unit").toString()); // 取样时间单位
            result.append(query.value("sample_group_number").toString()); // 采样组数
            result.append(query.value("thoroughfare").toString()); // 通道
            result.append(query.value("nominal_value").toString()); // 标称值           55
            result.append(query.value("nominal_value_unit").toString()); // 标称值单位
            result.append(query.value("signal_source_output_amplitude").toString()); // 信号源输出幅度
            result.append(query.value("signal_source_output_amplitude_unit").toString()); // 信号源输出幅度单位
            result.append(query.value("signal_source_max_input_level").toString()); // 信号源最大输入电平
            result.append(query.value("signal_source_max_input_level_unit").toString()); // 信号源最大输入电平单位  60
            result.append(query.value("signal_type").toString()); // 信号类型
            result.append(query.value("on_time").toString()); // 开机时间
            result.append(query.value("on_time_unit").toString()); // 开机时间单位

            result.append(query.value("create_time").toString()); // 创建时间
            result.append(query.value("update_time").toString()); // 更新时间
            resultData.append(result);
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    DataMapList = GetDataReturnItemListBySmallPower(
        itemCatoryName, resultData, DataMapList, headList);
    return DataMapList;
}

QMap<QString, QList<QStringList>>
BaseCommonApi::GetDataReturnItemListByCesiumAtomicFrequency(
    QString itemCatoryName, QList<QStringList> ResultData,
    QMap<QString, QList<QStringList>> DataMapList,
    SequentialMap<QString, QStringList>* HeadMapList)
{
    if (itemCatoryName == "铷钟") {
        QList<QStringList> outputsignalList;
        QList<QStringList> warm_upList;
        QList<QStringList> dc_voltageList;
        QList<QStringList> rise_timeList;
        QList<QStringList> sinusoid_flatnessList;
        QList<QStringList> harmonic_distortionList;
        QStringList outputsignalHead;
        QStringList warm_upHead;
        QStringList dc_voltageHead;
        QStringList rise_timelHead;
        QStringList sinusoid_flatnessHead;
        QStringList harmonic_distortionHead;
        for (const QStringList& row : ResultData) {
            QString VerificationItem = NULL;
            // state   data type
            if (!row.isEmpty() && row[13] == "1" && row[2] == "10") {
                VerificationItem = "输出信号";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "信号类型"
                     << "被测信号频率"
                     << "幅度"
                     << "脉冲宽度"
                     << "上升时间"
                     << "幅度技术指标"
                     << "脉冲宽度技术指标"
                     << "不确定度U(k=2)"
                     << "结论";
                outputsignalHead = head;
                // HeadMapList->insert(VerificationItem, head);
                //  添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[11] + row[12]); // 标称值
                Data.append(row[13] + row[14]); // 幅度
                Data.append(QString::number(row[11].toDouble() - (row[11].toDouble() * row[16].toDouble())) + row[12]); // 指标下限
                Data.append(""); // 测量值
                Data.append(QString::number(row[11].toDouble() + (row[11].toDouble() * row[16].toDouble())) + row[12]); // 指标上限
                Data.append(""); // 结论
                Data.append(row[20]); // 有效位数
                outputsignalList.append(Data);
            } else if (!row.isEmpty() && row[14] == "1" && row[2] == "11") {
                VerificationItem = "开机特性";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "被测信号频率"
                     << "闸门时间"
                     << "倍增次数"
                     << "开机时间"
                     << "相对频率偏差y(τ)"
                     << "技术指标"
                     << "不确定度U(k=2)"
                     << "结论";
                HeadMapList->insert(VerificationItem, head);
                // 添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[11] + row[12]); // 标称值
                Data.append(row[13] + row[14]); // 幅度
                Data.append(row[18] + row[19]); // 交流频率
                Data.append(QString::number(row[11].toDouble() - (row[11].toDouble() * row[16].toDouble())) + row[12]); // 指标下限
                Data.append(""); // 测量值
                Data.append(QString::number(row[11].toDouble() + (row[11].toDouble() * row[16].toDouble())) + row[12]); // 指标上限
                Data.append(""); // 结论
                // ac_voltageList.append(Data);
            } else if (!row.isEmpty() && row[15] == "1" && row[2] == "12") {
                VerificationItem = "频率复现性";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "被测信号频率"
                     << "闸门时间"
                     << "倍增次数"
                     << "T1"
                     << "T2"
                     << "T3"
                     << "相对频率偏差γ1(τ)"
                     << "相对频率偏差γ2(τ)"
                     << "频率复现性R"
                     << "技术指标"
                     << "不确定度U(k=2)"
                     << "结论";
                HeadMapList->insert(VerificationItem, head);
                // 添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[11] + row[12]); // 标称值
                Data.append(row[13] + row[14]); // 幅度
                Data.append(QString::number(row[11].toDouble() - row[16].toDouble()) + row[12]); // 指标下限
                Data.append(""); // 测量值
                Data.append(QString::number(row[11].toDouble() + row[16].toDouble()) + row[12]); // 指标上限
                Data.append(""); // 结论
                dc_voltageList.append(Data);
            } else if (!row.isEmpty() && row[16] == "1" && row[2] == "13") {
                VerificationItem = "相对频率偏差";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "被测信号频率"
                     << "闸门时间"
                     << "倍增次数"
                     << "开机时间"
                     << "相对频率偏差y(τ)"
                     << "技术指标"
                     << "不确定度U(k=2)"
                     << "结论";
                HeadMapList->insert(VerificationItem, head);
                // 添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[9] + row[10]); // 频率
                Data.append(row[13] + row[14]); // 幅度
                Data.append(""); // 上升时间
                Data.append(row[15] + row[16] + row[17]); // 技术指标
                Data.append(""); // 结论
                rise_timeList.append(Data);
            } else if (!row.isEmpty() && row[17] == "1" && row[2] == "14") {
                VerificationItem = "频率稳定度";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "被测信号频率"
                     << "取样时间"
                     << "采样组数"
                     << "频率稳定度σy(τ)"
                     << "技术指标"
                     << "不确定度uc"
                     << "结论";
                HeadMapList->insert(VerificationItem, head);
                // 添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[22] + row[23]); // 电压
                Data.append(row[9] + row[10]); // 频率点
                Data.append(""); // 测量值
                Data.append(""); // 平坦度
                Data.append(row[15] + row[16] + row[17]); // 技术指标
                Data.append(""); // 结论
                Data.append(row[21]); // 基准点
                sinusoid_flatnessList.append(Data);
            } else if (!row.isEmpty() && row[18] == "1" && row[2] == "15") {
                VerificationItem = "日频率漂移率";
                // 添加列头
                QStringList head;
                head << "序号"
                     << "检定项目"
                     << "被测信号频率"
                     << "闸门时间"
                     << "取样时间"
                     << "采样组数"
                     << "序号"
                     << "相对频率偏差y(τ)"
                     << "日漂移率K(计算值)"
                     << "相关系数r(计算值)"
                     << "日漂移率K(给出值)"
                     << "相关系数r(给出值)"
                     << "技术指标"
                     << "不确定度U(k=2)"
                     << "结论";
                HeadMapList->insert(VerificationItem, head);
                // 添加数据
                QStringList Data;
                Data.append(row[0]); // ID
                Data.append(VerificationItem); // 检定项目
                Data.append(row[9] + row[10]); // 频率点
                Data.append(""); // 测量值
                Data.append(row[15] + row[16] + row[17]); // 技术指标
                Data.append(""); // 结论
                harmonic_distortionList.append(Data);
            }
        }
    }

    return DataMapList;
}

//   【保存】核查项-小功率
//    static bool InsertStandardCheckItemDataLowFrequencySignal(StandardCheckDataLowFrequencySignal lowFrequencySignal);
StandardCheckInfo BaseCommonApi::getNewCheckResultData()
{
    StandardCheckInfo result;
    QSqlQuery query;
    query.prepare("select * from biz_equipment_standard_check_info as bz ORDER BY  bz.create_time DESC LIMIT 1");
    if (query.exec()) {
        while (query.next()) {
            result.id = query.value("id").toString();
            result.data_no = query.value("data_no").toString();
        }
    } else {
        qDebug() << "Query execution failed: " << query.lastError().text();
    }
    return result;
}

