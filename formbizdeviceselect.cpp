#include "formbizdeviceselect.h"
#include "PageControl.h"
#include "global.h"
#include "ui_formbizdeviceselect.h"
#include <QMessageBox>

FormBizDeviceSelect::FormBizDeviceSelect(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FormBizDeviceSelect)
{
    ui->setupUi(this);
    for (int i = 0; i < 8; i++) {
        ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
    }
    for (int i = 0; i < 7; i++) {
        ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
    }
    ui->tableWidget_2->hideColumn(1);
    ui->comboBox_3->addItem("请选择标准装置名称");
    ui->comboBox_3->addItems(equipNameList);
    getStandardInfo();
}

void FormBizDeviceSelect::getStandardInfo()
{
    stdInfo.clear();
    stdInfo = BaseCommonApi::getStandardInfo();

    if (ui->comboBox_3->currentIndex() != 0) {
        for (int i = stdInfo.length() - 1; i >= 0; i--) {
            if (stdInfo.at(i).standard_name != ui->comboBox_3->currentText())
                stdInfo.removeAt(i);
        }
    }
    pageIndex = -1;
    PagedTableWidget* pagedTable = new PagedTableWidget(this, ui->HBoxPageLayout);
    pagedTable->setData(stdInfo.count(), page); // 每页10行
    connect(pagedTable, SIGNAL(pageChanged(int)), this, SLOT(onpageChanged(int)));
    this->onpageChanged(0);
    //    ui->comboBox_3->clear();
    //    for(StandardInfo item : stdInfo){
    //        ui->comboBox_3->addItem(item.standard_name);
    //    }
}

void FormBizDeviceSelect::getBizDeviceInfo()
{
    if (stdIdx >= 0 && stdIdx < stdInfo.count()) {
        bizInfo.clear();
        QString id = stdInfo.at(stdIdx).id;
        loginNowVerifyDeviceId = id;
        bizInfo = BaseCommonApi::getStandardInfoListByStandardId(id);
        QString sFilterName = ui->lineEdit_7->text();
        QString sFilterModal = ui->lineEdit_8->text();
        QString sFilterNo = ui->lineEdit_9->text();
        for (int i = bizInfo.length() - 1; i >= 0; i--) {
            bool bAdd = true;
            if (sFilterName != "" && !bizInfo[i].equipment_name.contains(sFilterName)) {
                bAdd = false;
            }
            if (sFilterModal != "" && !bizInfo[i].model.contains(sFilterModal)) {
                bAdd = false;
            }
            if (sFilterNo != "" && !bizInfo[i].manufacture_no.contains(sFilterNo)) {
                bAdd = false;
            }
            if (!bAdd) {
                bizInfo.removeAt(i);
            }
        }
        ui->tableWidget_3->clearContents();
        ui->tableWidget_3->setRowCount(0);
        int rowCount = 0;
        int row = 0;
        for (BizDevice info : bizInfo) {
            rowCount = ui->tableWidget_3->rowCount();
            ui->tableWidget_3->insertRow(rowCount);

            QTableWidgetItem* p_check = new QTableWidgetItem();
            p_check->setCheckState(Qt::Unchecked); // 设置首列为 CheckBox 控件
            ui->tableWidget_3->setItem(row, 0, p_check);
            ui->tableWidget_3->setItem(row, 1, new QTableWidgetItem(info.equipment_name)); // 设置数据
            ui->tableWidget_3->setItem(row, 2, new QTableWidgetItem(info.model)); // 设置数据
            ui->tableWidget_3->setItem(row, 3, new QTableWidgetItem(info.manufacture_no));
            ui->tableWidget_3->setItem(row, 4, new QTableWidgetItem(info.manufacturer)); // 设置数据
            ui->tableWidget_3->setItem(row, 5, new QTableWidgetItem(info.type)); // 设置数据
            ui->tableWidget_3->setItem(row, 6, new QTableWidgetItem(info.measure_param));
            row++;
        }
    }
}

FormBizDeviceSelect::~FormBizDeviceSelect()
{
    stdInfo.clear();
    bizInfo.clear();
    delete ui;
}

