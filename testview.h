#ifndef TESTVIEW_H
#define TESTVIEW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QThread>

namespace Ui {
class TestView;
}

//继承线程类，定义1个信号关联1个槽函数
class mythread : public QThread
{
    Q_OBJECT
private:
    void run();

signals: //声明1个信号
    void over();
};


class TestView : public QMainWindow
{
    Q_OBJECT

public:
    explicit TestView(QWidget *parent = nullptr);
    ~TestView();

private slots:
    void initForm();


    void on_pushButton_clicked();

    void on_pushButton_6_clicked();

    void start_two_tree();
public:
    void keyPressEvent(QKeyEvent *event); //键盘按下事件  此声明必须实现 不然会报错
    //void keyReleaseEvent(QKeyEvent *event); //键盘松开事件

    //bool installEventFilter(QObject *filterObj);    //它负责在相应部件上安装事件过滤器
    bool eventFilter(QObject *watched, QEvent *event);     ////它负责在相应部件上安装事件过滤器
    qint8 count=0;
    //线程
    mythread *thread; //定义⼀个线程对象
private:
    Ui::TestView *ui;

};



#endif // TESTVIEW_H
