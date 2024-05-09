#ifndef CHECKDEVICE_H
#define CHECKDEVICE_H

#include <QWidget>
#include "basecommonapi.h"
#include "newcheckmethod.h"

namespace Ui {
class CheckDevice;
}

class CheckDevice : public QWidget
{
    Q_OBJECT

public:
    explicit CheckDevice(QWidget *parent = nullptr);
    ~CheckDevice();

private slots:
    void on_pushButton_19_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();
    void onpageChanged(int currPage);

    void OnitemChanged(int currPage,Qt::CheckState State);
private:
    void getCheckDevice();
    void getStandardDevice();

public:
    NewCheckMethod *newCheckWnd=nullptr;
private:
    Ui::CheckDevice *ui;
    QList<TestedDevice> testedDeviceList;
    QList<BizDevice> bizDeviceList;
    int page;
    int totalcount;
    int pageIndex;
    QMap<int,bool> CheckState;
};

#endif // CHECKDEVICE_H
