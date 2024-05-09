#ifndef DIALOGVERIFYRESULT_H
#define DIALOGVERIFYRESULT_H

#include <QDialog>
#include "basecommonapi.h"

namespace Ui {
class DialogVerifyResult;
}

class DialogVerifyResult : public QDialog
{
    Q_OBJECT

public:
    explicit DialogVerifyResult(QWidget *parent = nullptr);
    ~DialogVerifyResult();
    void setData(int iType,QMap<QString,QMap<QString, QList<QStringList>>>* data,
    QMap<QString,QMap<QString, QStringList>>* header,CheckProgramManage* program,QString stdId,QString bizIds);
private slots:
    void on_pushButtonSave_2_clicked();
    
private:
    void deleteItem(QLayout *layout);
    void initCheckTable(int idx,QString filter="ALL",bool bForce=false);
    QString getValueByHeader(QStringList dataList,QStringList headList,QString sName);
    void ItemUnitSplit(QString Value,QString &data,QString &unit);

private:
    Ui::DialogVerifyResult *ui;
    int mainTabIdx=-1;
    int iEquipType=1;
    QMap<QString,QMap<QString, QList<QStringList>>>* veriDataMapList;
    QMap<QString,QMap<QString, QStringList>>* veriHeadList;
    CheckProgramManage* programInfo;
    QString selectStdIds;
    QString selectBizIds;
};

#endif // DIALOGVERIFYRESULT_H
