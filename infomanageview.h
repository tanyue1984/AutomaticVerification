#ifndef INFOMANAGEVIEW_H
#define INFOMANAGEVIEW_H

#include <QWidget>
#include <QMainWindow>

class QAbstractButton;


namespace Ui {
class InfoManageView;
}

class InfoManageView : public QWidget
{
    Q_OBJECT

public:
    static QList<QColor> colors;

    explicit InfoManageView(QWidget *parent = nullptr);
    ~InfoManageView();
private slots:
    void initForm();        //初始化界面数据
    void initWidget();      //初始化子窗体
    void initNav();         //初始化导航按钮
    void initIcon();        //初始化导航按钮图标
    void buttonClicked();   //导航按钮单击事件


protected:
    void showEvent(QShowEvent *);
private:
    Ui::InfoManageView *ui;

    //左侧导航栏图标+按钮集合
    QList<int> icons;
    QList<QAbstractButton *> btns;
};

#endif // INFOMANAGEVIEW_H
