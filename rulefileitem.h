#ifndef RULEFILEITEM_H
#define RULEFILEITEM_H

#include <QWidget>
#include "InfoBase.h"

namespace Ui {
class RuleFileItem;
}

class RuleFileItem : public InfoBase
{
    Q_OBJECT

public:
    explicit RuleFileItem(QWidget *parent = nullptr);
    ~RuleFileItem();
private slots:
    void initForm();        //初始化界面数据
    void ShowMessageBox(QString title,QString value);
    //继承父类的Load方法
    void Load() override;
    void OnButtonSelectClicked(QString id);
    void OnButtonPreViewtClicked(QString id);
private:
    Ui::RuleFileItem *ui;
};

#endif // RULEFILEITEM_H
