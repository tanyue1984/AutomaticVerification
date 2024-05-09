#include "formcheckpoint.h"
#include "ui_formcheckpoint.h"
#include "basecommonapi.h"
#include <QTableWidget>
#include <QHeaderView>

FormCheckPoint::FormCheckPoint(QWidget *parent,QString sId) :
    QWidget(parent),
    checkId(sId),
    ui(new Ui::FormCheckPoint)
{
    ui->setupUi(this);
    if(checkId!="")
    {
        updateTable(checkId);
    }
}

FormCheckPoint::~FormCheckPoint()
{
    delete ui;
}

void FormCheckPoint::updateTable(QString id)
{
    //每个检定项目列头
    SequentialMap<QString, QStringList> headListItem;  //例 <DCV,对应列头>
    //每个检定项目数据
    QMap<QString, QList<QStringList>> dataListItem;  //例 <DCV,对应数据>
    dataListItem=BaseCommonApi::Search_CalibrationTableClass(id,&headListItem);
    QHBoxLayout *layout = ui->HeadListLayout;
    QStackedWidget *stackedWidget = ui->stackedWidgetData;
    //数据清空
    // 清空QStackedWidget中的所有页
    while (stackedWidget->count() > 0) {
        QWidget *widget = stackedWidget->widget(0); // 获取第一个子部件
        stackedWidget->removeWidget(widget); // 从堆叠部件中移除
        delete widget; // 删除部件，防止内存泄漏
    }
    // 清空QHBoxLayout中的所有QPushButton
    while (QLayoutItem *item = layout->takeAt(0)) {
        QWidget *widget = item->widget();
        if (widget && qobject_cast<QPushButton *>(widget)) {
            // 移除并删除QPushButton
            delete widget;
        }
    }
    int index = 0;
    for (const auto &item : headListItem) {
       QTableWidget *tableWidget = new QTableWidget(this);
       tableWidget->setColumnCount(item.count());
       tableWidget->setHorizontalHeaderLabels(item);
       for(int i=0;i<item.count();i++)
           tableWidget->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);
        //设置tableWidget样式表
       tableWidget->verticalHeader()->setParent(nullptr); //隐藏行头
       tableWidget->setStyleSheet("QHeaderView::section {"
                                   "border: 1px solid rgba(197, 196, 196, 1);"
                                   "border-left:none;"
                                   "border-top:none;"
                                   "background-color:white;"
                                  "}");

       // ========================================================================================
       // 数据循环显示
       // ====================================================================================
       int rowCount=0;
       const auto &dataRes = dataListItem.value(headListItem.keys()[index]);
       //tableWidget->setRowCount(data.count());

       for (int i = 0; i < dataRes.count(); ++i) {
           //获取行号 并进行添加行
           rowCount = tableWidget->rowCount();
           tableWidget->insertRow(rowCount);
           tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i+1)));
           for(int j=1;j<dataRes[i].count();j++)
           {
              tableWidget->setItem(i, j, new QTableWidgetItem(dataRes[i][j]));
           }

       }

       stackedWidget->insertWidget(index,tableWidget);
       // ========================================================================================
       // 按钮设置显示
       // ====================================================================================
       QPushButton *button = new QPushButton(headListItem.keys()[index], this);
       //设置样式
       // 设置按钮大小为 56x20
       button->setFixedSize(85, 40);
       button->setCheckable(true);
       button->setAutoExclusive(true);
       //设置第一个按钮选中状态
       if(index==0){  button->setChecked(true);}
       //button->setc(true);
       // 设置按钮样式表
       button->setStyleSheet("QPushButton {"
                               "border: 1px solid white;" // 白色边框
                               "border-bottom: 2px solid rgba(0, 255, 255, 1);" // 浅蓝色下划线
                               "background-color: rgba(242, 242, 242, 1);" // 浅灰色背景
                               "}"
                               "QPushButton:hover {"
                               "background-color: rgba(215, 215, 215, 1);;" // 鼠标悬停时变为深灰色背景
                               "}"
                               "QPushButton:checked {"
                               "color: white;"
                               "background-color: rgb(74, 102, 186);" // 选中时变为蓝色背景
                               "}");

       layout->addWidget(button);
       connect(button, &QPushButton::clicked, stackedWidget, [stackedWidget, index]() {
           stackedWidget->setCurrentIndex(index);
       });

       ++index;
   }
    //默认启动第一个页面
    stackedWidget->setCurrentIndex(0);
}

void FormCheckPoint::on_pushButton_14_clicked()
{
    this->close();
}
