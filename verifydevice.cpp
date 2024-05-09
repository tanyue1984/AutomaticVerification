#include "verifydevice.h"
#include "ui_verifydevice.h"
#include "basecommonapi.h"
#include <QCheckBox>
#include <QMessageBox>

VerifyDevice::VerifyDevice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VerifyDevice)
{
    ui->setupUi(this);
    ui->lineEdit->setPlaceholderText("设备名称");
    ui->tableWidget->setColumnWidth(0, 80);
    for(int i=1;i<5;i++)
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);
//    ui->tableWidget_2->setColumnWidth(0, 80);
//    for(int i=1;i<5;i++)
//        ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);
    getVerifyDevice();
    getCheckDevice();
}

VerifyDevice::~VerifyDevice()
{
    testedDeviceList.clear();
    bizDeviceList.clear();
    delete ui;
}

void VerifyDevice::getVerifyDevice()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    int rowCount=0;
    int row=0;
    bizDeviceList.clear();
    bizDeviceList = BaseCommonApi::getCheckDeviceList();
    QString sFilterName = ui->lineEdit->text();
    QString sFilterModel = ui->lineEdit_2->text();
    QString sFilterNo = ui->lineEdit_3->text();
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
    }
    for (const CheckAndTestedDevice& bizDevice : bizDeviceList) {
        rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(rowCount);
        QTableWidgetItem* p_check = new QTableWidgetItem();
        p_check->setCheckState(Qt::Unchecked);        //设置首列为 CheckBox 控件
        ui->tableWidget->setItem(row,0,p_check);
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(bizDevice.equipment_name));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(bizDevice.model));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(bizDevice.manufacture_no));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(bizDevice.manufacturer));
        row++;
    }
}

void VerifyDevice::getCheckDevice()
{
//    ui->tableWidget_2->clearContents();
//    ui->tableWidget_2->setRowCount(0);
//    int rowCount=0;
//    int row=0;
//    testedDeviceList.clear();
////    testedDeviceList = BaseCommonApi::getCheckAndTestedDeviceList();
//    QString sFilterName = ui->lineEdit_4->text();
//    QString sFilterModel = ui->lineEdit_5->text();
//    QString sFilterNo = ui->lineEdit_6->text();
//    for(int i=testedDeviceList.length()-1;i>=0;i--){
//        bool bAdd=true;
//        if(sFilterName!=""&&!testedDeviceList[i].equipment_name.contains(sFilterName)){
//            bAdd=false;
//        }
//        if(sFilterModel!=""&&!testedDeviceList[i].model.contains(sFilterModel)){
//            bAdd=false;
//        }
//        if(sFilterNo!=""&&!testedDeviceList[i].manufacture_no.contains(sFilterNo)){
//            bAdd=false;
//        }
//        if(!bAdd){
//            testedDeviceList.removeAt(i);
//        }
//    }
//    for (const CheckAndTestedDevice& bizDevice : testedDeviceList) {
//        rowCount = ui->tableWidget_2->rowCount();
//        ui->tableWidget_2->insertRow(rowCount);
//        QTableWidgetItem* p_check = new QTableWidgetItem();
//        p_check->setCheckState(Qt::Unchecked);        //设置首列为 CheckBox 控件
//        ui->tableWidget_2->setItem(row,0,p_check);
//        ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(bizDevice.equipment_name));
//        ui->tableWidget_2->setItem(row, 2, new QTableWidgetItem(bizDevice.model));
//        ui->tableWidget_2->setItem(row, 3, new QTableWidgetItem(bizDevice.manufacture_no));
//        ui->tableWidget_2->setItem(row, 4, new QTableWidgetItem(bizDevice.manufacturer));
//        row++;
//    }
}


void VerifyDevice::on_pushButton_14_clicked()
{
    QString checkIds="";
    QString bizIds = "";
    for(int i=0;i<ui->tableWidget->rowCount();i++){
        if(ui->tableWidget->item(i,0)->checkState()==Qt::Checked){
            if(bizIds=="")
                bizIds = bizDeviceList[i].id;
            else
                bizIds+=","+bizDeviceList[i].id;
        }
    }
//    for(int i=0;i<ui->tableWidget_2->rowCount();i++){
//        if(ui->tableWidget_2->item(i,0)->checkState()==Qt::Checked){
//            if(checkIds=="")
//                checkIds = testedDeviceList[i].id;
//            else
//                checkIds+=","+testedDeviceList[i].id;
//        }
//    }
//    if(checkIds==""){
//        QMessageBox::warning(this, "提示", "请选择核查件！");
//        return;
//    }
    if(bizIds==""){
        QMessageBox::warning(this, "提示", "请选择标准设备！");
        return;
    }
    emit selectStandard(bizIds);
    //newVerifyWnd->chooseDevices(bizIds,checkIds);
    this->close();
}

void VerifyDevice::on_pushButton_10_clicked()
{
    getVerifyDevice();
}

void VerifyDevice::on_pushButton_11_clicked()
{
    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
    ui->lineEdit_3->setText("");
    getVerifyDevice();
}

void VerifyDevice::on_pushButton_13_clicked()
{
    getCheckDevice();
}

void VerifyDevice::on_pushButton_12_clicked()
{
//    ui->lineEdit_4->setText("");
//    ui->lineEdit_5->setText("");
//    ui->lineEdit_6->setText("");
//    getCheckDevice();
}
