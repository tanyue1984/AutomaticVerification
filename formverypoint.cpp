#include "formverypoint.h"
#include "ui_formverypoint.h"
#include "basecommonapi.h"

FormVeryPoint::FormVeryPoint(QWidget *parent,QString sId) :
    QWidget(parent),
    checkId(sId),
    ui(new Ui::FormVeryPoint)
{
    ui->setupUi(this);
    if(checkId!="")
    {
        updateTable(checkId);
    }
}

FormVeryPoint::~FormVeryPoint()
{
    delete ui;
}

void FormVeryPoint::updateTable(QString id)
{
    //获取核查点tablewight的数据
     QStringList headlist;
     //QString DeviceName = BaseCommonApi::getApparatus(id);
     //QList<CheckDCPower> bizDeviceManageData=BaseCommonApi::getCheckDCPower(id,&headlist);
//     QList<QStringList> ptList = BaseCommonApi::Search_VerifyTable(id,&headlist);
     //QList<CheckDCPower> bizDeviceManageData = BaseCommonApi::getCheckDCPower(id);
     //设置tablewight
     ui->tableWidget->setRowCount(0);
     ui->tableWidget->setColumnCount(headlist.count());
     ui->tableWidget->setHorizontalHeaderLabels(headlist);
     ui->tableWidget->verticalHeader()->setParent(nullptr); //隐藏行头
     for(int i=0;i<headlist.count();i++)
     {
         ui->tableWidget->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);
     }
     //循环数据
     int rowCount=0;
     int row=0;

//     for (const QStringList& item : ptList)
//     {
//          rowCount = ui->tableWidget->rowCount();
//          ui->tableWidget->insertRow(rowCount);
//          /*ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(row+1)));
//          ui->tableWidget->setItem(row, 1, new QTableWidgetItem(checkDCPower.check_type)); //核查类型
//          ui->tableWidget->setItem(row, 2, new QTableWidgetItem(checkDCPower.params));    //参数
//          ui->tableWidget->setItem(row, 3, new QTableWidgetItem(checkDCPower.unit));    //单位
//          ui->tableWidget->setItem(row, 4, new QTableWidgetItem(checkDCPower.datum_point));//基准点
//          ui->tableWidget->setItem(row, 5, new QTableWidgetItem(checkDCPower.resolution));//分辨力
//          ui->tableWidget->setItem(row, 6, new QTableWidgetItem(checkDCPower.cycle_number));//循环次数
//          ui->tableWidget->setItem(row, 7, new QTableWidgetItem(checkDCPower.urel));//urel
//          ui->tableWidget->setItem(row, 8, new QTableWidgetItem(checkDCPower.resistance_value));//电阻值
//          ui->tableWidget->setItem(row, 9, new QTableWidgetItem(checkDCPower.resistance_value_unit));//电阻值单位*/
//          //ui->tableWidget->setItem(row, 0, new QTableWidgetItem(dept.id));

//                //QDebug
//          //循环读取值  headlist，count是列的数量
//          for(int i=0;i<headlist.count();i++)
//          {
//             if(i<item.length())
//                ui->tableWidget->setItem(row, i, new QTableWidgetItem(item[i]));
//          }
//         row++;

//     }
}
void FormVeryPoint::on_pushButton_14_clicked()
{
    this->close();
}
