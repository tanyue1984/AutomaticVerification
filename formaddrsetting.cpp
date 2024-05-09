#include "formaddrsetting.h"
#include "ui_formaddrsetting.h"
#include <QMessageBox>

FormAddrSetting::FormAddrSetting(QWidget *parent,int type,QString id,QString deviceId,bool check,QString equipId,QString catId) :
    QWidget(parent),
    iType(type),
    sId(id),
    sDeviceId(deviceId),
    checkMode(check),
    sEquipId(equipId),
    sCatId(catId),
    ui(new Ui::FormAddrSetting)
{
    ui->setupUi(this);
    //ui->tableWidgetMethod->setColumnCount(7);
    initCmdTable();
}
void FormAddrSetting::initCmdTable()
{
    //设置复选框
    ui->tableWidget->setStyleSheet("QHeaderView::section {"
                                "border: 1px solid rgba(197, 196, 196, 1);"
                                "border-left:none;"
                                "border-top:none;"
                                "background-color:white;"
                               "}");
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(7);
    QStringList headers;
    headers << "ID" <<""<< "序号" << "计量项目"
            << "配置命令"  << "读数命令" << "操作";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    if(header==NULL)
    {header= new CheckBoxTableHeader(Qt::Horizontal,  ui->tableWidget,1);}

    ui->tableWidget->setHorizontalHeader(header);

    ui->tableWidget->verticalHeader()->setParent(nullptr);
    for(int i=4;i<6;i++)
       ui->tableWidget->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);
    ui->tableWidget->setColumnHidden(0, true);
    ui->tableWidget->setColumnWidth(1, 80);
    ui->tableWidget->setColumnWidth(2, 80);
    ui->tableWidget->setColumnWidth(3, 100);
    cmdList = BaseCommonApi::getInstructionLib(sId);
    CheckItemNameList.clear();
    int rowCount = 0;
    int row=0;
    QString State;
    for (const InstructionLib& cmd : cmdList) {
        rowCount = ui->tableWidget->rowCount();

        ui->tableWidget->insertRow(rowCount);
        updateRow(row,cmd);
        CheckItemNameList.append(cmd.belong_vername);
        row++;

    }
    this->ui->comboBox->clear();
    this->ui->comboBox->addItem("INIT");
    this->ui->comboBox->addItem("RESET");
    this->ui->comboBox->addItem("CLOSE");
    if(checkMode)
    {
//        QStringList headlist;
//        QList<QStringList> caliList =BaseCommonApi::Search_CalibrationTable(sDeviceId,&headlist);
//        for (const QStringList& item : caliList){
//            this->ui->comboBox->addItem(item[2]);
//        }

        QList<Params> params =  BaseCommonApi::getParams(sEquipId,sCatId);
         for (const Params& item : params){

             this->ui->comboBox->addItem(item.param);
         }
    }
    else
    {
        QStringList headlist;

        //QString DeviceName = BaseCommonApi::getApparatus(id);
        //QList<CheckDCPower> bizDeviceManageData=BaseCommonApi::getCheckDCPower(sDeviceId,&headlist);
//        QList<QStringList> ptList = BaseCommonApi::Search_VerifyTable(sDeviceId,&headlist);
//        for (const QStringList& item : ptList){
//            this->ui->comboBox->addItem(item[2]);
//        }
    }

}

void FormAddrSetting::updateRow(int row,InstructionLib cmd)
{
    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(cmd.id));

    QCheckBox *checkBox = new QCheckBox();
    checkBox->setStyleSheet("QCheckBox { margin-left: 35px;"
                                        "margin-top:6px; }");
    ui->tableWidget->setCellWidget(row, 1,  checkBox);


    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(row+1)));
    /*if(cmd.type==0)
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(cmd.belong_vername)); //设置数据
    else
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(cmd.belong_checkname));*/
    if(checkMode)
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(cmd.belong_vername));
    else
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(cmd.belong_checkname));
    ui->tableWidget->setItem(row, 4, new QTableWidgetItem(cmd.instruct_config)); //设置数据
    ui->tableWidget->setItem(row, 5, new QTableWidgetItem(cmd.instruct_read)); //设置数据
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
    /*QPushButton *btn_2 = new QPushButton();
    btn_2->setText(tr("删除"));
    btn_2->setStyleSheet("QPushButton{"
                         "background-color:rgba(255,255,255,0);"
                         "color:rgba(170,17,17,100);"
                         "text-decoration:underline;"
                         "}");
    btn_2->setCursor(Qt::PointingHandCursor);
    connect(btn_2,SIGNAL(clicked()),this,SLOT(on_deleteButton_clicked()));
    btn_2->setIconSize(QSize(16,16));
    btn_2->setIcon(QIcon(":/image/Index/u2325.svg"));*/
    QWidget *tmp_widget = new QWidget();
    QHBoxLayout *tmp_layout = new QHBoxLayout(tmp_widget);
    tmp_layout->addWidget(btn_1);
    //tmp_layout->addWidget(btn_2);
    tmp_layout->setMargin(0);
    ui->tableWidget->setCellWidget(row,6,tmp_widget);
}

