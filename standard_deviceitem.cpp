#include "standard_deviceitem.h"
#include "ui_standard_deviceitem.h"
#include "InfoBase.h"
#include "QMessageBox"
#include "basecommonapi.h"
#include "PageControl.h"

//标准设备管理 .cpp
Standard_DeviceItem::Standard_DeviceItem(QWidget *parent) :
    InfoBase(parent),
    ui(new Ui::Standard_DeviceItem)
{
    ui->setupUi(this);
     this->initForm();
}

Standard_DeviceItem::~Standard_DeviceItem()
{
    delete ui;
}
void Standard_DeviceItem::initForm()
{

//主页面tableWidget设置
    //设置控件的边框显示
    ui->tableWidget->setStyleSheet("QHeaderView::section {"
                                "border: 1px solid rgba(197, 196, 196, 1);"
                                "border-left:none;"
                                "border-top:none;"
                                "background-color:white;"
                               "}");

   // ui->tableWidget->setRowCount(10); // 设置10行
    ui->tableWidget->setColumnCount(7); // 设置11列
    ui->tableWidget->setColumnWidth(0, 0); // 设置第一列的宽度为100像素
    ui->tableWidget->verticalHeader()->setParent(nullptr); //隐藏行头
    //QTableWidgetItem *item = new QTableWidgetItem("测试数据");
    //item->setTextAlignment(Qt::AlignCenter);
    //ui->tableWidget->setItem(0, 0, item); //设置数据
//    ui->tableWidget->setColumnWidth(0, 0); // 设置第一列的宽度为0
    //初始化TableWidget 设置列
    QStringList headers;
    headers << "ID"<< "序号" << "设备名称" << "型号规则" << "出厂编号"
            << "生产厂家" <<"操作";

    ui->tableWidget->setHorizontalHeaderLabels(headers);

//详情页面tableWidgetItem设置
    ui->tableWidgetItem->setStyleSheet("QHeaderView::section {"
                                "border: 1px solid rgba(197, 196, 196, 1);"
                                "border-left:none;"
                                "border-top:none;"
                                "background-color:white;"
                               "}");

   // ui->tableWidget->setRowCount(10); // 设置10行
    //ui->tableWidgetItem->setColumnCount(14); // 设置11列
    ui->tableWidgetItem->setColumnWidth(0, 80); // 设置第一列的宽度为100像素
    ui->tableWidgetItem->verticalHeader()->setParent(nullptr); //隐藏行头

    //初始化TableWidget 设置列
    //QStringList headersItem;
    //headersItem << "序号" << "核查类型" << "检测类型" << "参数"<< "单位"
           // << "频率" << "核查点"  << "分辨力" << "循环次数" << "Urel"
          //  << "通道" << "TFER" << "DCCP" << "FLIT";

    //ui->tableWidgetItem->setHorizontalHeaderLabels(headersItem);
}
void Standard_DeviceItem::ShowMessageBox(QString title,QString value) {
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
void Standard_DeviceItem::Load()
{

    //获取主页面内容
    //初始化下拉框
    ui->combox_devicename->clear();
    ui->combox_mode->clear();
    ui->combox_manufacturer_->clear();
    ui->combox_devicename->addItem("ALL");
    ui->combox_mode->addItem("ALL");
    ui->combox_manufacturer_->addItem("ALL");
    //进入主页面
     ui->stackedWidget->setCurrentIndex(0);
    //先设置为行为0
    ui->tableWidget->setRowCount(0);

    // 创建数据库管理器对象


    // 调用数据库管理器的函数获取数据

    data = BaseCommonApi::getBizDeviceManage();

    QString State;
    //设置单页显示数量
    page=20;
    // 创建分页表格控件
    PagedTableWidget *pagedTable = new PagedTableWidget(this,  ui->HBoxPageLayout);
    pagedTable->setData(data.count(), page); // 每页10行
    connect(pagedTable, &PagedTableWidget::pageChanged, this,&Standard_DeviceItem::onpageChanged);
    this->onpageChanged(0);

}
void Standard_DeviceItem::onpageChanged(int currPage){

    currPage+=1;
    // 计算当前页的开始和结束索引
    int start = (currPage - 1) * page;
    int end = std::min(start + page, data.size());
    // 从 QList 中筛选出当前页的数据
    QList<BizDevice> tempdate = data.mid(start, end - start);
    int rowCount = 0;
    int row=0;
    QString State;
    ui->tableWidget->setRowCount(0);
    for (const BizDevice& bizDevice : tempdate) {
         rowCount = ui->tableWidget->rowCount();

         ui->tableWidget->insertRow(rowCount);
         ui->tableWidget->setItem(row, 0, new QTableWidgetItem(bizDevice.id));
         ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(row+1)));
         ui->tableWidget->setItem(row, 2, new QTableWidgetItem(bizDevice.equipment_name));
         ui->tableWidget->setItem(row, 3, new QTableWidgetItem(bizDevice.model));
         ui->tableWidget->setItem(row, 4, new QTableWidgetItem(bizDevice.manufacture_no));
         ui->tableWidget->setItem(row, 5, new QTableWidgetItem(bizDevice.manufacturer));
         //添加操作按钮
         QPushButton *button = new QPushButton("查看");
         ui->tableWidget->setCellWidget(row, 6, button);
         //设置按钮简单样式
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
         connect(button, &QPushButton::clicked, this, [this, button, row]() {
             // 获取当前行第一列的 ID
             QTableWidgetItem *idItem = this->ui->tableWidget->item(row, 0);
             if (idItem) {
                 QString id = idItem->text();

                 OnButtonSelectClicked(id);
             }

         });

         //渲染下拉框
        SetHomePaheCombox(bizDevice);
        row++;
    }
}


