#ifndef BASECOMMONAPI_H
#define BASECOMMONAPI_H

#include "QList"
#include "SequentialMap.h"
#include "databaseconnection.h"
#include "id\IdWorker.h"
#include "id\TaskManager.h"
#include <QDebug>
#include <QString>
#include <QtSql>
#include <vector>

class BaseClass {
public:
    virtual ~BaseClass() { } // 虚析构函数
};

// 部门任务
class Dept {
public:
    QString id;
    QString eId; //
    QString sample_name; // 设备名字
    QString sample_model; // 设备型号
    QString sample_id; // 设备id
    QString orderId;
    QString help_instruction;
    QString manufacture_no;
    QString manufacturer;
    QString customer_name;
    QString FULL_NAME;
    QDateTime require_over_time;
    QString is_urgent;
    QString measure_status;
    QString create_user_id;
};

// 我的任务
class MyTask {
public:
    QString id;
    QString sampleName;
    QString sampleModel;
    QString check_cycle;
    QString help_instruction;
    QString manufactureNo;
    QString manufacturer;
    QString helpInstruction;
    QString customer_name;
    QString FULL_NAME;
    QDateTime require_over_time;
    QString is_urgent;
    QString measure_status;
    QString orderId;
    QString orderNo;
    QString sample_id;
    QString sampleNo;
    QString customer_id;
    QString deliverer;
    QString require_certifications;
    QString customer_address;
    QString measure_person_id;
    QString labelBind;
    QString certificateValid;
    QString measureDeptId;
    QString measureDeptName;
};
// 创建任务单-首页
class CreateTaskList {
public:
    QString id;
    QString order_no;
    QString customer_name;
    QString DEPT_FULL_NAME;
    QDateTime require_over_time;
    QString deliverer;
    QString is_urgent;
};
// 创建任务单-查看任务单
class CatTask {
public:
    QString id;
    QString order_no;
    QString customer_name;
    QString DEPT_FULL_NAME;
    QDateTime require_over_time;
    QString deliverer;
    QString deliverer_tel;
    QString undertaker_name;
    QDateTime undertake_time;
    QString is_urgent;
    QString receive_illustrate;
};
// 创建任务单-查看任务单-任务单列表
class CatTaskList {
public:
    QString id;
    QString sample_name;
    QString sample_model;
    QString manufacture_no;
    QString manufacturer;
    QString appendix_descn;
    QString special_require;
    QDateTime measure_complete_time;
    QString conclusion;
    QString limit_instruction;
};
// 先查询 指定字段 即下拉框
class SelectSpecifyFields {
public:
    QString id; // 委托方名录表id和任务单 委托方id 对应
    QString customer_no; // 委托方编号
    QString customer_name; // 委托方名称
    QString FULL_NAME; // 使用部门 全称
};
// 查询所有委托方
class biz_resource_customer_info {
public:
    QString id; // 委托方名录表id和任务单 委托方id 对应
    QString customer_name; // 委托方名称
};

// 创建任务单
class InsertTask : public BaseClass {
public:
    QString id;
    QString order_no; //  任务单编号
    QString customer_name; // 委托方名称
    QString dept_name; // 使用部门
    QString require_over_time; // 要求检完时间
    QString deliverer; // 送检人
    QString deliverer_tel; // 电话
    QString undertaker_name; // 承接人   当前用户
    QString undertake_time; // 承接时间 当前时间
    int is_urgent; // 是否加急
    QString create_user_id;
    QString receive_illustrate; // 接收说明
    QString customer_id; // 委托人id
    QString is_on_site_check; // 是否现场检定
    QDateTime on_site_check_time; // 现场检定时间
    QString create_user; // 创建人
    QDateTime create_time; // 创建时间
    QDateTime customer_address; // 委托方地址
    QString measure_company; // 检定(校准)单位(固定值)  西昌实验室、海口实验室
    QString receive_status; // 接收状态 1.未接收2.已接收3.已取消4.检测完成5.已退回  默认已接收
};
// 新增任务单样品表
class OrderSampleRelation : public BaseClass {
public:
    QString id;
    QString order_id; // 对应任务单id
    QString sample_id; // 被检设备的id
    QString sample_name; // 设备名称
    QString sample_model; // 型号规格
    QString create_user_id; // 型号规格
    QDateTime measure_complete_time; // 检定完成时间
    QDateTime create_time;
    QDateTime update_time;
};
// 新增任务单信息记录表
class biz_business_lab_executive_info : public BaseClass {
public:
    QString id;
    QString order_id; // 委托单ID
    QString sample_id; // 设备ID
    QString measure_dept_id; // 部门ID
    QString measure_person_id; // 检定人ID
    QString create_user_id; // 用户ID
    QString measure_status; // 默认填3为检测中
    QString is_auto; // 默认填0为自动系统检测
    QDateTime create_time;
    QDateTime update_time;
};
// 创建任务单编辑
class UpdateTask {
public:
    QString order_no;
    QString customer_name;
    QString require_over_time;
    QString deliverer;
    QString deliverer_tel;
    QDateTime undertake_time;
    QString is_urgent;
    QString receive_illustrate;
    QString customer_id;
};
//-------------------
// 标准设备管理
class BizDevice {
public:
    QString id;
    QString equipment_name;
    QString model;
    QString manufacture_no;
    QString manufacturer;
    QString type;
    QString measure_param;
};
// 标准设备管理-查看设备信息
class CatDeviceInfor {
public:
    QString equipment_name; // 设备名称
    QString model; // 规格型号
    QString manufacture_no; // 出厂编号
    QString measure_valid_date; // 检定有效期
    QString product_country; // 生产国家
    QString manufacturer; // 生产厂家
    QString produce_date; // 生产日期
    QString purchase_date; // 购进日期
    QString unit_price; // 设备单价
    QString usage_status; // 使用状态
    QString quality_status; // 质量状况
    QString dept_name; // 所属部门
    QString director_name; // 负责人
    QString supplier_name; // 设备供方
    QString location; // 位置信息
    QString type; // 设备类型
    QString standard_type; // 标准类型
    QString category; // 设备分类
    QString level; // 重要等级
    QString measure_cycle; // 检定周期
    QString unpack_record_name; // 对应开箱记录
    QString remark; // 备注
};
// 标准设备管理-核查项目及核查点-直流稳压电源检定装置  待修改 2024年3月21日09:18:12
class CheckDCPower {
public:
    QString id;
    QString params;
    QString check_point;
    QString unit;
    QString resolution;
    QString urel;
    QString check_type;
    QString datum_point;
    QString cycle_number;
    QString resistance_value;
    QString resistance_value_unit;
    QString remark;
};
//---------------------

// 被检设备管理
class TestedDevice {
public:
    QString id;
    QString equipment_name;
    QString model;
    QString help_instruction;
    QString manufacture_no;
    QString manufacturer;
    QString FULL_NAME;
    QString customer_name;
    QString meter_identify;
    QString check_cycle;
    QString certificate_valid;
};
// 被检设备管理-查看设备
class TestedCatDevice {
public:
    QString equipment_name;
    QString model;
    QString help_instruction;
    QString manufacture_no;
    QString product_country;
    QString manufacturer;
    QString product_date;
    QString purchase_date;
    QString unit_price;
    QString usage_status;
    QString quality_condition;
    QString FULL_NAME;
    QString category;
    QString check_cycle;
    QString level;
    QString remark;
};
// 判断 哪个 装置表
class JudgeCalibrationTable {
public:
    QString id;
    QString belong_standard_equipment;
};

// 被检设备管理-检定项目及检定点-检定项多功能校准源
class VerMultiCal {
public:
    QString id;
    QString item_id; // 检定项id
    QString params; // 检定项（参数）
    QString capacity; // 量
    QString unit; // 单位
    QString frequency; // 频率
    QString frequency_unit; // 频率单位
    QString range_range; // 量程
    QString standard_value; // 标准值
    QString resolution; // 分辨力
    QString maximum_error; // 最大允许误差(公式)
    QString error_param_a; // 误差参数a
    QString error_param_b; // 误差参数b
    QString line_mode; // 两线/四线模式
    QString coil_enlarge; // 线圈放大
    QString indicating_value; // 示值
    QString accuracy_a; // 准确度a
    QString full_scale_value; // 指针式仪表满量程值
    QString full_scale_value_unit; // 指针式仪表满量程值单位
    QString measure_indication_value; // 被检表示值
    QString accuracy_level; // 准确度等级
    QDateTime create_time;
    QDateTime update_time;
};

// 检定数据管理 首页
class VerificationData {
public:
    QString id;
    QString item_id; // 设备检定项表id(一条检定数据对应一个检定项)
    QString sample_name; // 设备名称
    QString sample_id; // 被检设备id
    QString model; // 型号规格
    QString help_instruction; // 辅助字段
    QString manufacture_no; // 出厂编号
    QString manufacturer; // 生产厂家
    QString customer_name; // 委托单位
    QString FULL_NAME; // 使用部门
    QString NAME; // 检定员
    QDate trace_date; // 检定时间
    QDateTime measure_valid_date; // 检定有效期
    QString meter_identify; // 计量标识
    QString restriction_instruction; // 限用说明
};
// 检定数据管理-检定基本信息-标准设备
class VerificationBasicDataDevice {
public:
    QString equipmentId; // 设备id
    QString model; // 型号规格
    QString equipment_no; // 出厂编号
    QString manufacturer; // 生产厂家
    QString equipment_name; // 设备名称
    QString uncertainty; // 不确定度
    QDateTime measure_valid_date; // 检定有效期
};
// 检定数据管理-检定基本信息
class VerificationBasicDataDetails {
public:
    QString id;
    QString device_model_id; // 受检设备规格型号表id
    QString device_type; // 设备分类(eqptDeviceType字典code)
    QString belong_standard_equipment; // 检校标准装置(字典code)
    QString help_field_instruction; // 辅助字段说明
    QString item_category_id; // 设备检定项分类表id
    QString item_category_name; // 设备检定项分类名字
    QString data_sync; // 检定项数据是否同步(1/0)
    QString sync_time; // 自动检定系统最新同步时间
    QString appearance_function_check; // 外观及功能性检查(1/0)
    QString appearance_remark; // 说明
    QString category; //.设备分类(字典code)
    QString equipment_name; // 设备名称
    QString model; // 型号规格
    QString help_instruction; // 辅助说明
    QString item_remark; // 检定项备注
    QString customer_name; // 委托单位
    QString humidity; // 湿度
    QString temperature; // 温度
    QString measure_address; // 检定地点
    QDateTime measure_valid_date; // 检定有效期
    QString conclusion; // 结论
    QString restriction_instruction; // 限用说明
    QString based_documents; // 技术文件
    QDate trace_date; // 检定日期
    QString NAME; // 检定员
    QDateTime create_time;
    QDateTime update_time;
};

// 检定数据管理-检定结果信息 未使用  2024年3月21日09:24:38 使用的是QStringList
class VerificationResultData {
public:
    QString id;
    QString params; // 检定项目  即参数
    QString capacity; // 量
    QString unit; // 单位
    QString frequency; // 频率
    QString range_range; // 量程
    QString standard_value; // 标准值
    QString resolution; // 示值
    QString maximum_error; // 最大允许误差
    QString error_param_a; // 误差参数a
    QString error_param_b; // 误差参数b
    QString other_outcome; // 合格率
};

// 核查数据管理 首页
class CheckData {
public:
    QString id;
    QString equipmentName; // 设备名称
    QString equipment_id; // 设备id
    QString equipmentModel; // 型号规格
    QString manufacture_no; // 出厂编号
    QString meter_standard_name; // 核查件
    QString meter_standard_id; // 核查件编号
    QString create_user_name; // 核查员
    QDate check_date; // 核查日期
    QString dept_name; // 部门
    QString manufacturer; // 生产厂家
};
// 核查数据管理-核查基本信息
class CheckDataManageBasicInfo {
public:
    QString id;
    QString data_no; // 数据编号
    QString equipment_name; // 设备名称
    QString model; // 型号规格
    QString manufacture_no; // 出厂编号
    QString manufacturer; // 生产厂家
    QString temperature; // 温度
    QString humidity; // 湿度
    QDate check_date; // 核查日期
    QString check_address; // 核查地点   保存查询出实验室的id
    QString create_user_name; // 核查员    保存的用户表  ID
    QString conclusion; // 结论
    QString check_accord; // 核查依据(即标准装置的作业指导书文件minio文件名，多个分号分割)
    QString stability_examine; // 是否用于稳定性考核(1/0)
    QString standard_equipment_id; // 被核查标准装置id
    QString equipment_id; // 被核查设备id
    QString item_category_id; // 核查项分类id
    QString remark; // 备注
    QString create_user_id; // 核查员id
    QDateTime create_time;
    QDateTime update_time;
};

////核查数据管理-核查结果信息-核查项多功能校准源数据
// class CheckDataManageResultManageMult
//{
// public:
//     QString id;//序号
//     QString check_type;//核查类型
//     QString params;//参数
//     QString unit;//单位
//     QString frequency;//频率
//     QString check_point;//核查点
//     QDate test_value_one_date;//核查日期1
//     QString test_value_one;//核查读数1（示值）
//     QDate test_value_two_date;//核查日期2
//     QString test_value_two;//核查读数2（示值）
//     QDate test_value_three_date;//核查日期3
//     QString test_value_three;//核查读数3（示值）
//     QDate test_value_four_date;//核查日期4
//     QString test_value_four;//核查读数4（示值）
//     QDate test_value_five_date;//核查日期5
//     QString test_value_five;//核查读数5（示值）
//     QDate test_value_six_date;//核查日期6
//     QString test_value_six;//核查读数6（本次示值）
//     QString dccp;//dccp
//     QString flit;//flit
// };

// 技术文件
class Technique_File {
public:
    QString id; // id
    QString file_distribute_no; // 文件号
    QString file_name; // 文件名
    QString version_no; // 版本号
    QDateTime active_date; // 颁布时间
    QString remark; // 备注
};
//--------------------------
// 检定程序管理
class VerificationProgramInfo {
public:
    QString id;
    QString program_name; // 程序名称
    QString create_name; // 创建人
    QDateTime create_time;
    QString remark; // 说明
    QString img_path; // 连接图片地址
    QString tested_device_id; // 被检设备id 可为空         按照原型参考
    QString standard_device_id; // 标准设备id存数组
    QDateTime update_time; // 更新时间
    QString belong_standard_equipment; // 检校标准装置  *
    QString equipment_item_category; // 设备检定项分类   *
    QString equipment_name; // 设备名称 可为空
    QString model; // 型号规格 可为空
    QString help_instruction; // 辅助字段 可为空
    QString biz_standard_name; // 标准器的装置名称按，分割
    QString biz_standard_id; // 标准器的装置id按，分割
};

// 检定程序管理-检定配置查询  废弃
// class VerificationProgramConfig
//{
// public:
//     QString id;
//     QString verification_id; //检定程序id
//     QString img_path; //连接图片地址
//     QString tested_device_id; //被检设备id
//     QString standard_device_id; //标准设备id
//     QString equipment_name;//设备名称
//     QString model; //型号规格
//     QString help_instruction; //辅助字段
//     QString manufacture_no; //出厂编号
//     QString manufacturer; //生产厂家
// };
// 装置下的检定项目
class Params {
public:
    QString param;
    QString itemCategoryName;
};

