#include "create_task.h"
#include "ui_create_task.h"
#include "QMessageBox"
#include "global.h"
#include "createtask_scandevice.h"


 QString Create_Task::State="";

Create_Task::Create_Task(QWidget *parent) :
    InfoBase(parent),
    ui(new Ui::Create_Task)
{
    ui->setupUi(this);
    this->initForm();

}

Create_Task::~Create_Task()
{
    delete ui;
}
void Create_Task::initForm()
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
    ui->tableWidget->setColumnCount(9); // 设置11列
    ui->tableWidget->setColumnWidth(0, 80); // 设置第一列的宽度为100像素
    ui->tableWidget->verticalHeader()->setParent(nullptr); //隐藏行头
    //QTableWidgetItem *item = new QTableWidgetItem("测试数据");
    //item->setTextAlignment(Qt::AlignCenter);
    //ui->tableWidget->setItem(0, 0, item); //设置数据
     ui->tableWidget->setColumnWidth(0, 0); // 设置第一列的宽度为0
      ui->tableWidget->setColumnWidth(8,180);
    //初始化TableWidget 设置列
    QStringList headers;
    headers <<"ID"<< "序号" << "任务单编号" << "委托方"<< "使用部门"
            << "要求检完时间" << "送检人"<<"是否加急"<<"操作";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    //
    //tableWidget设置
    //设置控件的边框显示
    ui->tableWidgetItem->setStyleSheet("QHeaderView::section {"
                                "border: 1px solid rgba(197, 196, 196, 1);"
                                "border-left:none;"
                                "border-top:none;"
                                "background-color:white;"
                               "}");

    //ui->tableWidget->setRowCount(10); // 设置10行
    ui->tableWidgetItem->setColumnCount(8); // 设置11列
    ui->tableWidgetItem->setColumnWidth(0,0); // 设置第一列的宽度为100像素
    ui->tableWidgetItem->verticalHeader()->setParent(nullptr); //隐藏行头
    // 设置选择行为为选择整行
    ui->tableWidgetItem->setSelectionBehavior(QAbstractItemView::SelectRows);
     //ui->tableWidgetItem->setColumnWidth(0, 0); // 设置第一列的宽度为0
      ui->tableWidgetItem->setColumnWidth(8,180);
    //初始化TableWidget 设置列

    headers << "序号" << "设备名称" << "规格型号"<< "辅助字段"
            << "出厂编号" << "生产厂家"<<"附件"<<"特殊要求";
    ui->tableWidgetItem->setHorizontalHeaderLabels(headers);
    //ShowMessageBox("标题","初始化完成");
}
void Create_Task::ShowMessageBox(QString title,QString value) {
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
void Create_Task::Load()
{

    //
    ui->combox_customer->clear();
    ui->combox_task_no->clear();
    ui->combox_dept->clear();
    ui->combox_customer->addItem("ALL");
    ui->combox_task_no->addItem("ALL");

    //先设置为行为0
    ui->tableWidget->setRowCount(0);
    ui->stackedWidget->setCurrentIndex(0);

    datares = dbManager.getCreateTaskList("");
    //根据用户查出部门
//    StaffInfo Deptinfo=dbManager.ConvertStaffInfo("用户ID目前固定");
    int rowCount = 0;
    int row=0;
    QString is_urgent;
    for (const CreateTaskList& item : datares) {
        rowCount = ui->tableWidget->rowCount();

        ui->tableWidget->insertRow(rowCount);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(item.id));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(row+1)));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(item.order_no));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(item.customer_name));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(loginMeasureDeptName));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(item.require_over_time.toString()));
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(item.deliverer));
        if(item.is_urgent=="1")
             is_urgent="是";
        else
            is_urgent="否";
        ui->tableWidget->setItem(row, 7, new QTableWidgetItem(is_urgent));

        //实现单元格添加两个按钮
        // 创建一个QWidget作为单元格的容器
        QWidget *widget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(widget); // 水平布局


        QPushButton *button = new QPushButton("查看");
        QPushButton *button2 = new QPushButton("编辑");
        QPushButton *button3 = new QPushButton("删除");

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
         connect(button, &QPushButton::clicked, this, [this, button, row]() {
             // 获取当前行第一列的 ID
             QTableWidgetItem *idItem = this->ui->tableWidget->item(row, 0);
             if (idItem) {
                 QString id = idItem->text();
                 OnButtonSelectClicked(id);
                 Create_Task::State="Select";
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
          button2->setIcon(QIcon(":/image/Index/u2324.svg"));
         // 连接按钮的 clicked 信号到自定义槽函数
          connect(button2, &QPushButton::clicked, this, [this, button2, row]() {
              // 获取当前行第一列的 ID
              QTableWidgetItem *idItem = this->ui->tableWidget->item(row, 0);
              if (idItem) {
                  QString id = idItem->text();
                  OnButtonUpdateClicked(id);
                  Create_Task::State="Update";
              }

          });
          //设置按钮3简单样式
          button3->setStyleSheet("QPushButton {"
                                  "    border: none; /* 无边框 */"
                                  "    color: red; /* 文本颜色 */"
                                  "    text-decoration: underline; /* 文本下划线 */"
                                  "}");

           // 设置鼠标悬停时的光标样式为小手图标
           button3->setCursor(Qt::PointingHandCursor);
           button3->setIconSize(QSize(16,16));
           button3->setIcon(QIcon(":/image/Index/u2325.svg"));
          // 连接按钮的 clicked 信号到自定义槽函数
           connect(button3, &QPushButton::clicked, this, [this, button3, row]() {
               // 获取当前行第一列的 ID
               QTableWidgetItem *idItem = this->ui->tableWidget->item(row, 0);
               if (idItem) {
                   QString id = idItem->text();
                   OnButtoDeleteClicked(id);
               }

           });
          //将按钮添加到布局控件
          layout->addWidget(button);
          layout->addWidget(button2);
          layout->addWidget(button3);
          layout->setContentsMargins(5, 5, 0, 0); // 设置布局边距为0
          widget->setLayout(layout);
          //将布局控件添加到 单元格
          ui->tableWidget->setCellWidget(row, 8, widget);
          // 调整表格大小以适应内容
           //ui->tableWidget->resizeColumnsToContents();
          // ui->tableWidget->resizeRowsToContents();
          SetHomePaheComboBox(item);
        row++;
       }
}
void Create_Task::OnButtonSelectClicked(QString id)
{
    // ========================================================================================
    // 查看按钮 点击事件  进入详情页面
    // ====================================================================================
     //DeviceInfoInit(id);
     HiddenButton();
     SelectPageData(id);
     ui->TitleLable->setText("查看任务单");
     ui->stackedWidget->setCurrentIndex(1);

     //::information(this, "查看按钮ID",id);
}
void Create_Task::OnButtonUpdateClicked(QString id)
{
    // ========================================================================================
    // 编辑按钮 点击事件  进入详情页面
    // ====================================================================================
     //DeviceInfoInit(id);


     DisplayButton();
     //查询出所有委托方
     //SetPartPageComBox();
     SelectPageData(id);
     TaskId=id;
     ui->TitleLable->setText("编辑任务单");
     ui->stackedWidget->setCurrentIndex(1);

     //QMessageBox::information(this, "编辑按钮ID",id);
}
void Create_Task::OnButtoDeleteClicked(QString id)
{
    // ========================================================================================
    // 删除按钮 点击事件  进入详情页面
    // ====================================================================================
     //DeviceInfoInit(id);
     //ui->stackedWidget->setCurrentIndex(1);
     bool IsSucc=dbManager.DeleteTask(id);
     if(IsSucc){
       QMessageBox::information(this, "提示","删除成功");
       Load();
     }else{
       QMessageBox::information(this, "提示","删除失败");
     }


}

void Create_Task::on_pushButton_2_clicked()
{
    // ========================================================================================
    // 创建任务单按钮
    // ====================================================================================
    //显示任务单编号
    ui->TitleLable->setText("创建任务单");

    //显示按钮
    DisplayButton();
    ui->is_urgent->clear();

    ui->customer_name->clear();
    ui->stackedWidget->setCurrentIndex(1);
    Create_Task::State="Create";
    //查询出所有委托方
    SetPartPageComBox();

    ui->tableWidgetItem->setRowCount(0);
    ui->is_urgent->addItem("是");
    ui->is_urgent->addItem("否");
    ui->LineEditNumber->setText(dbManager.GenerateTaskNumber());
    //添加设备table数据

}
void Create_Task::HiddenButton()
{
    ui->SaveButton->hide();
    ui->CreateButton->hide();
    ui->ScanButton->hide();
    ui->deletebutton->hide();
}
void Create_Task::DisplayButton()
{
    ui->SaveButton->show();
    ui->CreateButton->show();
    ui->ScanButton->show();
    ui->deletebutton->show();
}

void Create_Task::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Create_Task::on_ScanButton_clicked()
{
    // 创建新窗体实例并显示
    CreateTask_ScanDevice *ShowDataSyncView  = new CreateTask_ScanDevice();
    ShowDataSyncView->CreateTaskInst=this;
    //ShowDataSyncView->setWindowState(Qt::WindowMaximized);
    ShowDataSyncView->setWindowModality(Qt::ApplicationModal);
    ShowDataSyncView->show();
}
void Create_Task::DeviceInfoShow(QList<QString> ListId)
{
    if(ListId.count()!=0)
    {
        SelectDataItem.clear();
        _IdList=ListId;
        ui->tableWidgetItem->setRowCount(0);
        // 调用数据库管理器的函数获取数据
        int rowCount=0;
        int row=0;
        QList<TestedCatDevice> result;
        foreach(QString DeviceId,ListId)
        {
          result = dbManager.getTestedCatDevice(DeviceId);
          SelectDataItem.append(result[0]);
          if(result.length()>0)
          {
              rowCount = ui->tableWidgetItem->rowCount();
              ui->tableWidgetItem->insertRow(rowCount);
              ui->tableWidgetItem->setItem(row, 1, new QTableWidgetItem(QString::number(row+1)));
              ui->tableWidgetItem->setItem(row, 2, new QTableWidgetItem(result[0].equipment_name));
              ui->tableWidgetItem->setItem(row, 3, new QTableWidgetItem(result[0].model));
              ui->tableWidgetItem->setItem(row, 4, new QTableWidgetItem(result[0].help_instruction));
              ui->tableWidgetItem->setItem(row, 5, new QTableWidgetItem(result[0].manufacture_no));
              ui->tableWidgetItem->setItem(row, 6, new QTableWidgetItem(result[0].manufacturer));
              ui->tableWidgetItem->setItem(row, 7, new QTableWidgetItem("点击编辑"));
              ui->tableWidgetItem->setItem(row, 8, new QTableWidgetItem("点击编辑"));

              row++;
          }

        }

    }

}

void Create_Task::on_SaveButton_clicked()
{
    // ========================================================================================
    // 保存任务单按钮  先新增任务单  后新增任务单设备
    // ====================================================================================
     bool IsSUCC=false;

    //获取检定人ID
//    QString person_id=dbManager.ConvertStaffInfo("用户ID目前固定").id;
//    //根据用户查出部门ID
//    QString DeptId=dbManager.ConvertStaffInfo("函数内默认添加").dept_id;

    if(State=="Update")
    {
        if(TaskId=="")return;
        InsertTask insertTask=dbManager.SelectTaskSingle(TaskId);
        insertTask.order_no=ui->LineEditNumber->text();
        insertTask.require_over_time=ui->require_over_time->text();
        insertTask.deliverer=ui->deliverer->text();
        insertTask.deliverer_tel=ui->deliverer_tel->text();
        insertTask.undertaker_name=ui->undertaker_name->text();
        insertTask.undertake_time=ui->undertake_time->text();
        insertTask.receive_illustrate=ui->receive_illustrate->text();

        IsSUCC=dbManager.UpdateTask(insertTask);
        //设备更新  编辑设备待写
        //1、先删除之前的设备
        dbManager.DeleteTaskSameDevice(TaskId);
        dbManager.DeleteTaskInfoRecordDevice(TaskId);
        qint64 id = dbManager.generateId();
        //2、将新的设置重新增加到这两张表
        int count=0;
        if(SelectDataItem.count()!=0 && _IdList.count()!=0){
             //InsertOrderSampleRelation
             foreach(TestedCatDevice DeviceInfo,SelectDataItem)
             {
                 //样品表
                 OrderSampleRelation orderSampleRelation;
                 id = dbManager.generateId();
                 orderSampleRelation.id=QString::number(id);
                 orderSampleRelation.order_id=TaskId;
                 orderSampleRelation.sample_id=_IdList[count];
                 orderSampleRelation.sample_name=DeviceInfo.equipment_name;
                 orderSampleRelation.sample_model=DeviceInfo.model;
                 orderSampleRelation.measure_complete_time=QDateTime::currentDateTime();
                 IsSUCC=dbManager.InsertOrderSampleRelation(orderSampleRelation);

                 //每条设备对应一个信息表数据
                 //增加biz_business_lab_executive_info 总信息表
                 biz_business_lab_executive_info entity;
                 entity.id=QString::number(dbManager.generateId());
                 entity.order_id=TaskId;
                 entity.sample_id=_IdList[count];
                 entity.create_user_id="函数内默认添加";
                 entity.measure_dept_id=loginUserDeptId; //
                 entity.measure_person_id=loginMeasurePersonId;
                 IsSUCC=dbManager.InsertTaskInfoRecord(entity);

                 if(!IsSUCC){ShowMessageBox("提示",QString("设备名称为: 1% 添加失败").arg(DeviceInfo.equipment_name));}
                 count++;
             }
           }else{

            biz_business_lab_executive_info entity;
            entity.id=QString::number(dbManager.generateId());
            entity.order_id=TaskId;
            entity.sample_id="";
            entity.create_user_id="函数内默认添加";
            entity.measure_dept_id=loginUserDeptId; //
            entity.measure_person_id=loginMeasurePersonId;
            IsSUCC=dbManager.InsertTaskInfoRecord(entity);
        }
    }else{
        //获取选中的委托人ID
    QString CustomerID="";
    if(_model->rowCount()>0){
    QModelIndex currentIndex = _model->index(ui->customer_name->currentIndex(),0);
    CustomerID=currentIndex.data().toString(); }
    //新增任务单
    qint64 id = dbManager.generateId();
    InsertTask insertTask;
    insertTask.id=QString::number(id);
    insertTask.order_no=ui->LineEditNumber->text();
    insertTask.customer_name=ui->customer_name->currentText();
    insertTask.require_over_time=ui->require_over_time->text();
    insertTask.deliverer=ui->deliverer->text();
    insertTask.deliverer_tel=ui->deliverer_tel->text();
    insertTask.undertaker_name=ui->undertaker_name->text();
    insertTask.undertake_time=ui->undertake_time->text();
    //insertTask.=ui->comboBox_5->currentText();
    if(ui->is_urgent->currentText()=="是")
    {
        insertTask.is_urgent=1;
    }else{insertTask.is_urgent=0;}

    insertTask.receive_illustrate=ui->receive_illustrate->text();
    insertTask.customer_id=CustomerID;
     IsSUCC=dbManager.InsertTaskBasic(insertTask);
     //新增任务单设备
     QString NewTaskId=dbManager.getCreateTaskListNew()[0].id;
    if(IsSUCC)
    {


        int count=0;
        if(SelectDataItem.count()!=0 && _IdList.count()!=0){
             //InsertOrderSampleRelation
             foreach(TestedCatDevice DeviceInfo,SelectDataItem)
             {
                 //样品表
                 OrderSampleRelation orderSampleRelation;
                 id = dbManager.generateId();
                 orderSampleRelation.id=QString::number(id);
                 orderSampleRelation.order_id=NewTaskId;
                 orderSampleRelation.sample_id=_IdList[count];
                 orderSampleRelation.sample_name=DeviceInfo.equipment_name;
                 orderSampleRelation.sample_model=DeviceInfo.model;
                 orderSampleRelation.measure_complete_time=QDateTime::currentDateTime();
                 IsSUCC=dbManager.InsertOrderSampleRelation(orderSampleRelation);

                 //每条设备对应一个信息表数据
                 //增加biz_business_lab_executive_info 总信息表
                 biz_business_lab_executive_info entity;
                 entity.id=QString::number(dbManager.generateId());
                 entity.order_id=NewTaskId;
                 entity.sample_id=_IdList[count];
                 entity.create_user_id="函数内默认添加";
                 entity.measure_dept_id=loginUserDeptId; //
                 entity.measure_person_id=loginMeasurePersonId;
                 IsSUCC=dbManager.InsertTaskInfoRecord(entity);

                 if(!IsSUCC){ShowMessageBox("提示",QString("设备名称为: 1% 添加失败").arg(DeviceInfo.equipment_name));}
                 count++;
             }
           }else{
            //每条设备对应一个信息表数据
            //增加biz_business_lab_executive_info 总信息表
            biz_business_lab_executive_info entity;
            entity.id=QString::number(dbManager.generateId());
            entity.order_id=NewTaskId;
            entity.sample_id="";
            entity.create_user_id="函数内默认添加";
            entity.measure_dept_id=loginUserDeptId; //
            entity.measure_person_id=CustomerID;
            IsSUCC=dbManager.InsertTaskInfoRecord(entity);
        }


        if(IsSUCC){
            ShowMessageBox("提示","委托单添加成功");
            //显示任务单编号
            dbManager.AddTaskNumber();
            ui->LineEditNumber->setText(dbManager.GenerateTaskNumber());
        }

    }else
    {
        ShowMessageBox("提示","委托单添加失败");
        return;
    }
    }
    ui->stackedWidget->setCurrentIndex(0);
    Load();
}
void Create_Task::SelectPageData(QString ID)
{


    //顶部数据
   QList<CatTask> data= dbManager.getCatTask(ID);
   if(data.count()==0)return;
   ui->is_urgent->clear();

   ui->customer_name->clear();

   ui->LineEditNumber->setText(data[0].order_no);
   ui->customer_name->addItem(data[0].customer_name);
   ui->require_over_time->setDateTime(data[0].require_over_time);
   ui->deliverer->setText(data[0].deliverer);
   ui->deliverer_tel->setText(data[0].deliverer_tel);
   ui->undertaker_name->setText(data[0].undertaker_name);
   ui->undertake_time->setDateTime(data[0].undertake_time);
   if(data[0].is_urgent=="1"){
         ui->is_urgent->addItem("是");
   }else{
         ui->is_urgent->addItem("否");
   }
   ui->receive_illustrate->setText(data[0].receive_illustrate);
   //设备数据
   //重新设计列

   QStringList headers;
   headers <<"Id"<< "序号" << "设备名称" << "规格型号"<< "辅助字段"
           << "出厂编号" << "生产厂家"<<"附件"<<"特殊要求"
           <<"检定完成时间"<<"检定结果"<<"限用";
   ui->tableWidgetItem->setColumnCount(headers.count());
   ui->tableWidgetItem->setHorizontalHeaderLabels(headers);

  ui->tableWidgetItem->setRowCount(0);
  QList<CatTaskList> Devicedate= dbManager.getCatTaskList(ID);
  if(Devicedate.count()>0){
  // 调用数据库管理器的函数获取数据
  int rowCount=0;
  int row=0;
  foreach(CatTaskList item,Devicedate)
  {
      rowCount = ui->tableWidgetItem->rowCount();
      ui->tableWidgetItem->insertRow(rowCount);
      ui->tableWidgetItem->setItem(row, 0, new QTableWidgetItem(item.id));
      ui->tableWidgetItem->setItem(row, 1, new QTableWidgetItem(QString::number(row+1)));
      ui->tableWidgetItem->setItem(row, 2, new QTableWidgetItem(item.sample_name));
      ui->tableWidgetItem->setItem(row, 3, new QTableWidgetItem(item.sample_model));
      ui->tableWidgetItem->setItem(row, 4, new QTableWidgetItem("缺少"));
      ui->tableWidgetItem->setItem(row, 5, new QTableWidgetItem(item.manufacture_no));
      ui->tableWidgetItem->setItem(row, 6, new QTableWidgetItem(item.manufacturer));
      ui->tableWidgetItem->setItem(row, 7, new QTableWidgetItem(item.appendix_descn));
      ui->tableWidgetItem->setItem(row, 8, new QTableWidgetItem(item.special_require));
      ui->tableWidgetItem->setItem(row, 9, new QTableWidgetItem(item.measure_complete_time.toString()));
      ui->tableWidgetItem->setItem(row, 10, new QTableWidgetItem(item.conclusion));
      ui->tableWidgetItem->setItem(row, 11, new QTableWidgetItem(item.limit_instruction));
      row++;
  }
  }
}

void Create_Task::on_pushButton_8_clicked()
{
    Load();

}

void Create_Task::on_deletebutton_clicked()
{
    //删除按钮
    int row = ui->tableWidgetItem->currentRow();
    if (row >= 0) { // 确保有行被选中
        // 获取选中行第一列的 QTableWidgetItem
      QTableWidgetItem *item = ui->tableWidgetItem->item(row, 0);
    if (item) { // 确保 item 不是 nullptr
         // 获取并输出第一列的数据
         QString data = item->text();
         ShowMessageBox("",data) ;
         // 在这里可以使用 data 进行进一步处理
        if(dbManager.DeleteTaskDevice(data)){
             ShowMessageBox("","删除成功");
             SelectPageData(TaskId);
        }else{ ShowMessageBox("","删除失败");}
     } else {
         ShowMessageBox("","在所选行的第一列中没有项") ;

       }
   } else {
        ShowMessageBox("","请选择一行数据") ;

   }
}
void Create_Task::SetHomePaheComboBox(CreateTaskList entity)
{

    if(comboBoxContainsItem(ui->combox_dept,entity.DEPT_FULL_NAME)){
        ui->combox_dept->addItem(entity.DEPT_FULL_NAME);
    }
    if(comboBoxContainsItem(ui->combox_task_no,entity.order_no)){
        ui->combox_task_no->addItem(entity.order_no);
    }
    if(comboBoxContainsItem(ui->combox_customer,entity.customer_name)){
        ui->combox_customer->addItem(entity.customer_name);
    }


}
void Create_Task::SetPartPageComBox()
{

    QList<biz_resource_customer_info> data=BaseCommonApi::SelectCustomerData();

    _model = new QStandardItemModel(0, 2); // 0 rows, 2 columns
    _model->setHorizontalHeaderLabels(QStringList() << "ID" << "customer"); // 设置列标题（可选）
    foreach (biz_resource_customer_info item, data) {
        QList<QStandardItem *> items;
        items.append(new QStandardItem(item.id)); // ID
        items.append(new QStandardItem(item.customer_name)); // Name
        _model->appendRow(items);
    }
   ui->customer_name->setModel(_model);
   ui->customer_name->setModelColumn(1); // 显示 Name
}

bool Create_Task::comboBoxContainsItem(QComboBox *comboBox, const QString &itemText)
{
    for (int i = 0; i < comboBox->count(); ++i) {
        if (comboBox->itemText(i) == itemText) {
            return false; // 找到匹配项，返回 true
        }
    }
    return true; // 没有找到匹配项，返回 false
}




void Create_Task::on_Update_clicked()
{
    Load();
    ui->combox_customer->setCurrentIndex(0);
    ui->combox_task_no->setCurrentIndex(0);
    ui->combox_dept->setCurrentIndex(0);
}



void Create_Task::on_Select_clicked()
{
    //查询按钮 筛选
    //筛选操作
    if(datares.count()==0){
        ShowMessageBox("提示框","数据为空");
        return;
    }
    //筛选
    //根据用户查出部门
//    StaffInfo Deptinfo=dbManager.ConvertStaffInfo("用户ID目前固定");
    int rowCount = 0;
    int row=0;
    QString is_urgent;
    for (const CreateTaskList& item : datares) {
        bool shouldAddRow = true; // 初始假设应该添加这一行
        // 根据设备名称筛选
        if (ui->combox_customer->currentText() != "ALL") {
            if (ui->combox_customer->currentText()  != item.customer_name) {
                shouldAddRow = false; // 设备名称不匹配，不添加这一行
            }
        }
        if (ui->combox_task_no->currentText() != "ALL") {
            if (ui->combox_task_no->currentText() != item.order_no) {
                shouldAddRow = false; // 设备名称不匹配，不添加这一行
            }
        }
        if(shouldAddRow){
        rowCount = ui->tableWidget->rowCount();

        ui->tableWidget->insertRow(rowCount);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(item.id));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(row+1)));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(item.order_no));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(item.customer_name));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(loginMeasureDeptName));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(item.require_over_time.toString()));
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(item.deliverer));
        if(item.is_urgent=="1")
             is_urgent="是";
        else
            is_urgent="否";
        ui->tableWidget->setItem(row, 7, new QTableWidgetItem(is_urgent));

        //实现单元格添加两个按钮
        // 创建一个QWidget作为单元格的容器
        QWidget *widget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(widget); // 水平布局


        QPushButton *button = new QPushButton("查看");
        QPushButton *button2 = new QPushButton("编辑");
        QPushButton *button3 = new QPushButton("删除");

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
         connect(button, &QPushButton::clicked, this, [this, button, row]() {
             // 获取当前行第一列的 ID
             QTableWidgetItem *idItem = this->ui->tableWidget->item(row, 0);
             if (idItem) {
                 QString id = idItem->text();
                 OnButtonSelectClicked(id);
                 Create_Task::State="Select";
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
          button2->setIcon(QIcon(":/image/Index/u2324.svg"));
         // 连接按钮的 clicked 信号到自定义槽函数
          connect(button2, &QPushButton::clicked, this, [this, button2, row]() {
              // 获取当前行第一列的 ID
              QTableWidgetItem *idItem = this->ui->tableWidget->item(row, 0);
              if (idItem) {
                  QString id = idItem->text();
                  OnButtonUpdateClicked(id);
                  Create_Task::State="Update";
              }

          });
          //设置按钮3简单样式
          button3->setStyleSheet("QPushButton {"
                                  "    border: none; /* 无边框 */"
                                  "    color: red; /* 文本颜色 */"
                                  "    text-decoration: underline; /* 文本下划线 */"
                                  "}");

           // 设置鼠标悬停时的光标样式为小手图标
           button3->setCursor(Qt::PointingHandCursor);
           button3->setIconSize(QSize(16,16));
           button3->setIcon(QIcon(":/image/Index/u2325.svg"));
          // 连接按钮的 clicked 信号到自定义槽函数
           connect(button3, &QPushButton::clicked, this, [this, button3, row]() {
               // 获取当前行第一列的 ID
               QTableWidgetItem *idItem = this->ui->tableWidget->item(row, 0);
               if (idItem) {
                   QString id = idItem->text();
                   OnButtoDeleteClicked(id);
               }

           });
          //将按钮添加到布局控件
          layout->addWidget(button);
          layout->addWidget(button2);
          layout->addWidget(button3);
          layout->setContentsMargins(5, 5, 0, 0); // 设置布局边距为0
          widget->setLayout(layout);
          //将布局控件添加到 单元格
          ui->tableWidget->setCellWidget(row, 8, widget);
          // 调整表格大小以适应内容
           //ui->tableWidget->resizeColumnsToContents();
          // ui->tableWidget->resizeRowsToContents();

        row++;
       }
    };
}