void FormBizDeviceSelect::onpageChanged(int currPage)
{
    if (pageIndex == currPage)
        return;
    qDebug() << currPage;
    pageIndex = currPage;
    // 计算当前页的开始和结束索引
    int start = pageIndex * page;
    int end = std::min(start + page, stdInfo.size());
    ui->tableWidget_2->clearContents();
    ui->tableWidget_2->setRowCount(0);
    int rowCount = 0;
    int row = 0;
    for (int i = start; i < end; i++) {
        StandardInfo& info = stdInfo[i];
        rowCount = ui->tableWidget_2->rowCount();
        ui->tableWidget_2->insertRow(rowCount);

        QTableWidgetItem* p_check = new QTableWidgetItem();
        p_check->setCheckState(Qt::Unchecked); // 设置首列为 CheckBox 控件
        ui->tableWidget_2->setItem(row, 0, p_check);
        ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(info.id)); // 设置数据
        ui->tableWidget_2->setItem(row, 2, new QTableWidgetItem(info.standard_cert_no)); // 设置数据
        ui->tableWidget_2->setItem(row, 3, new QTableWidgetItem(info.standard_name)); // 设置数据
        ui->tableWidget_2->setItem(row, 4, new QTableWidgetItem(""));
        ui->tableWidget_2->setItem(row, 5, new QTableWidgetItem(info.dept_name)); // 设置数据
        ui->tableWidget_2->setItem(row, 6, new QTableWidgetItem(info.manage_status)); // 设置数据
        ui->tableWidget_2->setItem(row, 7, new QTableWidgetItem(info.measure_item));
        row++;
    }
}

void FormBizDeviceSelect::on_comboBox_3_currentIndexChanged(int index)
{
}

void FormBizDeviceSelect::on_pushButton_12_clicked()
{
    getStandardInfo();
}

void FormBizDeviceSelect::on_pushButton_13_clicked()
{
    ui->comboBox_3->setCurrentIndex(0);
    getStandardInfo();
}

void FormBizDeviceSelect::on_tableWidget_2_cellChanged(int row, int column)
{
    Qt::CheckState state = ui->tableWidget_2->item(row, 0)->checkState();
    if (state == Qt::Checked) {
        stdIdx = row + pageIndex * page;
        getBizDeviceInfo();
        for (int i = 0; i < ui->tableWidget_2->rowCount(); i++) {
            if (i != row) {
                ui->tableWidget_2->item(i, 0)->setCheckState(Qt::Unchecked);
            }
        }
    }
}

void FormBizDeviceSelect::on_pushButton_15_clicked()
{
    QString name, id;
    QString checkIds = "";
    QString bizIds = "";
    QString StanId = "";
    QString StanName = "";
    QList<QString> bizInfofo;
    for (int i = 0; i < ui->tableWidget_2->rowCount(); i++) {
        if (ui->tableWidget_2->item(i, 0)->checkState() == Qt::Checked) {
            id = ui->tableWidget_2->item(i, 1)->text();
            name = ui->tableWidget_2->item(i, 3)->text();
        }
    }
    for (int i = 0; i < ui->tableWidget_3->rowCount(); i++) {
        if (ui->tableWidget_3->item(i, 0)->checkState() == Qt::Checked) {
            if (bizIds == "") {
                bizIds = bizInfo[i].id;
                StanId = id;
                StanName = name;
            } else {
                bizIds += "," + bizInfo[i].id;
                StanId += "," + id;
                StanName += "," + name;
            }
        }
    }
    if (bizIds == "") {
        QMessageBox::warning(this, "提示", "请选择标准设备！");
        return;
    }
    bizInfofo.append(StanId);
    bizInfofo.append(StanName);
    bizInfofo.append(bizIds);

    emit selectBizDevice(bizInfofo);
    this->close();
}

void FormBizDeviceSelect::on_pushButton_clicked()
{
    this->close();
}

void FormBizDeviceSelect::on_pushButton_16_clicked()
{
    getBizDeviceInfo();
}

void FormBizDeviceSelect::on_pushButton_17_clicked()
{
    ui->lineEdit_7->setText("");
    ui->lineEdit_8->setText("");
    ui->lineEdit_9->setText("");
    getBizDeviceInfo();
}
