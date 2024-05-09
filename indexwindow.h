#ifndef INDEXWINDOW_H
#define INDEXWINDOW_H

#include <QMainWindow>
#include "datasyncView.h"
#include "infomanageview.h"



namespace Ui {
class IndexWindow;
}

class IndexWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit IndexWindow(QWidget *parent = nullptr);
    ~IndexWindow();
private slots:
    void initForm();
    void loadStyle(const QString &qssFile);
    void showCheckWindow();
    void showCheckMethod();
    void showVerifyWindow();
    void showVerifyMethod();


    void on_tb3_clicked();

    void on_tb6_clicked();

    //页面关闭响应函数
    void onNewWindowClosed();



    void ShowMessageBox(QString title,QString value);
private:
    Ui::IndexWindow *ui;

    //数据同步页面 指针
    //DataSyncView *ShowDataSyncView;
    //InfoManageView *ShowInfoManageView;
};

#endif // INDEXWINDOW_H