// 检定程序管理-检定配置设备通信接口查询
class VerificationProgramConfigInterface {
public:
    QString id;
    QString device_id; // 设备id
    QString communication_type; // 通讯类型
    QString serial_port; // 串口
    QString visa; // visa
    QDateTime create_time;
    QDateTime update_time;
};
// 指令库
class InstructionLib {
public:
    QString id;
    QString program_id; // 检定程序id
    QString param; // 检定|核查项目
    QString device_id; // 设备id  可选
    QString device_name; // 被检设备名称 可选
    QString instruct_config; // 配置指令
    QString instruct_read; // 读数指令
    QString type; // 区分被检即检定程序和标准设备 0 & 1
    QString belong_vername; // 检定项名称 字典
    QString belong_checkname; // 核查项名称 字典
    QDateTime create_time;
    QDateTime update_time;
};
// 自动检定-结果保存-biz_business_device_measure_info 基本信息表
class SaveDeviceMeasureInfo : public BaseClass {
public:
    QString id; // id
    QString data_no; // 数据编号     jdsj+年月+当天顺序自动生成编号 例如：jdsj202310020001、jdsj202310020002
    QString order_id; // 任务单id
    QString sample_id; // 受检设备id
    QString item_id; // 具体检定项表的id   例如：多功能校准源检定项的id
    QDate trace_date; // 测试、校准或检定日期
    QDateTime measure_valid_date; // 检定有效期
    QString measure_address; // 测试、校准或检定地址
    QString temperature; // 温度
    QString humidity; // 湿度
    QString data_source; // 数据来源(自动检定系统/计量业务系统)
    QString outcome; // 结果(合格/不合格)
    QDateTime create_time;
    QDateTime update_time;
    QString measure_category; // 检校类别
    QString sample_name; // 受检设备名字
    QString model; // 受检设备型号
    QString manufacture_no; // 受检设备出厂编号
    QString manufacturer; // 受检设备生产厂家
    QString ipri; // 电流转换系统Ipri(多功能电力参数测量仪)-多功能校准源
    QString ipri_unit; // 电流转换系统Ipri单位(直接存字典value)-多功能校准源
    QString isec; // 电流转换系统Isec(多功能电力参数测量仪)-多功能校准源
    QString isec_unit; // 电流转换系统Isec单位(直接存字典value)-多功能校准源
    QString vpri; // 电压转换系统Vpri(多功能电力参数测量仪)-多功能校准源
    QString vpri_unit; // 电压转换系统Vpri单位(直接存字典value)-多功能校准源
    QString vsec; // 电压转换系统Vsec(多功能电力参数测量仪)-多功能校准源
    QString vsec_unit; // 电压转换系统Vsec单位(直接存字典value)-多功能校准源
    QString appearance_function_check; // 外观及功能性检查
    QString appearance; // 外观(1合格/0不合格)-0.02活塞式压力计
    QString appearance_remark; // 外观备注说明-0.02活塞式压力计
    QString pointer_deflection_stability; // 指针偏转平稳性(1/0)-0.02活塞式压力计-精密压力表
    QString pointer_deflection_stability_remark; // 指针偏转平稳性备注说明-0.02活塞式压力计-精密压力表
    QString tightness; // 密封性(1/0)-0.02活塞式压力计
    QString tightness_remark; // 密封性备注说明-0.02活塞式压力计
    QString insulation_resistance; // 绝缘电阻(1/0)-0.02活塞式压力计
    QString insulation_resistance_remark; // 绝缘电阻备注说明-0.02活塞式压力计
    QString measure_medium; // 检定介质(字典value)-0.02活塞式压力计
    QString division; // 分度值的五分之一-0.02活塞式压力计
    QString zero_drift_result; // 零位飘移结果（合格/不合格，直接存文字）-0.02活塞式压力计-数字压力表
    QString work_line_equation; // 工作直线方程(直接存字典value)-压力传感器
    QString sealing_test; // 密封试验(1/0)(安全阀)
    QString insulation_resistance_value; // 绝缘电阻(二等铂)
    QString angle_adjust_error; // 角度调整误差(二等铂)
    QString actual_division_value; // 实际分度值d
    QString actual_division_value_unit; // 实际分度值d单位
    QString conclusion; // 结论(合格/不合格/限用,用户手选)
    QString restriction_instruction; // 结果(合格/不合格)
    QString other_outcome; // 结果(检定数据中有2个表格情况下的第2个表中的合格/不合格)
    QString remark; // 备注/情况说明(安全阀)
    QString other_remark; // 备注(检定数据中有2个表格情况下的第2个表中的备注)
    QString meter_identify; // 计量标识(生成证书后需同步更新至受检设备)
    QString create_user_id; // 检定员id(系统用户id)
    QString measure_person_id; // 检定员id(人员id)
    QString create_user_name; // 创建用户名
};

// 自动检定-根据被检设备 查询检定项分类 在查询检定程序
class DeviceMeasureEquipmentCatory {
public:
    QString id; // 检定项id
    QString device_model_id; // 受检设备规格型号表id
    QString device_type; // 设备分类(eqptDeviceType字典code)
    QString belong_standard_equipment; // 检校标准装置(字典code)
    QString help_field_instruction; // 辅助字段说明
    QString item_category_id; // 设备检定项分类表id
    QString item_category_name; // 设备检定项分类名字
    QString deviceType; // 设备分类(字典code)
    QString deviceName; // 设备名称
    QString model; // 型号规格
    QString helpInstruction; // 辅助说明
    QString instructionsFile; // 说明书(minio存储文件名)（多个用,拼接）
    QString checkCycle; // 检定周期(月)
    QString eqptDeviceModelId; // 设备型号表id
    QString eqptDeviceType; // 设备类型(1通用设备;2特种设备)
};

// 自动检定-结果保存-检定数据管理多功能校准源数据(自动检定输出或业务创建)
class SaveDeviceMeasureCalibrator : public BaseClass {
public:
    QString id;
    QString data_id; // 检定数据管理基础信息表id
    QString item_data_id; // 检定项数据表id
    QString maximum_error_formula; // 最大允许误差(公式)
    QString error_param_a; // 误差参数a
    QString error_param_b; // 误差参数b
    QString params; // 参数
    QString capacity; // 量
    QString unit; // 单位
    QString frequency; // 频率
    QString frequency_unit; // 频率单位
    QString range_range; // 量程
    QString standard_value; // 标准值(输入字段,特殊:指针式万用表输出字段)
    QString indicating_value; // 示值(输出字段,特殊:指针式万用表输入字段)
    QString maximum_error; // 最大允许误差(值)
    QString absolute_error; // 绝对误差
    QString resolution; // 分辨力
    QString line_mode; // 两线/四线模式
    QString remark; // 输出备注
    QString an; // AN
    QString bn;
    QString cn;
    QString delta_an; // 德尔塔AN
    QString delta_bn;
    QString delta_cn;
    QString measure_indication_value; // 被检表示值
    QString standard_value_upper_limit; // 标准值上限
    QString accuracy_level; // 准确度等级
    QDateTime create_time;
    QDateTime update_time;
};
// 检定项0.02活塞式压力计数据(包含字典的字段都存为字典value)
class DeviceMeasureCalibratorPistonGauge {
public:
    QString id;
    QString item_id; // 检定项id
    QString indicating_value; // 标准器示值
    QString return_error; // 回程误差允许值
    QString friction_error; // 轻敲位移允许值
    QString indicating_error; // 示值误差允许值
    QString zero_position; // 零位是否带止销
    QString measure_medium; // 检定介质(字典value)
    QString accuracy_class; // 准确度等级
    QString maximum_error; // 最大允许误差
    QString theoretical_output_value; // 理论输出值
    QString theoretical_output_value_unit; // 理论输出值单位
    QDateTime create_time;
    QDateTime update_time;
    QString appearance_function_check;
    QString division;
    QString appearance;
    QString tightness;
    QString pointer_deflection_stability;
    QString insulation_resistance;
    QString zero_drift;
};

//-----------------------------------------------------
// 核查程序管理
class CheckProgramManage {
public:
    QString id; // id
    QString check_name; // 核查程序名称
    QString belong_standard_equipment_name; // 核查标准装置
    QString category_name; // 核查项分类名称
    QString create_name; // 创建人
    QDateTime create_time; // 创建时间
    QString remark; // 说明
    QString img_path; // 图片路径
    QString standard_device_id; // 标准设备id
    QString verification_documents_id; // 核查件设备id
    QDateTime update_time; // 更新时间
    QString belong_standard_equipment; // 核查标准装置 code
    QString equipment_type; // 核查项分类 id
    QString standard_id; // 被核查设备的装置id
};
// 核查程序管理-编辑查询 选择设备
class CheckAndTestedDevice {
public:
    QString id;
    QString equipment_name; // 设备名称
    QString model; // 型号规格
    QString manufacture_no; // 出厂编号
    QString manufacturer; // 生产厂家
};
// 核查项及核查点--多功能校准源  biz_equipment_standard_check_item_data_calibrator 未使用  使用的是QStringList
class VerificationItemsMulti {
public:
    QString id;
    QString item_category_id; // 核查项分类id(能够确定是哪个标准装置)
    QString equipment_id; // 配套设备id
    QString check_type; // 核查类型
    QString test_type; // 核查点类型(直接存字典value)
    QString params; // 核查项目(直接存字典value)
    QString unit; // 单位
    QString frequency; // 频率
    QString check_point; // 核查点
    QString resolution; // 分辨力
    QString frequency_unit; // 频率单位(直接存字典value)
    QString cycle_number; // 循环次数
    QString urel; // urel
    QString thoroughfare; // 通道(直接存字典value,多个分号分割)
    QString tfer; // tfer
    QString dccp; // dccp
    QString flit; // flit
    QString remark; // 核查项备注
    QDateTime create_time; // 创建时间
    QDateTime update_time; // 更新时间
};
// 设备与装置-标准装置管理-核查数据基本信息关联的核查标准设备，即给设备做核查的设备
class StandCheckInfoEquipent {
public:
    QString id;
    QString check_equipment_id; // 做核查标准设备id
    QString data_id; // 核查数据基本信息表id
};

// 核查结果信息数据保存-多功能校准源-设备与装置-标准装置管理-核查数据管理多功能校准源数据表
class StandCheckDataCalibrator {
public:
    QString id;
    QString data_id; // 核查数据管理基础信息表id
    QString check_type; // 核查类型(直接存字典value)
    QString test_type; // 核查点类型(直接存字典value)
    QString params; // 核查项目(直接存字典value)
    QString unit; // 单位(直接存字典value)
    QString frequency; // 频率
    QString frequency_unit; // 频率单位
    QString check_point; // 核查点
    QDate test_value_one_date; // 核查读数1日期（稳定性）
    QString test_value_one; // 核查读数1（示值）
    QDate test_value_two_date; // 核查读数2日期（稳定性）
    QString test_value_two; // 核查读数2（示值）
    QDate test_value_three_date; // 核查读数3日期（稳定性）
    QString test_value_three; // 核查读数3（示值）
    QDate test_value_four_date; // 核查读数4日期（稳定性）
    QString test_value_four; // 核查读数4（示值）
    QDate test_value_five_date; // 核查读数5日期（稳定性）
    QString test_value_five; // 核查读数5（示值）
    QDate test_value_six_date; // 核查读数6日期（稳定性）
    QString test_value_six; // 核查读数6（本次示值）
    QString average_value; // 核查读数平均值(算数平均值)
    QString standard_deviation; // Sn(x)(标准偏差)
    QString relative_repeatability; // Sn(x)/核查读数平均值(相对重复性)
    QString urel; // UREL
    QString less_than; // 是否小于相对扩展不确定度Urel(0/1)
    QString qualified; // 稳定性考核是否合格
    QDateTime create_time;
    QDateTime update_time;
};

// 【新版】核查数据表-biz_equipment_standard_check_data_piston_gauge 设备与装置-标准装置管理-核查数据管理0.02级活塞式压力计数据表
class StandardCheckDatePistonGauge {
public:
    QString id;
    QString data_id; // 核查数据管理基础信息表id
    QString unit; // 单位
    QString check_point; // 核查点
    QString first_forward_stroke; // 第一次正行程
    QString first_reverse_stroke; // 第一次反行程
    QString second_forward_stroke; // 第二次正行程
    QString second_reverse_stroke; // 第二次反行程
    QString third_forward_stroke; // 第三次正行程
    QString third_reverse_stroke; // 第三次反行程
    QString average_value; // 平均值
    QString average_average_value; // 测量标准示值平均值（电信号中）
    QString max_indicating_error; // 示值误差最大值
    QString max_return_error; // 回程误差最大值
    QString params; // 核查项目
    QString test_value_one; // 核查标准示值1
    QString measure_value_one; // 测量标准示值1
    QString test_value_two; // 核查标准示值2
    QString measure_value_two; // 测量标准示值2
    QString test_value_three; // 核查标准示值3
    QString measure_value_three; // 测量标准示值3
    QString test_value_four; // 核查标准示值4
    QString measure_value_four; // 测量标准示值4
    QString test_value_five; // 核查标准示值5
    QString measure_value_five; // 测量标准示值5
    QString test_value_six; // 核查标准示值6
    QString measure_value_six; // 测量标准示值6
    QString maximum_error; // 最大允许误差
    QDateTime create_time; // 创建时间
    QDateTime update_time; // 更新时间
};
// 实验室环境监控系统-地点表和环境表
class LocationInfo {
public:
    QString id;
    QString location_no; // 地点编号
    QString location_name; // 地点名称
    QString location_position; // 地点位置
    QString create_user_id; // 创建人id
    QString create_user; // 创建人
    QDateTime create_time;
    QDateTime update_time;
    QString location_lab; // 实验室
    QString temperature; // 温度
    QString humidity; // 湿度
    QString location_id; // 实验室地点id 环境表中
};

// 人员表  有需求在加字段
class User {
public:
    QString ID;
    QString NAME; // 名字
    QString DEPT_ID; // 部门id
};
// 计量检定业务-受检设备检定管理-检定数据管理-所使用的标准、主要测量设备  2024年3月21日10:26:01
class DeviceMeasureEquipment : public BaseClass {
public:
    QString id;
    QString data_id; // 检定数据管理基础信息表id
    QString equipment_id; // 设备台账id
    QString standard_id; // 标准装置id
    QString standard_name; // 标准装置名称
    QDateTime create_time;
    QDateTime update_time;
};
// 资源管理-人员管理基础信息
class StaffInfo {
public:
    QString id;
    QString account; // 登录账号
    QString staff_no; // 人员编号
    QString staff_name; // 姓名
    QString picture_file; // 照片
    QString id_card; // 身份证
    QString birthday; // 生日
    QString native_place; // 籍贯
    QString education; // 学历
    QString record_date; // 建档时间
    QString officer_no; // 军官证号
    QString gender; // 新别
    QString graduation_date; // 毕业时间
    QString nation; // 民族
    QString degree; // 学位
    QString dept_id; // 部门id
    QString dept_name; // 部门名字
    QString station; // 岗位
    QString major; // 计量专业
    QString certificate_number; // 证号
    QString engage_date; // 从事时间
    QString working_age; // 干龄
    QString title_initial_date; // 职称 初评
    QString remark; // 备注
    QDateTime create_time;
    QDateTime update_time;
};

