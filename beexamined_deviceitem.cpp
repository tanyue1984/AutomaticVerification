#include "beexamined_deviceitem.h"
#include "ui_beexamined_deviceitem.h"
#include "QMessageBox.h"
#include "basecommonapi.h"
//#include "tested_device_manage_serve.h"

#include "QStringList"

//被检设备
BeExamined_DeviceItem::BeExamined_DeviceItem(QWidget *parent) :
    InfoBase(parent),
    ui(new Ui::BeExamined_DeviceItem)
{
    ui->setupUi(this);
    this->initForm();
}

BeExamined_DeviceItem::~BeExamined_DeviceItem()
{
    delete ui;
}

void BeExamined_DeviceItem::initForm()
{

    //tableWidget设置
    //设置控件的边框显示
    ui->tableWidget->setStyleSheet("QHeaderView::section {"
                                "border: 1px solid rgba(197, 196, 196, 1);"
                                "border-left:none;"
                                "border-top:none;"
                                "background-color:white;"
                               "}");

   // ui->tableWidget->setRowCount(10); // 设置10行
    ui->tableWidget->setColumnCount(13); // 设置11列
    ui->tableWidget->setColumnWidth(0, 80); // 设置第一列的宽度为100像素
    ui->tableWidget->verticalHeader()->setParent(nullptr); //隐藏行头
    //QTableWidgetItem *item = new QTableWidgetItem("测试数据");
   // item->setTextAlignment(Qt::AlignCenter);
    //ui->tableWidget->setItem(0, 0, item); //设置数据
    ui->tableWidget->setColumnWidth(0, 0); // 设置第一列的宽度为0
    //初始化TableWidget 设置列
    QStringList headers;
    headers << "ID" << "序号" << "设备名称" << "型号规则" << "辅助字段" << "出厂编号"
            << "生产厂家"<<"所在单位" <<"使用部门"<<"计量标识"<<"检定周期"<<"证书有效期"<<"操作";
    ui->tableWidget->setHorizontalHeaderLabels(headers);



}
void BeExamined_DeviceItem::ShowMessageBox(QString title,QString value) {
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
void BeExamined_DeviceItem::Load()
{
    // ========================================================================================
    // 主页面初始化
    // ====================================================================================
    //
    ui->name->clear();
    ui->mode->clear();
    ui->dept_2->clear();
    ui->Buildinguser->clear();
    ui->manufacturer_2->clear();
    ui->name->addItem("ALL");
    ui->mode->addItem("ALL");
    ui->dept_2->addItem("ALL");
    ui->Buildinguser->addItem("ALL");
    ui->manufacturer_2->addItem("ALL");
    ui->number->setText("");

    ui->stackedWidget->setCurrentIndex(0);
    //先设置为行为0
    ui->tableWidget->setRowCount(0);
    // 调用数据库管理器的函数获取数据
    int rowCount=0;
    int row=0;
    data = dbManager.getTestedDevice();
    for (const TestedDevice& bizDevice : data) {
         rowCount = ui->tableWidget->rowCount();

         ui->tableWidget->insertRow(rowCount);
         ui->tableWidget->setItem(row, 0, new QTableWidgetItem(bizDevice.id));
         ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(row+1)));
         ui->tableWidget->setItem(row, 2, new QTableWidgetItem(bizDevice.equipment_name));
         ui->tableWidget->setItem(row, 3, new QTableWidgetItem(bizDevice.model));
         ui->tableWidget->setItem(row, 4, new QTableWidgetItem(bizDevice.help_instruction));
         ui->tableWidget->setItem(row, 5, new QTableWidgetItem(bizDevice.manufacture_no));
         ui->tableWidget->setItem(row, 6, new QTableWidgetItem(bizDevice.manufacturer));
         ui->tableWidget->setItem(row, 8, new QTableWidgetItem(bizDevice.FULL_NAME));
         ui->tableWidget->setItem(row, 9, new QTableWidgetItem(bizDevice.meter_identify));
         ui->tableWidget->setItem(row, 10, new QTableWidgetItem(bizDevice.check_cycle));
         ui->tableWidget->setItem(row, 11, new QTableWidgetItem(bizDevice.certificate_valid));

         //添加操作按钮
         QPushButton *button = new QPushButton("查看");
         button->setIconSize(QSize(16,16));
         button->setIcon(QIcon(":/image/Index/u2327.svg"));
         ui->tableWidget->setCellWidget(row, 12, button);
         //设置按钮简单样式
         button->setStyleSheet("QPushButton {"
                                 "    border: none; /* 无边框 */"
                                 "    color: green; /* 文本颜色 */"
                                 "    text-decoration: underline; /* 文本下划线 */"
                                 "}");

          // 设置鼠标悬停时的光标样式为小手图标
          button->setCursor(Qt::PointingHandCursor);
         // 连接按钮的 clicked 信号到自定义槽函数
          connect(button, &QPushButton::clicked, this, [this, button, row]() {
              // 获取当前行第一列的 ID
              QTableWidgetItem *idItem = this->ui->tableWidget->item(row, 0);
              if (idItem) {
                  QString id = idItem->text();

                  OnButtonSelectClicked(id);
              }

          });

          SetHomePaheCombox(bizDevice);
         row++;
        }

}
void BeExamined_DeviceItem::OnButtonSelectClicked(QString id)
{
    // ========================================================================================
    // 查看按钮 点击事件  进入详情页面
    // ====================================================================================

     DeviceInfoInit(id);
     ui->stackedWidget->setCurrentIndex(1);
     QMessageBox::information(this, "ID",id);
}
//返回按钮
void BeExamined_DeviceItem::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
void BeExamined_DeviceItem::DeviceInfoInit(QString id)
{
    // ========================================================================================
    // 初始化详情
    // ====================================================================================

   QList<TestedCatDevice> data = dbManager.getTestedCatDevice(id);
   for (const TestedCatDevice& testedCatDevice : data) {

       ui->equipment_name->setText(testedCatDevice.equipment_name);
       ui->model->setText(testedCatDevice.model);
       ui->help_instruction->setText(testedCatDevice.help_instruction);
       ui->manufacture_no->setText(testedCatDevice.manufacture_no);
       ui->product_country->setText(testedCatDevice.product_country);
       ui->manufacturer->setText(testedCatDevice.manufacturer);
       ui->product_date->setText(testedCatDevice.product_date);
       ui->purchase_date->setText(testedCatDevice.purchase_date);
       ui->unit_price->setText(testedCatDevice.unit_price);
       ui->usage_status->setText(testedCatDevice.usage_status);
       ui->quality_condition->setText(testedCatDevice.quality_condition);
       ui->dept_name->setText(testedCatDevice.FULL_NAME);
       ui->category->setText(testedCatDevice.category);
       ui->check_cycle->setText(testedCatDevice.check_cycle);
       ui->level->setText(testedCatDevice.level);
       ui->remark->setText(testedCatDevice.remark);
       }


   // ========================================================================================
   // 初始化检定项目及核查点
   // ====================================================================================

    //重写核查点显示
    //每个检定项目列头
    SequentialMap<QString, QStringList> headListItem;  //例 <DCV,对应列头>
    //每个检定项目数据
    QMap<QString, QList<QStringList>> dataListItem;  //例 <DCV,对应数据>
    dataListItem=dbManager.Search_CalibrationTableClass(id,&headListItem);
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
                               "background-color: blue;" // 选中时变为蓝色背景
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

void BeExamined_DeviceItem::SetHomePaheCombox(TestedDevice entity)
{
    if(comboBoxContainsItem(ui->name,entity.equipment_name)){
        ui->name->addItem(entity.equipment_name);
    }
    if(comboBoxContainsItem(ui->mode,entity.model)){
        ui->mode->addItem(entity.model);
    }
    if(comboBoxContainsItem(ui->manufacturer_2,entity.manufacturer)){
        ui->manufacturer_2->addItem(entity.manufacturer);
    }
    if(comboBoxContainsItem(ui->Buildinguser,entity.customer_name)){
        ui->Buildinguser->addItem(entity.customer_name);
    }
    if(comboBoxContainsItem(ui->dept_2,entity.FULL_NAME)){
        ui->dept_2->addItem(entity.FULL_NAME);
    }

}
bool BeExamined_DeviceItem::comboBoxContainsItem(QComboBox *comboBox, const QString &itemText)
{
    for (int i = 0; i < comboBox->count(); ++i) {
        if (comboBox->itemText(i) == itemText) {
            return false; // 找到匹配项，返回 true
        }
    }
    return true; // 没有找到匹配项，返回 false
}



void BeExamined_DeviceItem::on_select_clicked()
{
    if(data.count()==0){
        ShowMessageBox("提示框","数据为空");
        return;
    }
    //筛选
    ui->tableWidget->setRowCount(0);
    int rowCount=0;
    int row=0;
    for (const TestedDevice& bizDevice : data) {
        bool shouldAddRow = true; // 初始假设应该添加这一行
        // 根据设备名称筛选
        if (ui->name->currentText() != "ALL") {
            if (ui->name->currentText()  != bizDevice.equipment_name) {
                shouldAddRow = false; // 设备名称不匹配，不添加这一行
            }
        }
        if (ui->mode->currentText() != "ALL") {
            if (ui->mode->currentText() != bizDevice.model) {
                shouldAddRow = false; // 设备名称不匹配，不添加这一行
            }
        }
        if (ui->number->text() != "") {
            if (ui->number->text() != bizDevice.manufacture_no) {
                shouldAddRow = false; // 设备名称不匹配，不添加这一行
            }
        }
        if (ui->manufacturer_2->currentText() != "ALL") {
            if (ui->manufacturer_2->currentText() != bizDevice.manufacturer) {
                shouldAddRow = false; // 设备名称不匹配，不添加这一行
            }
        }
        if (ui->Buildinguser->currentText() != "ALL") {
            if (ui->Buildinguser->currentText() != bizDevice.customer_name) {
                shouldAddRow = false; // 设备名称不匹配，不添加这一行
            }
        } if (ui->dept_2->currentText() != "ALL") {
            if (ui->dept_2->currentText() != bizDevice.FULL_NAME) {
                shouldAddRow = false; // 设备名称不匹配，不添加这一行
            }
        }

        if(shouldAddRow){
         rowCount = ui->tableWidget->rowCount();

         ui->tableWidget->insertRow(rowCount);
         ui->tableWidget->setItem(row, 0, new QTableWidgetItem(bizDevice.id));
         ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(row+1)));
         ui->tableWidget->setItem(row, 2, new QTableWidgetItem(bizDevice.equipment_name));
         ui->tableWidget->setItem(row, 3, new QTableWidgetItem(bizDevice.model));
         ui->tableWidget->setItem(row, 4, new QTableWidgetItem(bizDevice.help_instruction));
         ui->tableWidget->setItem(row, 5, new QTableWidgetItem(bizDevice.manufacture_no));
         ui->tableWidget->setItem(row, 6, new QTableWidgetItem(bizDevice.manufacturer));
         ui->tableWidget->setItem(row, 8, new QTableWidgetItem(bizDevice.FULL_NAME));
         ui->tableWidget->setItem(row, 9, new QTableWidgetItem(bizDevice.meter_identify));
         ui->tableWidget->setItem(row, 10, new QTableWidgetItem(bizDevice.check_cycle));
         ui->tableWidget->setItem(row, 11, new QTableWidgetItem(bizDevice.certificate_valid));

         //添加操作按钮
         QPushButton *button = new QPushButton("查看");
         button->setIconSize(QSize(16,16));
         button->setIcon(QIcon(":/image/Index/u2327.svg"));
         ui->tableWidget->setCellWidget(row, 12, button);
         //设置按钮简单样式
         button->setStyleSheet("QPushButton {"
                                 "    border: none; /* 无边框 */"
                                 "    color: green; /* 文本颜色 */"
                                 "    text-decoration: underline; /* 文本下划线 */"
                                 "}");

          // 设置鼠标悬停时的光标样式为小手图标
          button->setCursor(Qt::PointingHandCursor);
         // 连接按钮的 clicked 信号到自定义槽函数
          connect(button, &QPushButton::clicked, this, [this, button, row]() {
              // 获取当前行第一列的 ID
              QTableWidgetItem *idItem = this->ui->tableWidget->item(row, 0);
              if (idItem) {
                  QString id = idItem->text();

                  OnButtonSelectClicked(id);
              }

          });


         row++;
         }
        }
}

void BeExamined_DeviceItem::on_update_clicked()
{
    Load();
    ui->name->setCurrentIndex(0);
    ui->mode->setCurrentIndex(0);
    ui->dept_2->setCurrentIndex(0);
    ui->Buildinguser->setCurrentIndex(0);
    ui->manufacturer_2->setCurrentIndex(0);
}
