#include "datasyncview.h"
#include "ui_datasyncview.h"
#include "syncdialog.h"


QString filePath = "C:\\AutoVerScheme\\sync_info.txt";
// 获取当前时间戳
DataSyncView::DataSyncView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataSyncView)
{
    ui->setupUi(this);
    this->initForm();
}

DataSyncView::~DataSyncView()
{
    delete ui;
}
void DataSyncView::initForm()
{
    ui->stackedWidget->setCurrentIndex(0);
    //设置控件的边框显示
    ui->tableWidget->setStyleSheet("QHeaderView::section {"
                                "border: 1px solid grey;"
                                "border-left:none;"
                                "border-top:none;"
                                 "background-color:white;"
                                 " color:black;"
                                   "font: bold 18px Arial, sans-serif;"
                               "}");

    ui->tableWidget->setRowCount(10); // 设置10行
    ui->tableWidget->setColumnCount(7); // 设置11列
    ui->tableWidget->setColumnWidth(0, 0); // 设置第一列的宽度为100像素
    ui->tableWidget->setColumnWidth(2, 200); // 设置第一列的宽度为100像素
    ui->tableWidget->verticalHeader()->setParent(nullptr); //隐藏行头



    //初始化TableWidget 设置列
    QStringList headers;
    headers <<"id"  << "序号" << "同步时间" << "是否同步成功" << "备注"
            <<"类型" <<"操作";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    //详情内

    //设置控件的边框显示
    ui->tableWidget_2->setStyleSheet("QHeaderView::section {"
                                "border: 1px solid grey;"
                                "border-left:none;"
                                "border-top:none;"
                                 "background-color:white;"
                                 " color:black;"
                                   "font: bold 18px Arial, sans-serif;"
                               "}");

    ui->tableWidget_2->setRowCount(10); // 设置10行
    ui->tableWidget_2->setColumnCount(3); // 设置11列
    ui->tableWidget_2->setColumnWidth(0, 80); // 设置第一列的宽度为100像素
    ui->tableWidget_2->verticalHeader()->setParent(nullptr); //隐藏行头
    ui->tableWidget_2->setColumnWidth(1, 1000); // 设置第一列的宽度为100像素

    //初始化TableWidget 设置列

    headers << "序号" << "信息" << "状态" ;
    ui->tableWidget_2->setHorizontalHeaderLabels(headers);

    //数据显示

    //先设置为行为0
    ui->tableWidget->setRowCount(0);
    int rowCount=0;
    int row=0;
   QList<zdLog> zdlog= BaseCommonApi::getzdLogByuserId("");
   for(const zdLog & zd:zdlog)
   {
       rowCount = ui->tableWidget->rowCount();
       ui->tableWidget->insertRow(rowCount);
       ui->tableWidget->setItem(row, 0, new QTableWidgetItem(zd.id));
       ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(row+1)));
       ui->tableWidget->setItem(row, 2, new QTableWidgetItem(zd.sync_time.toString()));
       ui->tableWidget->setItem(row, 3, new QTableWidgetItem(zd.is_sync));
       ui->tableWidget->setItem(row, 4, new QTableWidgetItem(zd.remark));
       if(zd.type=="1")
       {
          ui->tableWidget->setItem(row, 5, new QTableWidgetItem("下载"));
       }else{ ui->tableWidget->setItem(row, 5, new QTableWidgetItem("上传")); }

       //添加操作按钮
       QPushButton *button = new QPushButton("查询信息");
       button->setIconSize(QSize(16,16));
       button->setIcon(QIcon(":/image/Index/u2327.svg"));
       ui->tableWidget->setCellWidget(row, 6, button);
       //设置按钮简单样式
       button->setStyleSheet("QPushButton {"
                               "    border: none; /* 无边框 */"
                               "    color: green; /* 文本颜色 */"
                               "    text-decoration: underline; /* 文本下划线 */"
                               "}");

        // 设置鼠标悬停时的光标样式为小手图标
        button->setCursor(Qt::PointingHandCursor);
       // 连接按钮的 clicked 信号到自定义槽函数
        connect(button, &QPushButton::clicked, this, [this, button, zd]() {
            // 获取当前行第一列的 ID

               OnButtonSelectClicked(zd);

        });
    row++;
   }
}
void DataSyncView::on_UploadDataButton_clicked()
{
        SyncDialog *syncDialog = new SyncDialog(this,"1");
         syncDialog->exec();
         this->initForm();
}
void DataSyncView::on_DownloadDataButton_clicked()
{

    SyncDialog *syncDialog = new SyncDialog(this,"0");
    syncDialog->exec(); // 显示进度条窗体并阻塞主窗口

    this->initForm();


}
void DataSyncView::OnButtonSelectClicked(zdLog log)
{
     ui->stackedWidget->setCurrentIndex(1);
     ui->tableWidget_2->setRowCount(0);
     int rowCount=0;
     int row=0;
    QList<zdDataSync> data= BaseCommonApi::getzdDataSyncByuserId(log.id);
    for(const zdDataSync & item:data)
    {
        rowCount = ui->tableWidget_2->rowCount();
        ui->tableWidget_2->insertRow(rowCount);
        ui->tableWidget_2->setItem(row, 0, new QTableWidgetItem(QString::number(row+1)));
         ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(item.remark));
         QTableWidgetItem *itemRow;
        if(item.sync_type=="0"){
           itemRow = new QTableWidgetItem("未同步");
           itemRow->setBackground(QBrush(QColor(255, 255, 192)));
        }else if (item.sync_type=="1")
        {
            itemRow = new QTableWidgetItem("同步成功");
            itemRow->setBackground(QBrush(QColor(173, 255, 47))); // 浅绿色
        }else{
            itemRow = new QTableWidgetItem("同步失败");
            itemRow->setBackground(QBrush(QColor(255, 105, 180))); // 浅红色
        }
          itemRow->setTextAlignment(Qt::AlignCenter);
          ui->tableWidget_2->setItem(row, 2, itemRow);

       row++;
    }
}

void DataSyncView::on_button_return_clicked()
{
     ui->stackedWidget->setCurrentIndex(0);
}


