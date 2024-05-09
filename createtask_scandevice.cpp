#include "createtask_scandevice.h"
#include "ui_createtask_scandevice.h"
#include "QCheckBox"
#include "CheckBoxTableHeader.h"
#include "QMessageBox"

CreateTask_ScanDevice::CreateTask_ScanDevice(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CreateTask_ScanDevice)
{
    ui->setupUi(this);
    this->initData();
}

CreateTask_ScanDevice::~CreateTask_ScanDevice()
{
    delete ui;
}

void CreateTask_ScanDevice::on_AddButton_clicked()
{
    //添加设备
    //1、获取当前选择的设备ID 存入List
    QList<QString> idList;
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        // 获取第1列的 QCheckBox
        QCheckBox *checkBox = qobject_cast<QCheckBox *>(ui->tableWidget->cellWidget(row, 1));
        if (checkBox && checkBox->isChecked()) {
            // 如果 checkbox 被选中，获取第0列的 ID
            QTableWidgetItem *idItem = ui->tableWidget->item(row, 0);
            if (idItem) {
                // 获取 ID 的文本或数据
                idList.append(idItem->text()); // 或者使用其他方法来获取数据，比如 idItem->data(Qt::UserRole).toString();
            }
        }
    }

    CreateTaskInst->DeviceInfoShow(idList);
    if(idList.count()>0){QMessageBox::information(this, "提示", "添加成功");};

    this->close();
}

void CreateTask_ScanDevice::on_CancelButton_clicked()
{
   this->close();
}
void CreateTask_ScanDevice::initData()
{
    //初始化方法 查询设备表所有设备
    //tableWidget设置
    //设置控件的边框显示
    ui->tableWidget->setStyleSheet("QHeaderView::section {"
                                "border: 1px solid rgba(197, 196, 196, 1);"
                                "border-left:none;"
                                "border-top:none;"
                                "background-color:white;"
                               "}");
    ui->tableWidget->setColumnCount(7); // 设置11列

    ui->tableWidget->verticalHeader()->setParent(nullptr); //隐藏行头

    QStringList headers;
    headers << "ID" <<""<< "序号" << "设备名称"
            << "型号规则"  << "出厂编号" << "生产厂家";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    // 设置自定义列头
    CheckBoxTableHeader *header = new CheckBoxTableHeader(Qt::Horizontal,  ui->tableWidget);
    ui->tableWidget->setHorizontalHeader(header);

    ui->tableWidget->setColumnWidth(0, 0); // 设置第一列的宽度为0
    ui->tableWidget->setColumnWidth(1, 80); // 设置第一列的宽度为0

    int rowCount=0;
    int row=0;
    //添加数据
    QList<TestedDevice> data =  dbManager.getTestedDevice();
    for (const TestedDevice& bizDevice : data) {
         rowCount = ui->tableWidget->rowCount();

         ui->tableWidget->insertRow(rowCount);
         ui->tableWidget->setItem(row, 0, new QTableWidgetItem(bizDevice.id));

          QCheckBox *checkBox = new QCheckBox();
          checkBox->setStyleSheet("QCheckBox { margin-left: 35px;"
                                              "margin-top:6px; }");
         ui->tableWidget->setCellWidget(row, 1,  checkBox);

         ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(row+1)));
         ui->tableWidget->setItem(row, 3, new QTableWidgetItem(bizDevice.equipment_name));
         ui->tableWidget->setItem(row, 4, new QTableWidgetItem(bizDevice.model));
         ui->tableWidget->setItem(row, 5, new QTableWidgetItem(bizDevice.manufacture_no));
         ui->tableWidget->setItem(row, 6, new QTableWidgetItem(bizDevice.manufacturer));

        row++;
        }












}
