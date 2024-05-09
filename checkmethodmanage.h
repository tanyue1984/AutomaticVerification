#ifndef CHECKMETHODMANAGE_H
#define CHECKMETHODMANAGE_H

#include <QWidget>
#include "basecommonapi.h"

namespace Ui {
class CheckMethodManage;
}

class CheckMethodManage : public QWidget
{
    Q_OBJECT

public:
    explicit CheckMethodManage(QWidget *parent = nullptr);
    ~CheckMethodManage();
    void updateTable(bool insert,VerificationProgramInfo program);
private:
    void getCheckMethod();
    void updateRow(int row,VerificationProgramInfo program);
private slots:
    void showNewMethod();
    void on_editButton_clicked(VerificationProgramInfo program);
    void on_deleteButton_clicked(VerificationProgramInfo program);

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();
    void onpageChanged(int currPage);

private:
    Ui::CheckMethodManage *ui;
    QList<VerificationProgramInfo> programList;
    int editIdx=-1;
    QStringList equipList={"多功能校准源标准装置","直流稳压电源检定装置","多功能电气安全标准器标准装置","0.02级活塞式压力计标准装置","二等铂电阻温度计标准装置","安全阀校验装置",
                          "精密露点仪标准装置","E2等级砝码标准装置","频谱分析仪标准装置","信号发生器标准装置","微波衰减标准装置","频率短期稳定度标准装置","示波器标准装置","铯原子频率标准装置","小功率标准装置"
                          ,"低频信号发生器标准装置","S参数标准装置","失真度标准装置"};
     QList<itemCategory> catList;
     int page;
     int totalcount;
     int pageIndex;
};

#endif // CHECKMETHODMANAGE_H
