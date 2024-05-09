#include "checkdevice.h"
#include "ui_checkdevice.h"
#include "basecommonapi.h"
#include <QCheckBox>
#include <QMessageBox>
#include "PageControl.h"

CheckDevice::CheckDevice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CheckDevice)
{
    ui->setupUi(this);
    /*ui->tableWidget_3->setColumnWidth(0, 80);
    for(int i=1;i<4;i++)
        ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);*/
    ui->tableWidget_4->setColumnWidth(0, 80);
    for(int i=1;i<5;i++)
        ui->tableWidget_4->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);
    //getCheckDevice();
    getStandardDevice();

}

CheckDevice::~CheckDevice()
{
    testedDeviceList.clear();
    bizDeviceList.clear();
    delete ui;
}

void CheckDevice::getCheckDevice()
{
    /*testedDeviceList.clear();
    ui->tableWidget_3->clearContents();
    ui->tableWidget_3->setRowCount(0);
    int rowCount=0;
    int row=0;
    testedDeviceList = BaseCommonApi::getTestedDevice();
    QString sFilterName = ui->lineEdit_7->text();
    QString sFilterModel = ui->lineEdit_8->text();
    for(int i=testedDeviceList.length()-1;i>=0;i--){
        bool bAdd=true;
        if(sFilterName!=""&&!testedDeviceList[i].equipment_name.contains(sFilterName)){
            bAdd=false;
        }
        if(sFilterModel!=""&&!testedDeviceList[i].model.contains(sFilterModel)){
            bAdd=false;
        }
        if(!bAdd){
            testedDeviceList.removeAt(i);
        }
    }
    for (const TestedDevice& bizDevice : testedDeviceList) {
        rowCount = ui->tableWidget_3->rowCount();
        ui->tableWidget_3->insertRow(rowCount);
        //QCheckBox *checkBox = new QCheckBox;
        //ui->tableWidget_3->setCellWidget(row, 0, checkBox);
        QTableWidgetItem* p_check = new QTableWidgetItem();
        p_check->setCheckState(Qt::Unchecked);        //设置首列为 CheckBox 控件
        ui->tableWidget_3->setItem(row,0,p_check);
        ui->tableWidget_3->setItem(row, 1, new QTableWidgetItem(bizDevice.equipment_name));
        ui->tableWidget_3->setItem(row, 2, new QTableWidgetItem(bizDevice.model));
        ui->tableWidget_3->setItem(row, 3, new QTableWidgetItem(bizDevice.help_instruction));
        row++;
    }*/
}

void CheckDevice::getStandardDevice()
{
    bizDeviceList.clear();
    CheckState.clear();
    bizDeviceList = BaseCommonApi::getBizDeviceManage();
    QString sFilterName = ui->lineEdit_10->text();
    QString sFilterModel = ui->lineEdit_11->text();
    QString sFilterNo = ui->lineEdit_12->text();
    for(int i=bizDeviceList.length()-1;i>=0;i--){
        bool bAdd=true;
        if(sFilterName!=""&&!bizDeviceList[i].equipment_name.contains(sFilterName)){
            bAdd=false;
        }
        if(sFilterModel!=""&&!bizDeviceList[i].model.contains(sFilterModel)){
            bAdd=false;
        }
        if(sFilterNo!=""&&!bizDeviceList[i].manufacture_no.contains(sFilterNo)){
            bAdd=false;
        }
        if(!bAdd){
            bizDeviceList.removeAt(i);
        }
        CheckState.insert(i,false);
    }
    page=20;
    pageIndex=-1;
    PagedTableWidget *pagedTable = new PagedTableWidget(this,  ui->HBoxPageLayout);
    pagedTable->setData(bizDeviceList.count(), page); // 每页10行
    connect(pagedTable, SIGNAL(pageChanged(int)), this,SLOT(onpageChanged(int)));
    this->onpageChanged(0);
}

void CheckDevice::onpageChanged(int currPage)
{
    if(pageIndex==currPage)
        return;
    qDebug() << currPage;
    pageIndex =currPage;
    // 计算当前页的开始和结束索引
    int start = pageIndex * page;
    int end = std::min(start + page, bizDeviceList.size());
    ui->tableWidget_4->clearContents();
    ui->tableWidget_4->setRowCount(0);
    int rowCount=0;
    int row=0;
    for (int i=start;i<end; i++) {
        const BizDevice& bizDevice = bizDeviceList[i];
        rowCount = ui->tableWidget_4->rowCount();
        ui->tableWidget_4->insertRow(rowCount);
        //QCheckBox *checkBox = new QCheckBox;
        //ui->tableWidget_4->setCellWidget(row, 0, checkBox);
        //checkBox->setChecked(CheckState.value(i));
        QTableWidgetItem* p_check = new QTableWidgetItem();
        p_check->setCheckState(Qt::Unchecked);
        ui->tableWidget_4->setItem(row,0,p_check);
        //connect(checkBox, &QCheckBox::stateChanged, this, [this,i,checkBox]() {
            // 获取当前行第一列的 ID

          //   OnitemChanged(i,checkBox->checkState());


        //});

        ui->tableWidget_4->setItem(row, 1, new QTableWidgetItem(bizDevice.equipment_name));
        ui->tableWidget_4->setItem(row, 2, new QTableWidgetItem(bizDevice.model));
        ui->tableWidget_4->setItem(row, 3, new QTableWidgetItem(bizDevice.manufacture_no));
        ui->tableWidget_4->setItem(row, 4, new QTableWidgetItem(bizDevice.manufacturer));
        row++;


    }
}

void CheckDevice::on_pushButton_19_clicked()
{
    QString checkIds="";
    QString bizIds = "";
    /*for(int i=0;i<ui->tableWidget_3->rowCount();i++){
        if(ui->tableWidget_3->item(i,0)->checkState()==Qt::Checked){
            if(checkIds=="")
                checkIds = testedDeviceList[i].id;
            else
                checkIds+=","+testedDeviceList[i].id;
        }
    }*/
    int start = pageIndex * page;
    for(int i=0;i<ui->tableWidget_4->rowCount();i++){
        if(ui->tableWidget_4->item(i,0)->checkState()==Qt::Checked){
            if(bizIds=="")
                bizIds = bizDeviceList[i+start].id;
            else
                bizIds+=","+bizDeviceList[i+start].id;
        }
    }
    /*if(checkIds==""){
        QMessageBox::warning(this, "提示", "请选择被检设备！");
        return;
    }*/
    if(bizIds==""){
        QMessageBox::warning(this, "提示", "请选择标准设备！");
        return;
    }
    //newCheckWnd->chooseDevices(checkIds,bizIds);
    this->close();
}

void CheckDevice::on_pushButton_15_clicked()
{
    //getCheckDevice();
}

void CheckDevice::on_pushButton_16_clicked()
{
    //ui->lineEdit_7->setText("");
    //ui->lineEdit_8->setText("");
    //getCheckDevice();
}

void CheckDevice::on_pushButton_17_clicked()
{
    getStandardDevice();
}

void CheckDevice::on_pushButton_18_clicked()
{
    ui->lineEdit_10->setText("");
    ui->lineEdit_11->setText("");
    ui->lineEdit_12->setText("");
    getStandardDevice();
}
void CheckDevice::OnitemChanged(int index,Qt::CheckState CurrState)
{
    if(CurrState==Qt::Unchecked){
       CheckState[index] = false;
    }else{
        CheckState[index] = true;
    }

}
