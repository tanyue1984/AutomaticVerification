#ifndef GLOBAL_H
#define GLOBAL_H
#include <QString>
#include <QStringList>
// 声明全局变量
extern QString loginUserId; // 登录用户id
extern QString loginUserName; // 登录用户名
extern QString loginUserDeptId; // 登录用户部门id
extern QString loginMeasurePersonId; // 登录用户检定员id
extern QString loginMeasureDeptName; // 登录用户部门
extern QString loginNowVerifyProgramId; // 当次核查程序id
extern QString loginNowVerifyDeviceId; // 当次核查程序选择的标准装置id
extern QString loginNowVerifyEquimentId; // 当次核查程序选择的被核查设备id
extern QStringList equipIdList;
extern QStringList equipNameList;
// extern QString loginOrderId;//当次订单id
#endif // GLOBAL_H
