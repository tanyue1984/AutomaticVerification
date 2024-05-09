#ifndef DIALOGCHECKRESULT_H
#define DIALOGCHECKRESULT_H

#include <QDialog>
#include "basecommonapi.h"
#include "wordengine.h"

namespace Ui {
class DialogCheckResult; 
}

class DialogCheckResult : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCheckResult(QWidget *parent = nullptr);
    ~DialogCheckResult();
    void initCheckTable(QList<QMap<QString, QList<QStringList>>> measList,QList<SequentialMap<QString, QStringList>> headList,QList<QStringList> StanInfo);

private slots:
    void on_pushButtonPreview_clicked();

    void on_pushButtonSave_clicked();

    void on_pushButtonPrint_clicked();


private:
    void deleteItem(QLayout *layout);
    void initCalDataTable(int idx);
    QString getValueByHeader(int idx,QString sLabel,QString sName);
    void on_deviceButton_clicked(int idx);
public:
    QList<MyTask> taskList;
    QString deviceId;
    QList<QStringList> StanDeviceInfo;
private:
    Ui::DialogCheckResult *ui;
    QList<SaveDeviceMeasureCalibrator> measureCalList;
    QStringList measHeadList;
    QMap<QString, QList<QStringList>> *caliDataList;
    SequentialMap<QString, QStringList> *headTableList;

    QList<QMap<QString, QList<QStringList>>> caliDataListAll;
    QList<SequentialMap<QString, QStringList>> headTableListAll;
    QStringList stdId;
    int curHeadIdx=-1;
    int iEquipType;
    QList<DeviceMeasureEquipmentCatory> catoryList;
    QMap<QString,QString> checkitemlist;
    QString getDictebyItemName(QString Name);
    void ItemUnitSplit(QString Value,QString &data,QString &unit);
    int curDevciceIdx=-1;
};

#endif // DIALOGCHECKRESULT_H
