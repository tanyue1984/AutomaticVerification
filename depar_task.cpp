#include "depar_task.h"
#include "ui_depar_task.h"
#include "QMessageBox"
#include "InfoBase.h"
#include "QStandardItemModel"
//部门任务
//#include "depar_task_server.h"
#include "basecommonapi.h"
#include "PageControl.h"
#include "global.h"



Depar_Task::Depar_Task(QWidget *parent) :
    InfoBase(parent),
    ui(new Ui::Depar_Task)
{
    ui->setupUi(this);
     //初始化列表
    this->initForm();

}

Depar_Task::~Depar_Task()
{
    delete ui;
}
void Depar_Task::initForm()
{
    //tableWidget设置
    //设置控件的边框显示
    ui->tableWidget->setStyleSheet("QHeaderView::section {"
                                "border: 1px solid rgba(197, 196, 196, 1);"
                                "border-left:none;"
                                "border-top:none;"
                                "background-color:white;"
                               "}");

    //ui->tableWidget->setRowCount(10);
    //ui->tableWidget->setRowCount(10); // 设置10行
    ui->tableWidget->setColumnCount(10); // 设置11列
    ui->tableWidget->setColumnWidth(0, 80); // 设置第一列的宽度为100像素
    ui->tableWidget->verticalHeader()->setParent(nullptr); //隐藏行头
    //QTableWidgetItem *item = new QTableWidgetItem("测试数据");
    //item->setTextAlignment(Qt::AlignCenter); //文件居中
    //ui->tableWidget->setItem(0, 0, item); //设置数据
    // 设置选择行为为选择整行
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //初始化TableWidget 设置列
    QStringList headers;
    headers << "序号" << "设备名称" << "型号规则" << "辅助字段" << "出厂编号"
            << "生产厂家" <<"委托方"<<"使用部门"<<"要求检完时间"<<"是否加急";
    ui->tableWidget->setHorizontalHeaderLabels(headers);



}

void Depar_Task::ShowMessageBox(QString title,QString value) {
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
void Depar_Task::Load()
{
    QList<QList<QString>> PageData;
    //添加Combox数据
    ui->name->clear();
    ui->mode->clear();
    ui->customer->clear();
    ui->dept->clear();
    ui->name->addItem("ALL");
    ui->mode->addItem("ALL");
    ui->customer->addItem("ALL");
    ui->dept->addItem("ALL");


    //先设置为行为0
    ui->tableWidget->setRowCount(0);

    data = BaseCommonApi::getAllData(loginUserId);

    int rowCount = 0;
    int row=0;
    QString State;
    for (const Dept& dept : data) {
        if(dept.measure_status=="3" || dept.measure_status=="4"){
     QList<QString> TempData;
     rowCount = ui->tableWidget->rowCount();
     ui->tableWidget->insertRow(rowCount);
     ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(row+1)));
     //ui->tableWidget->setItem(row, 0, new QTableWidgetItem(dept.id));
     ui->tableWidget->setItem(row, 1, new QTableWidgetItem(dept.sample_name)); //设置数
     ui->tableWidget->setItem(row, 2, new QTableWidgetItem(dept.sample_model)); //设置数据
     ui->tableWidget->setItem(row, 3, new QTableWidgetItem(dept.help_instruction)); //设置数据
     ui->tableWidget->setItem(row, 4, new QTableWidgetItem(dept.manufacture_no)); //设置数据
     ui->tableWidget->setItem(row, 5, new QTableWidgetItem(dept.manufacturer)); //设置数据
     ui->tableWidget->setItem(row, 6, new QTableWidgetItem(dept.customer_name)); //设置数据
     ui->tableWidget->setItem(row, 7, new QTableWidgetItem(dept.FULL_NAME)); //设置数据
     ui->tableWidget->setItem(row, 8, new QTableWidgetItem(dept.require_over_time.toString())); //设置数据
     if(dept.is_urgent=="1")
          State="是";
     else
         State="否";
     ui->tableWidget->setItem(row, 9, new QTableWidgetItem(State)); //设置数据
     SetHomePaheCombox(dept);
     //添加 PageData数据
     TempData.append(QString::number(row+1));
     TempData.append(dept.sample_name);
     TempData.append(dept.sample_model);
     TempData.append(dept.help_instruction);
     TempData.append(dept.manufacture_no);
     TempData.append(dept.manufacturer);
     TempData.append(dept.customer_name);
     TempData.append(dept.FULL_NAME);
     TempData.append(dept.require_over_time.toString());
     TempData.append(State);
     TempData.append(dept.measure_status);
     PageData.append(TempData);
      row++;
}

}
    //设置分页控件
    // 创建分页表格控件

