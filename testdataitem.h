#ifndef TESTDATAITEM_H
#define TESTDATAITEM_H

#include <QWidget>
#include "InfoBase.h"

namespace Ui {
class TestDataItem;
}

class TestDataItem : public InfoBase
{
    Q_OBJECT

public:
    explicit TestDataItem(QWidget *parent = nullptr);
    ~TestDataItem();
private slots:
    void initForm();        //初始化界面数据
    void ShowMessageBox(QString title,QString value);
    //继承父类的Load方法
    void Load() override;
    void OnButtonSelectClicked(VerificationData id);
    void on_pushButton_clicked();
    void OnButtonPreViewtClicked(QString id);
    void DeviceInfoInit(VerificationData id);
    bool comboBoxContainsItem(QComboBox *comboBox, const QString &itemText);
    void SetHomePaheCombox(VerificationData entity);
    void on_select_clicked();

    void on_groupBox_2_clicked();

private:
    Ui::TestDataItem *ui;
    QList<VerificationData> data;
};

#endif // TESTDATAITEM_H