// 计量检定业务-受检设备检定管理-检定数据管理检定数据历史修改记录
class MeasureModifyRecord : public BaseClass {
public:
    QString id;
    QString data_id; // 检定数据管理基础信息表id
    QString source_system; // 计量业务管理系统/自动检定系统
    QString modify_content; // 修改内容
    QString create_user_id; // 创建用户id
    QString create_user_name; // 创建用户名
    QDateTime create_time; // 创建时间
    QDateTime update_time; // 更新时间
};
// 设备与装置-标准装置管理-核查数据管理核查数据历史修改记录   与 MeasureModifyRecord 一致 复用

// 检定项分类
class itemCategory {
public:
    QString id;
    QString category_no; // 分类编号
    QString category_name; // 分类名称
    QString device_type; // 设备分类(eqptDeviceType字典code)
    QString belong_standard_equipment; // 检校标准装置(字典code)
    QString measure_category; // 检校类别(字典code)
    QString technology_file; // 依据技术文件(minio存储文件名，分号分割)
    QString lab_x; // 选择西昌实验室时西昌实验室名字
    QString lab_h; // 选择海口实验室时海口实验室名字
    QString wiring_diagram; // 接线图(minio存储文件名)
    QString wiring_illustrate; // 接线说明
    QDateTime create_time;
    QDateTime update_time;
};

// 数据同步-日志表
class zdLog {
public:
    QString id;
    QString is_sync; // 是否同步成功
    QDateTime sync_time; // 同步时间
    QString remark; // 备注（同步失败原因）
    QString userid; // 用户id
    QString type; // 类型(0上传，1下载)
    QDateTime create_time;
    QDateTime update_time;
};

// 数据同步-数据表
class zdDataSync {
public:
    QString id; // id
    QString type; // sql类型
    QString ssql; // sql代码
    QString userid; // 用户id
    QString remark; // 备注
    QDateTime create_time;
    QDateTime update_time;
    QString log_sync_id; // 日志表id
    QString sync_type; // 0未同步，1成功,2失败
};

