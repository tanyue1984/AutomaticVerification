#ifndef BEEXAMINED_DEVICEITEM_H
#define BEEXAMINED_DEVICEITEM_H

#include <QWidget>
#include "InfoBase.h"

namespace Ui {
class BeExamined_DeviceItem;
}
//被检设备管理界面
class BeExamined_DeviceItem : public InfoBase
{
    Q_OBJECT

public:
    explicit BeExamined_DeviceItem(QWidget *parent = nullptr);
    ~BeExamined_DeviceItem();

private slots:
    void initForm();        //初始化界面数据
    void ShowMessageBox(QString title,QString value);
    //继承父类的Load方法
    void Load() override;
    void OnButtonSelectClicked(QString id);

    void DeviceInfoInit(QString id);

    void on_pushButton_2_clicked();
     bool comboBoxContainsItem(QComboBox *comboBox, const QString &itemText);
    void SetHomePaheCombox(TestedDevice entity);
    void on_select_clicked();

    void on_update_clicked();

private:
    Ui::BeExamined_DeviceItem *ui;
    QList<TestedDevice> data;
};

#endif // BEEXAMINED_DEVICEITEM_H
