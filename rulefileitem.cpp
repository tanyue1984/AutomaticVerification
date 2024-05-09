#include "rulefileitem.h"
#include "ui_rulefileitem.h"
#include "QMessageBox"


RuleFileItem::RuleFileItem(QWidget *parent) :
    InfoBase(parent),
    ui(new Ui::RuleFileItem)
{
    ui->setupUi(this);
    this->initForm();
}

RuleFileItem::~RuleFileItem()
{
    delete ui;
}
void RuleFileItem::initForm()
{



    //tableWidget设置
    //设置控件的边框显示
    ui->tableWidget->setStyleSheet("QHeaderView::section {"
                                "border: 1px solid rgba(197, 196, 196, 1);"
                                "border-left:none;"
                                "border-top:none;"
                                "background-color:white;"
                               "}");

    //ui->tableWidget->setRowCount(10); // 设置10行
    ui->tableWidget->setColumnCount(10); // 设置11列
    ui->tableWidget->setColumnWidth(0, 80); // 设置第一列的宽度为100像素
    ui->tableWidget->verticalHeader()->setParent(nullptr); //隐藏行头
    //QTableWidgetItem *item = new QTableWidgetItem("测试数据");
    //item->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setColumnWidth(0, 0); //设置数据

    //初始化TableWidget 设置列
    QStringList headers;
    headers <<"ID"<< "序号" << "文件号" << "文件名称" << "版本号" << "文件类型"<< "颁布时间"
            << "实施时间" <<"备注"<<"操作";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
}
void RuleFileItem::ShowMessageBox(QString title,QString value) {
    // 创建一个QMessageBox对象
    QMessageBox msgBox;

    // 设置消息框的标题
    msgBox.setWindowTitle(title);

    // 设置消息框的文本内容
    msgBox.setText(value);

    // 设置消息框的标准按钮，这里是“确定”按钮
    msgBox.setStandardButtons(QMessageBox::Ok);

    // 显示消息框
    msgBox.exec();
}
void RuleFileItem::Load()
{// ========================================================================================
    // 主页面初始化 技术文件初始化
    // ====================================================================================
    ui->tableWidget->setRowCount(0);

    //先设置为行为0
    ui->tableWidget->setRowCount(0);
    // 调用数据库管理器的函数获取数据
    int rowCount=0;
    int row=0;
    QList<Technique_File> data = dbManager.getTechnique_File();
    for (const Technique_File& item : data) {
         rowCount = ui->tableWidget->rowCount();

         ui->tableWidget->insertRow(rowCount);
         ui->tableWidget->setItem(row, 0, new QTableWidgetItem(item.id));
         ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(row+1)));
         ui->tableWidget->setItem(row, 2, new QTableWidgetItem(item.file_distribute_no));
         ui->tableWidget->setItem(row, 3, new QTableWidgetItem(item.file_name));
         ui->tableWidget->setItem(row, 4, new QTableWidgetItem("缺少"));
         ui->tableWidget->setItem(row, 5, new QTableWidgetItem(item.version_no));
         ui->tableWidget->setItem(row, 6, new QTableWidgetItem(item.active_date.toString()));
         ui->tableWidget->setItem(row, 7, new QTableWidgetItem("缺少"));
         ui->tableWidget->setItem(row, 8, new QTableWidgetItem(item.remark));


         //实现单元格添加两个按钮
         // 创建一个QWidget作为单元格的容器
         QWidget *widget = new QWidget();
         QHBoxLayout *layout = new QHBoxLayout(widget); // 水平布局


         QPushButton *button = new QPushButton("预览");
         QPushButton *button2 = new QPushButton("下载");

         //设置按钮1简单样式
         button->setStyleSheet("QPushButton {"
                                 "    border: none; /* 无边框 */"
                                 "    color: green; /* 文本颜色 */"
                                 "    text-decoration: underline; /* 文本下划线 */"
                                 "}");

          // 设置鼠标悬停时的光标样式为小手图标
          button->setCursor(Qt::PointingHandCursor);
          button->setIconSize(QSize(16,16));
             button->setIcon(QIcon(":/image/Index/u5496.svg"));

         // 连接按钮的 clicked 信号到自定义槽函数
          connect(button, &QPushButton::clicked, this, [this, button, row]() {
              // 获取当前行第一列的 ID
              QTableWidgetItem *idItem = this->ui->tableWidget->item(row, 0);
              if (idItem) {
                  QString id = idItem->text();
                  OnButtonSelectClicked(id);
              }

          });
          //设置按钮2简单样式
          button2->setStyleSheet("QPushButton {"
                                  "    border: none; /* 无边框 */"
                                  "    color: green; /* 文本颜色 */"
                                  "    text-decoration: underline; /* 文本下划线 */"
                                  "}");

           // 设置鼠标悬停时的光标样式为小手图标
           button2->setCursor(Qt::PointingHandCursor);
           button2->setIconSize(QSize(16,16));
                button2->setIcon(QIcon(":/image/Index/u7313.svg"));
          // 连接按钮的 clicked 信号到自定义槽函数
           connect(button2, &QPushButton::clicked, this, [this, button2, row]() {
               // 获取当前行第一列的 ID
               QTableWidgetItem *idItem = this->ui->tableWidget->item(row, 0);
               if (idItem) {
                   QString id = idItem->text();
                   OnButtonPreViewtClicked(id);
               }

           });
           //将按钮添加到布局控件
           layout->addWidget(button);
           layout->addWidget(button2);
           layout->setContentsMargins(5, 5, 0, 0); // 设置布局边距为0
           widget->setLayout(layout);
           //将布局控件添加到 单元格
           ui->tableWidget->setCellWidget(row, 9, widget);
           // 调整表格大小以适应内容
            //ui->tableWidget->resizeColumnsToContents();
           // ui->tableWidget->resizeRowsToContents();
         row++;
        }
}
void RuleFileItem::OnButtonSelectClicked(QString id)
{
    // ========================================================================================
    // 详情按钮 点击事件  进入详情页面
    // ====================================================================================

     QMessageBox::information(this, "详情按钮ID",id);
}
void RuleFileItem::OnButtonPreViewtClicked(QString id)
{
    // ========================================================================================
    // 下载按钮 点击事件  进入详情页面
    // ====================================================================================
     //DeviceInfoInit(id);
     //ui->stackedWidget->setCurrentIndex(1);
     QMessageBox::information(this, "下载按钮ID",id);
}
