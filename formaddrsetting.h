#ifndef FORMADDRSETTING_H
#define FORMADDRSETTING_H

#include <QWidget>
#include "basecommonapi.h"
#include "CheckBoxTableHeader.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

namespace Ui {
class FormAddrSetting;
}

class FormAddrSetting : public QWidget
{
    Q_OBJECT

public:
    explicit FormAddrSetting(QWidget *parent = nullptr,int type=0,QString id="",QString deviceId="",bool check=true,QString equipId="",QString catId="");
    ~FormAddrSetting();
private:
    void initCmdTable();
    void updateRow(int row,InstructionLib cmd);

private slots:
    void on_pushButton_15_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_18_clicked();
    void on_editButton_clicked();
    void on_deleteButton_clicked();
    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_ExportCmdInfo_clicked();

private:
    Ui::FormAddrSetting *ui;
    QList<InstructionLib> cmdList;
    QString sId;
    QString sDeviceId;
    int iType=0;
    int editIdx=-1;
    bool checkMode=true;
    QString sEquipId;
    QString sCatId;
    QList<QString> CheckItemNameList;
    CheckBoxTableHeader *header=NULL;
};

#endif // FORMADDRSETTING_H
