#include "inspectdataitem.h"
#include "ui_inspectdataitem.h"
#include "QMessageBox.h"

InspectDataItem::InspectDataItem(QWidget *parent) :
    InfoBase(parent),
    ui(new Ui::InspectDataItem)
{
    ui->setupUi(this);
    this->initForm();
}

InspectDataItem::~InspectDataItem()
{
    delete ui;
}
void InspectDataItem::initForm()
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
    ui->tableWidget->setColumnCount(13); // 设置11列
    ui->tableWidget->setColumnWidth(0, 80); // 设置第一列的宽度为100像素
    ui->tableWidget->verticalHeader()->setParent(nullptr); //隐藏行头
    //QTableWidgetItem *item = new QTableWidgetItem("测试数据");
    //item->setTextAlignment(Qt::AlignCenter);
    //ui->tableWidget->setItem(0, 0, item); //设置数据
    ui->tableWidget->setColumnWidth(0,0);
    //初始化TableWidget 设置列
    QStringList headers;
    headers <<"ID"<< "序号" << "设备名称" << "型号规则" << "出厂编号"
            <<"核查件名称"<<"核查件型号"<<"核查件编号" <<"核查员"
           << "核查日期" <<"实验室"<<"部门"<<"操作";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    //设置tableWidgetItem
    ui->tableWidgetItem->setStyleSheet("QHeaderView::section {"
                                "border: 1px solid rgba(197, 196, 196, 1);"
                                "border-left:none;"
                                "border-top:none;"
                                "background-color:white;"
                               "}");

   // ui->tableWidget->setRowCount(10); // 设置10行
    ui->tableWidgetItem->setColumnWidth(1, 80); // 设置第一列的宽度为100像素
    ui->tableWidgetItem->verticalHeader()->setParent(nullptr); //隐藏行头
  //QTableWidgetItem *item = new QTableWidgetItem("测试数据");
    //item->setTextAlignment(Qt::AlignCenter);
    //ui->tableWidget->setItem(0, 0, item); //设置数据
    ui->tableWidgetItem->setColumnWidth(0,0);

    //tableWidgetdevice设置
    //设置控件的边框显示
    ui->tableWidgetdevice->setStyleSheet("QHeaderView::section {"
                                "border: 1px solid rgba(197, 196, 196, 1);"
                                "border-left:none;"
                                "border-top:none;"
                                "background-color:white;"
                               "}");

   // ui->tableWidget->setRowCount(10); // 设置10行
    ui->tableWidgetdevice->setColumnCount(6); // 设置11列
    ui->tableWidgetdevice->verticalHeader()->setParent(nullptr); //隐藏行头
    //初始化TableWidget 设置列
    headers << "类型" << "设备名称" << "型号规则" << "出厂编号"
            << "生产厂家";
    ui->tableWidgetdevice->setHorizontalHeaderLabels(headers);
}
void InspectDataItem::ShowMessageBox(QString title,QString value) {
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
void InspectDataItem::Load()
{
    // ========================================================================================
    // 主页面初始化 核查管理初始化
    // ====================================================================================

    //
    ui->name->clear();
    ui->mode->clear();
    ui->Checker->clear();
    ui->name->addItem("ALL");
    ui->mode->addItem("ALL");
    ui->Checker->addItem("ALL");

    ui->tableWidget->setRowCount(0);
   ui->stackedWidget->setCurrentIndex(0);
    //先设置为行为0
    ui->tableWidget->setRowCount(0);
    // 调用数据库管理器的函数获取数据
    int rowCount=0;
    int row=0;
    data = dbManager.getCheckData();
    for (const CheckData& item : data) {
         rowCount = ui->tableWidget->rowCount();

         ui->tableWidget->insertRow(rowCount);
         ui->tableWidget->setItem(row, 0, new QTableWidgetItem(item.id));
         ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(row+1)));
         ui->tableWidget->setItem(row, 2, new QTableWidgetItem(item.equipmentName));
         ui->tableWidget->setItem(row, 3, new QTableWidgetItem(item.equipmentModel));
         ui->tableWidget->setItem(row, 4, new QTableWidgetItem(item.manufacture_no));
         ui->tableWidget->setItem(row, 5, new QTableWidgetItem(item.meter_standard_name));
         ui->tableWidget->setItem(row, 6, new QTableWidgetItem("缺少"));
         ui->tableWidget->setItem(row, 7, new QTableWidgetItem(item.meter_standard_id));
         ui->tableWidget->setItem(row, 8, new QTableWidgetItem(item.create_user_name));
         ui->tableWidget->setItem(row, 9, new QTableWidgetItem(item.check_date.toString()));
         ui->tableWidget->setItem(row, 10, new QTableWidgetItem("缺少"));
         ui->tableWidget->setItem(row, 11, new QTableWidgetItem(item.dept_name));

         //实现单元格添加两个按钮
         // 创建一个QWidget作为单元格的容器
         QWidget *widget = new QWidget();
         QHBoxLayout *layout = new QHBoxLayout(widget); // 水平布局


         QPushButton *button = new QPushButton("详情");
         QPushButton *button2 = new QPushButton("预览");

         //设置按钮1简单样式
         button->setStyleSheet("QPushButton {"
                                 "    border: none; /* 无边框 */"
                                 "    color: green; /* 文本颜色 */"
                                 "    text-decoration: underline; /* 文本下划线 */"
                                 "}");

          // 设置鼠标悬停时的光标样式为小手图标
          button->setCursor(Qt::PointingHandCursor);
          button->setIconSize(QSize(16,16));
          button->setIcon(QIcon(":/image/Index/u2327.svg"));
         // 连接按钮的 clicked 信号到自定义槽函数
          connect(button, &QPushButton::clicked, this, [this, button, item]() {

                  OnButtonSelectClicked(item);


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
           button2->setIcon(QIcon(":/image/Index/u5496.svg"));
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
           ui->tableWidget->setCellWidget(row, 12, widget);
           //
           SetHomePaheCombox(item);
         row++;
        }
}
void InspectDataItem::OnButtonSelectClicked(CheckData entity)
{
    // ========================================================================================
    // 编辑按钮 点击事件  进入详情页面
    // ====================================================================================
     DeviceInfoInit(entity);
     ui->stackedWidget->setCurrentIndex(1);
     QMessageBox::information(this, "ID",entity.id);
}
void InspectDataItem::OnButtonPreViewtClicked(QString id)
{
    // ========================================================================================
    // 预览按钮 点击事件  进入详情页面
    // ====================================================================================
     //DeviceInfoInit(id);
     //ui->stackedWidget->setCurrentIndex(1);
     QMessageBox::information(this, "ID",id);
}

void InspectDataItem::on_pushButton_clicked()

{
     ui->stackedWidget->setCurrentIndex(0);
}
void InspectDataItem::DeviceInfoInit(CheckData entity){
    // ========================================================================================
    // 设备信息 设备-核查件
    // ====================================================================================
    ui->tableWidgetdevice->setRowCount(0);
    int rowCount=0;
    int row=0;
    QList<CheckDataManageBasicInfo> data=dbManager.getCheckDataManageBasicInfoDevice(entity.id);
    for(const CheckDataManageBasicInfo& item:data)
    {
        //获取行号 并进行添加行
        rowCount = ui->tableWidgetdevice->rowCount();
          ui->tableWidgetdevice->insertRow(rowCount);
        ui->tableWidgetdevice->setItem(row, 0, new QTableWidgetItem("核查件"));
        ui->tableWidgetdevice->setItem(row, 1, new QTableWidgetItem(item.equipment_name));
        ui->tableWidgetdevice->setItem(row, 2, new QTableWidgetItem(item.model));
        ui->tableWidgetdevice->setItem(row, 3, new QTableWidgetItem(item.manufacture_no));
        ui->tableWidgetdevice->setItem(row, 4, new QTableWidgetItem(item.manufacturer));
        row++;
    }
    //添加标准设备
    //获取行号 并进行添加行
    rowCount = ui->tableWidgetdevice->rowCount();
      ui->tableWidgetdevice->insertRow(rowCount);
    ui->tableWidgetdevice->setItem(row, 0, new QTableWidgetItem("标准设备"));
    ui->tableWidgetdevice->setItem(row, 1, new QTableWidgetItem(entity.equipmentName));
    ui->tableWidgetdevice->setItem(row, 2, new QTableWidgetItem(entity.equipmentModel));
    ui->tableWidgetdevice->setItem(row, 3, new QTableWidgetItem(entity.manufacture_no));
    ui->tableWidgetdevice->setItem(row, 4, new QTableWidgetItem(entity.manufacturer));
    // ========================================================================================
    // 基本信息
    // ====================================================================================
    CheckDataManageBasicInfo CheckData= dbManager.getCheckDataManageBasicInfo(entity.equipment_id);

    ui->temperature->setText(CheckData.temperature);
    ui->humidity->setText(CheckData.humidity);
    ui->check_date->setText(CheckData.check_date.toString());
    ui->check_address->setText(CheckData.check_address);
    ui->create_user_name->setText(CheckData.create_user_name);
    ui->conclusion->setText(CheckData.conclusion);
    // ========================================================================================
    // 核查项目及核查点
    // ====================================================================================

  QStringList headlist;
//  QList<QStringList> Data = dbManager.Search_VerifyDataTable(entity.equipment_id,entity.id,&headlist);
  ui->tableWidgetItem->setRowCount(0);
  ui->tableWidgetItem->setColumnCount(headlist.count());
  ui->tableWidgetItem->setHorizontalHeaderLabels(headlist);
  //循环数据
   rowCount=0;
   row=0;
//  for (const QStringList& item : Data)
//  {
//       //获取行号 并进行添加行
//       rowCount = ui->tableWidgetItem->rowCount();
//       ui->tableWidgetItem->insertRow(rowCount);
//       //添加 序号
//       ui->tableWidgetItem->setItem(row, 0, new QTableWidgetItem(QString::number(row+1)));
//       //循环读取值  headlist，count是列的数量
//       for(int i=1;i<headlist.count();i++)
//       {
//          ui->tableWidgetItem->setItem(row, i, new QTableWidgetItem(item[i]));
//       }

//      row++;

//  }
}
void InspectDataItem::SetHomePaheCombox(CheckData entity)
{
    if(comboBoxContainsItem(ui->name,entity.equipmentName)){
        ui->name->addItem(entity.equipmentName);
    }
    if(comboBoxContainsItem(ui->mode,entity.equipmentModel)){
        ui->mode->addItem(entity.equipmentModel);
    }
    if(comboBoxContainsItem(ui->Checker,entity.create_user_name)){
        ui->Checker->addItem(entity.create_user_name);
    }


}
bool InspectDataItem::comboBoxContainsItem(QComboBox *comboBox, const QString &itemText)
{
    for (int i = 0; i < comboBox->count(); ++i) {
        if (comboBox->itemText(i) == itemText) {
            return false; // 找到匹配项，返回 true
        }
    }
    return true; // 没有找到匹配项，返回 false
}

void InspectDataItem::on_select_clicked()
{
    if(data.count()==0){
        ShowMessageBox("提示框","数据为空");
        return;
    }
    //筛选
     ui->tableWidget->setRowCount(0);
    int rowCount=0;
    int row=0;
    data = dbManager.getCheckData();
    for (const CheckData& item : data) {

        bool shouldAddRow = true; // 初始假设应该添加这一行
        // 根据设备名称筛选
        if (ui->name->currentText() != "ALL") {
            if (ui->name->currentText()  != item.equipmentName) {
                shouldAddRow = false; // 设备名称不匹配，不添加这一行
            }
        }
        if (ui->mode->currentText() != "ALL") {
            if (ui->mode->currentText() != item.equipmentModel) {
                shouldAddRow = false; // 设备名称不匹配，不添加这一行
            }
        }
        if (ui->number->text() != "") {
            if (ui->number->text() != item.manufacture_no) {
                shouldAddRow = false; // 设备名称不匹配，不添加这一行
            }
        }
        if (ui->Checker->currentText() != "ALL") {
            if (ui->Checker->currentText() != item.create_user_name) {
                shouldAddRow = false; // 设备名称不匹配，不添加这一行
            }
        }

         if(shouldAddRow==true){
         rowCount = ui->tableWidget->rowCount();

         ui->tableWidget->insertRow(rowCount);
         ui->tableWidget->setItem(row, 0, new QTableWidgetItem(item.id));
         ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(row+1)));
         ui->tableWidget->setItem(row, 2, new QTableWidgetItem(item.equipmentName));
         ui->tableWidget->setItem(row, 3, new QTableWidgetItem(item.equipmentModel));
         ui->tableWidget->setItem(row, 4, new QTableWidgetItem(item.manufacture_no));
         ui->tableWidget->setItem(row, 5, new QTableWidgetItem(item.meter_standard_name));
         ui->tableWidget->setItem(row, 6, new QTableWidgetItem("缺少"));
         ui->tableWidget->setItem(row, 7, new QTableWidgetItem(item.meter_standard_id));
         ui->tableWidget->setItem(row, 8, new QTableWidgetItem(item.create_user_name));
         ui->tableWidget->setItem(row, 9, new QTableWidgetItem(item.check_date.toString()));
         ui->tableWidget->setItem(row, 10, new QTableWidgetItem("缺少"));
         ui->tableWidget->setItem(row, 11, new QTableWidgetItem(item.dept_name));

         //实现单元格添加两个按钮
         // 创建一个QWidget作为单元格的容器
         QWidget *widget = new QWidget();
         QHBoxLayout *layout = new QHBoxLayout(widget); // 水平布局


         QPushButton *button = new QPushButton("详情");
         QPushButton *button2 = new QPushButton("预览");

         //设置按钮1简单样式
         button->setStyleSheet("QPushButton {"
                                 "    border: none; /* 无边框 */"
                                 "    color: green; /* 文本颜色 */"
                                 "    text-decoration: underline; /* 文本下划线 */"
                                 "}");

          // 设置鼠标悬停时的光标样式为小手图标
          button->setCursor(Qt::PointingHandCursor);
          button->setIconSize(QSize(16,16));
          button->setIcon(QIcon(":/image/Index/u2327.svg"));
         // 连接按钮的 clicked 信号到自定义槽函数
          connect(button, &QPushButton::clicked, this, [this, button, item]() {

                  OnButtonSelectClicked(item);


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
           button2->setIcon(QIcon(":/image/Index/u5496.svg"));
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
           ui->tableWidget->setCellWidget(row, 12, widget);

         row++;
        }
    }
}

void InspectDataItem::on_update_clicked()
{
    Load();
    ui->name->setCurrentIndex(0);
    ui->mode->setCurrentIndex(0);
    ui->Checker->setCurrentIndex(0);
}
