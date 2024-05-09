#ifndef NEWCHECKMETHOD_H
#define NEWCHECKMETHOD_H

#include <QWidget>
#include "basecommonapi.h"
#include "checkmethodmanage.h"

namespace Ui {
class NewCheckMethod;
}

class NewCheckMethod : public QWidget
{
    Q_OBJECT

public:
    explicit NewCheckMethod(QWidget *parent = nullptr,QString sId="",bool bEdit=true);
    ~NewCheckMethod();
    void getMethodInfo();
    
    void setMethodName();
private slots:
    void chooseDevices(QList<QString> bizIds);
    void showCheckDevice();
    void choosePicture();
    void deletePicture();
    void showCheckPoint();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();
    void on_cmdButton_clicked(int type,QString id);
    void on_commButton_clicked(int type,QString id);
    void on_comboBoxEquipment_currentIndexChanged(int index);

    void on_comboBoxCategory_currentIndexChanged(int index);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_pushButton_clicked();

protected:
    void showEvent(QShowEvent *event);
public:
    CheckMethodManage *checkManageWnd=nullptr;
private:
    QString methodId="";
    QString checkId="";
    Ui::NewCheckMethod *ui;
    VerificationProgramInfo programInfo;
    bool bEditMode=true;
    QList<itemCategory> catList;
};

#endif // NEWCHECKMETHOD_H