void Standard_DeviceItem::OnButtonSelectClicked(QString id)
{
    //添加
    //设备信息页面 初始化方法
     DeviceInfoInit(id);
     ui->stackedWidget->setCurrentIndex(1);
     QMessageBox::information(this, "ID",id);
}
//返回按钮
void Standard_DeviceItem::on_pushButton_clicked()
{
      ui->stackedWidget->setCurrentIndex(0);
}
void Standard_DeviceItem::DeviceInfoInit(QString id)
{



     QList<CatDeviceInfor> data = dbManager.getCatDeviceInfor(id);

     // 获取当前日期 使用年限=当前日期减去购进日期
     QDate currentDate = QDate::currentDate();
     QDate purchase_date;

     int yearsDiff ;

     // 定义日期的格式，这里假设是"yyyy-MM-dd"
     QString format = "yyyy-MM-dd";
     //解析数据
     for (const CatDeviceInfor& catDeviceInfor : data) {
          ui->equipment_name->setText(catDeviceInfor.equipment_name);
          ui->model->setText(catDeviceInfor.model);
          ui->manufacture_no->setText(catDeviceInfor.manufacture_no);
          ui->measure_valid_date->setText(catDeviceInfor.measure_valid_date);
          ui->product_country->setText(catDeviceInfor.product_country);
          ui->manufacturer->setText(catDeviceInfor.manufacturer);
          ui->produce_date->setText(catDeviceInfor.produce_date);
          ui->purchase_date->setText(catDeviceInfor.purchase_date);
          ui->unit_price->setText(catDeviceInfor.unit_price);
          ui->usage_status->setText(catDeviceInfor.usage_status);
          ui->quality_status->setText(catDeviceInfor.quality_status);
          ui->dept_name->setText(catDeviceInfor.dept_name);
          ui->director_name->setText(catDeviceInfor.director_name);
          ui->supplier_name->setText(catDeviceInfor.supplier_name);
          ui->location->setText(catDeviceInfor.location);
          ui->type->setText(catDeviceInfor.type);
          ui->standard_type->setText(catDeviceInfor.standard_type);
          ui->category->setText(catDeviceInfor.category);
          ui->level->setText(catDeviceInfor.level);
          ui->measure_cycle->setText(catDeviceInfor.measure_cycle);
          ui->unpack_record_name->setText(catDeviceInfor.unpack_record_name);
          ui->remark->setText(catDeviceInfor.remark);

          //将catDeviceInfor.purchase_date转为Qdate类型
          purchase_date=QDate::fromString(catDeviceInfor.purchase_date,format);
          //计算天数差
          yearsDiff=purchase_date.daysTo(currentDate) /365;
          //使用年限
          ui->DurableYears->setText(QString::number(yearsDiff));
       }


     //获取核查点tablewight的数据
      QStringList headlist;
      //QString DeviceName = BaseCommonApi::getApparatus(id);
//     QList<QStringList> Data=dbManager.Search_VerifyTable(id,&headlist);
      //QList<CheckDCPower> bizDeviceManageData = BaseCommonApi::getCheckDCPower(id);
      //设置tablewight
      ui->tableWidgetItem->setRowCount(0);
      ui->tableWidgetItem->setColumnCount(headlist.count());
      ui->tableWidgetItem->setHorizontalHeaderLabels(headlist);
      //循环数据
      int rowCount=0;
      int row=0;

//      for (const QStringList& item : Data)
//      {
//           //获取行号 并进行添加行
//           rowCount = ui->tableWidgetItem->rowCount();
//           ui->tableWidgetItem->insertRow(rowCount);
//           //添加 序号
//           ui->tableWidgetItem->setItem(row, 0, new QTableWidgetItem(QString::number(row+1)));
//           //循环读取值  headlist，count是列的数量
//           for(int i=1;i<headlist.count();i++)
//           {
//              ui->tableWidgetItem->setItem(row, i, new QTableWidgetItem(item[i]));
//           }

//          row++;

//      }
}
void Standard_DeviceItem::SetHomePaheCombox(BizDevice entity)
{
    if(comboBoxContainsItem(ui->combox_mode,entity.model)){
        ui->combox_mode->addItem(entity.model);
    }
    if(comboBoxContainsItem(ui->combox_devicename,entity.equipment_name)){
        ui->combox_devicename->addItem(entity.equipment_name);
    }
    if(comboBoxContainsItem(ui->combox_manufacturer_,entity.manufacturer)){
        ui->combox_manufacturer_->addItem(entity.manufacturer);
    }


}
bool Standard_DeviceItem::comboBoxContainsItem(QComboBox *comboBox, const QString &itemText)
{
    for (int i = 0; i < comboBox->count(); ++i) {
        if (comboBox->itemText(i) == itemText) {
            return false; // 找到匹配项，返回 true
        }
    }
    return true; // 没有找到匹配项，返回 false
}