FormAddrSetting::~FormAddrSetting()
{
    cmdList.clear();
    delete ui;
}

void FormAddrSetting::on_pushButton_15_clicked()
{
    editIdx=-1;
    if(this->ui->comboBox->count()>0)
        this->ui->comboBox->setCurrentIndex(0);
    this->ui->textEdit->setText("");
    this->ui->textEdit_2->setText("");
    this->ui->stackedWidget->setCurrentIndex(1);
}

void FormAddrSetting::on_pushButton_14_clicked()
{
    this->close();
}

void FormAddrSetting::on_pushButton_18_clicked()
{
    if(editIdx==-1){
         //判断添加的检定项目 是否已经添加过
        if(CheckItemNameList.contains(ui->comboBox->currentText())){
             QMessageBox::information(this, "提示", "已包含此项目！");
            return;
        }


        InstructionLib cmd;
        BaseCommonApi baseApi;
        qint64 id = baseApi.generateId();
        cmd.id = QString::number(id);
        cmd.type =QString::number(iType);
        QString sName = "";
        if(iType==0){
            if(checkMode){
                cmd.program_id = sId;
                cmd.device_id = sId;
            }else{
                QList<TestedCatDevice> result = BaseCommonApi::getTestedCatDevice(sId);
                if(result.length()>0){
                    sName = result[0].equipment_name;
                }
                cmd.program_id = sId;
                cmd.device_id = sId;
                cmd.device_name = sName;
            }
        }else{
            QList<CatDeviceInfor> result = BaseCommonApi::getCatDeviceInfor(sId);
            if(result.length()>0){
                sName = result[0].equipment_name;
            }
            cmd.program_id = sId;
            cmd.device_id = sId;
            cmd.device_name = sName;
        }
        if(checkMode)
            cmd.belong_vername = this->ui->comboBox->currentText();
        else
            cmd.belong_checkname = this->ui->comboBox->currentText();
        cmd.instruct_config = this->ui->textEdit->toPlainText();
        cmd.instruct_read = this->ui->textEdit_2->toPlainText();
        cmd.create_time = QDateTime::currentDateTime();
        cmd.update_time = QDateTime::currentDateTime();
        BaseCommonApi::InsertInstructionLib(cmd);
        cmdList.append(cmd);
        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(rowCount);
        updateRow(rowCount,cmd);
        CheckItemNameList.append(cmd.belong_vername);
    }else{
        if(checkMode)
            cmdList[editIdx].belong_vername = this->ui->comboBox->currentText();
        else
            cmdList[editIdx].belong_checkname = this->ui->comboBox->currentText();
        cmdList[editIdx].instruct_config = this->ui->textEdit->toPlainText();
        cmdList[editIdx].instruct_read = this->ui->textEdit_2->toPlainText();
        cmdList[editIdx].program_id = sId;
        cmdList[editIdx].update_time = QDateTime::currentDateTime();
        InstructionLib cmd = cmdList[editIdx];
        BaseCommonApi::UpdateInstructionLib(cmd);
        updateRow(editIdx,cmd);
    }
    this->ui->stackedWidget->setCurrentIndex(0);
}

void FormAddrSetting::on_editButton_clicked()
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
    QModelIndex index = ui->tableWidget->indexAt(QPoint(x, y));
    // 获取该按钮所在表格的行号和列号
    int row = index.row();
    //int column = index.column();
    //QString sId =  cmdList[row].id;
    editIdx = row;
    InstructionLib cmd = cmdList[row];

    if(checkMode)
        this->ui->comboBox->setCurrentText(cmd.belong_vername);
    else
        this->ui->comboBox->setCurrentText(cmd.belong_checkname);
    this->ui->textEdit->setText(cmd.instruct_config);
    this->ui->textEdit_2->setText(cmd.instruct_read);
    this->ui->stackedWidget->setCurrentIndex(1);
}