// 计量检定业务-受检设备检定管理-检定数据管理0.02级活塞式压力计数据(自动检定输出或业务创建)
class DeviceMeasureDataPistonGauge : public BaseClass {
public:
    QString id;
    QString data_id; // 检定数据管理基础信息表id
    QString data_type; // 检定数据类型(电接点一般压力表一个检定数据中区分两个表格)(字典code)
    QString data_category; // 检定数据类别(1检定数据，2计算结果数据)
    QString indicating_value; // 标准器示值
    QString before_friction_forward_stroke; // 轻敲前被检设备示值-正行程
    QString before_friction_reverse_stroke; // 轻敲前被检设备示值-反行程
    QString after_friction_forward_stroke; // 轻敲后被检设备示值-正行程
    QString after_friction_reverse_stroke; // 轻敲后被检设备示值-反行程
    QString variation_forward_stroke; // 轻敲前后变动量-正行程
    QString variation_reverse_stroke; // 轻敲前后变动量-反行程
    QString theoretical_output_value; // 理论输出值
    QString first_forward_stroke; // 第一次正行程
    QString first_reverse_stroke; // 第一次反行程
    QString second_forward_stroke; // 第二次正行程
    QString second_reverse_stroke; // 第二次反行程
    QString third_forward_stroke; // 第三次正行程
    QString third_reverse_stroke; // 第三次反行程
    QString indicating_error; // 示值误差
    QString return_error; // 回程误差
    QString average_forward_stroke; // 正行程平均值
    QString average_reverse_stroke; // 反行程平均值
    QString average_value; // 平均值
    QString average_return_error; // 回程误差平均值
    QString item; // 项目(结果表)
    QString max_indicating_error; // 示值误差最大值(结果表)
    QString max_return_error; // 回程误差最大值(结果表)
    QString max_friction_error; // 轻敲位移最大值(结果表)
    QString maximum_error_absolute; // 最大允许误差绝对值
    QString forward_stroke_switch_value; // 正行程切换值(电接点一般压力表)
    QString reverse_stroke_switch_value; // 反行程切换值(电接点一般压力表)
    QString point_deviation; // 设定点偏差(电接点一般压力表)
    QString different_gap; // 切换差(电接点一般压力表)
    QString max_point_deviation; // 最大设定点偏差绝对值(结果表)(电接点一般压力表的第2个结果表)
    QString allow_point_deviation; // 允许设定点偏差绝对值(结果表)(电接点一般压力表的第2个结果表)
    QString max_different_gap; // 最大切换差(结果表)(电接点一般压力表的第2个结果表)
    QString allow_different_gap; // 允许切换差(结果表)(电接点一般压力表的第2个结果表)
    QString zero_drift_one; // 零位漂移(数字压力计手工输入的第1个示值)
    QString zero_drift_two; // 零位漂移(数字压力计手工输入的第2个示值)
    QString zero_drift_three; // 零位漂移(数字压力计手工输入的第3个示值)
    QString zero_drift_four; // 零位漂移(数字压力计手工输入的第4个示值)
    QString zero_drift_five; // 零位漂移(数字压力计手工输入的第5个示值)
    QString intercept_a; // 截距a(结果表)
    QString slope_b; // 斜率b(结果表)
    QString sensitivity_b; // 灵敏度b(结果表)
    QString repeatability_r; // 重复性ξR(结果表)
    QString hysteresis_h; // 迟滞ξH(结果表)
    QString linear_l; // 线性ξL(结果表)
    QString basic_error_a; // 基本误差A(结果表)
    QString periodic_stability_s_b; // 周期稳定性Sb(结果表)
    QDateTime create_time;
    QDateTime update_time;
};
// 检定项-直流稳压电源
class DeviceMeasureItemDataDcPower {
public:
    QString id;
    QString item_id; // 检定项id
    QString data_type; // 检定项数据类型(一个检定项中区分两个表格)(字典code)
    QString data_type_type; // 检定项数据类型的类型(一个检定项中一个表格切换的类型)(字典code)
    QString output_channel; // 输出通道
    QString unit; // 单位
    QString voltage_regulator_output_value; // 调压器输出值
    QString technical_index; // 被检电源技术指标/仪器技术指标
    QString technical_index_symbol; // 被检电源技术指标/仪器技术指标前符号
    QString load_situation; // 负载情况
    QString measure_indication_value; // 被检表示值
    QString full_scale_value; // 指针式仪表满量程值
    QString standard_resistance_value; // 标准电阻值
    QString standard_resistance_value_unit; // 标准电阻值单位
    QString error_param_a; // 误差参数a
    QString accuracy_a; // 准确度a
    QString error_param_b; // 误差参数b
    QString maximum_error; // 最大允许误差
    QString resolution; // 分辨力
    QDateTime create_time;
    QDateTime update_time;
};
// 结果保存-直流稳压电源 - 查询
class bizBusinessDeviceMeasureDataDcPower : public BaseClass {
public:
    QString id;
    QString data_id; // 检定数据管理基础信息表id
    //    dataType: 1电源电压调整率,2负载调整率,3电压示值误差,4电流示值误差(直接测量),5电流示值误差(间接测量),6纹波电压,7输出电压短期稳定性
    QString data_type; // 检定数据类型(一个检定数据中区分多个表格)(字典code)
    //    dataTypeType: 1数字式、2指针式
    QString data_type_type; // 检定项数据类型的类型(一个检定项中一个表格切换的类型)(字典value)
    QString output_channel; // 输出通道
    QString unit; // 单位
    QString voltage_regulator_output_value; // 调压器输出值
    QString voltage_max_difference; // 被检电源输出电压最大差值
    QString technical_index; // 被检电源技术指标/仪器技术指标
    QString technical_index_symbol; // 被检电源技术指标/仪器技术指标前符号
    QString voltage_regulation; // 电源电压调整率
    QString load_situation; // 负载情况
    QString voltage_output_value; // 被检电源输出电压值
    QString voltage_difference; // 电压差值
    QString load_regulation; // 负载调整率
    QString measure_indication_value; // 被检表示值
    QString standard_value; // 标准值
    QString maximum_error; // 最大允许误差
    QString maximum_error_unit; // 最大允许误差单位
    QString absolute_error; // 绝对误差
    QString resolution; // 分辨力
    QString standard_voltage_value; // 标准电压值
    QString standard_voltage_value_unit; // 标准电压值单位
    QString standard_resistance_value; // 标准电阻值
    QString standard_resistance_value_unit; // 标准电阻值单位
    QString ripple_voltage; // 纹波电压
    QString output_voltage_max_change; // 输出电压最大变化值
    QString short_term_stability; // 短期稳定性
    QString exceed_mark; // 超出范围标*(1/0)
    QString create_time;
    QString update_time;
};
// 结果保存-微波衰减器-查询
class MicrowaveAttenuationCalibration : public BaseClass {
public:
    QString id;
    QString data_id; // 检定数据管理基础信息表id
    QString data_type; // 检定数据类型(一个检定数据中区分两个表格)(字典code)
    QString frequency;
    QString frequency_unit;
    QString nominal_value;
    QString nominal_value_unit;
    QString composition;
    QString direct_measure_value;
    QString add_measure_value;
    QString attenuation_amount;
    QString lower_index;
    QString upper_index;
    QString urel;
    QString conclusion;
    QString exceed_mark;
    QDateTime create_time;
    QDateTime update_time;
};
// 结果保存-小功率-查询
class SmallPowerCalibration : public BaseClass {
public:
    QString id;
    QString data_id; // 检定数据管理基础信息表id
    QString data_type; // 检定数据类型(一个检定数据中区分两个表格)(字典code)
    QString frequency;
    QString frequency_unit;
    QString pcu;
    QString kc;
    QString probe_pbu;
    QString standard_reflection_coefficient;
    QString device_reflection_coefficient;
    QString probe_calibration_factor;
    QString urel;
    QString exceed_mark;
    QDateTime create_time;
    QDateTime update_time;
};
// 结果保存-低频信号-查询
class LFSignalGeneratoCalibration : public BaseClass {
public:
    QString id;
    QString data_id; // 检定数据管理基础信息表id
    QString data_type; // 检定数据类型(一个检定数据中区分两个表格)(字典code)
    QString frequency;
    QString frequency_unit;
    QString nominal_value;
    QString unit;
    QString lower_index;
    QString upper_index;
    QString urel;
    QString measure_value;
    QString measure_value_unit;
    QString conclusion;
    QString convert_deviation;
    QString voltage;
    QString voltage_unit;
    QString rise_time;
    QString datum_point;
    QString ac_frequency;
    QString ac_frequency_unit;
    QString amplitude;
    QString amplitude_unit;
    QString flatness;
    QString technical_index_symbol;
    QString technical_index;
    QString technical_index_unit;
    QString exceed_mark;
    QDateTime create_time;
    QDateTime update_time;
};
// 结果保存-示波器装置-查询
class DeviceOscilloscopeCalibration : public BaseClass {
public:
    QString id;
    QString data_id; // 检定数据管理基础信息表id
    // 1时基 2直流增益 3直流偏置 4频带宽度 5触发灵敏度 6上升时间 7输入电阻 8校准信号
    QString data_type; // 检定数据类型(一个检定数据中区分两个表格)(字典code)
    QString cycle_signal; // 周期时标信号（方法一）/ 参考输出（方法二）
    QString cycle_signal_unit; // 周期时标信号单位（方法一）/ 参考输出单位（方法二）
    QString time_base_absolute_error; // 时基绝对误差（方法一）/ 差拍波形频率测量值（方法二）
    QString time_base_absolute_error_unit; // 时基绝对误差单位（方法一）/ 差拍波形频率测量值单位（方法二）
    QString time_base_relative_error; // 时基相对误差
    QString time_base_relative_error_unit; // 时基相对误差单位
    QString technical_index_symbol; // 技术指标前符号
    QString technical_index; // 技术指标
    QString technical_index_unit; // 技术指标单位
    QString conclusion; // 结论
    QString thoroughfare; // 通道
    QString impedance; // 阻抗
    QString impedance_unit; // 阻抗单位
    QString vertical_deflect_coefficient; // 垂直偏转系数
    QString vertical_deflect_coefficient_unit; // 垂直偏转系数单位
    QString standard_value; // 标准值
    QString standard_value_unit; // 标准值单位
    QString measure_value; // 测量值
    QString measure_value_unit; // 测量值单位
    QString relative_error; // 相对误差
    QString error_value; // 误差
    QString band_width; // 频带宽度
    QString band_width_unit; // 频带宽度单位
    QString reference_amplitude; // 参考幅度
    QString reference_amplitude_unit; // 参考幅度单位
    QString measure_amplitude; // 测量幅度
    QString measure_amplitude_unit; // 测量幅度单位
    QString decibel_number; // 分贝数
    QString trigger_type; // 触发类型(字典value)
    QString signal_frequency; // 信号频率
    QString signal_frequency_unit; // 信号频率单位
    QString item_type; // 项目(字典value)
    QString nominal_value; // 标称值
    QString nominal_value_unit; // 标称值单位
    QString top_measure_value; // 顶部测量值
    QString top_measure_value_unit; // 顶部测量值单位
    QString bottom_measure_value; // 底部测量值
    QString bottom_measure_value_unit; // 底部测量值单位
    QString scan_type; // 扫描方式(模拟示波器)(字典value)
    QString horizontal_scale_gear; // 水平刻度档位(模拟示波器)
    QString horizontal_scale_gear_unit; // 水平刻度档位单位(模拟示波器)
    QString vertical_scale_gear; // 垂直刻度档位(模拟示波器)
    QString vertical_scale_gear_unit; // 垂直刻度档位单位(模拟示波器)
    QString frequency_upper; // 频率上限(模拟示波器)
    QString frequency_upper_unit; // 频率上限单位(模拟示波器)
    QString output_signal_amplitude; // 输出信号幅度
    QString rise_time; // 上升时间
    QString correct_rise_time; // 修正上升时间
    QString signal_type; // 信号类型(模拟示波器)(字典value)
    QString nominal_amplitude_value; // 幅度标称值(模拟示波器)
    QString nominal_amplitude_value_unit; // 幅度标称值单位(模拟示波器)
    QString nominal_frequency_value; // 频率标称值(模拟示波器)
    QString nominal_frequency_value_unit; // 频率标称值单位(模拟示波器)
    QString measure_amplitude_value; // 幅度测量值(模拟示波器)
    QString measure_amplitude_value_unit; // 幅度测量值单位(模拟示波器)
    QString measure_frequency_value; // 频率测量值(模拟示波器)
    QString measure_frequency_value_unit; // 频率测量值单位(模拟示波器)
    QString frequency_deviation; // 频率偏差(模拟示波器)
    QString amplitude_deviation; // 幅度偏差(模拟示波器)
    QString exceed_mark; // 超出范围标*(1/0)
    QDateTime create_time;
    QDateTime update_time;
};
// 结果保存-二等铂电阻-查询
class DeviceMeasureDataResistanceThermometer : public BaseClass {
public:
    QString id;
    QString data_id; // 检定数据管理基础信息表id
    QString data_type; // 检定数据类型(一个检定数据中区分两个表格)(字典code)
    QString data_category; // 检定数据类别(1检定数据，2计算结果数据)
    QString upper_lower_point; // 上/下限点
    QString location; // 位置
    QString unit; // 单位
    QString one_value; // 示值1(波动性测试)/ROA1(均匀性测试)/标准器示值1(工业铂铜热电阻)
    QString two_value; // 示值2(波动性测试)/RA1(均匀性测试)/被检示值1(工业铂铜热电阻)
    QString three_value; // 示值3(波动性测试)/ROA2(均匀性测试)/标准器示值2(工业铂铜热电阻)
    QString four_value; // 示值4(波动性测试)/RA2(均匀性测试)/被检示值2(工业铂铜热电阻)
    QString five_value; // 示值5(波动性测试)/ROA3(均匀性测试)/标准器示值3(工业铂铜热电阻)
    QString six_value; // 示值6(波动性测试)/RA3(均匀性测试)/被检示值3(工业铂铜热电阻)
    QString seven_value; // 示值7(波动性测试)/ROA4(均匀性测试)/标准器示值4(工业铂铜热电阻)
    QString eight_value; // 示值8(波动性测试)/RA4(均匀性测试)/被检示值4(工业铂铜热电阻)
    QString nine_value; // 示值9(波动性测试)/ROA均值(均匀性测试)/标准器示值5(工业铂铜热电阻)
    QString ten_value; // 示值10(波动性测试)/RA均值(均匀性测试)/被检示值5(工业铂铜热电阻)
    QString eleven_value; // 示值11(波动性测试)/标准器示值6(工业铂铜热电阻)
    QString twelve_value; // 示值12(波动性测试)/被检示值6(工业铂铜热电阻)
    QString thirteen_value; // 示值13(波动性测试)/标准器示值均值(工业铂铜热电阻)
    QString fourteen_value; // 示值14(波动性测试)/被检示值均值(工业铂铜热电阻)
    QString fifteen_value; // 示值15(波动性测试)
    QString max_value; // 最大值
    QString min_value; // 最小值
    QString volatility; // 波动性
    QString upper_temperature_different; // 上水平面温差
    QString lower_temperature_different; // 下水平面温差
    QString max_horizontal_temperature_different; // 水平最大温差
    QString max_work_temperature_different; // 工作区域最大温差
    QString calibrated_point; // 校准点
    QString indicating_value_forward_stroke; // 标准器示值正行程
    QString indicating_value_reverse_stroke; // 标准器示值反行程
    QString measure_indication_value_forward_stroke; // 被检示值正行程
    QString measure_indication_value_reverse_stroke; // 被检示值反行程
    QString indicating_error_forward_stroke; // 示值误差正行程
    QString indicating_error_reverse_stroke; // 示值误差反行程
    QString maximum_error; // 最大允许误差
    QString urel; // 扩展不确定度
    QString rank_level; // 级别(字典value)
    QString nominal_temperature; // 标称温度
    QString item; // 项目(结果表)
    QString allow_error_lower; // 允差下限(结果表)
    QString allow_error_upper; // 允差上限(结果表)
    QString measure_value; // 测量值(结果表)
    QDateTime create_time;
    QDateTime update_time;
};
// 结果保存-频谱分析仪-查询
class DeviceMeasureDataSpectrumAnalyzer : public BaseClass {
public:
    QString id;
    QString data_id; // 检定数据管理基础信息表id
    // 1:频率显示2：扫频宽度3：中频带宽4：中频带宽转换偏差5：参考电平6：垂直刻度7：输入频响
    // 8：平均噪声电平9：谐波失真10：输入衰减11：1db增益压缩点12：校准信号
    QString data_type; // 检定数据类型(一个检定数据中区分两个表格)(字典code)
    QString nominal_value; // 标称值
    QString unit; // 标称值单位
    QString sweep_width; // 扫频宽度
    QString sweep_width_unit; // 扫频宽度单位
    QString horizontal_resolution; // 水平分辨率
    QString horizontal_resolution_unit; // 水平分辨率单位
    QString resolution_bandwidth; // 分辨率带宽
    QString resolution_bandwidth_unit; // 分辨率带宽单位
    QString one_value; // 测量值1(频率显示/输入频响/1dB增益压缩点)/fl1(扫频宽度/中频带宽)/测量值(中频带宽转换偏差/参考电平/平均噪声电平/输入衰减)/基波幅度(谐波失真)  (注：7个值的单位取标称值单位/指标上下限单位/固定前端页面补充单位)
    QString two_value; // 测量值2(频率显示/输入频响)/fr1(扫频宽度/中频带宽)/二次谐波幅度(谐波失真)
    QString three_value; // 测量值3(频率显示/输入频响)/fl2(扫频宽度/中频带宽)/二次谐波失真(谐波失真)
    QString four_value; // fr2(扫频宽度)/三次谐波幅度(谐波失真)
    QString five_value; // fl3(扫频宽度)/三次谐波失真(谐波失真)
    QString six_value; // fr3(扫频宽度)/四次谐波幅度(谐波失真)
    QString seven_value; // 四次谐波失真(谐波失真)
    QString eight_value; // Δf1
    QString nine_value; // Δf2
    QString ten_value; // Δf3
    QString upper_index_twice; // 二次谐波失真指标上限
    QString upper_index_twice_unit; // 二次谐波失真指标上限单位
    QString upper_index_thrice; // 三次谐波失真指标上限
    QString upper_index_thrice_unit; // 三次谐波失真指标上限单位
    QString upper_index_quartic; // 四次谐波失真指标上限
    QString upper_index_quartic_unit; // 四次谐波失真指标上限单位
    QString lower_index; // 指标下限/下限(输入衰减/1dB增益压缩点)
    QString lower_index_unit; // 指标下限单位/下限单位(输入衰减/1dB增益压缩点)
    QString average_value; // 均值
    QString upper_index; // 指标上限/上限(输入衰减/1dB增益压缩点)
    QString upper_index_unit; // 指标上限单位/上限单位(输入衰减/1dB增益压缩点)
    QString conclusion; // 结论
    QString convert_deviation; // 转换偏差(单位固定前端页面补充)
    QString lower_convert_deviation; // 转换偏差下限(单位固定前端页面补充)
    QString upper_convert_deviation; // 转换偏差上限(单位固定前端页面补充)
    QString standard_attenuator_attenuation; // 标准衰减器衰减量
    QString standard_attenuator_attenuation_unit; // 标准衰减器衰减量单位
    QString signal_source_amplitude; // 信号源幅度
    QString signal_source_amplitude_unit; // 信号源幅度单位
    QString vertical_scale; // 垂直刻度
    QString frequency; // 频率
    QString frequency_unit; // 频率单位
    QString input_frequency; // 输入频响(单位固定前端页面补充)
    QString start_frequency; // 起始频率
    QString start_frequency_unit; // 起始频率单位
    QString end_frequency; // 终止频率
    QString end_frequency_unit; // 终止频率单位
    QString revised_result; // 修正后结果
    QString input_attenuator; // 输入衰减器
    QString input_attenuator_unit; // 输入衰减器单位
    QString input_attenuator_conversion_effect; // 输入衰减器转换影响
    QString spectrum_level; // 参考电平
    QString spectrum_level_unit; // 参考电平单位
    QString gain_compression; // 增益压缩
    QString exceed_mark; // 超出范围标*(1/0)/二次谐波失真(谐波失真)
    QString exceed_mark_one; // 超出范围标*(1/0) 三次谐波失真(谐波失真)
    QString exceed_mark_two; // 超出范围标*(1/0) 四次谐波失真(谐波失真)
    QString measure_result; // 测量结果(校准信号)
    QString measure_result_unit; // 测量结果单位(校准信号)
    QString technical_index; // 技术指标
    QString technical_index_left; // 技术指标左
    QString technical_index_right; // 技术指标右
    QString corrected_value; // 修正值
    QString type_value; // 类型
    QDateTime create_time;
    QDateTime update_time;
};
// 结果保存-信号发生器-查询
class DeviceMeasureDataSignalGenerator : public BaseClass {
public:
    QString id; //
    QString data_id; // 检定数据管理基础信息表id
    QString data_type; // 检定数据类型(一个检定数据中区分两个表格)(字典code)
    QString nominal_value; // 标称值
    QString unit; // 标称值单位
    QString upper_index; // 指标上限
    QString upper_thd; // THD上限
    QString lower_index; // 指标下限
    QString measure_value; // 测量值
    QString measure_value_unit; // 测量值单位
    QString relative_error; // 相对误差
    QString urel; // U(k=2)
    QString frequency; // 频率
    QString frequency_unit; // 频率单位
    QString power; // 功率
    QString power_unit; // 功率单位
    QString amplitude; // 幅度
    QString amplitude_unit; // 幅度单位
    QString high_pass_filter; // 高通滤波
    QString high_pass_filter_unit; // 高通滤波单位
    QString lower_pass_filter; // 低通滤波
    QString lower_pass_filter_unit; // 低通滤波单位
    QString modulation_rate; // 调制速率
    QString modulation_rate_unit; // 调制速率单位
    QString thd; // THD
    QString upper_index_symbol; // THD上限符号
    QString upper_index_unit; // THD上限单位
    QString remain_amplitude_modulation; // 剩余调幅/剩余调频
    QString harmonic_number; // 谐波次数
    QString fundamental_amplitude; // 基波幅度
    QString fundamental_amplitude_unit; // 基波幅度单位
    QString harmonic_amplitude; // 谐波幅度
    QString harmonic_amplitude_unit; // 谐波幅度单位
    QString offset_frequency; // 偏移频率
    QString offset_frequency_unit; // 偏移频率单位
    QString exceed_mark; // 超出范围标*(1/0)
    QString exceed_mark_one; // 超出范围标*(1/0)(第二个需要标*的)
    QDateTime create_time;
    QDateTime update_time;
};
// 检定项数据-s参数
class DeviceMeasureDataSParam {
public:
    QString id;
    QString data_id; // 检定数据管理基础信息表id
    QString data_type; // 检定数据类型(一个检定数据中区分两个表格)(字典code)
    QString frequency; // 频率
    QString frequency_unit; // 频率单位
    QString output_power; // 输出功率
    QString output_power_unit; // 输出功率单位
    QString test_value_one; // 测量值1/耦合性1
    QString other_value_one; // 另一测量值1/方向性1
    QString test_value_two; // 测量值2/耦合性2
    QString other_value_two; // 另一测量值2/方向性2
    QString test_value_three; // 测量值3/耦合性3
    QString other_value_three; // 另一测量值3/方向性3
    QString test_value_four; // 测量值4/耦合性4
    QString other_value_four; // 另一测量值4/方向性4
    QString test_value_five; // 测量值5/耦合性5
    QString other_value_five; // 另一测量值5/方向性5
    QString test_value_six; // 测量值6/耦合性6
    QString other_value_six; // 另一测量值6/方向性6
    QString test_value_seven; // 测量值7/耦合性7
    QString other_value_seven; // 另一测量值7/方向性7
    QString test_value_eight; // 测量值8/耦合性8
    QString other_value_eight; // 另一测量值8/方向性8
    QString test_value_nine; // 测量值9/耦合性9
    QString other_value_nine; // 另一测量值9/方向性9
    QString test_value_ten; // 测量值10/耦合性10
    QString other_value_ten; // 另一测量值10/方向性10
    QString group_delay_unit; // 群延时单位
    QString urel; // U(k=2)/增益U(k=2)/耦合性U(k=2)/输出功率U(k=2)
    QString gain_flatness; // 增益平坦度(dB)/3dB带宽U(k=2)
    QString flatness_urel; // 增益平坦度U(k=2)/方向性U(k=2)/60dB带宽U(k=2)/输出功率平坦度U(k=2)
    QString average_value; // 平均值/耦合性平均值
    QString directivity_average_value; // 方向性平均值
    QString upper_frequency; // 上边频频率
    QString upper_frequency_unit; // 上边频频率单位
    QString lower_frequency; // 下边频频率
    QString lower_frequency_unit; // 下边频频率单位
    QString passband_upper_frequency; // 通带3dB功率点上边频频率
    QString passband_lower_frequency; // 通带3dB功率点下边频频率
    QString passband_width; // 通带3dB宽度Bw
    QString passband_upper_frequency_two; // 通带60dB功率点上边频频率
    QString passband_lower_frequency_two; // 通带60dB功率点下边频频率
    QString passband_width_two; // 通带60dB宽度Bw
    QString min_loss; // 通带最小损耗
    QString max_loss; // 阻带最大损耗
    QString min_loss_urel; // 通带最小损耗U(k=2)
    QString max_loss_urel; // 阻带最大损耗U(k=2)
    QString output_power_flatness; // 输出功率平坦度
    QString spectrum_level; // 参考电平
    QString spectrum_level_unit; // 参考电平单位
    QString intermediate_frequency; // 中频频率
    QString intermediate_frequency_unit; // 中频频率单位
    QString resolution_bandwidth; // 分辨带宽
    QString resolution_bandwidth_unit; // 分辨带宽单位
    QString fundamental_frequency; // 基波频率
    QString fundamental_frequency_unit; // 基波频率单位
    QString mixed_frequency; // 杂波频率
    QString mixed_frequency_unit; // 杂波频率单位
    QString fundamental_amplitude; // 基波幅度
    QString fundamental_amplitude_unit; // 基波幅度单位
    QString mixed_amplitude; // 杂波幅度
    QString mixed_amplitude_unit; // 杂波幅度单位
    QString spurious_suppression; // 杂散抑制
    QString amplitude; // 幅度
    QString amplitude_unit; // 幅度单位
    QString harmonic_number; // 谐波次数
    QString harmonic_amplitude; // 谐波幅度
    QString harmonic_amplitude_unit; // 谐波幅度单位
    QString harmonic_distortion; // 谐波失真
    QString random_noise_correct_value; // 随机噪声修正值
    QString random_noise_correct_value_unit; // 随机噪声修正值单位
    QString sideband_frequency; // 边带频率
    QString sideband_frequency_unit; // 边带频率单位
    QString carrier_level; // 载波电平
    QString sideband_level; // 边带电平
    QString ssb_hase_noise; // 单边带相位噪声
    QString type_value; // 类型
    QString vswr; // VSWR
    QString standard_value; // 标准值
    QString standard_value_value; // 标准值单位
    QString technical_index_symbol; // 技术指标前符号
    QString technical_index; // 技术指标
    QString technical_index_unit; // 技术指标单位
    QString error_value; // 误差
    QString exceed_mark; // 超出范围标*(1/0)
    QDateTime create_time;
    QDateTime update_time;
};

