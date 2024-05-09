#include "my_task.h"
#include "ui_my_task.h"
#include "QMessageBox"
#include "basecommonapi.h"
#include "PageControl.h"
#include "global.h"

My_Task::My_Task(QWidget *parent) :
    InfoBase(parent),
    ui(new Ui::My_Task)
{
    ui->setupUi(this);
    //初始化QStandardItemModel
    model=new QStandardItemModel(parent);
    this->initForm();
}

My_Task::~My_Task()
{
    delete ui;
}
void My_Task::initForm()
{
    //tableWidget设置
    //设置控件的边框显示
    ui->tableWidget->setStyleSheet("QHeaderView::section {"
                                "border: 1px solid rgba(197, 196, 196, 1);"
                                "border-left:none;"
                                "border-top:none;"
                                "background-color:white;"
                               "}");

    ui->tableWidget->setRowCount(10); // 设置10行
    ui->tableWidget->setColumnCount(11); // 设置11列
    ui->tableWidget->setColumnWidth(0, 80); // 设置第一列的宽度为100像素
    ui->tableWidget->verticalHeader()->setParent(nullptr); //隐藏行头
    QTableWidgetItem *item = new QTableWidgetItem("测试数据");
    item->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(0, 0, item); //设置数据

    //初始化TableWidget 设置列
    QStringList headers;
    headers << "序号" << "设备名称" << "型号规则" << "辅助字段" << "出厂编号"
            << "生产厂家" <<"委托方"<<"使用部门"<<"要求检完时间"<<"是否加急"<<"检测状态";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

}
void My_Task::ShowMessageBox(QString title,QString value) {
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
void My_Task::Load()
{

    //QList<QList<QString>> PageData;
    //先设置为行为0
    ui->tableWidget->setRowCount(0);
    //添加comboBox
    ui->combox_state->clear();
    ui->combox_dept->clear();
    ui->combox_type->clear();
    ui->combox_name->clear();
    ui->combox_customer->clear();
    ui->combox_state->addItem("ALL");
    ui->combox_state->addItem("待检测");
    ui->combox_state->addItem("检测完成");
    ui->combox_dept->addItem("ALL");
    ui->combox_name->addItem("ALL");
    ui->combox_type->addItem("ALL");
    ui->combox_customer->addItem("ALL");

    BaseCommonApi dbManager;

    // 调用数据库管理器的函数获取数据
    //模拟用户ID
//    QString userId="";
    data = BaseCommonApi::getMyTasks(loginUserId);

    QString State;
    //设置单页显示数量
    page=20;
    // 创建分页表格控件
    PagedTableWidget *pagedTable = new PagedTableWidget(this,  ui->HBoxPageLayout);
    pagedTable->setData(data.count(), page); // 每页10行
    connect(pagedTable, &PagedTableWidget::pageChanged, this,&My_Task::onpageChanged);
    this->onpageChanged(0);

}
void My_Task::onpageChanged(int currPage){
    currPage+=1;
    // 计算当前页的开始和结束索引
    int start = (currPage - 1) * page;
    int end = std::min(start + page, data.size());
    // 从 QList 中筛选出当前页的数据
    QList<MyTask> tempdate = data.mid(start, end - start);
    int rowCount = 0;
    int row=0;
    QString State;
    ui->tableWidget->setRowCount(0);
    for (const MyTask& myTask : tempdate) {

               QList<QString> TempData;

               rowCount = ui->tableWidget->rowCount();
               ui->tableWidget->insertRow(rowCount);
               ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(row+1)));
               //ui->tableWidget->setItem(row, 0, new QTableWidgetItem(dept.id));
               ui->tableWidget->setItem(row, 1, new QTableWidgetItem(myTask.sampleName)); //设置数据
               ui->tableWidget->setItem(row, 2, new QTableWidgetItem(myTask.sampleModel)); //设置数据
               ui->tableWidget->setItem(row, 3, new QTableWidgetItem(myTask.help_instruction)); //设置数据
               ui->tableWidget->setItem(row, 4, new QTableWidgetItem(myTask.manufactureNo)); //设置数据
               ui->tableWidget->setItem(row, 5, new QTableWidgetItem(myTask.manufacturer)); //设置数据
               ui->tableWidget->setItem(row, 6, new QTableWidgetItem(myTask.customer_name)); //设置数据
               ui->tableWidget->setItem(row, 7, new QTableWidgetItem(myTask.FULL_NAME)); //设置数据
               ui->tableWidget->setItem(row, 8, new QTableWidgetItem(myTask.require_over_time.toString())); //设置数据
               if(myTask.is_urgent=="1")
                    State="是";
               else
                   State="否";
               ui->tableWidget->setItem(row, 9, new QTableWidgetItem(State)); //设置数据
               if(myTask.measure_status=="3")
               {
                    ui->tableWidget->setItem(row, 10,new QTableWidgetItem("待检测"));
               }else if(myTask.measure_status=="4")
               {
                     ui->tableWidget->setItem(row, 10,new QTableWidgetItem("检测完成"));
               }

         //渲染下拉框
         SetHomePaheCombox(myTask);
         row++;


    };
    //按钮点击事件
   // QMessageBox::information(this, "点击按钮",QString::number(page));
   // qDebug() << "Page changed to:" << page;
}
void My_Task::SetHomePaheCombox(MyTask entity)
{
    if(comboBoxContainsItem(ui->combox_name,entity.sampleName)){
        ui->combox_name->addItem(entity.sampleName);
    }
    if(comboBoxContainsItem(ui->combox_dept,entity.FULL_NAME)){
        ui->combox_dept->addItem(entity.FULL_NAME);
    }
    if(comboBoxContainsItem(ui->combox_type,entity.sampleModel)){
        ui->combox_type->addItem(entity.sampleModel);
    }
    if(comboBoxContainsItem(ui->combox_customer,entity.customer_name)){
        ui->combox_customer->addItem(entity.customer_name);
    }


}
bool My_Task::comboBoxContainsItem(QComboBox *comboBox, const QString &itemText)
{
    for (int i = 0; i < comboBox->count(); ++i) {
        if (comboBox->itemText(i) == itemText) {
            return false; // 找到匹配项，返回 true
        }
    }
    return true; // 没有找到匹配项，返回 false
}