void FormAddrSetting::on_deleteButton_clicked()
{
    /*
    QPushButton *pushButton_ = dynamic_cast<QPushButton*>(this->sender());
    if(NULL == pushButton_)
    {
        return;
    }
    // 获取按钮的x坐标和y坐标
    int x = pushButton_->parentWidget()->frameGeometry().x();
    int y = pushButton_->parentWidget()->frameGeometry().y();
    // 根据按钮的x和y坐标来定位对应的单元格
    QModelIndex index = ui->tableWidget->indexAt(QPoint(x, y));
    // 获取该按钮所在表格的行号和列号
    int row = index.row();
    //int column = index.column();
    if(QMessageBox::question(this, "确认", "确认删除吗？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes)==QMessageBox::Yes)
    {
        if(BaseCommonApi::DeleteInstructionLib(cmdList[row].id))
        {
            cmdList.removeAt(row);
            ui->tableWidget->removeRow(row);
        }
    }
    */
    QList<QString> idList;
    bool IsSucc=false;
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        // 获取第1列的 QCheckBox
        QCheckBox *checkBox = qobject_cast<QCheckBox *>(ui->tableWidget->cellWidget(row, 1));
        if (checkBox && checkBox->isChecked()) {
            // 如果 checkbox 被选中，获取第0列的 ID
            QTableWidgetItem *idItem = ui->tableWidget->item(row, 0);
            if (idItem) {
                IsSucc=BaseCommonApi::DeleteInstructionLib(idItem->text());
            }
        }
    }
    //this->initCmdTable();
}

void FormAddrSetting::on_pushButton_3_clicked()
{
    if(QMessageBox::question(this, "确认", "确认删除吗？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes)==QMessageBox::Yes)
    {/*

        if(BaseCommonApi::DeleteInstructionLib(sId))
        {
            cmdList.clear();
            ui->tableWidget->clearContents();
            ui->tableWidget->setRowCount(0);
        }*/
        QList<QString> idList;
        bool IsSucc=false;
        for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
            // 获取第1列的 QCheckBox
            QCheckBox *checkBox = qobject_cast<QCheckBox *>(ui->tableWidget->cellWidget(row, 1));
            if (checkBox && checkBox->isChecked()) {
                // 如果 checkbox 被选中，获取第0列的 ID
                QTableWidgetItem *idItem = ui->tableWidget->item(row, 0);
                if (idItem) {
                      IsSucc=BaseCommonApi::DeleteInstructionLib(idItem->text());
                }
            }
        }
    }
    this->initCmdTable();
}

void FormAddrSetting::on_pushButton_2_clicked()
{
    // 获取应用程序的根目录
    QString path =  QFileDialog::getOpenFileName(this, tr("通信命令"), "", tr("Text Files (*.txt)"));;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // 错误处理
        qDebug() << "无法打开文件：" << file.errorString();
        QString str=file.errorString();
        return;
    }

    for (const InstructionLib& cmd : cmdList) {
        BaseCommonApi::DeleteInstructionLib(cmd.id);
    }
    cmdList.clear();
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString line;
    while (!in.atEnd()) {
        line = in.readLine();
        // 使用'|'分割键和值
        QStringList list = line.split(QRegExp("\\s*\\|\\s*"), QString::KeepEmptyParts);
        if(list.length()==5){
            InstructionLib cmd;
            QString sName = "";
            if(iType==0){
                if(checkMode){
                    cmd.program_id = sId;
                    cmd.device_id = sId;
                }else{
                    QList<TestedCatDevice> result = BaseCommonApi::getTestedCatDevice(sId);
                    if(result.length()>0){
                        sName = result[0].equipment_name;
                    }
                    cmd.program_id = sId;
                    cmd.device_id = sId;
                    cmd.device_name = sName;
                }
            }else{
                QList<CatDeviceInfor> result = BaseCommonApi::getCatDeviceInfor(sId);
                if(result.length()>0){
                    sName = result[0].equipment_name;
                }
                cmd.program_id = sId;
                cmd.device_id = sId;
                cmd.device_name = sName;
            }
            BaseCommonApi baseApi;
            qint64 id = baseApi.generateId();
            cmd.id = QString::number(id);
            cmd.type = list[0];
            cmd.belong_vername=list[1];
            cmd.belong_checkname = list[2];
            cmd.instruct_config=list[3];
            cmd.instruct_read=list[4];
            cmd.create_time = QDateTime::currentDateTime();
            cmd.update_time = QDateTime::currentDateTime();
            BaseCommonApi::InsertInstructionLib(cmd);
        }
    }
    file.close();
    initCmdTable();
}

void FormAddrSetting::on_pushButton_clicked()
{

}

void FormAddrSetting::on_pushButton_4_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(0);
}

void FormAddrSetting::on_ExportCmdInfo_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("通信命令"), "", tr("Text Files (*.txt)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if(file.open(QIODevice::WriteOnly| QIODevice::Text)){
            QTextStream out(&file);
            out.setCodec("UTF-8");
            for (const InstructionLib& cmd : cmdList) {
                QString sLine = QString("%1|%2|%3|%4|%5").arg(cmd.type).arg(cmd.belong_vername).arg(cmd.belong_checkname).arg(cmd.instruct_config).arg(cmd.instruct_read);
                out << sLine << "\n";
            }
            file.close();
        }
    }
}
