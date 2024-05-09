#ifndef FORMCOMMSETTING_H
#define FORMCOMMSETTING_H

#include <QWidget>
#include "basecommonapi.h"

namespace Ui {
class FormCommSetting;
}

class FormCommSetting : public QWidget
{
    Q_OBJECT

public:
    explicit FormCommSetting(QWidget *parent = nullptr,int type=1,QString id="");
    ~FormCommSetting();

private slots:
    void on_pushButton_14_clicked();

    void on_radioButton_toggled(bool checked);

    void on_radioButton_2_toggled(bool checked);

private:
    Ui::FormCommSetting *ui;
    QList<VerificationProgramConfigInterface> addrList;
    QString deviceId;
};

#endif // FORMCOMMSETTING_H
