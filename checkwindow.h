#ifndef CHECKWINDOW_H
#define CHECKWINDOW_H

#include <QWidget>
#include "basecommonapi.h"
#include "visacommonengine.h"
#include "QPushButton"
#include <QtCharts>
#include <QSplineSeries>
#include <QHBoxLayout>
#include <QValueAxis>
#include <QChart>
#include <QChartView>
#include <QTimer>
#include "dialogdcstabinput.h"
#include "SequentialMap.h"

using namespace QtCharts;

namespace Ui {
class CheckWindow; 
}

class ConnectInfo {
public:
   QString id;
   int type;
   QString deviceName;
   QString state;
   QString rolenName;
   VerificationProgramConfigInterface programInterface;
   QList<InstructionLib> cmdList;
   VisaCommonEngine* visaEngine=nullptr;
};

class CheckWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CheckWindow(QWidget *parent = nullptr);
    ~CheckWindow();
private slots:

    void showMethod();
    void showDevice();
    void showCheck();
    void showMethodWidget();
    void showResult();
    void on_editButton_clicked();
    void on_tableWidgetDevice_cellChanged(int row, int column);

    void on_tableWidgetMethod_cellChanged(int row, int column);

    void on_pushButton_9_clicked();

    void on_tableWidget_3_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_deviceButton_clicked(int idx);
    void on_pushButton_start_clicked();

    void on_pushButton_pause_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

   void on_comboBox_visa_OnCurrentTextChanged(QString arg1);
   bool getJoinState(QString arg1);
   void gageDataChange();

   void addGageData(bool bAdd=true);
   void on_pushButton_next_clicked();
   void onComboBoxCurrTextChanged(QString value,QString device_id,int row);
   QMap<QStringList, QStringList> readDataFromFile(QString filePath);
   void RefreshTime_Slot();
   void on_pushButtonGetTemp_clicked();
   void readDCStabData(QString sLabel);
   void sendDCStabData(QString sLabel);
   void on_pushButton_Hidd_clicked();
   void SignalGeneratortextflow();

private:
    void getCheckDevice(bool bInit=true);
    void getCheckMethod();
    void initDeviceInfo(QList<QString> sId);
    void deleteItem(QLayout *layout);
    void initCheckTable(int idx);
    InstructionLib* getCmdByCheckName(int idx,QString name);
    void Delay_MSec(unsigned int msec);
    void SetHomePaheCombox(MyTask task);
    bool comboBoxContainsItem(QComboBox *comboBox, const QString &itemText);
    void showStandard();
    void connectDevice();
    int  getIndexByHeader(QString sLabel,QStringList sName);
    double transUnit(QString sData,QString sUnit,bool reverse=false);
    QString transCmd(QString sCmd,QString sLabel,QStringList data,bool reverse=false);
    void SetStandardCombox(CatDeviceInfor info);
    void updateStandardTable();
    QString TranDecimals(QString sLabel,QStringList data,double TestValue,QString ColName=NULL);
    void autoAdujstData(QString sLabel,int i,double dStdValue,QList<QStringList>& dataRes);
    QString TranTypeUnit(QString sUnit);
    void Oscilloscopetextflow();
    void Spectrumtextflow();
    void closeDevice();
    double tranunitSplit(QString sData,bool reverse=false);
    void ItemUnitSplit(QString Value,QString &data,QString &unit);
    void CheckBackColorUpdate(bool State,int row);
    QString Removeunit(QString data);
    bool showDialog(QString title,QString mess);
    int TextCount;
    void openGageResult();
    void stepGageCheck();
    QStringList getsParambyHeader(QString sParam);
    QStringList QStrTranQStrList(QString value);
    void changeDevice(int idx);
    void keyPressEvent(QKeyEvent *ev);
    QString GetdeviceidByRole(QString device_id);
    InstructionLib* getCmdByRoleName(QString RoleName,QString name);
    VisaCommonEngine getVisaCommbyRoleName(QString RoleName);
    QString ReadDevice(InstructionLib* Inst,VisaCommonEngine* Visa,bool IsUnitTran=false);
    void SendDevice(InstructionLib* Inst,VisaCommonEngine* Visa,bool IsUnitTran=false);
    void InItcmd();
    void closeCmd();
    void CreateCharts();
    double InputDialog(QString Content);
private:
    Ui::CheckWindow *ui;
    QList<VerificationProgramInfo> programList;
//    QList<Dept> myTaskList;
    QList<MyTask>  myTaskList;
    QList<MyTask> selectTaskList;
    VerificationProgramInfo programInfo;
    QList<ConnectInfo> connectList;
    QStringList headList;
    QList<QStringList> calibrationList;
    VisaCommonEngine deviceEngine;
    VisaCommonEngine standardEngine;
    VisaCommonEngine engine;;
    QMap<QString,VisaCommonEngine> VisaStanEngineList;
    QList<VisaCommonEngine> measureCa3lList;
    QList<SaveDeviceMeasureCalibrator> measureCalList;
    SaveDeviceMeasureInfo measureInfo;
    QMap<QString, QList<QStringList>> *caliDataList;
    SequentialMap<QString, QStringList> *headTableList;
    QList<CatDeviceInfor> catDeviceList;
    QStringList catIdList;
    int curHeadIdx=-1;
    int iEquipType=1;

    QMap<QStringList, QStringList> unitTransList;
    QMap<QStringList, QStringList> resultfillingList;
    QMap<QStringList, QStringList> unitTypeTransList;
    QMap<QStringList, QStringList> cmdPlaceHolderList;

    QList<QString> StandarIdList;
    QList<QString> StanDeviceIDList;
    QList<QString> StanDeviceNameList;
    QString standardId;
    int currConnectIdx;
    QList<QPushButton *> pageButtons;
    QList<QString> chList;
    QList<QStringList> gaugeInfo;
    bool isBack = false; // 是否处于倒序状态
    int lastSelectedIndex = -1; // 记录上一次选中的行号

    QList<QMap<QString, QList<QStringList>>> caliDataListAll;
    QList<SequentialMap<QString, QStringList>> headTableListAll;
    QString curDeviceId="";
    int curDevciceIdx=-1;
    int nextGageIndex=-1;

    QList<DeviceMeasureEquipmentCatory> DeviceMeasureEquipmentCatorycatory;
    QList<DeviceRole> RoleList;

    QString constsLable;
    QStringList constDataRes;

    QChartView *chartView;
    QTimer* timerChart;
    SyncRunStatus Teststate;
    DialogDCStabInput* stabInputdlg;

    int NumberCount;
};

#endif // CHECKWINDOW_H
