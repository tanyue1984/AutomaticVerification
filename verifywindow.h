#ifndef VERIFYWINDOW_H
#define VERIFYWINDOW_H

#include <QWidget>
#include "basecommonapi.h"
#include "checkwindow.h"

namespace Ui {
class VerifyWindow;
}

class VerifyWindow : public QWidget
{
    Q_OBJECT

public:
    explicit VerifyWindow(QWidget *parent = nullptr);
    ~VerifyWindow();
private:
    void getVerifyMethod();
    void initDeviceInfo(QString stdId,QString bizId);
    void deleteItem(QLayout *layout);
    void initCheckTable(int idx);
    void initCheckItemData(int idx,QString filter="ALL",bool bForce=false);
    InstructionLib* getCmdByCheckName(int idx,QString name);
    void Delay_MSec(unsigned int msec);
    void selectStandInfo();
    void selectBizInfo();
    void getCheckItemData();
    void startVerify();
    int getIndexByHeader(QString sLabel,QString sLabel2,QStringList sName);
    QStringList QStrTranQStrList(QString value);
    bool showDialog(QString title,QString mess);
    QString ReadDevice(InstructionLib* Inst,VisaCommonEngine* Visa,bool IsUnitTran=false);
    void SendDevice(InstructionLib* Inst,VisaCommonEngine* Visa,bool IsUnitTran=false);
    void InItcmd();
    void closeCmd();
    void connectDevice();
    void closeDevice();
    QString transCmd(QString sCmd,QString sLabel,QString sLabelItem,QStringList data,bool reverse=false);
    QMap<QStringList, QStringList> readDataFromFile(QString filePath);
    double transUnit(QString sData,QString sUnit,bool reverse=false);
    void  autoAdujstData(QString sLabel,QString labelItem,QString header,int i,double dStdValue,QList<QStringList>& dataRes,QMap<QString, QList<QStringList>>& veriData);
    QString TranTypeUnit(QString sUnit);
    double  tranunitSplit(QString sData,bool reverse);
    void  ItemUnitSplit(QString Value,QString &data,QString &unit);
    void  CheckBackColorUpdate(bool State,int row);
    VisaCommonEngine* getVisaEngineByIdx(int idx);
    QString FormatDecimals(double dVal,int pos=-1);
    double getRepeatData(QStringList dataStable,int idx,double& dMean);
private slots:
    void showDevice();
    void on_editButton_clicked();

    void on_tableWidgetMethod_cellChanged(int row, int column);

    void on_tableWidget_3_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_pushButton_13_clicked();

    void on_pushButton_start_clicked();

    void on_pushButton_pause_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_12_clicked();
    void on_deviceButton_clicked(CatDeviceInfor info,int idx);

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_tableWidgetStd_cellChanged(int row, int column);

    void on_tableWidgetBiz_cellChanged(int row, int column);
    void on_comboBox_visa_OnCurrentTextChanged(QString arg1);
    bool getJoinState(QString arg1);

    void on_pushButtonSaveAddress_clicked();

    void on_tableWidgetCheck_cellChanged(int row, int column);
    void setReadItemHeader();

private:
    Ui::VerifyWindow *ui;
    QList<CheckProgramManage> programList;
    CheckProgramManage programInfo;
    QList<ConnectInfo> connectList;
    QList<CatDeviceInfor> catDeviceList;
    QStringList headList;
    QList<QStringList> calibrationList;
    QList<StandCheckDataCalibrator> measureCalList;
    //VisaCommonEngine deviceEngine;
    //VisaCommonEngine standardEngine;
    QList<CatDeviceInfor> stdList;
    QList<CatDeviceInfor> bizList;
    QStringList stdIdList;
    QStringList bizIdList;
    QString selectStdIds;
    QString selectBizIds;
    QMap<QString,QMap<QString, QList<QStringList>>> veriDataMapList;
    QMap<QString,QMap<QString, QStringList>> veriHeadList;
    int iEquipType=1;
    int mainTabIdx=-1;
    int itemTabIdx=-1;
    QString constsLable;
    QString constsLableItem;
    QStringList constDataRes;
    SyncRunStatus Teststate;
    QMap<QStringList, QStringList> unitTransList;
    QMap<QStringList, QStringList> resultfillingList;
    QMap<QStringList, QStringList> unitTypeTransList;
    QMap<QStringList, QStringList> cmdPlaceHolderList;
    QStringList readItems;
    int currConnectIdx;
    QList<DeviceMeasureEquipmentCatory> DeviceMeasureEquipmentCatorycatory;
    QList<DeviceRole> RoleList;
    bool bizOutMode =true; //被核查件输出，反之标准器输出


    void autoCalculateGaugeCheckData(int row, int col);
};

#endif // VERIFYWINDOW_H
