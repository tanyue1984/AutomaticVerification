#ifndef DEPAR_TASK_H
#define DEPAR_TASK_H


#include "InfoBase.h"
#include "QStandardItemModel"

namespace Ui {
class Depar_Task;
}

class Depar_Task :public InfoBase
{
    Q_OBJECT

public:
     Depar_Task(QWidget *parent = nullptr);
    ~Depar_Task();

    QList<Dept> data;
private slots:
    void initForm();        //初始化界面
        //初始化数据
    void ShowMessageBox(QString title,QString value);
    //继承父类的Load方法
    void Load() override;

    void on_SelectButton_clicked();

    void on_ResetButton_clicked();
    bool comboBoxContainsItem(QComboBox *comboBox, const QString &itemText);
    void SetHomePaheCombox(Dept entity);
private:
    Ui::Depar_Task *ui;
   // QStandardItemModel *model;
};

#endif // DEPAR_TASK_H
