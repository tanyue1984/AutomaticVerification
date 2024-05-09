#ifndef STANDARD_DEVICEITEM_H
#define STANDARD_DEVICEITEM_H

#include <QWidget>
#include "InfoBase.h"

namespace Ui {
class Standard_DeviceItem;
}

class Standard_DeviceItem : public InfoBase
{
    Q_OBJECT

public:
    explicit Standard_DeviceItem(QWidget *parent = nullptr);
    ~Standard_DeviceItem();
private slots:
    void initForm();        //初始化界面数据
    void ShowMessageBox(QString title,QString value);
    //继承父类的Load方法
    void Load() override;
    void OnButtonSelectClicked(QString id);
    void on_pushButton_clicked();
    void DeviceInfoInit(QString id);
    void SetHomePaheCombox(BizDevice entity);
    bool comboBoxContainsItem(QComboBox *comboBox, const QString &itemText);
    void on_Select_clicked();

    void on_pushButton_8_clicked();
public:
       QList<BizDevice> data;
       void onpageChanged(int currPage);
       int page;
       int totalcount;
private:
    Ui::Standard_DeviceItem *ui;
};

#endif // STANDARD_DEVICEITEM_H
