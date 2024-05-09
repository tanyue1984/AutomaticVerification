#include "formgageresult.h"
#include "ui_formgageresult.h"
#include "basecommonapi.h"
#include "SequentialMap.h"


FormGageResult::FormGageResult(QWidget *parent,QString deviceId) :
    QWidget(parent),
    sDeviceId(deviceId),
    ui(new Ui::FormGageResult)
{
    ui->setupUi(this);

    SequentialMap<QString, QStringList> headList;
    QMap<QString, QList<QStringList>> dataList = BaseCommonApi::Search_GenerateTableClass(sDeviceId,&headList);

    if(headList.count()>0){
        qDebug()<<headList.count();
        QStringList strHeadList = headList.values().first();
        ui->tableWidget->setColumnCount(strHeadList.count());
        ui->tableWidget->setHorizontalHeaderLabels(strHeadList);
           for (const auto& key : dataList.keys()) {
               const QList<QStringList>& rowDataList = dataList[key];
               int numRows = rowDataList.count();
               ui->tableWidget->setRowCount( numRows); //ui->tableWidget->rowCount() +
               // 填表格
               int currentRow = ui->tableWidget->rowCount() - numRows;
               for (const QStringList& rowData : rowDataList) {
                   for (int col = 0; col < rowData.size() && col < strHeadList.count(); ++col) {
                       QTableWidgetItem* item = new QTableWidgetItem(rowData[col]);
                       ui->tableWidget->setItem(currentRow, col, item);
                   }
                   ++currentRow;
               }
           }



//        for(int i=0;i<dataList.values().first().count();i++)
//        {
//         ui->tableWidget->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);
//        }

    }
}

FormGageResult::~FormGageResult()
{
    delete ui;
}
