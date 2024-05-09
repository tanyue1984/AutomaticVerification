#ifndef FORMBIZDEVICESELECT_H
#define FORMBIZDEVICESELECT_H

#include <QWidget>
#include "basecommonapi.h"

namespace Ui {
class FormBizDeviceSelect;
}

class FormBizDeviceSelect : public QWidget
{
    Q_OBJECT

public:
    explicit FormBizDeviceSelect(QWidget *parent = nullptr);
    ~FormBizDeviceSelect();

private slots:
    void onpageChanged(int currPage);

    void on_comboBox_3_currentIndexChanged(int index);

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_tableWidget_2_cellChanged(int row, int column);

    void on_pushButton_15_clicked();

    void on_pushButton_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();
signals:
    void selectBizDevice(QList<QString> ids);
private:
    void getStandardInfo();
    void getBizDeviceInfo();
private:
    int page=20;
    int totalcount;
    int pageIndex=-1;
    int stdIdx=-1;
    QList<StandardInfo> stdInfo;
    QList<BizDevice> bizInfo;
private:
    Ui::FormBizDeviceSelect *ui;
};

#endif // FORMBIZDEVICESELECT_H