void My_Task::on_pushButtonUpdate_clicked()
{
    Load();
    ui->combox_dept->setCurrentIndex(0);
    ui->combox_name->setCurrentIndex(0);
    ui->combox_type->setCurrentIndex(0);
    ui->combox_customer->setCurrentIndex(0);
    ui->combox_state->setCurrentIndex(0);
}

void My_Task::on_SeachButton_clicked()
{
    //查询按钮
    //筛选操作
    if(data.count()==0){
        ShowMessageBox("提示框","数据为空");
        return;
    }
    //筛选
    ui->tableWidget->setRowCount(0);
    int statecount=0;
    int rowCount=0;
    int row=0;
    QString State;
    for (const MyTask& myTask : data) {
           bool shouldAddRow = true; // 初始假设应该添加这一行
        // 根据设备名称筛选
        if (ui->combox_dept->currentText() != "ALL") {
            if (ui->combox_dept->currentText()  != myTask.FULL_NAME) {
                shouldAddRow = false; // 设备名称不匹配，不添加这一行
            }
        }
        if (ui->combox_name->currentText() != "ALL") {
            if (ui->combox_name->currentText() != myTask.sampleName) {
                shouldAddRow = false; // 设备名称不匹配，不添加这一行
            }
        }
        if (ui->combox_type->currentText() != "ALL") {
            if (ui->combox_type->currentText() !=myTask.sampleModel) {
                shouldAddRow = false; // 设备名称不匹配，不添加这一行
            }
        }
        if (ui->combox_customer->currentText() != "ALL") {
            if (ui->combox_customer->currentText() != myTask.customer_name) {
                shouldAddRow = false; // 设备名称不匹配，不添加这一行
            }
        }
        if(shouldAddRow){
            if (ui->combox_state->currentText() == "ALL" || // 如果是"ALL"，显示所有设备
                   (ui->combox_state->currentText() == "待检测" && myTask.measure_status == "3") || // 如果是"待检测"，只显示state为3的设备
                   (ui->combox_state->currentText() == "检测完成" && myTask.measure_status == "4")){

                  rowCount = ui->tableWidget->rowCount();
                  ui->tableWidget->insertRow(rowCount);
                  ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(row+1)));

                  ui->tableWidget->setItem(row, 1, new QTableWidgetItem(myTask.sampleName)); //设置数据
                  ui->tableWidget->setItem(row, 2, new QTableWidgetItem(myTask.sampleModel)); //设置数据
                  ui->tableWidget->setItem(row, 3, new QTableWidgetItem(myTask.help_instruction)); //设置数据
                  ui->tableWidget->setItem(row, 4, new QTableWidgetItem(myTask.manufactureNo)); //设置数据
                  ui->tableWidget->setItem(row, 5, new QTableWidgetItem(myTask.manufacturer)); //设置数据
                  ui->tableWidget->setItem(row, 6, new QTableWidgetItem(myTask.customer_name)); //设置数据
                  ui->tableWidget->setItem(row, 7, new QTableWidgetItem(myTask.FULL_NAME)); //设置数据
                  ui->tableWidget->setItem(row, 8, new QTableWidgetItem(myTask.require_over_time.toString())); //设置数据
                  if(myTask.is_urgent=="1")
                       State="是";
                  else
                      State="否";
                  ui->tableWidget->setItem(row, 9, new QTableWidgetItem(State)); //设置数据
                  if(myTask.measure_status=="3")
                  {
                       ui->tableWidget->setItem(row, 10,new QTableWidgetItem("待检测"));
                  }else if(myTask.measure_status=="4")
                  {
                        ui->tableWidget->setItem(row, 10,new QTableWidgetItem("检测完成"));
                  }


           row++;
           }
        };


    }
}
/*
void My_Task::PageLoad()
{


     //每页显示的数量
    int rowsPerPage=10;
     //获取数据总数量
    int totalPages = (data.count() + rowsPerPage - 1) / rowsPerPage;
    for (int i = 0; i < totalPages; ++i) {
            QPushButton *button = new QPushButton(QString::number(i + 1), this);
            button->setCheckable(true);
            connect(button, &QPushButton::clicked, this, [this, i] { onPageChanged(i); });
            pageButtons[i] = button;
            pageLayout->addWidget(button);
        }
}
void My_Task::onPageChanged(int pageIndex) {
    currentPage = pageIndex;
    updateTable(pageIndex);
}  */

