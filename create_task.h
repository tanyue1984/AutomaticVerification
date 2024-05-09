#ifndef CREATE_TASK_H
#define CREATE_TASK_H

#include "InfoBase.h"
#include <QWidget>

namespace Ui {
class Create_Task;
}

class Create_Task : public InfoBase
{
    Q_OBJECT

public:
    explicit Create_Task(QWidget *parent = nullptr);
    ~Create_Task();

    //全局静态变量
    static QString State;
private slots:
    void initForm();        //初始化界面数据
    void ShowMessageBox(QString title,QString value);
    //继承父类的Load方法
    void Load() override;
    void OnButtonSelectClicked(QString id);
    void OnButtonUpdateClicked(QString id);
    void OnButtoDeleteClicked(QString id);
    void on_pushButton_2_clicked();
    void HiddenButton();
    void DisplayButton();

    void on_pushButton_clicked();

    void on_ScanButton_clicked();
   // void UpdatePartTable();
    void on_SaveButton_clicked();
    void SelectPageData(QString id);
    void on_pushButton_8_clicked();

    void on_deletebutton_clicked();
    void SetPartPageComBox();
    void SetHomePaheComboBox(CreateTaskList entity);
    bool comboBoxContainsItem(QComboBox *comboBox, const QString &itemText);

    void on_Update_clicked();


    void on_Select_clicked();

private:

public:
      void DeviceInfoShow(QList<QString> List);
      QList<TestedCatDevice> SelectDataItem;
      QList<QString> _IdList;
      QString TaskId;
      QStandardItemModel *_model;
      QList<CreateTaskList> datares;
private:
    Ui::Create_Task *ui;
};

#endif // CREATE_TASK_H
