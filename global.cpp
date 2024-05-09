#include "global.h"

// 定义全局变量
QString loginUserId = "0";
QString loginUserName = "";
QString loginUserDeptId = "0";
QString loginMeasurePersonId = "0";
QString loginMeasureDeptName = "";
QString loginNowVerifyProgramId; // 当次核查程序id
QString loginNowVerifyDeviceId; // 当次核查程序选择的标准装置id
QString loginNowVerifyEquimentId; // 当次核查程序选择的被核查设备id
QStringList equipNameList ={"多功能校准源标准装置","0.02级活塞式压力计标准装置","频谱分析仪检定装置","直流稳压电源检定装置","二等铂电阻温度计标准装置"
,"示波器检定装置","信号发生器检定装置","微波衰减标准装置","小功率标准装置","铯原子频率标准装置","低频信号源检定装置","S参数标准装置","失真度标准装置","短期频率稳定度标准装置"};
QStringList equipIdList ={"1","4","9","2","5","13","10","11","15","14","16","17","18","12"};