// 角色
class DeviceRole {
public:
    QString id; // 角色表id
    QString belong_vername; // 检定装置名称字典
    QString belong_checkname; // 核查装置名称字典
    QString cateory_id; // 分类id
    QString device_id; // 设备id
    QString device_type; // 被检0 | 标准1
    QString role; // 角色
    QDateTime create_time;
    QDateTime update_time;
};
// 新建检定程序，选择标准装置
class StandardInfo {
public:
    QString id;
    //    QString standard_no;//标准代码(手填)
    QString standard_cert_no;
    QString standard_name; // 标准装置名称
    QString dept_name; // 部门
    QString manage_status; // 管理状态(字典code)
    QString measure_item; // 检定校准项目
};
// 核查项数据-多功能校准源数据表
class StandardCheckDataCalibrator {
public:
    QString id;
    QString data_id; // 核查数据管理基础信息表id
    QString check_type; // 核查类型(直接存字典value)
    QString test_type; // 核查点类型(直接存字典value)
    QString params; // 核查项目(直接存字典value)
    QString unit; // 单位(直接存字典value)
    QString frequency; // 频率
    QString frequency_unit; // 频率单位
    QString check_point; // 核查点
    QString test_value_one_date; // 核查读数1日期（稳定性）
    QString test_value_one; // 核查读数1（示值）
    QString test_value_two_date; // 核查读数2日期（稳定性）
    QString test_value_two; // 核查读数2（示值）
    QString test_value_three_date; // 核查读数3日期（稳定性）
    QString test_value_three; // 核查读数3（示值）
    QString test_value_four_date; // 核查读数4日期（稳定性）
    QString test_value_four; // 核查读数4（示值）
    QString test_value_five_date; // 核查读数5日期（稳定性）
    QString test_value_five; // 核查读数5（示值）
    QString test_value_six_date; // 核查读数6日期（稳定性）
    QString test_value_six; // 核查读数6（本次示值）
    QString average_value; // 核查读数平均值(算数平均值)
    QString standard_deviation; // Sn(x)(标准偏差)
    QString relative_repeatability; // Sn(x)/核查读数平均值(相对重复性)
    QString urel; // UREL
    QString less_than; // 是否小于相对扩展不确定度Urel(0/1)
    QString qualified; // 稳定性考核是否合格
    QDateTime create_time;
    QDateTime update_time;
};
// 核查基本信息
class StandardCheckInfo {
public:
    QString id;
    QString data_no; // 数据编号
    QString check_date; // 核查日期
    QString check_address; // 核查地点
    QString temperature; // 温度
    QString humidity; // 湿度
    QString check_accord; // 核查依据(即标准装置的作业指导书文件minio文件名，多个分号分割)
    QString stability_examine; // 是否用于稳定性考核(1/0)
    QString standard_equipment_id; // 被核查标准装置id
    QString equipment_id; // 被核查设备id
    QString item_category_id; // 核查项分类id
    QString conclusion; // 结论(字典value)
    QString remark; // 备注
    QString create_user_id; // 核查员id
    QString create_user_name; // 核查员
    QDateTime create_time;
    QDateTime update_time;
};
// 设备与装置-标准装置管理-核查项分类
class StandardCheckItemCategory {
public:
    QString id;
    QString category_no;
    QString category_name;
    QString equipment_type;
    QString belong_standard_equipment;
    QString belong_standard_equipment_name;
    QDateTime create_time;
    QDateTime update_time;
};
// 核查项数据-直流稳压电源
class StandardCheckDataDcPower {
public:
    QString id;
    QString data_id; // 核查数据管理基础信息表id
    QString check_type; // 核查类型(直接存字典value)
    QString params; // 核查项目(直接存字典value)
    QString check_param; // 核查参数(直接电流(间接测量))
    QString check_point; // 核查点
    QString unit; // 单位(直接存字典value)
    QString average_value; // 算数平均值
    QString test_value_one_date; // 核查读数1日期（稳定性）
    QString test_value_one; // 核查读数1（示值）
    QString test_value_two_date; // 核查读数2日期（稳定性）
    QString test_value_two; // 核查读数2（示值）
    QString test_value_three_date; // 核查读数3日期（稳定性）
    QString test_value_three; // 核查读数3（示值）
    QString test_value_four_date; // 核查读数4日期（稳定性）
    QString test_value_four; // 核查读数4（示值）
    QString test_value_five_date; // 核查读数5日期（稳定性）
    QString test_value_five; // 核查读数5（示值）
    QString test_value_six_date; // 核查读数6日期（稳定性）
    QString test_value_six; // 核查读数6（本次示值）
    QString test_value_seven; // 核查读数7（示值，纹波电压重复性）
    QString test_value_eight; // 核查读数8（示值，纹波电压重复性）
    QString test_value_nine; // 核查读数9（示值，纹波电压重复性）
    QString test_value_ten; // 核查读数10（示值，纹波电压重复性）
    QString standard_deviation; // Sn(x)(标准偏差)
    QString relative_repeatability; // 相对重复性
    QString urel; // UREL
    QString less_than; // 是否小于相对扩展不确定度Urel(0/1)
    QString qualified; // 稳定性考核是否合格
    QDateTime create_time;
    QDateTime update_time;
};
// 核查项数据-信号发生器
class StandardCheckItemDataSignalGenerator {
public:
    QString id;
    QString data_id; // 核查数据管理基础信息表id
    QString check_type; // 核查类型(字典value)
    QString params; // 核查项目(字典value)
    QString check_point; // 频率点(字典value)
    QString unit; // 频率点单位(字典value)
    QString power; // 功率
    QString power_unit; // 功率单位
    QString relative_level; // 相对电平
    QString relative_level_unit; // 相对电平单位
    QString modulation; // 调幅度
    QString frequency_deviation; // 调频频偏
    QString frequency_deviation_unit; // 调频频偏单位
    QString phase_deviation; // 调相相偏
    QString phase_deviation_unit; // 调相相偏单位
    QString average_value; // 平均值
    QString test_value_one_date; // 核查读数1日期（稳定性）
    QString test_value_one; // 核查读数1（示值）
    QString test_value_two_date; // 核查读数2日期（稳定性）
    QString test_value_two; // 核查读数2（示值）
    QString test_value_three_date; // 核查读数3日期（稳定性）
    QString test_value_three; // 核查读数3（示值）
    QString test_value_four_date; // 核查读数4日期（稳定性）
    QString test_value_four; // 核查读数4（示值）
    QString test_value_five_date; // 核查读数5日期（稳定性）
    QString test_value_five; // 核查读数5（示值）
    QString test_value_six_date; // 核查读数6日期（稳定性）
    QString test_value_six; // 核查读数6（示值）
    QString test_value_seven_date; // 核查读数7日期（稳定性）
    QString test_value_seven; // 核查读数7（示值）
    QString test_value_eight_date; // 核查读数8日期（稳定性）
    QString test_value_eight; // 核查读数8（示值）
    QString test_value_nine_date; // 核查读数9日期（稳定性）
    QString test_value_nine; // 核查读数9（示值）
    QString test_value_ten_date; // 核查读数10日期（稳定性）
    QString test_value_ten; // 核查读数10（示值）
    QString standard_deviation; // 重复性S(x)/重复性S(x)/x
    QString urel; // U(k=2)
    QString less_than; // 结论,是否小于Urel(0/1)
    QString stability; // 稳定性
    QString qualified; // 稳定性考核是否合格
    QDateTime create_time;
    QDateTime update_time;
};
// 核查项数据-0.02压力
class StandardCheckItemDataPistonGauge {
public:
    QString id;
    QString data_id; // 核查数据管理基础信息表id
    QString unit; // 单位(直接存字典value)
    QString check_point; // 核查点
    QString first_forward_stroke; // 第一次正行程
    QString first_reverse_stroke; // 第一次反行程
    QString second_forward_stroke; // 第二次正行程
    QString second_reverse_stroke; // 第二次反行程
    QString third_forward_stroke; // 第三次正行程
    QString third_reverse_stroke; // 第三次反行程
    QString average_value; // 平均值
    QString average_average_value; // 测量标准示值平均值(电信号中)
    QString max_indicating_error; // 示值误差最大值
    QString max_return_error; // 回程误差最大值
    QString params; // 核查项目(直接存字典value)
    QString test_value_one; // 核查标准示值1
    QString measure_value_one; // 测量标准示值1
    QString test_value_two; // 核查标准示值2
    QString measure_value_two; // 测量标准示值2
    QString test_value_three; // 核查标准示值3
    QString measure_value_three; // 测量标准示值3
    QString test_value_four; // 核查标准示值4
    QString measure_value_four; // 测量标准示值4
    QString test_value_five; // 核查标准示值5
    QString measure_value_five; // 测量标准示值5
    QString test_value_six; // 核查标准示值6
    QString measure_value_six; // 测量标准示值6
    QString maximum_error; // 最大允许误差
    QDateTime create_time;
    QDateTime update_time;
};
// 核查项数据-二等铂电阻温度计
class StandardCheckDataResistanceThermometer {
public:
    QString id;
    QString data_id; // 核查数据管理基础信息表id
    QString check_type; // 核查类型(直接存字典value)
    QString this_item_check; // 是否进行此项核查(1/0)
    QString params; // 核查项目(直接存字典value)
    QString check_point; // 温度点/水三相点/确认点/核查点
    QString unit; // 单位(直接存字典value)
    QString thoroughfare; // 通道
    QString test_value_one_date; // 核查读数1日期（稳定性）
    QString test_value_one; // 核查标准示值1/第一次
    QString measure_value_one; // 测量标准示值1
    QString different_value_one; // 差值1（重复性）/测量差值1（稳定性）
    QString test_value_two_date; // 核查读数2日期（稳定性）
    QString test_value_two; // 核查标准示值2/第二次
    QString measure_value_two; // 测量标准示值2
    QString different_value_two; // 差值2（重复性）/测量差值2（稳定性）
    QString test_value_three_date; // 核查读数3日期（稳定性）
    QString test_value_three; // 核查标准示值3/第三次
    QString measure_value_three; // 测量标准示值3
    QString different_value_three; // 差值3（重复性）/测量差值3（稳定性）
    QString test_value_four_date; // 核查读数4日期（稳定性）
    QString test_value_four; // 核查标准示值4/第四次
    QString measure_value_four; // 测量标准示值4
    QString different_value_four; // 差值4（重复性）/测量差值4（稳定性）
    QString test_value_five_date; // 核查读数5日期（稳定性）
    QString test_value_five; // 核查标准示值5/第五次
    QString measure_value_five; // 测量标准示值5
    QString different_value_five; // 差值5（重复性）/测量差值5（稳定性）
    QString test_value_six_date; // 核查读数6日期（稳定性）
    QString test_value_six; // 核查标准示值6/第六次
    QString measure_value_six; // 测量标准示值6
    QString different_value_six; // 差值6（重复性）/测量差值6（稳定性）
    QString average_value; // 差值平均值(温度值-重复性)/平均值(温度值-稳定性)/平均值(RTP值)
    QString standard_deviation; // Sn(x)(标准偏差)
    QString relative_repeatability; // 相对重复性
    QString urel; // UREL
    QString less_than; // 是否小于相对扩展不确定度Urel(0/1)
    QString measure_value; // 测量极差值(RTP值)/测量值(绝缘电阻表校准)
    QString allow_value; // 允许值(RTP值)
    QString illustration; // 结论(RTP值)
    QString indicating_error; // 示值误差
    QString maximum_error; // 最大允许误差/最大允许误差绝对值
    QString maximum_deviation; // 最大偏差
    QString conclusion; // 结论(多个核查项的结论不同可使用该字段)
    QString create_time;
    QString update_time;
};
// 核查项数据-频谱分析仪
class StandardCheckItemDataSpectrumAnalyzer {
public:
    QString id;
    QString data_id; // 核查数据管理基础信息表id
    QString check_type; // 核查类型(字典value)
    QString params; // 核查项目(字典value)
    QString check_point; // 频率点(字典value)
    QString unit; // 频率点单位(字典value)
    QString power; // 功率(字典value)
    QString power_unit; // 功率单位(字典value)
    QString relative_level; // 相对电平(字典value)
    QString relative_level_unit; // 相对电平单位(字典value)
    QString composition; // 组合方式(字典value)
    QString average_value; // 平均值
    QString test_value_one_date; // 核查读数1日期（稳定性）
    QString test_value_one; // 核查读数1（示值）
    QString test_value_two_date; // 核查读数2日期（稳定性）
    QString test_value_two; // 核查读数2（示值）
    QString test_value_three_date; // 核查读数3日期（稳定性）
    QString test_value_three; // 核查读数3（示值）
    QString test_value_four_date; // 核查读数4日期（稳定性）
    QString test_value_four; // 核查读数4（示值）
    QString test_value_five_date; // 核查读数5日期（稳定性）
    QString test_value_five; // 核查读数5（示值）
    QString test_value_six_date; // 核查读数6日期（稳定性）
    QString test_value_six; // 核查读数6（本次示值）
    QString test_value_seven_date; // 核查读数7日期（稳定性）
    QString test_value_seven; // 核查读数7（示值）
    QString test_value_eight_date; // 核查读数8日期（稳定性）
    QString test_value_eight; // 核查读数8（示值）
    QString test_value_nine_date; // 核查读数9日期（稳定性）
    QString test_value_nine; // 核查读数9（示值）
    QString test_value_ten_date; // 核查读数10日期（稳定性）
    QString test_value_ten; // 核查读数10（示值）
    QString standard_deviation; // Sn(x)(标准偏差)/S(x)/x
    QString urel; // U(k=2)
    QString less_than; // 结论,是否小于Urel(0/1)
    QString stability; // 稳定性
    QString qualified; // 稳定性考核是否合格
    QDateTime create_time;
    QDateTime update_time;
};
// 核查项数据-示波器
class StandardCheckDataOscilloscope {
public:
    QString id;
    QString data_id; //  核查数据管理基础信息表id
    QString check_type; // 核查类型(字典value)
    QString params; // 核查项目(字典value)
    QString check_point; // 核查点/快沿(上升时间)(字典value)
    QString unit; // 核查点单位/快沿单位(上升时间)(字典value)
    QString type_value; // 类型
    QString test_value_unit; // 测量值单位
    QString convert_voltage_unit; // 转换为电压值单位
    QDate test_value_one_date; // 核查读数1日期（稳定性）
    QString test_value_one; // 测量值1/测量值(上升时间)
    QDate convert_voltage_one; // 转换为电压值1
    QString flatness_one; // 平坦度1/%
    QDate test_value_two_date; // 核查读数2日期（稳定性）
    QString test_value_two; // 测量值2（示值）
    QString convert_voltage_two; // 转换为电压值2
    QString flatness_two; // 平坦度2/%
    QDate test_value_three_date; // 核查读数3日期（稳定性）
    QString test_value_three; // 测量值3（示值）
    QString convert_voltage_three; // 转换为电压值3
    QString flatness_three; // 平坦度3/%
    QDate test_value_four_date; // 核查读数4日期（稳定性）
    QString test_value_four; // 测量值4（示值）
    QString convert_voltage_four; // 转换为电压值4
    QString flatness_four; // 平坦度4/%
    QDate test_value_five_date; // 核查读数5日期（稳定性）
    QString test_value_five; // 测量值5（示值）
    QString convert_voltage_five; // 转换为电压值5
    QString flatness_five; // 平坦度5/%
    QDate test_value_six_date; // 核查读数6日期（稳定性）
    QString test_value_six; // 测量值6（本次示值）
    QString convert_voltage_six; // 转换为电压值6
    QString flatness_six; // 平坦度6/%
    QDate test_value_seven_date; // 核查读数7日期（稳定性）
    QString test_value_seven; // 核查读数7
    QString convert_voltage_seven; // 转换为电压值7
    QString flatness_seven; // 平坦度7/%
    QDate test_value_eight_date; // 核查读数8日期（稳定性）
    QString test_value_eight; // 核查读数8
    QString convert_voltage_eight; // 转换为电压值8
    QString flatness_eight; // 平坦度8/%
    QDate test_value_nine_date; // 核查读数9日期（稳定性）
    QString test_value_nine; // 核查读数9
    QString convert_voltage_nine; // 转换为电压值9
    QString flatness_nine; // 平坦度9/%
    QDate test_value_ten_date; // 核查读数10日期（稳定性）
    QString test_value_ten; // 核查读数10
    QString convert_voltage_ten; // 转换为电压值10
    QString flatness_ten; // 平坦度10/%
    QString average_value; // 平均值
    QString manufacture_no; // 探头出厂编号
    QString equipment_name; // 探头设备名称
    QString model; // 探头规格型号
    QString standard_deviation; // 重复性Sn(x)
    QString maximum_error_absolute; // 最大允许误差的绝对值
    QString maximum_error_absolute_unit; // 最大允许误差的绝对值单位
    QString conclusion; // 结论(1/0)
    QString technical_index_symbol; // 技术指标前符号
    QString technical_index; // 技术指标
    QString technical_index_unit; // 技术指标单位
    QString reference_point; // 是否为参考点
    QString stability; // 稳定性
    QString qualified; // 稳定性考核是否合格
    QDateTime create_time;
    QDateTime update_time;
};
// 核查项数据-微波衰减装置
class StandardCheckItemDataMicrowaveAttenuation {
public:
    QString id;
    QString data_id; // 核查数据管理基础信息表id
    QString check_type; // 核查类型(字典value)
    QString params; // 核查项目(字典value)
    QString frequency; // 频率点
    QString frequency_unit; // 频率点单位
    QString attenuation_amount; // 衰减量
    QString attenuation_amount_unit; // 衰减量单位
    QString composition; // 组合方式
    QString gear_type; // 档位类型
    QString average_value; // 平均值
    QDate test_value_one_date; // 核查读数1日期（稳定性）
    QString test_value_one; // 测量值1
    QDate test_value_two_date; // 核查读数2日期（稳定性）
    QString test_value_two; // 测量值2
    QDate test_value_three_date; // 核查读数3日期（稳定性）
    QString test_value_three; // 测量值3
    QDate test_value_four_date; // 核查读数4日期（稳定性）
    QString test_value_four; // 测量值4
    QDate test_value_five_date; // 核查读数5日期（稳定性）
    QString test_value_five; // 测量值5
    QDate test_value_six_date; // 核查读数6日期（稳定性）
    QString test_value_six; // 测量值6
    QDate test_value_seven_date; // 核查读数7日期（稳定性）
    QString test_value_seven; // 测量值7
    QDate test_value_eight_date; // 核查读数8日期（稳定性）
    QString test_value_eight; // 测量值8
    QDate test_value_nine_date; // 核查读数9日期（稳定性）
    QString test_value_nine; // 测量值9
    QDate test_value_ten_date; // 核查读数10日期（稳定性）
    QString test_value_ten; // 测量值10
    QString standard_deviation; // 重复性S(x)
    QString urel; // U(k=2)
    QString less_than; // 结论,是否小于Urel(0/1)
    QString stability; // 稳定性
    QString qualified; // 稳定性考核是否合格
    QDateTime create_time;
    QDateTime update_time;
};
// 核查项数据-失真度
class StandardCheckDataDistortionFactor {
public:
    QString id;
    QString data_id; // 核查数据管理基础信息表id
    QString check_type; // 核查类型(字典value)
    QString params; // 核查项目(字典value)
    QString frequency; // 频率
    QString frequency_unit; // 频率单位
    QString nominal_value; // 标称值/%
    QString average_value; // 平均值
    QDate test_value_one_date; // 核查读数1日期（稳定性）
    QString test_value_one; // 测量值1
    QDate test_value_two_date; // 核查读数2日期（稳定性）
    QString test_value_two; // 测量值2
    QDate test_value_three_date; // 核查读数3日期（稳定性）
    QString test_value_three; // 测量值3
    QDate test_value_four_date; // 核查读数4日期（稳定性）
    QString test_value_four; // 测量值4
    QDate test_value_five_date; // 核查读数5日期（稳定性）
    QString test_value_five; // 测量值5
    QDate test_value_six_date; // 核查读数6日期（稳定性）
    QString test_value_six; // 测量值6
    QDate test_value_seven_date; // 核查读数7日期（稳定性）
    QString test_value_seven; // 测量值7
    QDate test_value_eight_date; // 核查读数8日期（稳定性）
    QString test_value_eight; // 测量值8
    QDate test_value_nine_date; // 核查读数9日期（稳定性）
    QString test_value_nine; // 测量值9
    QDate test_value_ten_date; // 核查读数10日期（稳定性）
    QString test_value_ten; // 测量值10
    QString standard_deviation; // 重复性S(x)
    QString urel; // U(k=2)
    int less_than; // 结论,是否小于Urel(0/1)
    QString stability; // 稳定性
    int qualified; // 稳定性考核是否合格
    QDateTime create_time;
    QDateTime update_time;
};
// 核查项数据-低频信号
class StandardCheckDataLowFrequencySignal {
public:
    QString id;
    QString data_id; // 核查数据管理基础信息表id
    QString check_type; // 核查类型(字典value)
    QString params; // 核查项目(字典value)
    QString nominal_value; // 标称值
    QString unit; // 标称值单位/单位
    QString frequency; // 频率
    QString frequency_unit; // 频率单位
    QString amplitude; // 幅度
    QString amplitude_unit; // 幅度单位
    QString voltage; // 电压
    QString voltage_unit; // 电压单位
    QString average_value; // 平均值/测量值平均数
    QString average_value_two; // 平均值/平坦度平均值
    QDate test_value_one_date; // 核查读数1日期（稳定性）
    QString test_value_one; // 核查读数1（示值）
    QString flatness_value_one; // 平坦度1
    QDate test_value_two_date; // 核查读数2日期（稳定性）
    QString test_value_two; // 核查读数2（示值）
    QString flatness_value_two; // 平坦度2
    QDate test_value_three_date; // 核查读数3日期（稳定性）
    QString test_value_three; // 核查读数3（示值）
    QString flatness_value_three; // 平坦度3
    QDate test_value_four_date; // 核查读数4日期（稳定性）
    QString test_value_four; // 核查读数4（示值）
    QString flatness_value_four; // 平坦度4
    QDate test_value_five_date; // 核查读数5日期（稳定性）
    QString test_value_five; // 核查读数5（示值）
    QString flatness_value_five; // 平坦度5
    QDate test_value_six_date; // 核查读数6日期（稳定性）
    QString test_value_six; // 核查读数6（本次示值）
    QString flatness_value_six; // 平坦度6
    QDate test_value_seven_date; // 核查读数7日期（稳定性）
    QString test_value_seven; // 核查读数7（示值）
    QString flatness_value_seven; // 平坦度7
    QDate test_value_eight_date; // 核查读数8日期（稳定性）
    QString test_value_eight; // 核查读数8（示值）
    QString flatness_value_eight; // 平坦度8
    QDate test_value_nine_date; // 核查读数9日期（稳定性）
    QString test_value_nine; // 核查读数9（示值）
    QString flatness_value_nine; // 平坦度9
    QDate test_value_ten_date; // 核查读数10日期（稳定性）
    QString test_value_ten; // 核查读数10（示值）
    QString flatness_value_ten; // 平坦度10
    QString standard_deviation; // 重复性S(x)/重复性S(x)/x
    QString urel; // U(k=2)
    QString less_than; // 结论,是否小于Urel(0/1)
    QString stability; // 稳定性
    QString qualified; // 稳定性考核是否合格
    QDateTime create_time;
    QDateTime update_time;
};
// 核查项数据-小功率
class StandardCheckDataLowPower {
public:
    QString id;
    QString data_id; // 核查数据管理基础信息表id
    QString check_type; // 核查类型(字典value)
    QString params; // 核查项目(字典value)
    QString frequency; // 频率
    QString frequency_unit; // 频率单位
    QString average_value; // 平均值
    QDate test_value_one_date; // 核查读数1日期（稳定性）
    QString pcu_value_one; // Pcu1/mW
    QString pbu_value_one; // Pbu1/mW
    QString ku_value_one; // 校准因子Ku1/%
    QDate test_value_two_date; // 核查读数2日期（稳定性）
    QString pcu_value_two; // Pcu2/mW
    QString pbu_value_two; // Pbu2/mW
    QString ku_value_two; // 校准因子Ku2/%
    QDate test_value_three_date; // 核查读数3日期（稳定性）
    QString pcu_value_three; // Pcu3/mW
    QString pbu_value_three; // Pbu3/mW
    QString ku_value_three; // 校准因子Ku3/%
    QDate test_value_four_date; // 核查读数4日期（稳定性）
    QString pcu_value_four; // Pcu4/mW
    QString pbu_value_four; // Pbu4/mW
    QString ku_value_four; // 校准因子Ku4/%
    QDate test_value_five_date; // 核查读数5日期（稳定性）
    QString pcu_value_five; // Pcu5/mW
    QString pbu_value_five; // Pbu5/mW
    QString ku_value_five; // 校准因子Ku5/%
    QDate test_value_six_date; // 核查读数6日期（稳定性）
    QString pcu_value_six; // Pcu6/mW
    QString pbu_value_six; // Pbu6/mW
    QString ku_value_six; // 校准因子Ku6/%
    QDate test_value_seven_date; // 核查读数7日期（稳定性）
    QString pcu_value_seven; // Pcu7/mW
    QString pbu_value_seven; // Pbu7/mW
    QString ku_value_seven; // 校准因子Ku7/%
    QDate test_value_eight_date; // 核查读数8日期（稳定性）
    QString pcu_value_eight; // Pcu8/mW
    QString pbu_value_eight; // Pbu8/mW
    QString ku_value_eight; // 校准因子Ku8/%
    QDate test_value_nine_date; // 核查读数9日期（稳定性）
    QString pcu_value_nine; // Pcu9/mW
    QString pbu_value_nine; // Pbu9/mW
    QString ku_value_nine; // 校准因子Ku9/%
    QDate test_value_ten_date; // 核查读数10日期（稳定性）
    QString pcu_value_ten; // Pcu10/mW
    QString pbu_value_ten; // Pbu10/mW
    QString ku_value_ten; // 校准因子Ku10/%
    QString standard_deviation; // 重复性S(x)
    QString urel; // U(k=2)
    QString less_than; // 结论,是否小于Urel(0/1)
    QString stability; // 稳定性
    QString qualified; // 稳定性考核是否合格
    QDateTime create_time;
    QDateTime update_time;
};
// 核查项数据-s参数
class StandardCheckDataSParam {
public:
    QString id;
    QString data_id; // 核查数据管理基础信息表id
    QString check_type; // 核查类型(字典value)
    QString params; // 核查项目(字典value)
    QString frequency; // 频率
    QString frequency_unit; // 频率单位
    QString amplitude; // 幅度
    QString amplitude_unit; // 幅度单位
    QString phase; // 相位
    QString phase_unit; // 相位单位
    QString type_value; // 类型
    QString vsmr; // VSMR
    QString average_value; // 平均值
    QDate test_value_one_date; // 核查读数1日期（稳定性）
    QString test_value_one; // 测量值1
    QDate test_value_two_date; // 核查读数2日期（稳定性）
    QString test_value_two; // 测量值2
    QDate test_value_three_date; // 核查读数3日期（稳定性）
    QString test_value_three; // 测量值3
    QDate test_value_four_date; // 核查读数4日期（稳定性）
    QString test_value_four; // 测量值4
    QDate test_value_five_date; // 核查读数5日期（稳定性）
    QString test_value_five; // 测量值5
    QDate test_value_six_date; // 核查读数6日期（稳定性）
    QString test_value_six; // 测量值6
    QDate test_value_seven_date; // 核查读数7日期（稳定性）
    QString test_value_seven; // 测量值7
    QDate test_value_eight_date; // 核查读数8日期（稳定性）
    QString test_value_eight; // 测量值8
    QDate test_value_nine_date; // 核查读数9日期（稳定性）
    QString test_value_nine; // 测量值9
    QDate test_value_ten_date; // 核查读数10日期（稳定性）
    QString test_value_ten; // 测量值10
    QString standard_deviation; // 重复性S(x)
    QString urel; // U(k=2)
    int less_than; // 结论,是否小于Urel(0/1)
    QString stability; // 稳定性
    int qualified; // 稳定性考核是否合格
    QDateTime create_time;
    QDateTime update_time;
};
// 核查项数据-铯原子
class StandardCheckDataCesiumAtom {
public:
    QString id;
    QString data_id;
    QString check_type;
    QString params;
    QString check_point;
    QString unit;
    QString electrical_level;
    QString electrical_level_unit;
    QString amplitude;
    QString amplitude_unit;
    QString check_method;
    QDate test_value_one_date;
    QString test_value_one;
    QDate test_value_two_date;
    QString test_value_two;
    QDate test_value_three_date;
    QString test_value_three;
    QDate test_value_four_date;
    QString test_value_four;
    QDate test_value_five_date;
    QString test_value_five;
    QDate test_value_six_date;
    QString test_value_six;
    QDate test_value_seven_date;
    QString test_value_seven;
    QDate test_value_eight_date;
    QString test_value_eight;
    QDate test_value_nine_date;
    QString test_value_nine;
    QDate test_value_ten_date;
    QString test_value_ten;
    QString average_value;
    QString standard_deviation;
    QString urel;
    QString conclusion;
    QString stability;
    QString qualified;
    QDateTime create_time;
    QDateTime update_time;
};

