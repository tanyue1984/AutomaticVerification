#include "verifymethodmanage.h"
#include "ui_verifymethodmanage.h"
#include "newverifymethod.h"
#include <QMessageBox>
#include "PageControl.h"

VerifyMethodManage::VerifyMethodManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VerifyMethodManage)
{
    ui->setupUi(this);
    ui->tableWidget_2->verticalHeader()->setParent(nullptr);
    for(int i=1;i<5;i++)
        ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);
    ui->tableWidget_2->setColumnWidth(0, 60);
    ui->tableWidget_2->setColumnWidth(5, 200);
    getVerifyMethod();
}

VerifyMethodManage::~VerifyMethodManage()
{
    programList.clear();
    delete ui;
}

void VerifyMethodManage::showNewMethod()
{
    NewVerifyMethod* checkWindow = new NewVerifyMethod;
    checkWindow->verifyManageWnd=this;
    checkWindow->setWindowState(Qt::WindowMaximized);
    checkWindow->setWindowModality(Qt::ApplicationModal);
    checkWindow->show();
}

void VerifyMethodManage::getVerifyMethod()
{
    programList.clear();
    programList = BaseCommonApi::getCheckProgramManage();
    QString sFilterName = ui->lineEdit_9->text();
    QString sFilterUser = ui->lineEdit_10->text();
    for(int i=programList.length()-1;i>=0;i--){
        bool bAdd=true;
        if(sFilterName!=""&&!programList[i].check_name.contains(sFilterName)){
            bAdd=false;
        }
        if(sFilterUser!=""&&!programList[i].create_name.contains(sFilterUser)){
            bAdd=false;
        }
        if(!bAdd){
            programList.removeAt(i);
        }
    }
    page=20;
    pageIndex=-1;
    PagedTableWidget *pagedTable = new PagedTableWidget(this,  ui->HBoxPageLayout);
    pagedTable->setData(programList.count(), page); // 每页10行
    connect(pagedTable, SIGNAL(pageChanged(int)), this,SLOT(onpageChanged(int)));
    this->onpageChanged(0);
}

void VerifyMethodManage::onpageChanged(int currPage){
    if(pageIndex==currPage)
        return;
    qDebug() << currPage;
    pageIndex =currPage;
    // 计算当前页的开始和结束索引
    int start = pageIndex * page;
    int end = std::min(start + page, programList.size());
    ui->tableWidget_2->clearContents();
    ui->tableWidget_2->setRowCount(0);
    int rowCount = 0;
    int row=0;
    for (int i=start;i<end; i++) {
        CheckProgramManage& program =programList[i];
        rowCount = ui->tableWidget_2->rowCount();
        ui->tableWidget_2->insertRow(rowCount);
        updateRow(row,program);
        row++;
    }
}

void VerifyMethodManage::updateTable(bool insert,CheckProgramManage program)
{
    if(insert){
        //int rowCount = ui->tableWidget_2->rowCount();
        //ui->tableWidget_2->insertRow(rowCount);
        /*VerificationProgramInfo info;
        info.id = program.id;
        info.program_name = program.program_name;
        info.create_name = program.create_name;
        info.create_time = program.create_time;
        info.remark = program.remark;*/
        //updateRow(rowCount,program);
        //programList.append(program);
        //pagedTable->setData(programList.count(), page);
        getVerifyMethod();
    }else{
        programList[editIdx] =program;
        updateRow(editIdx,program);
    }
}

