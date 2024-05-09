#ifndef VERIFYDEVICE_H
#define VERIFYDEVICE_H

#include <QWidget>
#include "basecommonapi.h"
#include "newverifymethod.h"

namespace Ui {
class VerifyDevice;
}

class VerifyDevice : public QWidget
{
    Q_OBJECT

public:
    explicit VerifyDevice(QWidget *parent = nullptr);
    ~VerifyDevice();
private slots:
    void on_pushButton_14_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_12_clicked();
signals:
    void selectStandard(QString ids);
private:
    void getVerifyDevice();
    void getCheckDevice();

private:
    Ui::VerifyDevice *ui;
    QList<CheckAndTestedDevice> testedDeviceList;
    QList<CheckAndTestedDevice> bizDeviceList;
};

#endif // VERIFYDEVICE_H