class DataCalibratorStability {
public:
    QDate date;
    QString value;
    QString params;
    QDate check_date;
};

enum class SyncRunStatus {
    Running,
    Stopped,
    Paused
};

using namespace Jiawa::Core;
class BaseCommonApi {
public:
    BaseCommonApi();
    const static QString temporary_user_id;
    const static QString temporary_costomer_id;
    const static QString measure_person_id;

    QString encrypt(const QString& str, const QString& publicKey);
    static bool SysUserLogin(QString account, QString password);

    // 判断检定装置表并查询    参数设备id
    static QList<QStringList> Search_CalibrationTable(QString DeiverId, QStringList* HeadList);
    static QMap<QString, QList<QStringList>> Search_CalibrationTableClass(QString DeiverId, SequentialMap<QString, QStringList>* HeadList);

    // 判断核查装置表并查询  参数设备id
    //    static QList<QStringList> Search_VerifyTable(QString DeiverId,QStringList *HeadList);

    // 判断检定数据管理 结果信息表
    static QList<QStringList> Search_CalibrationDataTable(QString DeiverId, QString dataId, QStringList* HeadList);

    // 判断核查数据管理 结果信息表
    //     static QList<QStringList> Search_VerifyDataTable(QString DeiverId,QString dataId,QStringList *HeadList);

