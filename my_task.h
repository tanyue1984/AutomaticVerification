#ifndef MY_TASK_H
#define MY_TASK_H

#include <QWidget>
#include "InfoBase.h"

namespace Ui {
class My_Task;
}

class My_Task :public InfoBase
{
    Q_OBJECT

public:
    explicit My_Task(QWidget *parent = nullptr);
    ~My_Task();


private slots:
     void initForm();        //初始化界面数据
     void ShowMessageBox(QString title,QString value);
     void Load() override;
     bool comboBoxContainsItem(QComboBox *comboBox, const QString &itemText);
     void SetHomePaheCombox(MyTask entity);
     void on_pushButtonUpdate_clicked();

     void on_SeachButton_clicked();
     void onpageChanged(int page);
public:
     QList<MyTask> data;
     int page;
     int totalcount;
private:
    Ui::My_Task *ui;
};

#endif // MY_TASK_H