void VerifyMethodManage::updateRow(int row,CheckProgramManage program)
{
    ui->tableWidget_2->setItem(row, 0, new QTableWidgetItem(QString::number(row+1)));

    //ui->tableWidget->setItem(row, 0, new QTableWidgetItem(dept.id));
    ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(program.check_name)); //设置数据
    ui->tableWidget_2->setItem(row, 2, new QTableWidgetItem(program.create_name)); //设置数据
    ui->tableWidget_2->setItem(row, 3, new QTableWidgetItem(program.create_time.toString("yyyy-MM-dd"))); //设置数据
    ui->tableWidget_2->setItem(row, 4, new QTableWidgetItem(program.remark)); //设置数据

    QPushButton *btn_1 = new QPushButton();
    btn_1->setText(tr("编辑"));
    btn_1->setStyleSheet("QPushButton{"
                         "background-color:rgba(255,255,255,0);"
                         "color:rgba(92,170,54,100);"
                         "text-decoration:underline;"
                         "}");
    btn_1->setCursor(Qt::PointingHandCursor);
    connect(btn_1,SIGNAL(clicked()),this,SLOT(on_editButton_clicked()));

    btn_1->setIconSize(QSize(16,16));
    btn_1->setIcon(QIcon(":/image/Index/u2324.svg"));
    QPushButton *btn_2 = new QPushButton();
    btn_2->setText(tr("删除"));
    btn_2->setStyleSheet("QPushButton{"
                         "background-color:rgba(255,255,255,0);"
                         "color:rgba(170,17,17,100);"
                         "text-decoration:underline;"
                         "}");
    btn_2->setCursor(Qt::PointingHandCursor);
    connect(btn_2,SIGNAL(clicked()),this,SLOT(on_deleteButton_clicked()));
    btn_2->setIconSize(QSize(16,16));
    btn_2->setIcon(QIcon(":/image/Index/u2325.svg"));
    QWidget *tmp_widget = new QWidget();
    QHBoxLayout *tmp_layout = new QHBoxLayout(tmp_widget);
    tmp_layout->addWidget(btn_1);
    tmp_layout->addWidget(btn_2);
    tmp_layout->setMargin(0);
    ui->tableWidget_2->setCellWidget(row,5,tmp_widget);
}

void VerifyMethodManage::on_editButton_clicked()
{
    QPushButton *pushButton_ = dynamic_cast<QPushButton*>(this->sender());
    if(NULL == pushButton_)
    {
        return;
    }
    // 获取按钮的x坐标和y坐标
    int x = pushButton_->parentWidget()->frameGeometry().x();
    int y = pushButton_->parentWidget()->frameGeometry().y();
    // 根据按钮的x和y坐标来定位对应的单元格
    QModelIndex index = ui->tableWidget_2->indexAt(QPoint(x, y));
    // 获取该按钮所在表格的行号和列号
    editIdx = index.row();
    //int column = index.column();
    QString sId =  programList[editIdx].id;
    NewVerifyMethod* checkWindow = new NewVerifyMethod(nullptr,sId);
    checkWindow->verifyManageWnd=this;
    checkWindow->setWindowState(Qt::WindowMaximized);
    checkWindow->setWindowModality(Qt::ApplicationModal);
    checkWindow->show();
}

void VerifyMethodManage::on_deleteButton_clicked()
{
    QPushButton *pushButton_ = dynamic_cast<QPushButton*>(this->sender());
    if(NULL == pushButton_)
    {
        return;
    }
    // 获取按钮的x坐标和y坐标
    int x = pushButton_->parentWidget()->frameGeometry().x();
    int y = pushButton_->parentWidget()->frameGeometry().y();
    // 根据按钮的x和y坐标来定位对应的单元格
    QModelIndex index = ui->tableWidget_2->indexAt(QPoint(x, y));
    // 获取该按钮所在表格的行号和列号
    int row = index.row();
    //int column = index.column();
    if(QMessageBox::question(this, "确认", "确认删除吗？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes)==QMessageBox::Yes)
    {
        if(BaseCommonApi::DeleteCheckProgramManage(programList[row].id))
        {
            //programList.removeAt(row);
            //ui->tableWidget_2->removeRow(row);
            getVerifyMethod();
        }
    }
}

void VerifyMethodManage::on_pushButton_10_clicked()
{
    getVerifyMethod();
}

void VerifyMethodManage::on_pushButton_11_clicked()
{
    ui->lineEdit_9->setText("");
    ui->lineEdit_10->setText("");
    getVerifyMethod();
}
