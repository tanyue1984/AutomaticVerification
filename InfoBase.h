#ifndef INFOBASE_H
#define INFOBASE_H

#include <QWidget>
#include "QStandardItemModel"
#include "databaseconnection.h"
#include "basecommonapi.h"
///信号管理页面的基类 所有信息管理页面都继承于InfoBase
/// 便于统一管理 所有子类必须实现Load方法
/// 每次按钮点击切换都会调用Load方法 用于每次页面的数据刷新
class InfoBase : public QWidget {
    Q_OBJECT
public:
    InfoBase(QWidget *parent = nullptr): QWidget(parent){};
    // 纯虚函数
    virtual void Load()=0;
    //QStandardItemModel 的实例。这个模型将用于存储和管理数据。
    QStandardItemModel *model;
    //信息管理页面 基类定义Conn对象
    BaseCommonApi dbManager;
    //筛选

};
#endif // INFOBASE_H