    // sys_user表的id转换为measure_person_id 检定人id（meter_staff表id）
    //     static StaffInfo ConvertStaffInfo(QString userId);

    // 部门任务 废弃
    //    static QList<Dept> getAllData();
    //    static QString loginUserId;
    // 部门任务  参数登录用户的部门id和状态
    static QList<Dept> getAllData(QString userId);
    // 我的任务
    static QList<MyTask> getMyTasks(QString id);
    // 获取创建任务首页数据
    static QList<CreateTaskList> getCreateTaskList(QString id);
    // 获取创建任务最新数据
    static QList<CreateTaskList> getCreateTaskListNew();
    // 创建任务-查看任务单顶部
    static QList<CatTask> getCatTask(QString orderNo);
    // 创建任务-查看任务列表底部
    static QList<CatTaskList> getCatTaskList(QString orderId);
    // 查询指定下拉框的字段
    static QList<SelectSpecifyFields> getSelectSpecifyFields();
    // 插入任务单基本信息
    static bool InsertTaskBasic(InsertTask insertTask);
    // 插入任务单 新增任务单样品表
    static bool InsertOrderSampleRelation(OrderSampleRelation orderSample);
    // 插入任务单 新增任务单信息记录表
    static bool InsertTaskInfoRecord(biz_business_lab_executive_info eneity);
    // 更新任务单 编辑
    static bool UpdateTask(InsertTask updateTask);
    // 删除任务单
    static bool DeleteTask(QString id);
    static bool DeleteTaskDevice(QString id);
    // 获取标准设备管理 首页
    static QList<BizDevice> getBizDeviceManage();
    // 标准设备管理-设备基本信息
    static QList<CatDeviceInfor> getCatDeviceInfor(QString id);
    // 判断是哪个核查项
    //    static  QString  getApparatus(QString id,QString name);
    // 直流稳压电源检定装置
    //    static QList<CheckDCPower>getCheckDCPower(QString item_category_id,QStringList *headList);
    // 获取被检设备管理首页数据
    static QList<TestedDevice> getTestedDevice();
    // 被检设备管理查看设备信息
    static QList<TestedCatDevice> getTestedCatDevice(QString id);
    // 判断 检定哪个 装置表 传参 设备id  返回itemId
    static QList<JudgeCalibrationTable> getJudgeCalibrationTable(QString id);
    // 判断 核查哪个 装置表 传参 设备id
    //    static JudgeCalibrationTable getJudgeVerifyTable(QString id);
    // 被检设备管理-检定项目及检定点-多功能校准源
    static QList<QStringList> getVerMultiCal(QString itemId, QStringList* headList);
    static QMap<QString, QList<QStringList>> getVerMultiCalClass(QString itemCatoryName, QString itemId, SequentialMap<QString, QStringList>* headList);
    static QMap<QString, QList<QStringList>> GetDataReturnItemList(QString itemCatoryName, QList<QStringList> ResultData,
        QMap<QString, QList<QStringList>> DataMapList,
        SequentialMap<QString, QStringList>* HeadMapList);
    // 获取检定数据管理 首页
    static QList<VerificationData> getVerificationData();
    // 检定数据管理-检定基本信息-标准设备
    static QList<VerificationBasicDataDevice> getVerificationBasicDataDevice(QString id);
    // 检定数据管理-基本信息  参数首页数据的id
    static VerificationBasicDataDetails getVerificationBasicDataDetails(QString dataId);
    // 检定数据管理-检定结果信息-多功能校准源
    static QList<QStringList> getVerificationMultiCalResultData(QString dataId, QStringList* HeadList);
    // 检定数据管理-检定结果信息-0.02压力
    static QList<QStringList> getDeviceMeasureDataPistonGauge(QString data_id, QStringList* HeadList);
    // 检定数据管理-检定结果信息-直流稳压电源
    static QList<QStringList> getbizBusinessDeviceMeasureDataDcPower(QString data_id, QStringList* HeadList);
    // 【查询】二等铂电阻结果
    static QList<QStringList> getDeviceMeasureDataResistanceThermometer(QString data_id, QStringList* HeadList);
    // 核查数据管理-首页
    static QList<CheckData> getCheckData();
    // 核查数据管理-基本信息-核查件
    static QList<CheckDataManageBasicInfo> getCheckDataManageBasicInfoDevice(QString id);
    // 核查数据管理-基本信息-基本信息  参数设备id
    static CheckDataManageBasicInfo getCheckDataManageBasicInfo(QString equipment_id);

    // 核查数据管理-核查结果信息-核查数据 多功能校准源数据
    static QList<QStringList> getCheckDataManageResultManageMult(QString data_id, QStringList* HeadList);
    // 技术文件
    static QList<Technique_File> getTechnique_File();
    // 检定程序管理首页查询
    static QList<VerificationProgramInfo> getVerificationProgramInfoMain();
    // 查询检定项分类
    static QList<itemCategory> getitemCategoryList(QString item);
    // 检定程序中设置指令时选择检定项目  参数 装置字典 1 OR 2  ，参数2： 检定项分类id ,在新建程序时选择的 例如：数字多用表（手持）的id
    static QList<Params> getParams(QString belong_standard_equipment_id, QString equipment_item_category_id);
    // 检定程序管理 编辑
    static VerificationProgramInfo getVerificationProgramInfo(QString id);
    // 查询设备通信接口
    static QList<VerificationProgramConfigInterface> getVerificationProgramConfigInterface(QString deviceId);
    // 新建检定程序
    static bool InsertVerificationProgramInfo(VerificationProgramInfo verInfo);
    // 查询指令库  参数 检定程序id
    static QList<InstructionLib> getInstructionLib(QString verification_program_id);
    // 新增指令库
    static bool InsertInstructionLib(InstructionLib instructionLib);
    // 删除指令库 参数 指令表的id
    static bool DeleteInstructionLib(QString id);
    // 更新指令库
    static bool UpdateInstructionLib(InstructionLib instructionLib);
    // 新增通讯接口
    static bool InsertVerificationProgramConfigInterface(VerificationProgramConfigInterface configInterdace);
    // 更新检定程序
    static bool UpdateVerificationProgramInfo(VerificationProgramInfo verInfo);
    // 更新通讯接口
    static bool UpdateVerificationProgramConfigInterface(VerificationProgramConfigInterface configInterdace);
    // 删除检定程序
    static bool DeleteVerificationProgramInfo(QString id);
    // 雪花id生成工具
    qint64 generateId();
    // 雪花算法生成id
    IdWorker& idWorker = Singleton<IdWorker>::instance();
    // 生成任务单编号
    QString GenerateTaskNumber();
    // 生成任务单编号当天天数增加
    bool AddTaskNumber();
    // 自动检定-根据被检设备 查询检定项分类
    static QList<DeviceMeasureEquipmentCatory> getDeviceMeasureEquipmentCatory(QString deviceId);
    // 自动检定-根据被检设备 查询检定项分类 在查询检定程序
    static QList<VerificationProgramInfo> getVerificationProgramInfoByDeviceId(QString deviceId);
    // 自动检定-结果保存-基本信息
    static bool InsertSaveDeviceMeasureInfo(SaveDeviceMeasureInfo saveInfo);
    // 自动检定-结果保存-数据关联-计量检定业务-受检设备检定管理-检定数据管理-所使用的标准、主要测量设备
    static bool InsertDeviceMeasureEquipment(DeviceMeasureEquipment deviceMeasure);
    // 自动检定-结果保存-检定结果信息-多功能校准源检定装置
    static bool InsertVerMultiCal(SaveDeviceMeasureCalibrator measureCalibrator);

    // 核查程序-首页查询
    static QList<CheckProgramManage> getCheckProgramManage();
    // 核查程序-编辑查询单个
    static CheckProgramManage getCheckProgramManageById(QString id);
    // 核查程序-编辑查询 选择设备 选择标准
    static QList<CheckAndTestedDevice> getCheckDeviceList();
    // 核查程序-编辑查询 选择核查件    参数标准装置字典
    //    static QList<CheckAndTestedDevice> getCheckAndTestedDeviceList(QString standardId);
    // 多功能校准源  核查项及核查点 参数设备id
    //    static QList<QStringList> getVerificationItemsMultiList(QString equipment_id,QStringList *headList);

    // 新增核查程序
    static bool InsertCheckProgramManage(CheckProgramManage check);
    // 更新核查程序  参数核查程序的id
    static bool UpdateCheckProgramManage(CheckProgramManage check);
    // 删除核查程序  参数核查程序的id
    static bool DeleteCheckProgramManage(QString id);
    // 核查程序-标准装置-分类
    static QList<StandardCheckItemCategory> getStandardCheckItemCategory(QString item);

    //    设备与装置-标准装置管理-核查数据基本信息关联的核查标准设备，即给设备做核查的设备
    static bool InsertStandCheckInfoEquipent(StandCheckInfoEquipent equiment);
    //    自动核查-根据 设备id查询核查件设备信息
    //    static CheckAndTestedDevice getCheckAndTestedDeviceById(QString deviceId);

    //    //自动核查--结果保存-设备与装置-标准装置管理-多功能校准源
    //    static bool InsertStandCheckDataCalibrator(StandCheckDataCalibrator calibrator);
    //    //自动核查-结果保存-设备与装置-标准装置管理-核查数据管理0.02级活塞式压力计数据表
    //    static bool InsertStandardCheckDatePistonGauge(StandardCheckDatePistonGauge gauge);
    // 自动核查-结果保存-基本信息
    //    static bool InsertCheckDataManageBasicInfo(CheckDataManageBasicInfo baseInfo);
    // 查询实验室及温湿度
    static QList<LocationInfo> getLocationInfo();
    // 查询出所有实验室地点，用于自动检定/核查 结果保存 地点下拉框查询
    static QList<LocationInfo> getLocationInfoList();
    // 查询所有用户，用于自动检定/核查 结果保存 地点下拉框查询
    static QList<User> getUser();
    // 根据用户ID查询 当前部门ID和部门名称全程
    static QList<Dept> SelectDeptData(QString UserId);
    // 查询出所有委托方
    static QList<biz_resource_customer_info> SelectCustomerData();
    // 计量检定业务-受检设备检定管理-检定数据管理检定数据历史修改记录[新增]
    static bool InsertModifyRecord(MeasureModifyRecord record);
    // 计量检定业务-受检设备检定管理-检定数据管理检定数据历史修改记录[查询]  参数 检定数据管理基础信息表id
    static QList<MeasureModifyRecord> getModifyRecord(QString data_id);
    // 设备与装置-标准装置管理-核查数据管理核查数据历史修改记录[新增]   与 MeasureModifyRecord 一致 复用
    static bool InsertCheckModifyRecord(MeasureModifyRecord record);
    // 设备与装置-标准装置管理-核查数据管理核查数据历史修改记录[查询] 参数 检定数据管理基础信息表id
    static QList<MeasureModifyRecord> getMeasureModifyRecord(QString data_id);
    // 查询最新结果保存记录
    static SaveDeviceMeasureInfo getNewResultData();
    // 删除任务详情表同一任务ID的设备
    static bool DeleteTaskSameDevice(QString order_id);
    // 删除任务单信息记录表同一任务ID的设备
    static bool DeleteTaskInfoRecordDevice(QString order_id);
    // 单独查询任务单表
    // 更新 任务单
    static InsertTask SelectTaskSingle(QString order_id);

    // log文件
    static bool SaveLogInfo(int pathIndex, QString Info);

    // 新增日志表数据
    static bool InsertzdLog(zdLog log);
    // 查询日志表数据
    static QList<zdLog> getzdLogByuserId(QString userid);
    // 新增数据同步表
    static bool InsertzdDataSync(zdDataSync data);
    // 根据log_sync_id查询数据同步表
    static QList<zdDataSync> getzdDataSyncByuserId(QString log_sync_id);
    // 根据不同装置保存结果数据
    static bool SearchSelectTable(QString id, BaseClass* object);
    // 查询测试页面中 生成结果处理的表头及数据
    static QMap<QString, QList<QStringList>> Search_GenerateTableClass(QString DeiverId, SequentialMap<QString, QStringList>* HeadList);
    // 【查询】检定项0.02活塞式压力计数据(包含字典的字段都存为字典value)
    //    static QList<QStringList> getDeviceMeasureCalibratorPistonGauge(QString itemId,QStringList *headList);
    static QMap<QString, QList<QStringList>> getDeviceMeasureCalibratorPistonGauge(QString itemCatoryName, QString itemId, SequentialMap<QString, QStringList>* headList);
    static QMap<QString, QList<QStringList>> GetDataReturnItemListByPistonGauge(QString itemCatoryName, QList<QStringList> ResultData, QMap<QString, QList<QStringList>> DataMapList, SequentialMap<QString, QStringList>* HeadMapList);
    // 0.02压力 测试界面  查询生成结果处理界面的头和数据
    static QMap<QString, QList<QStringList>> getDeviceMeasureCalibratorPistonGaugeGenerate(QString itemCatoryName, QString itemId, SequentialMap<QString, QStringList>* headList);
    static QMap<QString, QList<QStringList>> GetDataReturnItemListByPistonGaugeResult(QString itemCatoryName, QList<QStringList> ResultData, QMap<QString, QList<QStringList>> DataMapList, SequentialMap<QString, QStringList>* HeadMapList);
    // 【结果保存】压力
    static bool InsertDeviceMeasureDataPistonGauge(DeviceMeasureDataPistonGauge gauge);

    static QList<QStringList> searchStatus(QString deviceId);

    // 测试页面 压力  外观及功能性检查和检定数据头
    static QList<QStringList> getDeviceMeasureCalibratorPistonGaugeBystatus(QString itemId);
    //    【查询】检定项直流稳压电源
    static QMap<QString, QList<QStringList>> getDeviceMeasureItemDataDcPower(QString itemCatoryName, QString itemId, SequentialMap<QString, QStringList>* headList);
    static QMap<QString, QList<QStringList>> GetDataReturnItemListByDcPower(QString itemCatoryName, QList<QStringList> ResultData, QMap<QString, QList<QStringList>> DataMapList, SequentialMap<QString, QStringList>* HeadMapList);
    // 【结果保存】直流稳压电源
    static bool InsertbizBusinessDeviceMeasureDataDcPower(bizBusinessDeviceMeasureDataDcPower power);

