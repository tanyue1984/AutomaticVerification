#ifndef INSPECTDATAITEM_H
#define INSPECTDATAITEM_H

#include <QWidget>
#include "InfoBase.h"

namespace Ui {
class InspectDataItem;
}

class InspectDataItem : public InfoBase
{
    Q_OBJECT

public:
    explicit InspectDataItem(QWidget *parent = nullptr);
    ~InspectDataItem();
private slots:
    void initForm();        //初始化界面数据
    void ShowMessageBox(QString title,QString value);
    //继承父类的Load方法
    void Load() override;
    void OnButtonSelectClicked(CheckData entity);
    void OnButtonPreViewtClicked(QString id);
    void on_pushButton_clicked();
     void DeviceInfoInit(CheckData entity);
     void SetHomePaheCombox(CheckData entity);
     bool comboBoxContainsItem(QComboBox *comboBox, const QString &itemText);
     void on_select_clicked();

     void on_update_clicked();

private:
    Ui::InspectDataItem *ui;
     QList<CheckData> data;
};

#endif // INSPECTDATAITEM_H
