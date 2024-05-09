#ifndef NEWVERIFYMETHOD_H
#define NEWVERIFYMETHOD_H

#include <QWidget>
#include "verifymethodmanage.h"
#include "basecommonapi.h"

namespace Ui {
class NewVerifyMethod;
}

class NewVerifyMethod : public QWidget
{
    Q_OBJECT

public:
    explicit NewVerifyMethod(QWidget *parent = nullptr,QString sId="",bool bEdit=true);
    ~NewVerifyMethod();
    void getMethodInfo();
    void chooseDevices(QString bizIds,QString checkIds);
private slots:
    void showVerifyDevice();
    void showVeryPoint();
    void on_cmdButton_clicked(int type,QString id);
    void on_commButton_clicked(int type,QString id);
    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_comboBoxEquipment_currentIndexChanged(int index);
    void selectStandard(QString ids);
    void selectBizDevice(QList<QString> ids);

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

public:
    VerifyMethodManage *verifyManageWnd=nullptr;
private:
    Ui::NewVerifyMethod *ui;
    QString methodId="";
    QString checkId="";
    CheckProgramManage programInfo;
    bool bEditMode=true;
    QList<StandardCheckItemCategory> catList;
};

#endif // NEWVERIFYMETHOD_H