void Standard_DeviceItem::on_Select_clicked()
{
    //筛选操作
    if(data.count()==0){
        ShowMessageBox("提示框","数据为空");
        return;
    }
    //筛选
    ui->tableWidget->setRowCount(0);
    int rowCount=0;
    int row=0;
    for (const BizDevice& bizDevice : data) {
         bool shouldAddRow = true; // 初始假设应该添加这一行
         // 根据设备名称筛选
         if (ui->combox_devicename->currentText() != "ALL") {
             if (ui->combox_devicename->currentText()  != bizDevice.equipment_name) {
                 shouldAddRow = false; // 设备名称不匹配，不添加这一行
             }
         }
         if (ui->combox_mode->currentText() != "ALL") {
             if (ui->combox_mode->currentText() != bizDevice.model) {
                 shouldAddRow = false; // 设备名称不匹配，不添加这一行
             }
         }
         if (ui->combox_manufacturer_->currentText() != "ALL") {
             if (ui->combox_manufacturer_->currentText() != bizDevice.manufacturer) {
                 shouldAddRow = false; // 设备名称不匹配，不添加这一行
             }
         }
         if (ui->number->text() != "") {
             if (ui->number->text() != bizDevice.manufacture_no) {
                 shouldAddRow = false; // 设备名称不匹配，不添加这一行
             }
         }

         if(shouldAddRow==true){
         rowCount = ui->tableWidget->rowCount();
         ui->tableWidget->insertRow(rowCount);
         ui->tableWidget->setItem(row, 0, new QTableWidgetItem(bizDevice.id));
         ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(row+1)));
         ui->tableWidget->setItem(row, 2, new QTableWidgetItem(bizDevice.equipment_name));
         ui->tableWidget->setItem(row, 3, new QTableWidgetItem(bizDevice.model));
         ui->tableWidget->setItem(row, 4, new QTableWidgetItem(bizDevice.manufacture_no));
         ui->tableWidget->setItem(row, 5, new QTableWidgetItem(bizDevice.manufacturer));
         //添加操作按钮
         QPushButton *button = new QPushButton("查看");
         ui->tableWidget->setCellWidget(row, 6, button);
         //设置按钮简单样式
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

void Standard_DeviceItem::on_pushButton_8_clicked()
{
    Load();
    ui->combox_devicename->setCurrentIndex(0);
    ui->combox_mode->setCurrentIndex(0);
    ui->combox_manufacturer_->setCurrentIndex(0);
    ui->number->setText("");
}



