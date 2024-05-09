#ifndef CREATETASK_SCANDEVICE_H
#define CREATETASK_SCANDEVICE_H

#include <QMainWindow>
#include "basecommonapi.h"
#include "create_task.h"

namespace Ui {
class CreateTask_ScanDevice;
}

class CreateTask_ScanDevice : public QMainWindow
{
    Q_OBJECT

public:
    explicit CreateTask_ScanDevice(QWidget *parent = nullptr);
    ~CreateTask_ScanDevice();

    BaseCommonApi dbManager;
private slots:
    void on_AddButton_clicked();
    void initData();
    void on_CancelButton_clicked();
public:
    Create_Task *CreateTaskInst=nullptr;
private:

    Ui::CreateTask_ScanDevice *ui;
};

#endif // CREATETASK_SCANDEVICE_H