//  PagedTableWidget *pagedTable = new PagedTableWidget(this,  ui->HBoxPageLayout);
  //data转换位QList<QList<QString>>
 // pagedTable->setData(PageData, 20,ui->tableWidget); // 每页20行
}

void Depar_Task::on_SelectButton_clicked()
{
    //查看按钮 筛选
    //筛选操作
    if(data.count()==0){
        ShowMessageBox("提示框","数据为空");
        return;
    }
    //筛选
    ui->tableWidget->setRowCount(0);
    int rowCount=0;
    int row=0;
    QString State;
    for (const Dept& dept : data) {
    if(dept.measure_status=="3" || dept.measure_status=="4"){
     bool shouldAddRow = true; // 初始假设应该添加这一行
     // 根据设备名称筛选
     QString names;
     if (ui->name->currentText() != "ALL") {
          names=ui->name->currentText();
         if (ui->name->currentText()  != dept.sample_name) {
             shouldAddRow = false; // 设备名称不匹配，不添加这一行
         }
     }
     if (ui->mode->currentText() != "ALL") {
         names=ui->mode->currentText();
         if (ui->mode->currentText() != dept.sample_model) {
             shouldAddRow = false; // 设备名称不匹配，不添加这一行
         }
     }
     if (ui->customer->currentText() != "ALL") {

         if (ui->customer->currentText() != dept.customer_name) {
             shouldAddRow = false; // 设备名称不匹配，不添加这一行
         }
     }
     if (ui->dept->currentText() != "ALL") {
         if (ui->dept->currentText() != dept.FULL_NAME) {
             shouldAddRow = false; // 设备名称不匹配，不添加这一行
         }
     }
     if(shouldAddRow){
     rowCount = ui->tableWidget->rowCount();
     ui->tableWidget->insertRow(rowCount);
     ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(row+1)));
     //ui->tableWidget->setItem(row, 0, new QTableWidgetItem(dept.id));
     ui->tableWidget->setItem(row, 1, new QTableWidgetItem(dept.sample_name)); //设置数
     ui->tableWidget->setItem(row, 2, new QTableWidgetItem(dept.sample_model)); //设置数据
     ui->tableWidget->setItem(row, 3, new QTableWidgetItem(dept.help_instruction)); //设置数据
     ui->tableWidget->setItem(row, 4, new QTableWidgetItem(dept.manufacture_no)); //设置数据
     ui->tableWidget->setItem(row, 5, new QTableWidgetItem(dept.manufacturer)); //设置数据
     ui->tableWidget->setItem(row, 6, new QTableWidgetItem(dept.customer_name)); //设置数据
     ui->tableWidget->setItem(row, 7, new QTableWidgetItem(dept.FULL_NAME)); //设置数据
     ui->tableWidget->setItem(row, 8, new QTableWidgetItem(dept.require_over_time.toString())); //设置数据
     if(dept.is_urgent=="1")
          State="是";
     else
         State="否";
     ui->tableWidget->setItem(row, 9, new QTableWidgetItem(State)); //设置数据
     //

     //QDebug
        row++;
        }
}

}
}


void Depar_Task::on_ResetButton_clicked()
{
    Load();
    ui->name->setCurrentIndex(0);
    ui->mode->setCurrentIndex(0);
    ui->customer->setCurrentIndex(0);
    ui->dept->setCurrentIndex(0);
}
bool Depar_Task::comboBoxContainsItem(QComboBox *comboBox, const QString &itemText)
{
    for (int i = 0; i < comboBox->count(); ++i) {
        if (comboBox->itemText(i) == itemText) {
            return false; // 找到匹配项，返回 true
        }
    }
    return true; // 没有找到匹配项，返回 false
}
void Depar_Task::SetHomePaheCombox(Dept entity)
{
    if(comboBoxContainsItem(ui->name,entity.sample_name)){
        ui->name->addItem(entity.sample_name);
    }
    if(comboBoxContainsItem(ui->mode,entity.sample_model)){
        ui->mode->addItem(entity.sample_model);
    }
    if(comboBoxContainsItem(ui->customer,entity.customer_name)){
        ui->customer->addItem(entity.customer_name);
    }
    if(comboBoxContainsItem(ui->dept,entity.FULL_NAME)){
        ui->dept->addItem(entity.FULL_NAME);
    }
}