    // 示波器
    static QMap<QString, QList<QStringList>> getDeviceOscilloscopeCalibration(QString itemCatoryName, QString itemId, SequentialMap<QString, QStringList>* headList);
    static QMap<QString, QList<QStringList>> GetDataReturnItemListByOscilloscope(QString itemCatoryName, QList<QStringList> ResultData, QMap<QString, QList<QStringList>> DataMapList, SequentialMap<QString, QStringList>* HeadMapList);
    // 【结果保存】示波器
    static bool InsertDeviceOscilloscopeCalibration(DeviceOscilloscopeCalibration scope);
    // 【查询】检定项二等铂电阻
    static QMap<QString, QList<QStringList>> getDeviceMeasureItemDataResistanceThermometer(QString itemCatoryName, QString itemId, SequentialMap<QString, QStringList>* headList);
    static QMap<QString, QList<QStringList>> GetDataReturnItemListByResistanceThermometer(QString itemCatoryName, QList<QStringList> ResultData, QMap<QString, QList<QStringList>> DataMapList, SequentialMap<QString, QStringList>* HeadMapList);
    // 结果保存-二等铂电阻
    static bool InsertDeviceMeasureDataResistanceThermometer(DeviceMeasureDataResistanceThermometer thermometer);
    // 【查询】检定项-频谱分析仪
    static QMap<QString, QList<QStringList>> getDeviceMeasureItemDataSpectrumAnalyzer(QString itemCatoryName, QString itemId, SequentialMap<QString, QStringList>* headList);
    static QMap<QString, QList<QStringList>> GetDataReturnItemListBySpectrumAnalyzer(QString itemCatoryName, QList<QStringList> ResultData, QMap<QString, QList<QStringList>> DataMapList, SequentialMap<QString, QStringList>* HeadMapList);
    // 【结果保存】频谱分析仪
    static bool InsertDeviceMeasureDataSpectrumAnalyzer(DeviceMeasureDataSpectrumAnalyzer analyzer);
    // 保留5位小数，参数1：值，参数2:保留几位有效数字
    static QString formatDoubleWithEffectiveDigits(double value, int effectiveDigits);
    // 保留小数点后几位
    static QString formatDoubleWithDecimalPlaces(double value, int decimalPlaces);
    // 【查询】角色 根据分类id
    static QList<DeviceRole> getDeviceRoleBydeviceId(QString cateory_id);
    // 【新增】角色
    static bool InsertDeviceRole(DeviceRole role);
    // 【更新】角色
    static bool UpdateDeviceRole(DeviceRole role);
    // 【查询】检定项-信号发生器
    static QMap<QString, QList<QStringList>> getDeviceMeasureItemDataSignalGenerator(QString itemCatoryName, QString itemId, SequentialMap<QString, QStringList>* headList);
    static QMap<QString, QList<QStringList>> GetDataReturnItemListBySignalGenerator(QString itemCatoryName, QList<QStringList> ResultData, QMap<QString, QList<QStringList>> DataMapList, SequentialMap<QString, QStringList>* HeadMapList);
    //  【保存】信号发生器
    static bool InsertDeviceMeasureDataSignalGenerator(DeviceMeasureDataSignalGenerator generator);

    static bool saveLogToSql(QString ssql, BaseClass* entity);
    // 新建检定\核查程序中查询所有的标准装置
    static QList<StandardInfo> getStandardInfo();
    // 根据标准装置id查询标准设备
    static QList<BizDevice> getStandardInfoListByStandardId(QString standardId);

    // 【查询】检定项-微波衰减器
    static QMap<QString, QList<QStringList>> getDeviceMicrowaveAttenuation(QString itemCatoryName, QString itemId, SequentialMap<QString, QStringList>* headList);
    static QMap<QString, QList<QStringList>> GetDataReturnItemListByMicrowaveAttenuation(QString itemCatoryName, QList<QStringList> ResultData, QMap<QString, QList<QStringList>> DataMapList, SequentialMap<QString, QStringList>* HeadMapList);
    static bool InsertDeviceMicrowaveAttenuation(MicrowaveAttenuationCalibration scope);
    // 【查询】检定项-低频信号装置
    static QMap<QString, QList<QStringList>> getDeviceMeasureItemDataLFSignalGenerator(QString itemCatoryName, QString itemId, SequentialMap<QString, QStringList>* headList);
    static QMap<QString, QList<QStringList>> GetDataReturnItemListByLFSignalGenerator(QString itemCatoryName, QList<QStringList> ResultData, QMap<QString, QList<QStringList>> DataMapList, SequentialMap<QString, QStringList>* HeadMapList);
    static bool InsertDeviceLFSignalGenerator(LFSignalGeneratoCalibration scope);
    // 【查询】检定项-小功率装置
    static QMap<QString, QList<QStringList>> getDeviceSmallPower(QString itemCatoryName, QString itemId, SequentialMap<QString, QStringList>* headList);
    static QMap<QString, QList<QStringList>> GetDataReturnItemListBySmallPower(QString itemCatoryName, QList<QStringList> ResultData, QMap<QString, QList<QStringList>> DataMapList, SequentialMap<QString, QStringList>* HeadMapList);
    static bool InsertDeviceSmallPower(SmallPowerCalibration scope);

    // 【查询】检定项-s参数-失量
    static QMap<QString, QList<QStringList>> getDeviceMeasureItemDataSParam(QString itemCatoryName, QString itemId, SequentialMap<QString, QStringList>* headList);
    static QMap<QString, QList<QStringList>> GetDataReturnItemListBySParam(QString itemCatoryName, QList<QStringList> ResultData, QMap<QString, QList<QStringList>> DataMapList, SequentialMap<QString, QStringList>* HeadMapList);
    // 【保存】检定项-s参数-失量
    static bool InsertDeviceMeasureItemDataSParam(DeviceMeasureDataSParam sParam);

    // 【保存】核查基本信息
    static bool InsertStandardCheckInfo(StandardCheckInfo info);
    // 【查询】核查项-多功能校准源
    static QMap<QString, QMap<QString, QList<QStringList>>> getStandardCheckItemDataCalibrator(QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList);
    static QMap<QString, QMap<QString, QList<QStringList>>> GetDataReturnItemListByCheckItemDataCalibrator(QList<QStringList> ResultData, QMap<QString, QMap<QString, QList<QStringList>>> DataMapList, QMap<QString, QMap<QString, QStringList>>* headList);
    // 【保存】核查数据-多功能校准源
    static bool InsertStandardCheckDataCalibrator(StandardCheckDataCalibrator calibrator);
    // 【查询】核查项-二等铂电阻温度计
    static QMap<QString, QMap<QString, QList<QStringList>>> getStandardCheckItemDataResistanceThermometer(QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList);
    static QMap<QString, QMap<QString, QList<QStringList>>> GetDataReturnItemListByCheckItemDataResistanceThermometer(QList<QStringList> ResultData, QMap<QString, QMap<QString, QList<QStringList>>> DataMapList, QMap<QString, QMap<QString, QStringList>>* headList);
    // 【保存】核查项-二等铂电阻温度计
    static bool InsertStandardCheckItemDataResistanceThermometer(StandardCheckDataResistanceThermometer resistanceThermometer);

    // 【查询】核查项-直流稳压电源
    static QMap<QString, QMap<QString, QList<QStringList>>> getStandardCheckItemDataDcPower(QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList);
    static QMap<QString, QMap<QString, QList<QStringList>>> GetDataReturnItemListByCheckItemDataDcPower(QList<QStringList> ResultData, QMap<QString, QMap<QString, QList<QStringList>>> DataMapList, QMap<QString, QMap<QString, QStringList>>* headList);
    // 【保存】核查数据-直流稳压电源
    static bool InsertStandardCheckItemDataDcPower(StandardCheckDataDcPower power);

    // 【查询】核查项-信号发生器
    static QMap<QString, QMap<QString, QList<QStringList>>> getStandardCheckItemDataSignalGenerator(QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList);
    static QMap<QString, QMap<QString, QList<QStringList>>> GetDataReturnItemListByCheckItemDataSignalGenerator(QList<QStringList> ResultData, QMap<QString, QMap<QString, QList<QStringList>>> DataMapList, QMap<QString, QMap<QString, QStringList>>* headList);
    // 【保存】核查数据-信号发生器
    static bool InsertStandardCheckItemDataSignalGenerator(StandardCheckItemDataSignalGenerator signalGenerator);

    //   【查询】核查项-0.02压力
    static QMap<QString, QMap<QString, QList<QStringList>>> getStandardCheckItemDataPistonGauge(QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList);
    static QMap<QString, QMap<QString, QList<QStringList>>> GetDataReturnItemListByCheckItemDataPistonGauge(QList<QStringList> ResultData, QMap<QString, QMap<QString, QList<QStringList>>> DataMapList, QMap<QString, QMap<QString, QStringList>>* headList);
    //   【保存】核查项-0.02压力
    static bool InsertStandardCheckItemDataPistonGauge(StandardCheckItemDataPistonGauge pistonGauge);

    // 【查询】核查项-频谱分析仪
    static QMap<QString, QMap<QString, QList<QStringList>>> getStandardCheckItemDataSpectrumAnalyzer(QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList);
    static QMap<QString, QMap<QString, QList<QStringList>>> GetDataReturnItemListByCheckItemDataSpectrumAnalyzer(QList<QStringList> ResultData, QMap<QString, QMap<QString, QList<QStringList>>> DataMapList, QMap<QString, QMap<QString, QStringList>>* headList);
    //   【保存】核查项-频谱分析仪
    static bool InsertStandardCheckItemDataSpectrumAnalyzer(StandardCheckItemDataSpectrumAnalyzer spectrumAnalyzer);
    // 【查询】核查项-示波器
    static QMap<QString, QMap<QString, QList<QStringList>>> getStandardCheckItemDataOscilloscope(QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList);
    static QMap<QString, QMap<QString, QList<QStringList>>> GetDataReturnItemListByCheckItemDataOscilloscope(QList<QStringList> ResultData, QMap<QString, QMap<QString, QList<QStringList>>> DataMapList, QMap<QString, QMap<QString, QStringList>>* headList);
    //   【保存】核查项-示波器
    static bool InsertStandardCheckItemDataOscilloscope(StandardCheckDataOscilloscope oscilloscope);

    // 【查询】核查项-失真度
    static QMap<QString, QMap<QString, QList<QStringList>>> getStandardCheckItemDataDistortionFactor(QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList);
    static QMap<QString, QMap<QString, QList<QStringList>>> GetDataReturnItemListByCheckItemDataDistortionFactor(QList<QStringList> ResultData, QMap<QString, QMap<QString, QList<QStringList>>> DataMapList, QMap<QString, QMap<QString, QStringList>>* headList);
    //   【保存】核查项-失真度
    static bool InsertStandardCheckItemDataDistortionFactor(StandardCheckDataDistortionFactor distortionFactor);

    // 【查询】核查项-微波衰减标准装置
    static QMap<QString, QMap<QString, QList<QStringList>>> getStandardCheckItemDataMicrowaveAttenuation(QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList);
    static QMap<QString, QMap<QString, QList<QStringList>>> GetDataReturnItemListByCheckItemDataMicrowaveAttenuation(QList<QStringList> ResultData, QMap<QString, QMap<QString, QList<QStringList>>> DataMapList, QMap<QString, QMap<QString, QStringList>>* headList);
    //   【保存】核查项-微波衰减标准装置
    static bool InsertStandardCheckItemDataMicrowaveAttenuation(StandardCheckItemDataMicrowaveAttenuation microwaveAttenuation);

    // 【查询】核查项-低频信号源检定装置
    static QMap<QString, QMap<QString, QList<QStringList>>> getStandardCheckItemDataLowFrequencySignal(QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList);
    static QMap<QString, QMap<QString, QList<QStringList>>> GetDataReturnItemListByCheckItemDataLowFrequencySignal(QList<QStringList> ResultData, QMap<QString, QMap<QString, QList<QStringList>>> DataMapList, QMap<QString, QMap<QString, QStringList>>* headList);
    //   【保存】核查项-低频信号源检定装置
    static bool InsertStandardCheckItemDataLowFrequencySignal(StandardCheckDataLowFrequencySignal lowFrequencySignal);
    // 【查询】核查项-小功率
    static QMap<QString, QMap<QString, QList<QStringList>>> getStandardCheckItemDataLowerPower(QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList);
    static QMap<QString, QMap<QString, QList<QStringList>>> GetDataReturnItemListByCheckItemDataLowerPower(QList<QStringList> ResultData, QMap<QString, QMap<QString, QList<QStringList>>> DataMapList, QMap<QString, QMap<QString, QStringList>>* headList);
    //   【保存】核查项-小功率
    static bool InsertStandardCheckItemDataLowFrequencySignal(StandardCheckDataLowPower lowPower);
    // 【查询】核查项-s参数
    static QMap<QString, QMap<QString, QList<QStringList>>> getStandardCheckItemDataSParam(QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList);
    static QMap<QString, QMap<QString, QList<QStringList>>> GetDataReturnItemListByCheckItemDataSParam(QList<QStringList> ResultData, QMap<QString, QMap<QString, QList<QStringList>>> DataMapList, QMap<QString, QMap<QString, QStringList>>* headList);
    //   【保存】核查项-s参数
    static bool InsertStandardCheckItemDataSParam(StandardCheckDataSParam sParam);
    // 【查询】核查项-铯原子
    static QMap<QString, QMap<QString, QList<QStringList>>> getStandardCheckItemDataCesiumAtom(QString equipment_id, QMap<QString, QMap<QString, QStringList>>* headList);
    static QMap<QString, QMap<QString, QList<QStringList>>> GetDataReturnItemListByCheckItemDataCesiumAtom(QList<QStringList> ResultData, QMap<QString, QMap<QString, QList<QStringList>>> DataMapList, QMap<QString, QMap<QString, QStringList>>* headList);
    //   【保存】核查项-铯原子
    static bool InsertStandardCheckDataCesiumAtom(StandardCheckDataCesiumAtom cesiumAtom);

    static QList<DataCalibratorStability> GetEquipmentStandardCheckInfoByids(QString standard_equipment_id, QString equipment_id, QString item_category_id, int num);
    // 多功能校准-稳定性
    static QList<DataCalibratorStability> GetStabilityHeaderAndDataByDataId(QString data_id);
    // 二等铂电阻-稳定性
    static QList<DataCalibratorStability> GetStabilityHeaderAndDataThermometerByDataId(QString data_id);
    // 直流稳压-稳定性
    static QList<DataCalibratorStability> GetStabilityHeaderAndDataDcPowerByDataId(QString data_id);
    // 信号发生器-稳定性
    static QList<DataCalibratorStability> GetStabilityHeaderAndDataSignalGeneratorByDataId(QString data_id);
    // 频谱分析仪-稳定性
    static QList<DataCalibratorStability> GetStabilityHeaderAndDataSpectrumAnalyzerByDataId(QString data_id);
    // 获取核查装置字典：参数核查程序id
    //  static QMap<QString,QMap<QString, QList<QStringList>>> SearchBelongStandardEquipmentById(QString id,QString equipment_id,QMap<QString,QMap<QString, QStringList>> *headList);
    // 同步成功后更新检定项字段
    static bool UpdateMeasureItemInfo(QString item_id);
    // 配套设备关联表
    static bool UpdateStandardEquipmentRelation(QString equipment_id, QString standard_id);
    static QString getStandardItemInfo();
    // 【查询】检定项-铯原子频率
    static QMap<QString, QList<QStringList>> getDeviceCesiumAtomicFrequency(QString itemCatoryName, QString itemId, SequentialMap<QString, QStringList>* headList);
    static QMap<QString, QList<QStringList>> GetDataReturnItemListByCesiumAtomicFrequency(QString itemCatoryName, QList<QStringList> ResultData, QMap<QString, QList<QStringList>> DataMapList, SequentialMap<QString, QStringList>* HeadMapList);
    // 【保存】检定项-铯原子频率
    //     static bool InsertDeviceCesiumAtomicFrequency(CesiumAtomicFrequency scope);
};

#endif // BASECOMMONAPI_H
