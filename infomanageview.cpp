#include "infomanageview.h"
#include "ui_infomanageview.h"
#include <QScrollBar>
#include <QToolButton>
#include <QMainWindow>
//子窗体
#include "depar_task.h"
#include "my_task.h"
#include "create_task.h"
#include "beexamined_deviceitem.h"
#include "inspectdataitem.h"
#include "rulefileitem.h"
#include "standard_deviceitem.h"
#include "testdataitem.h"
#include "InfoBase.h"

InfoManageView::InfoManageView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoManageView)
{
    ui->setupUi(this);
    this->setWindowTitle("信息管理");

    //初始化调用所有函数
    this->initForm();
    this->initWidget();
    this->initNav();
    this->initIcon();
}
//左侧滚动条事件
void InfoManageView::showEvent(QShowEvent *)
{
    //滚动到底部
    QScrollBar *bar = ui->scrollArea->verticalScrollBar();
    bar->setValue(bar->maximum());
}

//左侧导航栏样式初始化
void InfoManageView::initIcon()
{
    //左侧导航样式,可以设置各种牛逼的参数,超级棒
   // IconHelper::StyleColor styleColor;
    //styleColor.defaultBorder = true;
   // IconHelper::setStyle(ui->widgetLeft, btns, icons, styleColor);
}

//颜色初始化设置
QList<QColor> InfoManageView::colors = QList<QColor>();
void InfoManageView::initForm()
{
    //颜色集合供其他界面使用
    colors << QColor(211, 78, 78) << QColor(29, 185, 242) << QColor(170, 162, 119) << QColor(255, 192, 1);
    colors << QColor(0, 176, 180) << QColor(0, 113, 193) << QColor(255, 192, 0);
    colors << QColor(72, 103, 149) << QColor(185, 87, 86) << QColor(0, 177, 125);
    colors << QColor(214, 77, 84) << QColor(71, 164, 233) << QColor(34, 163, 169);
    colors << QColor(59, 123, 156) << QColor(162, 121, 197) << QColor(72, 202, 245);
    colors << QColor(0, 150, 121) << QColor(111, 9, 176) << QColor(250, 170, 20);

    ui->scrollArea->setFixedWidth(170);
    ui->widgetLeft->setProperty("flag", "left");
}

//右侧可切换UI导入
void InfoManageView::initWidget()
{
   ui->stackedWidget->addWidget(new Depar_Task);                //部门任务
   ui->stackedWidget->addWidget(new My_Task);                   //我的任务
   ui->stackedWidget->addWidget(new Create_Task);               //创建任务单
   ui->stackedWidget->addWidget(new Standard_DeviceItem);       //标准设备管理
   ui->stackedWidget->addWidget(new BeExamined_DeviceItem);     //被检设备管理
   ui->stackedWidget->addWidget(new TestDataItem);              //检定数据管理
   ui->stackedWidget->addWidget(new InspectDataItem);           //核查数据管理
   ui->stackedWidget->addWidget(new RuleFileItem);              //技术文件管理


}
//左侧列表信息导入
void InfoManageView::initNav()
{
    //按钮文字集合
    QStringList names;
    names << "部门任务" << "我的任务" << "创建任务单" << "标准设备管理" << "被检设备管理"
          << "检定数据管理" << "核查数据管理" << "技术文件管理" ;

    //自动生成按钮
    for (int i = 0; i < names.count(); i++) {
        QToolButton *btn = new QToolButton;
        //设置按钮固定高度
        btn->setFixedHeight(35);
        //设置按钮的文字
        //btn->setText(QString("%1. %2").arg(i + 1, 2, 10, QChar('0')).arg(names.at(i)));
        btn->setText(QString("%1").arg(names.at(i)));
        //设置按钮可选中按下类似复选框的功能
        btn->setCheckable(true);
        //设置按钮图标在左侧文字在右侧
       // btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        //设置按钮拉伸策略为横向填充
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        //关联按钮单击事件
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
        //将按钮加入到布局
        ui->widgetLeft->layout()->addWidget(btn);
        //可以指定不同的图标
        //icons << 0xf061;
        btns << btn;
        //设置按钮大小
        btn->setFixedSize(170, 70);
        //设置鼠标经过和点击事件
        btn->setStyleSheet("QToolButton:hover "
                           "{ background-color: rgba(53, 91, 171, 1); }"
                           "QToolButton::checked {  "
                           "background-color: rgba(53, 91, 171, 1)}");

        //ui->widgetLeft->setFixedWidth(200);
    }

    //底部加个弹簧
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->widgetLeft->layout()->addItem(verticalSpacer);

    btns.at(0)->click();//选择启动页面  第一个
}
//按钮点击切换UI事件
void InfoManageView::buttonClicked()
{
    QAbstractButton *b = (QAbstractButton *)sender();
    int count = btns.count();
    int index = btns.indexOf(b);
    ui->stackedWidget->setCurrentIndex(index);


    // 获取当前在 QStackedWidget 中的部件
    QWidget* currentWidget = ui->stackedWidget->currentWidget();

    // 尝试将当前部件转换为 InfoBase 类型
    InfoBase* currQWidget = qobject_cast<InfoBase*>(currentWidget);
     // 检查转换是否成功
     if (currQWidget != nullptr)
     {
          // 如果转换成功，调用子类Load 方法
          currQWidget->Load();
     }

    for (int i = 0; i < count; i++) {
        QAbstractButton *btn = btns.at(i);
        btn->setChecked(btn == b);
    }
}

InfoManageView::~InfoManageView()
{
    delete ui;
}
