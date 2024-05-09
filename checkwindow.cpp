#include "checkwindow.h"
#include "ui_checkwindow.h"
#include "dialogcheckresult.h"
#include <QMessageBox>
#include "basecommonapi.h"
#include <QInputDialog>
#include "newcheckmethod.h"
#include "formcheckpoint.h"
#include "multiinputdialog.h"
#include "formgageresult.h"
#include "dialogdcstabinput.h"
#include "global.h"

CheckWindow::CheckWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CheckWindow)
{
    ui->setupUi(this);
    //ui->tableWidgetDevice->setColumnCount(8);
    //ui->tableWidgetMethod->setColumnCount(7);
    ui->tableWidgetDevice->verticalHeader()->setParent(nullptr);
    for(int i=2;i<8;i++)
        ui->tableWidgetDevice->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);
    ui->tableWidgetMethod->verticalHeader()->setParent(nullptr); //隐藏行头
    for(int i=1;i<5;i++)
        ui->tableWidgetMethod->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);
    ui->tableWidgetMethod->setColumnWidth(0, 60);
    ui->tableWidgetMethod->setColumnWidth(5, 100);
    ui->tableWidget_3->verticalHeader()->setParent(nullptr);
    for(int i=0;i<2;i++)
        ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);
    ui->tableWidget_3->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_Point->verticalHeader()->setParent(nullptr);
    ui->tableWidget_Point->setColumnWidth(0, 60);
    for(int i=1;i<3;i++)
        ui->tableWidget_Point->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);
    ui->tableWidget_Standard->verticalHeader()->setParent(nullptr);
    for(int i=0;i<2;i++)
        ui->tableWidget_Standard->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);
    ui->tableWidgetStd->verticalHeader()->setParent(nullptr); //隐藏行头
    for(int i=1;i<6;i++)
        ui->tableWidgetStd->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);
    ui->tableWidgetStd->setColumnWidth(0, 60);
    ui->tableWidgetStd->setColumnWidth(7, 120);
    ui->tableWidgetCheck->verticalHeader()->setParent(nullptr);
    ui->comboBoxName->addItem("ALL");
    ui->comboBoxModal->addItem("ALL");
    ui->comboBoxCustomer->addItem("ALL");
    ui->comboBoxName_2->addItem("ALL");
    ui->comboBoxModal_2->addItem("ALL");

    //统一定义单位转换系数
    unitTransList=readDataFromFile("unitTrans.txt");
     //统一定义命令占位符
    cmdPlaceHolderList=readDataFromFile("cmdPlaceHolder.txt");
    //单位值特殊转换
    unitTypeTransList=readDataFromFile("unitTypeTrans.txt");
    //测试结果值填充列
    resultfillingList=readDataFromFile("resultfilling.txt");


    getCheckDevice();
    //ui->stackedWidget_2->setCurrentIndex(1);
    //ui->tableWidgetDevice->verticalHeader()->sectionResizeMode(QHeaderView::Stretch);
    //this->grabKeyboard();
    setFocusPolicy(Qt::StrongFocus);
//    this->close()；
}

CheckWindow::~CheckWindow()
{
    myTaskList.clear();
    programList.clear();
    connectList.clear();
    selectTaskList.clear();
    headList.clear();
    calibrationList.clear();
    measureCalList.clear();
    caliDataListAll.clear();
    headTableListAll.clear();
  closeDevice();
VisaStanEngineList.clear();
delete ui;
}

void CheckWindow::showMethod()
{
    selectTaskList.clear();
    QString ids;
    for(int i=0;i<ui->tableWidgetDevice->rowCount();i++){
        Qt::CheckState state = ui->tableWidgetDevice->item(i,0)->checkState();
        if(state == Qt::Checked){
            selectTaskList.append(myTaskList[i]);
            measureInfo.order_id = myTaskList[i].orderId;
            qDebug()<<measureInfo.order_id;
            qDebug()<<measureInfo.sample_id;
            /*if(ids=="")
                ids = myTaskList[i].id;
            else
                ids +=";"+ myTaskList[i].id;*/
            //break;
        }
    }
    if(selectTaskList.length()==0){
        QMessageBox::warning(this, "提示", "请选择被检设备！");
    }else{
        //qDebug() << "select ids:" << ids;
        ui->stackedWidget->setCurrentIndex(1);
        getCheckMethod();
    }
}

void CheckWindow::updateStandardTable()
{


    QStringList ValueList;
    ValueList.append("请选择角色");
    for (int i=0;i<RoleList.count();i++) {
        ValueList.append(RoleList[i].role);
    }
    ui->tableWidgetStd->clearContents();
    ui->tableWidgetStd->setRowCount(0);
    ui->tableWidgetStd->hideColumn(1);
    QString sFilterName = ui->comboBoxName_2->currentText();
    QString sFilterModal = ui->comboBoxModal_2->currentText();
    QString sFilterDepart = ui->lineEdit_4->text();
    QString sFilterNo = ui->lineEditNo_2->text();

    //catDeviceList.clear();
    catIdList.clear();
    if(programInfo.standard_device_id!=""){
        catIdList = programInfo.standard_device_id.split(",");
        int row=0;
        for(int i=catIdList.length()-1;i>=0; i--)
        {
            QString id = catIdList[i];

            QString readraleName;
            QList<CatDeviceInfor> result = BaseCommonApi::getCatDeviceInfor(id);
            if(result.length()>0){
                CatDeviceInfor item = result[0];
                bool bAdd=true;
                if(sFilterName!="ALL"&&!item.equipment_name.contains(sFilterName)){
                    bAdd=false;
                }
                if(sFilterModal!="ALL"&&!item.model.contains(sFilterModal)){
                    bAdd=false;
                }
                if(sFilterDepart!=""&&!item.dept_name.contains(sFilterDepart)){
                    bAdd=false;
                }
                if(sFilterNo!=""&&!item.manufacture_no.contains(sFilterNo)){
                    bAdd=false;
                }
                if(bAdd){
                    //更新标准设备表格
                    //rowCount = ui->tableWidgetStd->rowCount();
                    ui->tableWidgetStd->insertRow(0);
                    QTableWidgetItem* p_check = new QTableWidgetItem();
                    p_check->setCheckState(Qt::Unchecked);        //设置首列为 CheckBox 控件
                    ui->tableWidgetStd->setItem(0,0,p_check);
                    if(programInfo.biz_standard_id == "" || programInfo.biz_standard_name==""){
                        ui->tableWidgetStd->setItem(0, 1, new QTableWidgetItem(""));//所属装置名称ID
                        ui->tableWidgetStd->setItem(0, 2, new QTableWidgetItem(""));//所属装置名称
                    }else {
                        ui->tableWidgetStd->setItem(0, 1, new QTableWidgetItem(programInfo.biz_standard_id.split(",")[i]));//所属装置名称ID
                        ui->tableWidgetStd->setItem(0, 2, new QTableWidgetItem(programInfo.biz_standard_name.split(",")[i]));//所属装置名称
                    }
                    ui->tableWidgetStd->setItem(0, 3, new QTableWidgetItem(item.equipment_name));
                    ui->tableWidgetStd->setItem(0, 4, new QTableWidgetItem(item.model)); //设置数据
                    ui->tableWidgetStd->setItem(0, 5, new QTableWidgetItem(item.manufacture_no));
                    ui->tableWidgetStd->setItem(0, 6, new QTableWidgetItem(item.dept_name));
                    ui->tableWidgetStd->setItem(0, 7, new QTableWidgetItem(item.director_name));
                    ui->tableWidgetStd->setItem(0, 8, new QTableWidgetItem(item.measure_valid_date));
                    if(RoleList.count()!=0){

                        QComboBox *comboBox=new QComboBox();
                        comboBox->addItems(ValueList);
                        ui->tableWidgetStd->setCellWidget(0, 9,comboBox);

                        readraleName=GetdeviceidByRole(id);
                        if(readraleName!=""){
                            comboBox->setCurrentText(readraleName);
                        }
                        // 连接下拉框的currentIndexChanged信号到槽函数
                        connect(comboBox, &QComboBox::currentTextChanged, this, [this, id,comboBox,i]() {
                            onComboBoxCurrTextChanged(comboBox->currentText(),id,i);

                        });
                    }else{ui->tableWidgetStd->setItem(0, 9,new QTableWidgetItem("标准设备"));}



                    //catDeviceList.append(item);
                }else{
                    catIdList.removeAt(i);
                }
            }
            result.clear();
            row++;
        }

    }
}
void CheckWindow::onComboBoxCurrTextChanged(QString Value,QString device_id,int row)
{
    if(RoleList.count()!=0){
        QString cateory_id=DeviceMeasureEquipmentCatorycatory[0].item_category_id;
        for (int i=0;i<RoleList.count();i++) {
            if(RoleList[i].device_id==device_id && RoleList[i].role!=Value){
                  RoleList[i].device_id="";
                  BaseCommonApi::UpdateDeviceRole(RoleList[i]);
            }
            if(RoleList[i].role==Value && RoleList[i].cateory_id==cateory_id)
            {

                 RoleList[i].device_id=device_id;
                 RoleList[i].device_type=QString::number(1);
                 BaseCommonApi::UpdateDeviceRole(RoleList[i]);

            }
        }
        for(int i=0;i<ui->tableWidgetStd->rowCount();i++){
            QComboBox *comboBox = qobject_cast<QComboBox *>(ui->tableWidgetStd->cellWidget(i, 7));
            if (comboBox) {
                QString str=comboBox->currentText();
                if(comboBox->currentText()==Value && i!=row){
                    comboBox->setCurrentIndex(0);
                }
            }
        }


    }

}
QString CheckWindow::GetdeviceidByRole(QString device_id)
{
    QString cateory_id=DeviceMeasureEquipmentCatorycatory[0].item_category_id;
    QString temprole="";
    for(int i=0;i<RoleList.count();i++)
    {
        if(cateory_id==RoleList[i].cateory_id && RoleList[i].device_id==device_id)
        {
            temprole= RoleList[i].role;
            break;
        }
    }
    return  temprole;
}
void CheckWindow::showStandard(){
    QString sId="";
    for(int i=0;i<ui->tableWidgetMethod->rowCount();i++){
        Qt::CheckState state = ui->tableWidgetMethod->item(i,0)->checkState();
        if(state == Qt::Checked){
            sId = programList[i].id;
            break;
        }
    }
    if(programList.count()==1)
    {
       sId = programList[0].id;
    }
    if(sId==""){

            QMessageBox::warning(this, "提示", "请选择检定程序！");


    }else{
        programInfo = BaseCommonApi::getVerificationProgramInfo(sId);
        //catDeviceList.clear();
        if(programInfo.standard_device_id!=""){
            QStringList idList = programInfo.standard_device_id.split(",");
            for(const QString& id : idList)
            {
                QList<CatDeviceInfor> result = BaseCommonApi::getCatDeviceInfor(id);
                if(result.length()>0){
                    SetStandardCombox(result[0]);
                    //catDeviceList.append(result[0]);
                }
                result.clear();
            }
            idList.clear();
            ui->comboBoxName_2->setCurrentIndex(0);
            ui->comboBoxModal_2->setCurrentIndex(0);
            ui->lineEditNo_2->setText("");
            ui->lineEdit_4->setText("");
            //获取检定项分类 及 检定项分类包含的角色
            DeviceMeasureEquipmentCatorycatory = BaseCommonApi::getDeviceMeasureEquipmentCatory(selectTaskList[0].sample_id);
            RoleList=BaseCommonApi::getDeviceRoleBydeviceId(DeviceMeasureEquipmentCatorycatory[0].item_category_id);
            updateStandardTable();
        }
        //查询角色表信息 getDeviceRoleBydeviceId
        //catory[0].item_category_id 检定项目ID
        ui->stackedWidget->setCurrentIndex(2);
    }
}

void CheckWindow::showDevice()
{
    //QStringList idList = programInfo.standard_device_id.split(",");
    QString sId="";
    StandarIdList.clear();
    /*for(int i=0;i<ui->tableWidgetStd->rowCount();i++){
        Qt::CheckState state = ui->tableWidgetStd->item(i,0)->checkState();
        if(state == Qt::Checked){
            sId = catIdList[i];
            break;
        }
    }*/

    if(catIdList.count()==1)
    {
       StandarIdList.append(catIdList[0]);
       StanDeviceIDList.append(ui->tableWidgetStd->item(0,1)->text());
      StanDeviceNameList.append(ui->tableWidgetStd->item(0,2)->text());
    }else {
        for(int i=0;i<ui->tableWidgetStd->rowCount();i++){
                Qt::CheckState state = ui->tableWidgetStd->item(i,0)->checkState();
                if(state == Qt::Checked){
                    StandarIdList.append(catIdList[i]);
                    StanDeviceIDList.append(ui->tableWidgetStd->item(i,1)->text());
                    StanDeviceNameList.append(ui->tableWidgetStd->item(i,2)->text());
                }
            }
    }
    if(StandarIdList.count()==0){

       QMessageBox::warning(this, "提示", "请选择标准设备！");

    }else{
        //programInfo = BaseCommonApi::getVerificationProgramInfo(sId);
        initDeviceInfo(StandarIdList);
        ui->stackedWidget->setCurrentIndex(3);
    }
}

void CheckWindow::initDeviceInfo(QList<QString> sId)
{
    StandarIdList = sId;
    QString strStyle = QString("QLabel{image:url(%1);}").arg(programInfo.img_path);
    qDebug() << "strStyle:" << strStyle;
    ui->label_8->setStyleSheet(strStyle);


    ui->tableWidget_3->clearContents();
    ui->tableWidget_3->setRowCount(0);
    ui->tableWidget_Standard->clearContents();
    ui->tableWidget_Standard->setRowCount(0);
    QStringList idList;
    int rowCount=0;
    int row=0;
    connectList.clear();
    if(selectTaskList.length()>0)
    {
        QList<DeviceMeasureEquipmentCatory> catory = DeviceMeasureEquipmentCatorycatory;
        if(catory.length() > 0 ){

            ui->lineEdit_category->setText(catory[0].item_category_name);
        }
        //idList = programInfo.tested_device_id.split(",");
        for(const MyTask& task : selectTaskList)
        {
            ConnectInfo info;
            info.id = task.sample_id;
            info.type = 0;
            info.state="未连接";
            info.deviceName = task.sampleName;
            QList<VerificationProgramConfigInterface> addrList = BaseCommonApi::getVerificationProgramConfigInterface(info.id);
            if(addrList.length()>0){
                info.programInterface = addrList[0];
            }
            info.cmdList = BaseCommonApi::getInstructionLib(programInfo.id);
            info.rolenName="被检设备";
            connectList.append(info);
            addrList.clear();
        }
        idList.clear();
    }

    if(StandarIdList.count()!=0){
        idList =StandarIdList;

        for(const QString& id : idList)
        {
            //if(id!=CheckId)
              //  continue;
            ConnectInfo info;
            info.id = id;
            info.type = 1;
            info.state="未连接";
            if(idList.count()==1)
                info.rolenName="标准设备";
            else
                info.rolenName=GetdeviceidByRole(id);
            QList<CatDeviceInfor> result = BaseCommonApi::getCatDeviceInfor(id);
            if(result.length()>0){
                info.deviceName = result[0].equipment_name;

                //更新标准设备表格
                rowCount = ui->tableWidget_Standard->rowCount();
                ui->tableWidget_Standard->insertRow(rowCount);
                ui->tableWidget_Standard->setItem(row, 0, new QTableWidgetItem(info.rolenName));
                ui->tableWidget_Standard->setItem(row, 1, new QTableWidgetItem(info.deviceName));
                ui->tableWidget_Standard->setItem(row, 2, new QTableWidgetItem("OFF")); //设置数据
                row++;
            }
            result.clear();
            QList<VerificationProgramConfigInterface> addrList = BaseCommonApi::getVerificationProgramConfigInterface(id);
            if(addrList.length()>0){
                info.programInterface = addrList[0];
            }
            info.cmdList = BaseCommonApi::getInstructionLib(id);
            connectList.append(info);
            addrList.clear();
        }

        idList.clear();
    }
    rowCount=0;
    row=0;
    for(const ConnectInfo& info : connectList){
        rowCount = ui->tableWidget_3->rowCount();
        ui->tableWidget_3->insertRow(rowCount);
        ui->tableWidget_3->setItem(row, 0, new QTableWidgetItem(info.rolenName));
        ui->tableWidget_3->setItem(row, 1, new QTableWidgetItem(info.deviceName));
        ui->tableWidget_3->setItem(row, 2, new QTableWidgetItem(info.state)); //设置数据
        row++;
    }
    //搜索仪器地址放入下拉框
    ui->comboBox_visa->clear();

    QStringList visaAddrs = engine.getVisaAddrs();
    ui->comboBox_visa->addItems(visaAddrs);
    ui->tableWidget_3->selectRow(0);
    connect(ui->comboBox_visa,&QComboBox::currentTextChanged,this,&CheckWindow::on_comboBox_visa_OnCurrentTextChanged);
}

void CheckWindow::showCheck()
{
    deleteItem(ui->horizontalLayout_14);
    int idx=0;
    curDevciceIdx=-1;
    curDeviceId = "";
    caliDataListAll.clear();
    headTableListAll.clear();
    for (const MyTask& task : selectTaskList)
    {
        SequentialMap<QString, QStringList> header;
        QMap<QString, QList<QStringList>> data = BaseCommonApi::Search_CalibrationTableClass(task.sample_id,&header);
        caliDataListAll.append(data);
        headTableListAll.append(header);

        QPushButton *btn_1 = new QPushButton();
        btn_1->setText(task.sampleName);
        btn_1->setStyleSheet("QPushButton{"
                             "background-color:rgb(240,240,240);"
                             "color:rgb(51,51,51);"
                             "}");
        //btn_1->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
        QFont font;
        font.setPointSize(12);//字体大小
        font.setBold(true);
        btn_1->setFont(font);
        btn_1->setCursor(Qt::PointingHandCursor);
        connect(btn_1, &QPushButton::clicked, this, [this,idx]() {
            on_deviceButton_clicked(idx);
        });
        ui->horizontalLayout_14->addWidget(btn_1);
        idx++;
    }
    ui->horizontalLayout_14->addStretch();
    on_deviceButton_clicked(0);
    ui->stackedWidget_2->setCurrentIndex(1);
}

void CheckWindow::changeDevice(int idx){
    if(idx>=0&&idx<selectTaskList.length())
    {
        //QStringList idList=programInfo.tested_device_id.split(",");
        QString deviceId = selectTaskList[idx].sample_id;
        if(deviceId==curDeviceId)
            return;
        curDeviceId = deviceId;
        QList<JudgeCalibrationTable> CliTable = BaseCommonApi::getJudgeCalibrationTable(curDeviceId);
        if(CliTable.length()>0){
            iEquipType = CliTable[0].belong_standard_equipment.toInt();
        }
        //QStringList headlist;
        pageButtons.clear();
        headList.clear();
        //calibrationList.clear();
        //headTableList.clear();
        chList.clear();
        //calibrationList =BaseCommonApi::Search_CalibrationTable(deviceId,&headList);
        //caliDataList = BaseCommonApi::Search_CalibrationTableClass(deviceId,&headTableList);
        caliDataList = &caliDataListAll[idx];
        headTableList = &headTableListAll[idx];
        if(caliDataList->count()<1){
           QMessageBox::warning(this, "提示", "此检定项未开发或此设备无检测点");
           return;
        };
        //tableWidgetItem
        QList<QStringList> calData = caliDataList->values().first();
        gaugeInfo = BaseCommonApi::searchStatus(curDeviceId);
        ui->tableWidget_Point->clearContents();
        ui->tableWidget_Point->setRowCount(0);
        deleteItem(ui->HeadListLayout);
        //循环数据
        int rowCount=0;
        int row=0;
//        for (const QStringList& item : calibrationList)
//        {
//            //获取行号 并进行添加行
//            rowCount = ui->tableWidget_Point->rowCount();
//            ui->tableWidget_Point->insertRow(rowCount);
//            QTableWidgetItem* p_check = new QTableWidgetItem();
//            p_check->setCheckState(Qt::Unchecked);        //设置首列为 CheckBox 控件
//            ui->tableWidget_Point->setItem(row,0,p_check);
//            ui->tableWidget_Point->setItem(row, 1, new QTableWidgetItem(item[8]));
//            ui->tableWidget_Point->setItem(row, 2, new QTableWidgetItem("1"));
//            row++;
//        }
        int index=0;
        curHeadIdx=-1;
        for (const auto &item : *headTableList) {
           // ========================================================================================
           // 数据循环显示
           // ===================================================================================
           QString sLabel = headTableList->keys()[index];
            const auto &dataRes = caliDataList->value(sLabel);
           //tableWidget->setRowCount(data.count());

           for (int i = 0; i < dataRes.count(); ++i) {
               //获取行号 并进行添加行
               rowCount = ui->tableWidget_Point->rowCount();
               ui->tableWidget_Point->insertRow(rowCount);


               int idx= getIndexByHeader(sLabel,QStrTranQStrList("检定项目"));
               int idx2= getIndexByHeader(sLabel,QStrTranQStrList("标准值"));
               int idx3= getIndexByHeader(sLabel,QStrTranQStrList("单位"));
               int chname= getIndexByHeader(sLabel,QStrTranQStrList("通道"));

               if(idx==-1){idx=1;};if(idx2==-1){idx2=5;};if(idx3==-1){idx3=4;};
               ui->tableWidget_Point->setItem(row, 0, new QTableWidgetItem(dataRes[i][idx]));
               ui->tableWidget_Point->setItem(row, 1, new QTableWidgetItem(dataRes[i][idx2]+"("+dataRes[i][idx3]+")"));
               ui->tableWidget_Point->setItem(row, 2, new QTableWidgetItem("1"));
               row++;

               if(chname!=-1&&!chList.contains(dataRes[i][chname]))
               {
                   chList.append(dataRes[i][chname]);
               }
           }

           // ========================================================================================
           // 按钮设置显示
           // ====================================================================================
           QPushButton *button = new QPushButton(sLabel, this);
           //设置样式
           // 设置按钮大小为 56x20
           //固定大小文字显示不全
           //button->setFixedSize(85, 40);
        button->setCheckable(true);
        button->setAutoExclusive(true);
           //设置第一个按钮选中状态
           if(index==0){  button->setChecked(true);}
           //button->setc(true);
                     // 设置按钮样式表
           button->setStyleSheet("QPushButton {"
                                   "border: 1px solid white;" // 白色边框
                                   "border-bottom: 2px solid rgba(0, 255, 255, 1);" // 浅蓝色下划线
                                   "background-color: rgba(242, 242, 242, 1);" // 浅灰色背景
                                  "height:40px;"
                                 "padding:5px;"
                                   "}"
                                   "QPushButton:hover {"
                                   "background-color: rgba(215, 215, 215, 1);;" // 鼠标悬停时变为深灰色背景
                                   "}"
                                   "QPushButton:checked {"
                                   "color: white;"
                                   "background-color: rgb(74, 102, 186);" // 选中时变为蓝色背景
                                   "}");

           ui->HeadListLayout->addWidget(button);
           connect(button, &QPushButton::clicked, this, [this,index]() {
               //tackedWidget->setCurrentIndex(index);
               initCheckTable(index);
           });
           pageButtons.append(button);
           ++index;
       }
        QStringList info;
        if(gaugeInfo.length()>0)
            info = gaugeInfo[0];

        ui->HeadListLayout->addStretch();
        if(iEquipType==4){
            ui->pushButton_next->show();
            QLabel* label = new QLabel(this);
            label->setText("单位：");
            ui->HeadListLayout->addWidget(label);
            QLineEdit* edit1 = new QLineEdit(this);
            edit1->setFixedWidth(100);
            edit1->setText(info.at(6));
            ui->HeadListLayout->addWidget(edit1);
            QLabel* label2 = new QLabel(this);
            label2->setText("检定介质");
            ui->HeadListLayout->addWidget(label2);
            QComboBox* combox = new QComboBox(this);
            combox->addItem(info.at(7));
//            combox->addItem("变压器油");
//            combox->addItem("空气");
//            combox->addItem("水");
//            combox->addItem("其他");
            ui->HeadListLayout->addWidget(combox);
            QLabel* label3 = new QLabel(this);
            label3->setText("分度值的1/5：");
            ui->HeadListLayout->addWidget(label3);
            QLineEdit* edit = new QLineEdit(this);
            edit->setFixedWidth(100);
            edit->setText(info.at(8));
            ui->HeadListLayout->addWidget(edit);
            QPushButton *button = new QPushButton("生成结果处理", this);
            button->setStyleSheet("QPushButton {"
                                    "color: white;"
                                    "background-color: rgba(0, 128, 254, 1);"
                                    "}");
            connect(button, &QPushButton::clicked, this, [this]() {
                //tackedWidget->setCurrentIndex(index);
                openGageResult();
            });
            ui->HeadListLayout->addWidget(button);
        }else{
            ui->pushButton_next->hide();
        }
        if(iEquipType==5){
            CreateCharts();
            ui->groupBoxChart->show();
            timerChart = new QTimer(this);
            connect(timerChart,SIGNAL(timeout()),this,SLOT(RefreshTime_Slot()));
            //timerChart->start(2000);
        }else{
            ui->groupBoxChart->hide();
        }
    }
    std::sort(chList.begin(), chList.end());
    if(chList.count()==0){chList.append("CH1");};
    initCheckTable(0);
}
void CheckWindow::openGageResult(){
    //QList<QStringList> calData = caliDataList.values().first();
    FormGageResult *checkWindow = new FormGageResult(nullptr,selectTaskList[0].sample_id);
    //checkWindow->setWindowState(Qt::WindowMaximized);
    checkWindow->setWindowModality(Qt::ApplicationModal);
    checkWindow->show();
}

void CheckWindow::on_deviceButton_clicked(int idx)
{
    if(curDevciceIdx == idx||idx<0||idx>=selectTaskList.length())
        return;
    curDevciceIdx = idx;
    for(int i=0;i<selectTaskList.length();i++){
        QLayoutItem *child = ui->horizontalLayout_14->itemAt(i);
        QPushButton *btn_1 = qobject_cast<QPushButton *>(child->widget());
        if(i==idx){
            btn_1->setStyleSheet("QPushButton{"
                                 "background-color:rgb(55,83,123);"
                                 "color:rgb(255,255,255);"
                                 "}");
        }else{
            btn_1->setStyleSheet("QPushButton{"
                                 "background-color:rgb(240,240,240);"
                                 "color:rgb(51,51,51);"
                                 "}");
        }
    }
    ui->label_21->setText(QString("设备名称: %1").arg(selectTaskList[curDevciceIdx].sampleName));
    ui->label_22->setText(QString("型号规格: %1").arg(selectTaskList[curDevciceIdx].sampleModel));
    ui->label_23->setText(QString("出厂编号: %1").arg(selectTaskList[curDevciceIdx].manufactureNo));
    ui->label_24->setText(QString("生产厂家: %1").arg(selectTaskList[curDevciceIdx].manufacturer));
    changeDevice(idx);
}

void CheckWindow::deleteItem(QLayout *layout)
{
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr)
    {
        //setParent为NULL，防止删除之后界面不消失
        if(child->widget())
        {
            child->widget()->setParent(nullptr);
            delete child->widget();
        }
        else if (child->layout())
        {
            deleteItem(child->layout());
            child->layout()->deleteLater();
        }
        delete child;
    }
}


void CheckWindow::showMethodWidget()
{
    if(iEquipType==5){
        if(timerChart->isActive())
            timerChart->stop();
    }
    ui->stackedWidget_2->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(0);
    getCheckDevice();
}

void CheckWindow::showResult()
{
    QList<QStringList> StanDeviceInfo;
    StanDeviceInfo.append(StanDeviceIDList);
    StanDeviceInfo.append(StanDeviceNameList);
    StanDeviceInfo.append(StandarIdList);
    DialogCheckResult* dlg = new DialogCheckResult;
    dlg->taskList =selectTaskList;
    //dlg->deviceId = selectTaskList[0].sample_id;
    dlg->initCheckTable(caliDataListAll,headTableListAll,StanDeviceInfo);
    dlg->setModal(true);
    dlg->show();
}

void CheckWindow::SetHomePaheCombox(MyTask task)
{
    if(comboBoxContainsItem(ui->comboBoxName,task.sampleName)){
        ui->comboBoxName->addItem(task.sampleName);
    }
    if(comboBoxContainsItem(ui->comboBoxModal,task.sampleModel)){
        ui->comboBoxModal->addItem(task.sampleModel);
    }
    if(comboBoxContainsItem(ui->comboBoxCustomer,task.customer_name)){
        ui->comboBoxCustomer->addItem(task.customer_name);
    }
}

void CheckWindow::SetStandardCombox(CatDeviceInfor info)
{
    if(comboBoxContainsItem(ui->comboBoxName_2,info.equipment_name)){
        ui->comboBoxName_2->addItem(info.equipment_name);
    }
    if(comboBoxContainsItem(ui->comboBoxModal_2,info.model)){
        ui->comboBoxModal_2->addItem(info.model);
    }
}

bool CheckWindow::comboBoxContainsItem(QComboBox *comboBox, const QString &itemText)
{
    for (int i = 0; i < comboBox->count(); ++i) {
        if (comboBox->itemText(i) == itemText) {
            return false; // 找到匹配项，返回 true
        }
    }
    return true; // 没有找到匹配项，返回 false
}

void CheckWindow::getCheckDevice(bool bInit)
{
    myTaskList.clear();
    ui->tableWidgetDevice->clearContents();
    ui->tableWidgetDevice->setRowCount(0);
    //模拟用户ID
//    QString userId="1695282436182188034";
    myTaskList = BaseCommonApi::getMyTasks(loginUserId);
//    myTaskList = BaseCommonApi::getAllData();
    if(bInit){
        for (int i=0;i<myTaskList.length();i++){
            if(myTaskList[i].measure_status=="3")
                SetHomePaheCombox(myTaskList[i]);
        }
        ui->comboBoxName->setCurrentIndex(0);
        ui->comboBoxModal->setCurrentIndex(0);
        ui->comboBoxCustomer->setCurrentIndex(0);
        ui->lineEditNo->setText("");
    }
    QString sFilterName = ui->comboBoxName->currentText();
    QString sFilterModal = ui->comboBoxModal->currentText();
    QString sFilterCustomer = ui->comboBoxCustomer->currentText();
    QString sFilterNo = ui->lineEditNo->text();
    for (int i=myTaskList.length()-1;i>=0;i--){
        bool bAdd=true;
        if(sFilterName!="ALL"&&sFilterName!=""&&!myTaskList[i].sampleName.contains(sFilterName)){
            bAdd=false;
        }
        if(sFilterModal!="ALL"&&sFilterModal!=""&&!myTaskList[i].sampleModel.contains(sFilterModal)){
            bAdd=false;
        }
        if(sFilterCustomer!="ALL"&&sFilterCustomer!=""&&!myTaskList[i].customer_name.contains(sFilterCustomer)){
            bAdd=false;
        }
        if(sFilterNo!=""&&!myTaskList[i].manufactureNo.contains(sFilterNo)){
            bAdd=false;
        }
        if(!bAdd||myTaskList[i].measure_status!="3"){
            myTaskList.removeAt(i);
        }
    }
    int rowCount = 0;
    int row=0;
    QString State;
    for (const MyTask& myTask : myTaskList) {
        rowCount = ui->tableWidgetDevice->rowCount();

        ui->tableWidgetDevice->insertRow(rowCount);
        QTableWidgetItem* p_check = new QTableWidgetItem();
        p_check->setCheckState(Qt::Unchecked);        //设置首列为 CheckBox 控件
        ui->tableWidgetDevice->setItem(row,0,p_check);

        ui->tableWidgetDevice->setItem(row, 1, new QTableWidgetItem(QString::number(row+1)));
        //ui->tableWidget->setItem(row, 0, new QTableWidgetItem(dept.id));
        ui->tableWidgetDevice->setItem(row, 2, new QTableWidgetItem(myTask.sampleName)); //设置数据
        ui->tableWidgetDevice->setItem(row, 3, new QTableWidgetItem(myTask.sampleModel)); //设置数据
        ui->tableWidgetDevice->setItem(row, 4, new QTableWidgetItem(myTask.help_instruction)); //设置数据
        ui->tableWidgetDevice->setItem(row, 5, new QTableWidgetItem(myTask.manufactureNo)); //设置数据
        ui->tableWidgetDevice->setItem(row, 6, new QTableWidgetItem(myTask.manufacturer)); //设置数据
        ui->tableWidgetDevice->setItem(row, 7, new QTableWidgetItem(myTask.customer_name)); //设置数据
        row++;
    }
}

void CheckWindow::getCheckMethod()
{
    programList.clear();
    ui->tableWidgetMethod->clearContents();
    ui->tableWidgetMethod->setRowCount(0);
    //programList = BaseCommonApi::getVerificationProgramInfoMain();
    programList = BaseCommonApi::getVerificationProgramInfoByDeviceId(selectTaskList[0].sample_id);
    QString sFilterName = ui->lineEditName->text();
    QString sFilterUser = ui->lineEditCreator->text();
    for(int i=programList.length()-1;i>=0;i--){
        bool bAdd=true;
        if(sFilterName!=""&&!programList[i].program_name.contains(sFilterName)){
            bAdd=false;
        }
        if(sFilterUser!=""&&!programList[i].create_name.contains(sFilterUser)){
            bAdd=false;
        }
        if(!bAdd){
            programList.removeAt(i);
        }
    }
    int rowCount = 0;
    int row=0;
    for (const VerificationProgramInfo& program : programList) {
         rowCount = ui->tableWidgetMethod->rowCount();

         ui->tableWidgetMethod->insertRow(rowCount);
         QTableWidgetItem* p_check = new QTableWidgetItem();
         //p_check->setCheckState(Qt::Checked);        //设置首列为 CheckBox 控件
         p_check->setCheckState(Qt::Unchecked);
         ui->tableWidgetMethod->setItem(row,0,p_check);
         ui->tableWidgetMethod->setItem(row, 1, new QTableWidgetItem(program.program_name)); //设置数据
         ui->tableWidgetMethod->setItem(row, 2, new QTableWidgetItem(program.create_name)); //设置数据
         ui->tableWidgetMethod->setItem(row, 3, new QTableWidgetItem(program.create_time.toString("yyyy-MM-dd"))); //设置数据
         ui->tableWidgetMethod->setItem(row, 4, new QTableWidgetItem(program.remark)); //设置数据

         QPushButton *btn_1 = new QPushButton();
         btn_1->setText(tr("详情"));
         btn_1->setStyleSheet("QPushButton{"
                              "background-color:rgba(255,255,255,0);"
                              "color:rgba(0,0,255,100);"
                              "text-decoration:underline;"
                              "}");
         btn_1->setCursor(Qt::PointingHandCursor);
         connect(btn_1,SIGNAL(clicked()),this,SLOT(on_editButton_clicked()));

         btn_1->setIconSize(QSize(16,16));
         btn_1->setIcon(QIcon(":/image/Index/u2324.svg"));
         QWidget *tmp_widget = new QWidget();
         QHBoxLayout *tmp_layout = new QHBoxLayout(tmp_widget);
         tmp_layout->addWidget(btn_1);
         tmp_layout->setMargin(0);
         ui->tableWidgetMethod->setCellWidget(row,5,tmp_widget);
         row++;
    }
}

void CheckWindow::on_editButton_clicked()
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
    QModelIndex index = ui->tableWidgetMethod->indexAt(QPoint(x, y));
    // 获取该按钮所在表格的行号和列号
    int editIdx = index.row();
    //int column = index.column();
    QString sId =  programList[editIdx].id;
    NewCheckMethod* checkWindow = new NewCheckMethod(nullptr,sId,false);
    //checkWindow->checkManageWnd=this;
    checkWindow->setWindowState(Qt::WindowMaximized);
    checkWindow->setWindowModality(Qt::ApplicationModal);
    checkWindow->show();
}

void CheckWindow::on_tableWidgetDevice_cellChanged(int row, int column)
{

}

void CheckWindow::on_tableWidgetMethod_cellChanged(int row, int column)
{
    Qt::CheckState state = ui->tableWidgetMethod->item(row,0)->checkState();
    if(state == Qt::Checked)
    {
        for(int i=0;i<ui->tableWidgetMethod->rowCount();i++){
            if(i!=row)
            {
                ui->tableWidgetMethod->item(i,0)->setCheckState(Qt::Unchecked);
            }
        }
    }
}

void CheckWindow::on_pushButton_9_clicked()
{
    //showDevice();
    showStandard();
}

void CheckWindow::on_tableWidget_3_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if(currentRow>=0&&currentRow<connectList.length()){
        currConnectIdx = currentRow;
        ConnectInfo info= connectList[currentRow];
        VerificationProgramConfigInterface addr = info.programInterface;
        if(addr.communication_type == "SerialPort")
            ui->radioButton_serial->setChecked(true);
        else
            ui->radioButton_visa->setChecked(true);
        ui->comboBox_visa->setCurrentText(addr.visa);

        if(addr.serial_port!=""){
            QStringList sList = addr.serial_port.split(";");
            if(sList.length()==5){
                ui->comboBox_port->setCurrentText(sList[0]);
                ui->comboBox_baurt->setCurrentText(sList[1]);
                ui->comboBox_check->setCurrentText(sList[2]);
                ui->comboBox_data->setCurrentText(sList[3]);
                ui->comboBox_stop->setCurrentText(sList[4]);
            }
        }
        //修改连接状态
        if(getJoinState(addr.visa))
        {
             ui->tableWidget_3->setItem(currentRow,2,new QTableWidgetItem("已连接"));
        }else{  ui->tableWidget_3->setItem(currentRow,2,new QTableWidgetItem("未连接"));}

    }
}

void CheckWindow::initCheckTable(int idx)
{
//    ui->tableWidgetCheck->clearContents();
//    ui->tableWidgetCheck->setRowCount(0);
//    ui->tableWidgetCheck->setColumnCount(0);
//    int rowCount = 0;
//    int row=0;
//    ui->tableWidgetCheck->setColumnCount(headList.count());
//    ui->tableWidgetCheck->setHorizontalHeaderLabels(headList);
//    for(int i=0;i<headList.count();i++)
//    {
//        ui->tableWidgetCheck->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);
//    }
//    measureCalList.clear();
//    for(int i=0;i<ui->tableWidget_Point->rowCount();i++){
//        Qt::CheckState state = ui->tableWidget_Point->item(i,0)->checkState();
//        if(state == Qt::Checked){
//            QStringList& item = calibrationList[i];
//            int cnt = ui->tableWidget_Point->item(i,2)->text().toInt();
//            for(int j=0;j<cnt;j++){
//                rowCount = ui->tableWidgetCheck->rowCount();
//                ui->tableWidgetCheck->insertRow(rowCount);
//                ui->tableWidgetCheck->setItem(row, 0, new QTableWidgetItem(QString::number(row+1)));
//                //循环读取值  headlist，count是列的数量
//                for(int k=1;k<headList.count();k++)
//                {
//                    ui->tableWidgetCheck->setItem(row, k, new QTableWidgetItem(item[k]));
//                }
//                SaveDeviceMeasureCalibrator measItem;
//                measItem.item_data_id = item[1];
//                measItem.maximum_error_formula = item[10];//最大允许误差(公式)
//                measItem.error_param_a = item[11];//误差参数a
//                measItem.error_param_b = item[12];//误差参数b
//                measItem.params = item[2];//参数
//                measItem.capacity=item[3];//量
//                measItem.unit=item[4];//单位
//                measItem.frequency=item[5];//频率
//                measItem.frequency_unit=item[6];//频率单位
//                measItem.range_range=item[7];//量程
//                measItem.standard_value=item[8];//标准值(输入字段,特殊:指针式万用表输出字段)
//                measItem.indicating_value=item[15];//示值(输出字段,特殊:指针式万用表输入字段)
//                measItem.maximum_error=item[10];//最大允许误差(值)
//                //measItem.absolute_error=item[11];//绝对误差
//                measItem.resolution=item[9];//分辨力
//                measItem.measure_indication_value=item[19];//被检表示值
//                measureCalList.append(measItem);
//                row++;
//            }
//        }
//    }
    if(curHeadIdx==idx||headTableList->keys().count()==0)
        return;


    pageButtons[idx]->setChecked(true);

    curHeadIdx = idx;
    QString sLabel = headTableList->keys()[curHeadIdx];
    const auto &dataRes = caliDataList->value(sLabel);
    const auto &headrs = headTableList->value(sLabel);
    ui->tableWidgetCheck->clearContents();
    ui->tableWidgetCheck->setRowCount(0);
    ui->tableWidgetCheck->setColumnCount(0);
    ui->tableWidgetCheck->setColumnCount(headrs.count());
    ui->tableWidgetCheck->setHorizontalHeaderLabels(headrs);
    for(int i=0;i<headrs.count();i++)
    {
        ui->tableWidgetCheck->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);
    }
    //tableWidget->setRowCount(data.count());
    int rowCount = 0;
    for (int i = 0; i < dataRes.count(); ++i) {
        //获取行号 并进行添加行
        rowCount = ui->tableWidgetCheck->rowCount();
        ui->tableWidgetCheck->insertRow(rowCount);
        ui->tableWidgetCheck->setItem(i, 0, new QTableWidgetItem(QString::number(i+1)));
        for(int j=1;j<dataRes[i].count();j++)
        {
           if(headrs[j]=="轻敲前示值-正"||headrs[j]=="轻敲前示值-反"||headrs[j]=="轻敲后示值-正"||headrs[j]=="轻敲后示值-反"){
               QLineEdit *label = new QLineEdit();
               label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
               label->setText(dataRes[i][j]);
               label->setStyleSheet("QLineEdit{border-with:0;border-style:outset}");
               connect(label, &QLineEdit::textChanged, this, [this]() {
                   //double dVal = label->text().toDouble();
                   //label->setText(QString::number(dVal-1));
                   gageDataChange();
               });
               QPushButton *btn_1 = new QPushButton();
               btn_1->setText(tr("-"));
               btn_1->setFont(QFont( "Timers",12,QFont::Bold));
//               btn_1->setStyleSheet("QPushButton{"
//                                    "background-color:rgba(255,255,255,0);"
//                                    "color:rgba(92,170,54,100);"
//                                    "text-decoration:underline;"
//                                    "}");
               btn_1->setCursor(Qt::PointingHandCursor);
               btn_1->setFixedWidth(24);
               btn_1->setFlat(true);
               //connect(btn_1,SIGNAL(clicked()),this,SLOT(on_editButton_clicked()));
               connect(btn_1, &QPushButton::clicked, this, [this]() {
                   //double dVal = label->text().toDouble();
                   //label->setText(QString::number(dVal-1));
                   addGageData(false);
               });
               QPushButton *btn_2 = new QPushButton();
               btn_2->setText(tr("+"));
               btn_2->setFont(QFont( "Timers",12,QFont::Bold));
//               btn_2->setStyleSheet("QPushButton{"
//                                    "background-color:rgba(255,255,255,0);"
//                                    "color:rgba(170,17,17,100);"
//                                    "text-decoration:underline;"
//                                    "}");
               btn_2->setCursor(Qt::PointingHandCursor);
               btn_2->setFixedWidth(24);
               btn_2->setFlat(true);
               //connect(btn_2,SIGNAL(clicked()),this,SLOT(on_deleteButton_clicked()));
               connect(btn_2, &QPushButton::clicked, this, [this]() {
                   //double dVal = label->text().toDouble();
                   //label->setText(QString::number(dVal+1));
                   addGageData();
               });
//               btn_2->setIconSize(QSize(16,16));
//               btn_2->setIcon(QIcon(":/image/Index/u2325.svg"));
               QWidget *tmp_widget = new QWidget();
               QHBoxLayout *tmp_layout = new QHBoxLayout(tmp_widget);
               tmp_layout->addWidget(btn_1,0);
               tmp_layout->addWidget(label,2);
               tmp_layout->addWidget(btn_2,0);
               tmp_layout->setMargin(0);
               ui->tableWidgetCheck->setCellWidget(i,j,tmp_widget);
           }else
               ui->tableWidgetCheck->setItem(i, j, new QTableWidgetItem(dataRes[i][j]));
           //需要界面Ui隐藏的列
           if(iEquipType==1){
                ui->tableWidgetCheck->hideColumn(getIndexByHeader(sLabel,QStrTranQStrList("a")));
                ui->tableWidgetCheck->hideColumn(getIndexByHeader(sLabel,QStrTranQStrList("b")));
                ui->tableWidgetCheck->hideColumn(getIndexByHeader(sLabel,QStrTranQStrList("最大允许误差公式")));
                /*if(dataRes[i][1] == "DCV" || dataRes[i][1] == "DCI"){
                     ui->tableWidgetCheck->hideColumn(10);
                     ui->tableWidgetCheck->hideColumn(11);
                }else if (dataRes[i][1] == "ACV" || dataRes[i][1] == "ACI") {
                     ui->tableWidgetCheck->hideColumn(12);
                     ui->tableWidgetCheck->hideColumn(13);
                }else if (dataRes[i][1] == "R") {
                    ui->tableWidgetCheck->hideColumn(11);
                    ui->tableWidgetCheck->hideColumn(12);
                }*/
            }else if(iEquipType==16){
                ui->tableWidgetCheck->hideColumn(getIndexByHeader(sLabel,QStrTranQStrList("基准点")));
                ui->tableWidgetCheck->hideColumn(getIndexByHeader(sLabel,QStrTranQStrList("有效位数")));
           }


        }
    }
    if(iEquipType==4&&nextGageIndex!=-1){
        if(isBack){
            ui->tableWidgetCheck->item(nextGageIndex, 0)->setSelected(true);
            ui->tableWidgetCheck->cellWidget(nextGageIndex,3)->setStyleSheet("color:white;background-color:rgb(1,10,128);");
            ui->tableWidgetCheck->cellWidget(nextGageIndex,5)->setStyleSheet("color:white;background-color:rgb(1,10,128);");
        }else{
            ui->tableWidgetCheck->item(nextGageIndex, 0)->setSelected(true);
            ui->tableWidgetCheck->cellWidget(nextGageIndex,2)->setStyleSheet("color:white;background-color:rgb(1,10,128);");
            ui->tableWidgetCheck->cellWidget(nextGageIndex,4)->setStyleSheet("color:white;background-color:rgb(1,10,128);");
        }
    }
    if(iEquipType==5){
        if(sLabel=="波动性测试"){
            for(int i=0;i<ui->tableWidgetCheck->rowCount()/8;i++){
                ui->tableWidgetCheck->setSpan(i*8,1,8,2);
                QString sLimit;
                if(dataRes[i*8][1]!="")
                    sLimit = dataRes[i*8][1];
                else
                    sLimit = dataRes[i*8][2];
                ui->tableWidgetCheck->item(i*8,1)->setText(sLimit);
                ui->tableWidgetCheck->setSpan(i*8,3,8,1);
                ui->tableWidgetCheck->setSpan(i*8,19,8,1);
                ui->tableWidgetCheck->setSpan(i*8,20,8,1);
                ui->tableWidgetCheck->setSpan(i*8,21,8,1);
            }
        }else if(sLabel=="均匀性测试"){
            for(int i=0;i<ui->tableWidgetCheck->rowCount()/8;i++){
                ui->tableWidgetCheck->setSpan(i*8,1,8,2);
                QString sLimit;
                if(dataRes[i*8][1]!="")
                    sLimit = dataRes[i*8][1];
                else
                    sLimit = dataRes[i*8][2];
                ui->tableWidgetCheck->item(i*8,1)->setText(sLimit);
                ui->tableWidgetCheck->setSpan(i*8,3,8,1);
                ui->tableWidgetCheck->setSpan(i*8,15,8,1);
                ui->tableWidgetCheck->setSpan(i*8,16,8,1);
                ui->tableWidgetCheck->setSpan(i*8,17,8,1);
                ui->tableWidgetCheck->setSpan(i*8,18,8,1);
            }
        }
    }
}
//压力测试 数据表格同步 计算改变
void CheckWindow::gageDataChange(){
    QLineEdit *pushButton_ = dynamic_cast<QLineEdit*>(this->sender());
    if(NULL == pushButton_)
    {
        return;
    }
    //qDebug() << pushButton_->text();
    // 获取按钮的x坐标和y坐标
    int x = pushButton_->parentWidget()->frameGeometry().x();
    int y = pushButton_->parentWidget()->frameGeometry().y();
    // 根据按钮的x和y坐标来定位对应的单元格
    QModelIndex index = ui->tableWidgetCheck->indexAt(QPoint(x, y));
    // 获取该按钮所在表格的行号和列号
    int row = index.row();
    int col = index.column();
    QString sLabel = headTableList->keys()[curHeadIdx];
    QList<QStringList> dataRes = caliDataList->value(sLabel);

    dataRes[row][col] = pushButton_->text();
    double dVal,dVal2;
    if(col==2||col==4){
        QLayoutItem *child = ui->tableWidgetCheck->cellWidget(row,2)->layout()->itemAt(1);
        QLineEdit* editCtrl = qobject_cast<QLineEdit *>(child->widget());
        QLayoutItem *child2 = ui->tableWidgetCheck->cellWidget(row,4)->layout()->itemAt(1);
        QLineEdit* editCtrl2 = qobject_cast<QLineEdit *>(child2->widget());
        if(editCtrl!=nullptr&&editCtrl2!=nullptr){
            dVal = editCtrl->text().toDouble();
            dVal2 = editCtrl2->text().toDouble();
            //qDebug() << dVal <<";" << dVal2;
            ui->tableWidgetCheck->setItem(row,6,new QTableWidgetItem(QString::number(dVal2-dVal)));
            dataRes[row][6] = QString::number(dVal2-dVal);
        }
    }else if(col==3||col==5){
        QLayoutItem *child = ui->tableWidgetCheck->cellWidget(row,3)->layout()->itemAt(1);
        QLineEdit* editCtrl = qobject_cast<QLineEdit *>(child->widget());
        QLayoutItem *child2 = ui->tableWidgetCheck->cellWidget(row,5)->layout()->itemAt(1);
        QLineEdit* editCtrl2 = qobject_cast<QLineEdit *>(child2->widget());
        if(editCtrl!=nullptr&&editCtrl2!=nullptr){
            dVal = editCtrl->text().toDouble();
            dVal2 = editCtrl2->text().toDouble();
            ui->tableWidgetCheck->setItem(row,7,new QTableWidgetItem(QString::number(dVal2-dVal)));
            dataRes[row][7] = QString::number(dVal2-dVal);
        }
    }
    caliDataList->insert(sLabel,dataRes);
}

void CheckWindow::stepGageCheck()
{
    QBrush brush;
    QTableWidgetItem *currentItem;

    if (ui->tableWidgetCheck->rowCount() > 0) {
        if (!isBack) {
            for (int i = 0; i < ui->tableWidgetCheck->rowCount(); ++i) {
                QTableWidgetItem *item = ui->tableWidgetCheck->item(i, 0);
                if (item->isSelected()) {
                    lastSelectedIndex = i;
                    break;
                }
            }
//            if (lastSelectedIndex == -1) {
//                lastSelectedIndex = 0;
//            }
            nextGageIndex = (lastSelectedIndex + 1) % ui->tableWidgetCheck->rowCount();
            if(lastSelectedIndex == ui-> tableWidgetCheck->rowCount() - 1 ){
                nextGageIndex = (lastSelectedIndex) % ui->tableWidgetCheck->rowCount();
            }

            BaseCommonApi::SaveLogInfo(1,QString("标准器输出"));
            QString sParam,newCmd;
            constsLable = headTableList->keys()[curHeadIdx];
            QList<QStringList> dataRes = caliDataList->value(constsLable);
            int idx= getIndexByHeader(constsLable,QStrTranQStrList("检定项目"));
            if(idx!=-1)
                sParam = dataRes[nextGageIndex][idx];
            else
                sParam = constsLable; //示值误差/回程误差/轻敲位移
            InstructionLib *instrcutLib = getCmdByCheckName(1,sParam);
            constDataRes= dataRes[nextGageIndex];
            SendDevice(instrcutLib,&standardEngine);
            /*if(standardEngine.getState()){
                InstructionLib *instrcutLib = getCmdByCheckName(1,sParam);
                if(instrcutLib!=nullptr){
                    QStringList sList = instrcutLib->instruct_config.split(";");
                    if(sList.length()>0){
                        for (QString& sCmd : sList){
                            newCmd = transCmd(sCmd,constsLable,dataRes[nextGageIndex],true);
                            if(newCmd!=""){
                                standardEngine.sendData(newCmd);
                                qDebug() << "standard cmds:" << newCmd;
                            }
                        }
                    }
                }
            }*/

            currentItem = ui->tableWidgetCheck->item(nextGageIndex, 0);
            currentItem->setSelected(true);
            //brush = QBrush(Qt::green);
            ui->tableWidgetCheck->cellWidget(nextGageIndex,2)->setStyleSheet("color:white;background-color:rgb(1,10,128);");
            ui->tableWidgetCheck->cellWidget(nextGageIndex,4)->setStyleSheet("color:white;background-color:rgb(1,10,128);");
            //currentItem->setBackground(brush);
            if (lastSelectedIndex != -1) {
                QTableWidgetItem *previousItem = ui->tableWidgetCheck->item(lastSelectedIndex, 0);
                previousItem->setSelected(false);
                ui->tableWidgetCheck->cellWidget(lastSelectedIndex,2)->setStyleSheet("");
                ui->tableWidgetCheck->cellWidget(lastSelectedIndex,4)->setStyleSheet("");
//                previousItem->setBackground(Qt::NoBrush);
            }
            if(lastSelectedIndex == ui->tableWidgetCheck->rowCount() - 1){
                isBack =!isBack;
                currentItem = ui->tableWidgetCheck->item(nextGageIndex, 0);
                currentItem->setSelected(true);
                ui->tableWidgetCheck->cellWidget(nextGageIndex,3)->setStyleSheet("color:white;background-color:rgb(1,10,128);");
                ui->tableWidgetCheck->cellWidget(nextGageIndex,5)->setStyleSheet("color:white;background-color:rgb(1,10,128);");
            }
        } else {
            for (int i = ui->tableWidgetCheck->rowCount() - 1; i >= 0; --i) {
                QTableWidgetItem *item = ui->tableWidgetCheck->item(i, 0);
                if (item->isSelected()) {
                    lastSelectedIndex = i;
                    break;
                }
            }
            if (lastSelectedIndex == -1) {
                lastSelectedIndex = ui->tableWidgetCheck->rowCount() - 1;
            }
            nextGageIndex = (lastSelectedIndex == 0) ? ui->tableWidgetCheck->rowCount() - 1 : lastSelectedIndex - 1;

            BaseCommonApi::SaveLogInfo(1,QString("标准器输出"));
            QString sParam,newCmd;
            constsLable = headTableList->keys()[curHeadIdx];
            QList<QStringList> dataRes = caliDataList->value(constsLable);
            int idx= getIndexByHeader(constsLable,QStrTranQStrList("检定项目"));
            if(idx!=-1)
                sParam = dataRes[nextGageIndex][idx];
            else
                sParam = constsLable; //示值误差/回程误差/轻敲位移

            InstructionLib *instrcutLib = getCmdByCheckName(1,sParam);
            constDataRes= dataRes[nextGageIndex];
            SendDevice(instrcutLib,&standardEngine);
            /*if(standardEngine.getState()){
                InstructionLib *instrcutLib = getCmdByCheckName(1,sParam);
                if(instrcutLib!=nullptr){
                    QStringList sList = instrcutLib->instruct_config.split(";");
                    if(sList.length()>0){
                        for (QString& sCmd : sList){
                            newCmd = transCmd(sCmd,sLabel,dataRes[nextGageIndex],true);
                            if(newCmd!=""){
                                standardEngine.sendData(newCmd);
                                qDebug() << "standard cmds:" << newCmd;
                            }
                        }
                    }
                }
            }*/

            currentItem = ui->tableWidgetCheck->item(nextGageIndex, 0);
            currentItem->setSelected(true);
            ui->tableWidgetCheck->cellWidget(nextGageIndex,3)->setStyleSheet("color:white;background-color:rgb(1,10,128);");
            ui->tableWidgetCheck->cellWidget(nextGageIndex,5)->setStyleSheet("color:white;background-color:rgb(1,10,128);");
            //brush = QBrush(Qt::blue);
            //currentItem->setBackground(brush);

            // 取消之前选中项的选中状态
            if (lastSelectedIndex != -1) {
                QTableWidgetItem *previousItem = ui->tableWidgetCheck->item(lastSelectedIndex, 0);
                previousItem->setSelected(false);
                ui->tableWidgetCheck->cellWidget(lastSelectedIndex,3)->setStyleSheet("");
                ui->tableWidgetCheck->cellWidget(lastSelectedIndex,5)->setStyleSheet("");
//                previousItem->setBackground(Qt::NoBrush);
            }
            if(lastSelectedIndex == 1){
                isBack = !isBack;
                ui->pushButton_start->setEnabled(true);
                ui->pushButton_next->setEnabled(false);
                closeDevice();
                nextGageIndex=-1;
            }
        }
        ui->tableWidgetCheck->scrollToItem(currentItem);
    }
}
//下一步
void CheckWindow::on_pushButton_next_clicked()
{
    stepGageCheck();
}

//压力测试 用于控制数据是增加还是减少
void CheckWindow::addGageData(bool bAdd){
    QPushButton *pushButton_ = dynamic_cast<QPushButton*>(this->sender());
    if(NULL == pushButton_)
    {
        return;
    }
    // 获取按钮的x坐标和y坐标
    int x = pushButton_->parentWidget()->frameGeometry().x();
    int y = pushButton_->parentWidget()->frameGeometry().y();
    // 根据按钮的x和y坐标来定位对应的单元格
    QModelIndex index = ui->tableWidgetCheck->indexAt(QPoint(x, y));
    // 获取该按钮所在表格的行号和列号
    int row = index.row();
    int col = index.column();
    QLayoutItem *child = pushButton_->parentWidget()->layout()->itemAt(1);
    QLineEdit* editCtrl = qobject_cast<QLineEdit *>(child->widget());
    double dStep=1;
    if(gaugeInfo.length()>0)
        dStep = gaugeInfo[0].at(8).toDouble();
    if(editCtrl!=nullptr){
        double dVal = editCtrl->text().toDouble();
        if(bAdd)
            dVal+=dStep;
        else
            dVal-=dStep;
        editCtrl->setText(QString::number(dVal));
    }
    if(col==2||col==3){
        QLayoutItem *child = ui->tableWidgetCheck->cellWidget(row,col+2)->layout()->itemAt(1);
        QLineEdit* editCtrl = qobject_cast<QLineEdit *>(child->widget());
        if(editCtrl!=nullptr){
            double dVal = editCtrl->text().toDouble();
            if(bAdd)
                dVal+=dStep;
            else
                dVal-=dStep;
            editCtrl->setText(QString::number(dVal));
        }
    }
}
void CheckWindow::InItcmd(){
    //发送INIt相关指令
    int count=0;
    for(const ConnectInfo &visa:connectList)
    {
        VisaCommonEngine *Engine;
        InstructionLib *instrcutLib;
        if(connectList.length()>0&&visa.type==0)
        {
            instrcutLib = getCmdByCheckName(count,"INIT");
            Engine=&deviceEngine;
        }
        if(connectList.length()>1&&visa.type==1)
        {
            instrcutLib = getCmdByCheckName(count,"INIT");
            if(connectList.count()==1){
                Engine=&standardEngine;
            }else{
               int stancount=0;
               for (VisaCommonEngine &item:VisaStanEngineList) {
                    if(stancount==count-1){
                        Engine=&item;
                        break;
                    }
                    stancount++;
               }
            }

        }
        SendDevice(instrcutLib,Engine);
        count++;
    }
}
void CheckWindow::connectDevice(){
    int count=0;
    QString rolename;
    for(const ConnectInfo &visa:connectList)
    {
        VisaCommonEngine Engine;
        if(Engine.openDevice(visa.programInterface.visa,"Visa")){
            if(connectList.length()>0&&visa.type==0)
            {
                deviceEngine=Engine;
            }
            if(connectList.length()>1&&visa.type==1)
            {
                if(connectList.count()==2){
                    standardEngine=Engine;
                }else{
                   rolename=this->GetdeviceidByRole(visa.programInterface.device_id);
                   Engine.role=rolename;
                   VisaStanEngineList.insert(rolename,Engine);
                }

            }
        }
        count++;

    }
}
void CheckWindow::closeCmd(){
    //发送Close相关指令
    int count=0;
    for(const ConnectInfo &visa:connectList)
    {
        VisaCommonEngine *Engine;
        InstructionLib *instrcutLib;
        if(connectList.length()>0&&visa.type==0)
        {
            instrcutLib = getCmdByCheckName(count,"CLOSE");
            Engine=&deviceEngine;
        }
        if(connectList.length()>1&&visa.type==1)
        {
            instrcutLib = getCmdByCheckName(count,"CLOSE");
            if(connectList.count()==1){
                Engine=&standardEngine;
            }else{
               int stancount=0;
               for (VisaCommonEngine &item:VisaStanEngineList) {
                    if(stancount==count-1){
                        Engine=&item;
                        break;
                    }
                    stancount++;
               }
            }

        }
        SendDevice(instrcutLib,Engine);
        count++;
    }
}
void CheckWindow::closeDevice(){
    //关闭设备连接
    int count=0;
    for(const ConnectInfo &visa:connectList)
    {
        VisaCommonEngine *Engine;
        if(connectList.length()>0&&visa.type==0)
        {
            Engine=&deviceEngine;
        }
        if(connectList.length()>1&&visa.type==1)
        {
            if(connectList.count()==1){
                Engine=&standardEngine;
            }else{
                int stancount=0;
                for (VisaCommonEngine &item:VisaStanEngineList) {
                     if(stancount==count-1){
                         Engine=&item;
                         break;
                     }
                }
            }

        }
        Engine->closeDevice();
        count++;

    }
}
InstructionLib* CheckWindow::getCmdByRoleName(QString RoleName,QString name){

    for (ConnectInfo& info : connectList) {
        if(info.rolenName==RoleName){
            for (InstructionLib& item : info.cmdList) {
                if(item.belong_vername==name)
                    return &item;
            }
        }

    }
    return nullptr;
}
VisaCommonEngine CheckWindow::getVisaCommbyRoleName(QString RoleName){
    VisaCommonEngine visa;
    for(VisaCommonEngine &item : VisaStanEngineList){
        if(item.role==RoleName){
            return  item;
        }
    }
    return visa;
}
InstructionLib* CheckWindow::getCmdByCheckName(int idx,QString name)
{
    if(idx>=0&&idx<connectList.length()){
        for (InstructionLib& item : connectList[idx].cmdList) {
            if(item.belong_vername==name)
                return &item;
        }
    }
    return nullptr;
}

int CheckWindow::getIndexByHeader(QString sLabel,QStringList sName){
     const auto &headrs = headTableList->value(sLabel);
     for(int j=0;j<sName.count();j++)
     {
         for(int i=0;i<headrs.length();i++){
             if(headrs[i]==sName[j])
                 return i;
         }
     }
     return -1;
}
QStringList CheckWindow::getsParambyHeader(QString sParam)
{
    QStringList temp;
    for(const QStringList &key: resultfillingList.keys()){
        for(int i=0;i<key.count();i++)
        {
            if(sParam==key[i])
            {
                temp=resultfillingList.value(key);
                return  temp;
            }
        }
    }
    return  temp;
}
double CheckWindow::transUnit(QString sData,QString sUnit,bool reverse){
    double dTmp = sData.toDouble();
    QString unit = sUnit.toUpper();
    QStringList TempValue;
    for(const QStringList &key : unitTransList.keys())
    {
        for (int i=0;i<key.count();i++) {
            if(key[i]==unit){
                if(reverse)
                {
                     dTmp/=unitTransList.value(key)[0].toDouble();
                     return dTmp;
                }else
                {
                     dTmp*=unitTransList.value(key)[0].toDouble();
                     return dTmp;
                }

            }
        }
    }
    return dTmp;
}
double CheckWindow::tranunitSplit(QString sData,bool reverse)
{

        QRegularExpression re("(?=.*[0-9])(?=.*[a-zA-ZΩΜΩ])");
        if(re.match(sData).hasMatch())
        {


            QString sData2=sData;
            QString  sNewData,sUnit;
            ItemUnitSplit(sData,sNewData,sUnit);
            return  transUnit(sNewData,sUnit,reverse);
        }else{
            return sData.toDouble();
        }

}
QString CheckWindow::TranTypeUnit(QString sUnit)
{
    QString NewsUnit = sUnit;
    for (QStringList keyList : unitTypeTransList.keys()) {
        if(keyList[0].contains(sUnit)){
           NewsUnit = unitTypeTransList.value(keyList)[0];
        }
    }
    return NewsUnit;
}
QString CheckWindow::TranDecimals(QString sLabel,QStringList data,double TestValue,QString ColName)
{
   QStringList ValueList;
   if(ColName==NULL){
   ValueList.append("标准值");
   ValueList.append("标准器示值");
   ValueList.append("标称值");
   }else{
       ValueList.append(ColName);
   }
   int dex=getIndexByHeader(sLabel,ValueList);
   if(dex==-1)
       return QString::number(TestValue);

    QString StanValue=Removeunit(data[dex]);
    int decimalPlaces = 0;
    int decimalPos = StanValue.indexOf('.');
       if (decimalPos == -1) {
          decimalPlaces=1;
       }else{
         decimalPlaces=StanValue.length() - decimalPos - 1;
       }
   QString formattedTestValue = QString::number(TestValue, 'f', decimalPlaces);
   return  formattedTestValue;
}
QString CheckWindow::transCmd(QString sCmd,QString sLabel,QStringList data,bool devicetype)
{
    QString sNewCmd = sCmd;
    if(sNewCmd.contains("{DELAY:")){
        QRegExp re("DELAY:(\\d+)");
        int delay=0;
        if (re.indexIn(sNewCmd) != -1) {
            delay= re.cap(1).toUInt();
        }
        BaseCommonApi::SaveLogInfo(1,QString("延迟 %1 ms").arg(delay));
        qDebug() << "delay time:" << delay;
        Delay_MSec(delay);
        return "";
    }
        QString NewUnit;
        QStringList cmdPlaceHolderValue;
        QStringList TemporaryValue;
        for (QStringList keyList : cmdPlaceHolderList.keys()) {
            if(sCmd.contains(keyList[0])){
                cmdPlaceHolderValue=cmdPlaceHolderList.value(keyList);

                    int idx= getIndexByHeader(sLabel,cmdPlaceHolderList.value(keyList));
                    if(idx!=-1){
                        if(devicetype || keyList[0]=="{CH}")
                        {
                           NewUnit = TranTypeUnit(data[idx]); //转换 Ω->OHM
                           sNewCmd = sNewCmd.replace(keyList[0],NewUnit);
                           continue;
                        }
                         double dTmp;
                        QRegularExpression re("(?=.*[0-9])(?=.*[a-zA-ZΩΜΩ])");
                        if(re.match(data[idx]).hasMatch())
                        {
                            dTmp = tranunitSplit(data[idx],true);
                        }else
                        {
                            TemporaryValue.append("单位");
                            int idx2= getIndexByHeader(sLabel,TemporaryValue);
                            if(idx2!=-1)
                            {
                                QString unit = data[idx2];
                                dTmp = transUnit(data[idx],unit,true);
                            }else{
                                dTmp=data[idx].toDouble();
                            };
                        }
                        sNewCmd = sNewCmd.replace(keyList[0],QString::number(dTmp));

                    }

        }
    }
    return sNewCmd;
}

//_text
void CheckWindow::on_pushButton_start_clicked()
{
    Teststate=SyncRunStatus::Running;
    if(headTableList->keys().count()==0)
        return;
    BaseCommonApi::SaveLogInfo(1,QString("-----------------------------------"));
    if(iEquipType==13)
    {
        //connectDevice();
        this->Oscilloscopetextflow();
        return;
    }
    else if (iEquipType==9) {
        this->Spectrumtextflow();
        return;
    }
    else if(iEquipType==4){
        isBack=false;
        nextGageIndex=-1;
        lastSelectedIndex=-1;
        ui->pushButton_next->setEnabled(true);
        ui->pushButton_start->setEnabled(false);
        for (int i = 0; i < ui->tableWidgetCheck->rowCount(); ++i) {
            QTableWidgetItem *item = ui->tableWidgetCheck->item(i, 0);
            item->setSelected(false);
            ui->tableWidgetCheck->cellWidget(i,2)->setStyleSheet("");
            ui->tableWidgetCheck->cellWidget(i,4)->setStyleSheet("");
            ui->tableWidgetCheck->cellWidget(i,3)->setStyleSheet("");
            ui->tableWidgetCheck->cellWidget(i,5)->setStyleSheet("");
        }
        connectDevice();
        stepGageCheck();
        return;
    }
    else if(iEquipType==5){
        //timerChart->start(5000);
        connectDevice();
        VisaCommonEngine tempEngine=getVisaCommbyRoleName("堆栈式测温仪"); //角色
        if(tempEngine.getState()){
            NumberCount=0;
            timerChart->start(5000);
        }else{
            QMessageBox::warning(this, "警告", "设备未连接！");
        }
        return;
    }else if(iEquipType==10 || iEquipType==16){
        //信号源采集读数 标准器
        this->SignalGeneratortextflow();
        return;
    }
    connectDevice();
    for(int i=0;i<headTableList->keys().length();i++)
    {
        if(Teststate==SyncRunStatus::Stopped){break;};

        //每次循环按钮变化
        initCheckTable(i);
        constsLable= headTableList->keys()[i];
        QList<QStringList> dataRes = caliDataList->value(constsLable);
        BaseCommonApi::SaveLogInfo(1,QString("开始 %1 测试").arg(constsLable));
        for (int i = 0; i < dataRes.count(); ++i) {
            if(Teststate==SyncRunStatus::Stopped){break;};
            constDataRes=dataRes[i];
            InItcmd();
            QString sRet="";
            QString newCmd;
            double dStdValue=FP_INFINITE,dError=0;
            QString newdStdValue,newdError;

            //标准器输出
            BaseCommonApi::SaveLogInfo(1,QString("标准器输出"));
            QString sParam;
            QStringList TempValue;
            TempValue.append("检定项目");
            int idx= getIndexByHeader(constsLable,TempValue);
            if(idx!=-1)
                sParam = dataRes[i][idx];
            CheckBackColorUpdate(true,i);

            //暂时加到这个地方 后面统一编写
            if(sParam=="DCI"||sParam=="ACI")
            {

                int idx2= getIndexByHeader(constsLable,QStrTranQStrList("单位"));
                if(idx2!=-1)
                {
                    QString ConstUnit="";
                    QString Item;
                    QString v=dataRes[i][idx2].toUpper();
                    if(i!=0){
                         ConstUnit=dataRes[i-1][idx2].toUpper();
                         Item=dataRes[i-1][idx];
                    }
                    if(Item!=sParam){
                        if(!this->showDialog("电流换线提醒",QString("换线提醒\n %1测试").arg(sParam).arg(v)))
                        {
                            continue;
                        }
                    }

                    BaseCommonApi::SaveLogInfo(1,ConstUnit+"-"+v);
                    if((ConstUnit=="MA" && v=="A") || (ConstUnit=="A" && v=="MA"))
                    {
                         if(!this->showDialog("电流换线提醒",QString("换线提醒\n %1 -> %2").arg(ConstUnit).arg(v)))
                         {
                             continue;
                         }
                    }
                }
            }
            if(iEquipType==2){
                BaseCommonApi::SaveLogInfo(1,QString("可编程交流电源输出"));
                VisaCommonEngine engineRole=getVisaCommbyRoleName("可编程交流电源供应器"); //角色
                InstructionLib *instrcutLibRole=getCmdByRoleName("可编程交流电源供应器",constsLable);
                SendDevice(instrcutLibRole,&engineRole);
                BaseCommonApi::SaveLogInfo(1,QString("被检电源输出"));
                InstructionLib *instrcutLib = getCmdByCheckName(0,constsLable);
                SendDevice(instrcutLib,&deviceEngine);
                BaseCommonApi::SaveLogInfo(1,QString("电子负载"));
                engineRole=getVisaCommbyRoleName("电子负载"); //角色
                instrcutLibRole=getCmdByRoleName("电子负载",constsLable);
                SendDevice(instrcutLibRole,&engineRole);
            }
            else if(iEquipType==11){

                BaseCommonApi::SaveLogInfo(1,QString("信号发生器"));
                VisaCommonEngine engineRole2=getVisaCommbyRoleName("信号发生器"); //角色
                InstructionLib *instrcutLibRole2=getCmdByRoleName("信号发生器",constsLable);
                SendDevice(instrcutLibRole2,&engineRole2);
                BaseCommonApi::SaveLogInfo(1,QString("测量接收机"));
                VisaCommonEngine engineRole=getVisaCommbyRoleName("测量接收机"); //角色
                InstructionLib *instrcutLibRole=getCmdByRoleName("测量接收机",constsLable);
                SendDevice(instrcutLibRole,&engineRole);
                //校准流程
                bool IsCli=true;
                if(i!=0){
                    idx = getIndexByHeader(constsLable,QStrTranQStrList("频率"));
                    QString FreqValue,PageFreqValue;
                    FreqValue=Removeunit(dataRes[i][idx]);
                    PageFreqValue=Removeunit(dataRes[i-1][idx]);
                    if(PageFreqValue==FreqValue){
                        IsCli=false;
                    }
                }
                if(IsCli){
                    //&& 只在频率切换时进行校准
                    //功率校准
                    InstructionLib *PowerPage=getCmdByRoleName("测量接收机","界面切换");
                    InstructionLib *PowerCali=getCmdByRoleName("信号发生器","校准");
                    InstructionLib *PowerCaliSt=getCmdByRoleName("测量接收机","校准");
                    if(PowerCali!=nullptr && PowerCaliSt!=nullptr && PowerCaliSt!=nullptr){
                        InstructionLib Temp;
                        InstructionLib TempSt;
                        for(QString item:PowerCali->instruct_read.split(';'))
                        {
                            Temp.instruct_config=item;
                            SendDevice(&Temp,&deviceEngine);
                            if(item.contains("20")){
                                TempSt.instruct_config=PowerPage->instruct_config.split(";")[0];
                                SendDevice(&TempSt,&standardEngine);
                                TempSt.instruct_config=PowerCaliSt->instruct_config.split(";")[0];
                                SendDevice(&TempSt,&standardEngine);
                            }else{
                                TempSt.instruct_config=PowerPage->instruct_config.split(";")[1];
                                SendDevice(&TempSt,&standardEngine);
                                TempSt.instruct_config=PowerCaliSt->instruct_config.split(";")[1];
                                SendDevice(&TempSt,&standardEngine);
                            }
                        }
                      }
                  }

                sRet=ReadDevice(instrcutLibRole,&engineRole);
            }
            else
            {

                InstructionLib *instrcutLibstan = getCmdByCheckName(1,sParam);
                SendDevice(instrcutLibstan,&standardEngine,true);
                //被检设备读数
                BaseCommonApi::SaveLogInfo(1,QString("被检设备输出"));
                InstructionLib *instrcutLib = getCmdByCheckName(0,sParam);
                SendDevice(instrcutLib,&deviceEngine);
                sRet=ReadDevice(instrcutLib,&deviceEngine);

            }
            if(sRet!=""){
                TempValue.clear();
                TempValue.append("单位");
                idx= getIndexByHeader(constsLable,TempValue);
                if(idx!=-1){
                  dStdValue = transUnit(sRet,dataRes[i][idx]);
                }else{
                    dStdValue=sRet.toDouble();
                }

            }
            autoAdujstData(constsLable,i,dStdValue,dataRes);
            CheckBackColorUpdate(false,i);
            closeCmd();
            CheckBackColorUpdate(false,i);
            Delay_MSec(2000);
        }
        caliDataList->insert(constsLable,dataRes);
    }
    closeDevice();
}
void CheckWindow::Spectrumtextflow()
{
    Teststate=SyncRunStatus::Running;
    if(headTableList->keys().count()==0)
        return;
    BaseCommonApi::SaveLogInfo(1,QString("-----------------------------------"));
    connectDevice();
    for(int i=0;i<headTableList->keys().length();i++)
    {
        if(Teststate==SyncRunStatus::Stopped){break;};
        //每次循环按钮变化
        initCheckTable(i);
        constsLable= headTableList->keys()[i];
        QList<QStringList> dataRes = caliDataList->value(constsLable);
        BaseCommonApi::SaveLogInfo(1,QString("开始 %1 测试").arg(constsLable));
        for (int i = 0; i < dataRes.count(); ++i) {
            if(Teststate==SyncRunStatus::Stopped){break;};
            constDataRes=dataRes[i];
            InItcmd();
            QString sRet="";
            QString newCmd;
            double dStdValue=FP_INFINITE;
            QString newdStdValue,newdError;
            //获取检定项目
            QString sParam,TempValue;
            TempValue.append("检定项目");
            int idx= getIndexByHeader(constsLable,QStrTranQStrList(TempValue));
            if(idx!=-1)
                 sParam = dataRes[i][idx];

            //背景颜色显示
            CheckBackColorUpdate(true,i);
            autoAdujstData(constsLable,i,dStdValue,dataRes);
            CheckBackColorUpdate(false,i);
            closeCmd();
            Delay_MSec(2000);
        }
        caliDataList->insert(constsLable,dataRes);
    }
    closeDevice();
}

void CheckWindow::autoAdujstData(QString sLabel,int i,double dStdValue,QList<QStringList>& dataRes){
    double dError;
    QString newdStdValue,newdError,Value;
    if(iEquipType==1 || iEquipType==13 ){
        if(dStdValue==FP_INFINITE){
            bool bOk = false;
            dStdValue = QInputDialog::getDouble(this,"手动测试","请输入被检示值",0,-100000,	100000,5,&bOk);

            if (bOk) {
                //sRet = QString::number(dStdValue);
            }else{
                dStdValue=0;
            }
        }
      //  ItemUnitSplit

        //newdStdValue= TranDecimals(sLabel, dataRes[i],dStdValue);
        int idxDecimls=getIndexByHeader(sLabel,QStrTranQStrList("分辨力"));
        if(idxDecimls==-1){
           newdStdValue=QString::number(dStdValue,'f',4);
        }else{
           newdStdValue=TranDecimals(sLabel,dataRes[i],dStdValue,"分辨力");
        }

        //QStringList ResultName=getsParambyHeader(sParam);

        int idx= getIndexByHeader(sLabel,QStrTranQStrList("示值"));

        if(idx!=-1)
        {
            dataRes[i][idx] = newdStdValue;

            ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue)); //示值
        }
        idx= getIndexByHeader(sLabel,QStrTranQStrList("绝对误差"));
        if(idx!=-1)
        {

            int idx2=getIndexByHeader(sLabel,QStrTranQStrList(("标准值")));
            if(idx2==-1)
                idx2=getIndexByHeader(sLabel,QStrTranQStrList("标称值"));
            if(idx2!=-1){
            double dTmp = dataRes[i][idx2].toDouble();
            dError = dStdValue-dTmp;
            if(idxDecimls==-1){
               newdError=TranDecimals(sLabel,dataRes[i],dError);
            }else{
               newdError=TranDecimals(sLabel,dataRes[i],dError,"分辨力");
            }

            dataRes[i][idx] = newdError;
            ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdError));
            }
        }

        idx= getIndexByHeader(sLabel,QStrTranQStrList("测量值"));
        if(idx!=-1)
        {
            dataRes[i][idx] = newdStdValue;
            ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue)); //测量值
            if(sLabel=="校准信号"){
                int topidx= getIndexByHeader(constsLable,QStrTranQStrList("顶部测量值"));
                int bomidx= getIndexByHeader(constsLable,QStrTranQStrList("底部测量值"));
                 ui->tableWidgetCheck->setItem(i,topidx,new QTableWidgetItem(dataRes[i][topidx])); //测量值
                  ui->tableWidgetCheck->setItem(i,bomidx,new QTableWidgetItem(dataRes[i][bomidx])); //测量值
            }
        }
        idx= getIndexByHeader(sLabel,QStrTranQStrList(("相对误差")));
        if(idx!=-1)
        {
            int idx2=getIndexByHeader(sLabel,QStrTranQStrList(("标准值")));
            if(idx2==-1)
                idx2=getIndexByHeader(sLabel,QStrTranQStrList("标称值"));
            if(idx2!=-1){
               double dTmp = dataRes[i][idx2].toDouble();
               dError = (dStdValue-dTmp);
               newdError=TranDecimals(sLabel,dataRes[i],dError);
               dataRes[i][idx] = newdError;
               ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdError));
            }

        }
        idx= getIndexByHeader(sLabel,QStrTranQStrList("最大允许误差公式"));
        if(idx!=-1){
            int idxa,idxb,idxRange,idxdef;
            double data=0;
            QString MaxError,newdata;
            idxa= getIndexByHeader(sLabel,QStrTranQStrList("a"));
            idxb= getIndexByHeader(sLabel,QStrTranQStrList("b"));

            MaxError=dataRes[i][idx];
            MaxError.replace(" ","");
            if(MaxError=="=a*示值+b*量程"){
                idxRange= getIndexByHeader(sLabel,QStrTranQStrList("量程"));
                idx= getIndexByHeader(sLabel,QStrTranQStrList("示值"));
                data=dataRes[i][idxa].toDouble()*dataRes[i][idx].toDouble()
                        +dataRes[i][idxb].toDouble()*dataRes[i][idxRange].toDouble();
            }else if(MaxError=="=a*示值+b*分辨力"){
                idxdef= getIndexByHeader(sLabel,QStrTranQStrList("分辨力"));
                data=dataRes[i][idxa].toDouble()*dataRes[i][idx].toDouble()
                        +dataRes[i][idxb].toDouble()*dataRes[i][idxdef].toDouble();
            }else if(MaxError=="=a*示值+常数b"){
                idx= getIndexByHeader(sLabel,QStrTranQStrList("示值"));
                data=dataRes[i][idxa].toDouble()*dataRes[i][idx].toDouble()
                        +dataRes[i][idxb].toDouble();
            }
             idx= getIndexByHeader(sLabel,QStrTranQStrList("最大允许误差"));
             newdata=TranDecimals(sLabel,dataRes[i],data,"分辨力");
             dataRes[i][idx]=newdata;
             ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdata)); //测量值
        }
        idx= getIndexByHeader(sLabel,QStrTranQStrList(("结论")));
        if(idx!=-1)
        {
            dataRes[i][idx] = "符合指标";
            ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem("符合指标")); //测量值
        }
        if(sLabel=="频带宽度"){
            idx= getIndexByHeader(sLabel,QStrTranQStrList("测量幅度"));
            if(idx!=-1)
            {
                QString textValue= ui->tableWidgetCheck->item(i,idx)->text();

                if(textValue==""){
                dataRes[i][idx] = newdStdValue;
                ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue)); //测试
                }else
                {
                   idx= getIndexByHeader(sLabel,QStrTranQStrList("参考幅度"));
                   QString refvalue= newdStdValue;
                   dataRes[i][idx] = refvalue;
                   ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(refvalue)); //参考
                   // 计算a除以b的对数（以10为底）
                   double logResult = std::log10( refvalue.toDouble() / textValue.toDouble());
                   double result = 20 * logResult;
                   idx= getIndexByHeader(sLabel,QStrTranQStrList(("分贝数(dB)")));
                   if(idx!=-1)
                   {
                        QString newresult;
                        newresult=TranDecimals(sLabel, dataRes[i],result);
                        dataRes[i][idx] = newresult;
                        ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newresult)); //分贝数
                   }

                }
            }
        }
        if(sLabel=="时基"){
            idx= getIndexByHeader(sLabel,QStrTranQStrList(("周期时标信号")));
            if(idx!=-1)
            {
                double ErrorCal;
                QString sign="";
                if(dStdValue>0.2){sign="-";};

                bool bOk = false;
                dStdValue = QInputDialog::getDouble(this,"手动输入","请输入△T/ns值",0,-100000,	100000,5,&bOk);
                if (bOk) {
                }else{
                    dStdValue=0;
                }
                dStdValue=transUnit(QString::number(dStdValue),"ns",true);
                ErrorCal=std::fabs(dStdValue);
                idx= getIndexByHeader(sLabel,QStrTranQStrList(("时基绝对误差△T/ns")));
                if(idx!=-1){
                  dataRes[i][idx] =sign+QString::number(ErrorCal);
                  ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(sign+QString::number(ErrorCal))); //测量值
                }

                idx= getIndexByHeader(sLabel,QStrTranQStrList(("时基相对误差△δ")));
                {
                  dataRes[i][idx] =sign+QString::number(ErrorCal/0.01);
                  ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(sign+QString::number(ErrorCal/0.01))); //测量值
                }
            }
        }

        BaseCommonApi::SaveLogInfo(1,QString("转换前 测试值 %1").arg(dStdValue));
        BaseCommonApi::SaveLogInfo(1,QString("转换后 测试值 %1").arg(newdStdValue));

        //Delay_MSec(2000);
    }
    else if(iEquipType==2){
//        if(qIsFinite(dStdValue)){
//            bool bOk = false;
//            dStdValue = QInputDialog::getDouble(this,"手动测试","请输入测量值",0,-100000,	100000,5,&bOk);
//            if (bOk) {
//                //sRet = QString::number(dStdValue);
//            }else{
//                dStdValue=0;
//            }
//        }
        QString sTips= "请调节电压和电流至额定状态后，点击测量。";
        if(sLabel=="电源电压调整率")
            sTips = "请调节电压和电流至额定状态后，点击测量。";
        else if(sLabel=="负载调整率"){
            int idx =getIndexByHeader(sLabel,QStrTranQStrList(("负载情况")));
            if(dataRes[i][idx]=="满载")
                sTips="请调节仪器至满载状态后，点击测量。";
            else
                 sTips="请调节仪器至空载状态后，点击测量。";
        }else if(sLabel=="电压示值误差"){
            int idx =getIndexByHeader(sLabel,QStrTranQStrList(("被检表示值")));
            int idx2 =getIndexByHeader(sLabel,QStrTranQStrList(("单位")));
            sTips = QString("调节输出至额定电压%1%2，并测量被检电源的实际电压，记录到蓝色区域。").arg(dataRes[i][idx]).arg(dataRes[i][idx2]);
        }else if(sLabel=="电流示值误差(直接测量)"){
            int idx =getIndexByHeader(sLabel,QStrTranQStrList(("被检表示值")));
            int idx2 =getIndexByHeader(sLabel,QStrTranQStrList(("单位")));
            sTips = QString("请调节仪器至%1%2额定电流状态，且电流接线状态正确后，点击测量按钮。").arg(dataRes[i][idx]).arg(dataRes[i][idx2]);
        }

        DialogDCStabInput dlg(this,sLabel,sTips);
        dlg.dataRes = &dataRes[i];
        stabInputdlg = &dlg;
        connect(&dlg,SIGNAL(readData(QString)),this,SLOT(readDCStabData(QString)));

        dlg.setModal(true);
        if(dlg.exec()==QDialog::Accepted){
            dStdValue = dlg.getInputData();
        }
        if(sLabel=="电压示值误差"||sLabel=="电流示值误差(直接测量)"){
            newdStdValue= TranDecimals(sLabel,dataRes[i],dStdValue);
            int idx= getIndexByHeader(sLabel,QStrTranQStrList(("标准值")));
            if(idx!=-1)
            {
                dataRes[i][idx] = newdStdValue;
                ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue));
            }
            idx= getIndexByHeader(sLabel,QStrTranQStrList(("绝对误差")));
            if(idx!=-1)
            {
                int idx2=getIndexByHeader(sLabel,QStrTranQStrList(("被检表示值")));
                double dTmp = dataRes[i][idx2].toDouble();
                dError = dTmp-dStdValue;
                newdError=TranDecimals(sLabel,dataRes[i],dError);
                dataRes[i][idx] = newdError;
                ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdError));
            }
        }
        else if(sLabel=="电流示值误差(间接测量)"){
            newdStdValue= TranDecimals(sLabel,dataRes[i],dStdValue);
            int idx= getIndexByHeader(sLabel,QStrTranQStrList(("标准电压值")));
            if(idx!=-1)
            {
                dataRes[i][idx] = newdStdValue;
                ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue));
            }
            idx= getIndexByHeader(sLabel,QStrTranQStrList(("标准值")));
            if(idx!=-1)
            {
                int idx2=getIndexByHeader(sLabel,QStrTranQStrList(("标准电阻值")));
                double dTmp = dataRes[i][idx2].toDouble();
                dError = dStdValue/dTmp;//计算电流
                newdError=TranDecimals(sLabel,dataRes[i],dError);
                dataRes[i][idx] = newdError;
                ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdError));

                idx2=getIndexByHeader(sLabel,QStrTranQStrList(("被检表示值")));
                dTmp = dataRes[i][idx2].toDouble();
                idx= getIndexByHeader(sLabel,QStrTranQStrList(("绝对误差")));
                dError = dTmp-dError;
                newdError=TranDecimals(sLabel,dataRes[i],dError);
                dataRes[i][idx] = newdError;
                ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdError));
            }
        }
        else if(sLabel=="电源电压调整率"){
            newdStdValue= TranDecimals(sLabel,dataRes[i],dStdValue);
            int idx= getIndexByHeader(sLabel,QStrTranQStrList("被检电源输出电压值"));
            if(idx!=-1)
            {
                dataRes[i][idx] = newdStdValue;
                ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue));
            }
            if((i+1)%3==0){
                int idx2=getIndexByHeader(sLabel,QStrTranQStrList("调压器输出值"));
                double dMin=0,dMax=0,dMid=0;
                QList<QStringList> outList;
                for(int k=i-2;k<=i;k++){
                    QStringList item = dataRes[k];
                    outList.append(item);
                }
                std::sort(outList.begin(),outList.end(),[idx2](const QStringList &outA,const QStringList &outB){return outA[idx2].toDouble()<outB[idx2].toDouble();});
                dMid = outList[1][idx2].toDouble();
                dMax = outList[2][idx].toDouble();
                dMin = outList[0][idx].toDouble();
                outList.clear();
                idx= getIndexByHeader(sLabel,QStrTranQStrList("被检电源输出电压最大差值"));
                if(idx!=-1)
                {
                    dError = abs(dMax-dMin);
                    newdError=TranDecimals(sLabel,dataRes[i],dError);
                    dataRes[i-2][idx]=dataRes[i-1][idx]=dataRes[i][idx] = newdError;
                    ui->tableWidgetCheck->setItem(i-2,idx,new QTableWidgetItem(newdError));
                    ui->tableWidgetCheck->setItem(i-1,idx,new QTableWidgetItem(newdError));
                    ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdError));
                    idx= getIndexByHeader(sLabel,QStrTranQStrList("电源电压调整率"));
                    if(idx!=-1){
                        dError = dError/dMid;
                        newdError=TranDecimals(sLabel,dataRes[i],dError)+"%";
                        dataRes[i-2][idx]=dataRes[i-1][idx]=dataRes[i][idx] = newdError;
                        ui->tableWidgetCheck->setItem(i-2,idx,new QTableWidgetItem(newdError));
                        ui->tableWidgetCheck->setItem(i-1,idx,new QTableWidgetItem(newdError));
                        ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdError));
                    }
                }
            }

        }
        else if(sLabel=="负载调整率"){
            newdStdValue= TranDecimals(sLabel,dataRes[i],dStdValue);
            int idx= getIndexByHeader(sLabel,QStrTranQStrList("被检电源输出电压值"));
            if(idx!=-1)
            {
                dataRes[i][idx] = newdStdValue;
                ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue));
            }
            if((i+1)%2==0){
                double dFull=dataRes[i-1][5].toDouble(),dEmpty=dStdValue;
                if(dataRes[i-1][4]=="满载"){
                    dFull=dataRes[i-1][5].toDouble();
                    dEmpty=dStdValue;
                }else{
                    dFull = dStdValue;
                    dEmpty=dataRes[i-1][5].toDouble();
                }
//                for(QStringList item:dataRes){
//                    if(item[4]=="满载"){
//                        dFull = item[5].toDouble();
//                    }else if(item[4]=="空载"){
//                        dEmpty = item[5].toDouble();
//                    }
//                }
                idx= getIndexByHeader(sLabel,QStrTranQStrList("电压差值"));
                if(idx!=-1)
                {
                    dError = abs(dFull-dEmpty);
                    newdError=TranDecimals(sLabel,dataRes[i],dError);
                    dataRes[i-1][idx] = newdError;
                    ui->tableWidgetCheck->setItem(i-1,idx,new QTableWidgetItem(newdError));
                    dataRes[i][idx] = newdError;
                    ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdError));

                    idx= getIndexByHeader(sLabel,QStrTranQStrList("负载调整率"));
                    if(idx!=-1){
                        dError = dError/dEmpty;
                        newdError=TranDecimals(sLabel,dataRes[i],dError)+"%";
                        dataRes[i-1][idx] = newdError;
                        ui->tableWidgetCheck->setItem(i-1,idx,new QTableWidgetItem(newdError));
                        dataRes[i][idx] = newdError;
                        ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdError));
                    }
                }
            }
        }
        else if(sLabel=="纹波电压"){
            newdStdValue= TranDecimals(sLabel,dataRes[i],dStdValue);
            int idx= getIndexByHeader(sLabel,QStrTranQStrList(("纹波电压")));
            if(idx!=-1)
            {
                dataRes[i][idx] = newdStdValue;
                ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue));
            }
        }
        else if(sLabel=="输出电压短期稳定性"){
            newdStdValue= TranDecimals(sLabel,dataRes[i],dStdValue);
            int idx= getIndexByHeader(sLabel,QStrTranQStrList(("被测电源输出电压")));
            if(idx!=-1)
            {
                dataRes[i][idx] = newdStdValue;
                ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue));
            }
            double dMin=0,dMax=0,dBase=1;
            int cnt=0;
            for(QStringList item:dataRes){
                if(item[idx]!=""){
                    if(cnt==0){
                        dMin = dMax= dBase = item[idx].toDouble();
                    }else{
                        dMin = qMin(dMin,item[idx].toDouble());
                        dMax = qMax(dMax,item[idx].toDouble());
                    }
                }
            }
            idx= getIndexByHeader(sLabel,QStrTranQStrList("输出电压最大变化值"));
            if(idx!=-1){
                dError = dMax-dMin;
                newdError=TranDecimals(sLabel,dataRes[i],dError);
                dataRes[i][idx] = newdError;
                ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdError));

                idx= getIndexByHeader(sLabel,QStrTranQStrList("短期稳定性"));
                dError = dError/dBase;
                newdError=TranDecimals(sLabel,dataRes[i],dError)+"%";
                dataRes[i][idx] = newdError;
                ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdError));
            }
        }
    }
    else if(iEquipType==4){
        if(qIsFinite(dStdValue)){
            MultiInputDialog dlg(4);
            QStringList titles ;
            titles << "轻敲前示值-正" <<"轻敲前示值-反"<< "轻敲后示值-正" << "轻敲后示值-反";
            dlg.setWindowTitle("输入测试值");
            dlg.resize(400,400);
            dlg.SetLabelTexts(titles);
            dlg.setModal(true);
            if(dlg.exec()==QDialog::Accepted){
                QList<double> results = dlg.GetAllTexts();
                int k=0;
                for(QString title:titles){
                    newdStdValue= TranDecimals(sLabel,dataRes[i],results[k]);
                    int idx= getIndexByHeader(sLabel,QStrTranQStrList((title)));
                    dataRes[i][idx] = newdStdValue;
                    ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue)); //示值
                    k++;
                    BaseCommonApi::SaveLogInfo(1,QString("测试项目: %1 测试值: %2").arg(title).arg(newdStdValue));
                }
                int idx= getIndexByHeader(sLabel,QStrTranQStrList("轻敲前后变动量-正"));
                if(idx!=-1){
                    newdError= TranDecimals(sLabel,dataRes[i],results[2]-results[0]);
                    dataRes[i][idx] = newdError;
                    ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdError));
                }
                idx= getIndexByHeader(sLabel,QStrTranQStrList("轻敲前后变动量-反"));
                if(idx!=-1){
                    newdError= TranDecimals(sLabel,dataRes[i],results[3]-results[1]);
                    dataRes[i][idx] = newdError;
                    ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdError));
                }
            }
        }
    }else if(iEquipType==9)
    {

        QStringList heads;
        QString pagehead;
        QString data,unit;
        //设置命令

        if(sLabel=="中频带宽转换偏差"||sLabel=="平均噪声电平"||sLabel=="谐波失真"||sLabel=="1db增益压缩点"){
            unit = "dBm";
        }else if(sLabel=="输入频响"||sLabel=="输入衰减"){
            int idx= getIndexByHeader(constsLable,QStrTranQStrList("指标下限"));
            QString value=dataRes[i][idx];
            ItemUnitSplit(value,data,unit);

        }else{
            int idx= getIndexByHeader(constsLable,QStrTranQStrList("标称值"));
            QString value=dataRes[i][idx];
            ItemUnitSplit(value,data,unit);

        }
        if(sLabel=="中频带宽"||sLabel=="扫频宽度"){
            heads  <<"fl1"<<"fr1"<<"fl2"<<"fr2"<<"fl3"<<"fr3";

        }else if(sLabel=="中频带宽转换偏差"||sLabel=="平均噪声电平"||sLabel=="1db增益压缩点"){
            heads  <<"测量值";
        }else if(sLabel=="参考电平"||sLabel=="垂直刻度"){
            heads  <<"信号源幅度";
        }
        else if(sLabel=="频率显示"||sLabel=="输入频响"||sLabel=="输入衰减"||sLabel=="校准信号"){
            heads  <<"测量值1"<<"测量值2"<<"测量值3";
        }else if(sLabel=="谐波失真"){
            heads  <<"基波幅度"<<"二次谐波幅度"<<"三次谐波幅度"<<"四次谐波幅度";
        }
        int d=0;
        for(QString &head:heads){
            if(Teststate==SyncRunStatus::Stopped){break;};
            QString sTips= "请调节频谱仪至额定状态后，点击测量："+head;

            DialogDCStabInput dlg(this,sLabel,sTips);
            dlg.dataRes = &dataRes[i];
            stabInputdlg = &dlg;

            if(sLabel=="中频带宽"||sLabel=="扫频宽度"){
                 if(d%2==0){
                     sendDCStabData(sLabel);
                 }
               d++;
            }else{
                     sendDCStabData(sLabel);
            }

            connect(&dlg,SIGNAL(readData(QString)),this,SLOT(readDCStabData(QString)));
            //connect(&dlg,SIGNAL(sendData(QString)),this,SLOT(sendDCStabData(QString)));
            dlg.setModal(true);
            QScreen *screen = QApplication::primaryScreen();
            if (screen) {
                // 计算屏幕中心偏上的位置
                int centerX = screen->size().width() / 2;
                int centerY = screen->size().height() / 4; // 假设你想在屏幕1/4处显示
                int dialogWidth = dlg.width();
                int dialogHeight = dlg.height();
                dlg.move(centerX - dialogWidth / 2, centerY - dialogHeight / 2);
            }

            if(dlg.exec()==QDialog::Accepted){
                dStdValue = dlg.getInputData();

                dStdValue=tranunitSplit(QString::number(dStdValue,'f',9)+unit);


                //newdStdValue= TranDecimals(sLabel,dataRes[i],dStdValue);
                newdStdValue=QString::number(dStdValue,'f',4);
                int idx= getIndexByHeader(sLabel,QStrTranQStrList(head));
                if(idx!=-1)
                {
                    dataRes[i][idx] = newdStdValue+unit;
                    ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue+unit));
                }
                //计算结果，判断是否合格
                if(sLabel=="中频带宽"){
                    double dMean=0;
                    int idx2=getIndexByHeader(sLabel,QStrTranQStrList("fr1"));
                    dError = Removeunit(dataRes[i][idx2]).toDouble();
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("fl1"));
                    dError -=Removeunit(dataRes[i][idx2]).toDouble();
                    dMean+=dError;
                    newdError=TranDecimals(sLabel,dataRes[i],dError);
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("△f1"));
                    dataRes[i][idx2] = newdError;
                    ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(newdError+unit));

                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("fr2"));
                    dError = Removeunit(dataRes[i][idx2]).toDouble();
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("fl2"));
                    dError -=Removeunit(dataRes[i][idx2]).toDouble();
                    dMean+=dError;
                    newdError=TranDecimals(sLabel,dataRes[i],dError);
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("△f2"));
                    dataRes[i][idx2] = newdError;
                    ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(newdError+unit));

                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("fr3"));
                    dError = Removeunit(dataRes[i][idx2]).toDouble();
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("fl3"));
                    dError -=Removeunit(dataRes[i][idx2]).toDouble();
                    dMean+=dError;
                    dMean/=3;
                    newdError=TranDecimals(sLabel,dataRes[i],dError);
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("△f3"));
                    dataRes[i][idx2] = newdError;
                    ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(newdError+unit));

                    newdError=QString::number(dMean, 'f', 2);
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("均值"));
                    dataRes[i][idx2] = newdError;
                    ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(newdError+unit));

                    QString sLow,sUpper,sUnit;
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("指标下限"));
                    ItemUnitSplit(dataRes[i][idx2],sLow,sUnit);
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("指标上限"));
                    ItemUnitSplit(dataRes[i][idx2],sUpper,sUnit);
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("结论"));
                    if(dMean>=sLow.toDouble()&&dMean<=sUpper.toDouble()){
                        sUnit = "符合指标";
                    }else{
                         sUnit = "不符合指标";
                    }
                    dataRes[i][idx2] = sUnit;
                    ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(sUnit));
                }else if(sLabel=="扫频宽度"){
                    double dMean=0;
                    int idx2=getIndexByHeader(sLabel,QStrTranQStrList("fr1"));
                    dError = Removeunit(dataRes[i][idx2]).toDouble();
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("fl1"));
                    dError -=dataRes[i][idx2].toDouble();
                    dMean+=dError;

                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("fr2"));
                    dError = Removeunit(dataRes[i][idx2]).toDouble();
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("fl2"));
                    dError -=Removeunit(dataRes[i][idx2]).toDouble();
                    dMean+=dError;

                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("fr3"));
                    dError = Removeunit(dataRes[i][idx2]).toDouble();
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("fl3"));
                    dError -=Removeunit(dataRes[i][idx2]).toDouble();
                    dMean+=dError;
                    dMean/=3;

                    newdError=QString::number(dMean, 'f', 5);
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("均值"));
                    dataRes[i][idx2] = newdError;
                    ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(newdError+unit));

                    QString sLow,sUpper,sUnit;
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("指标下限"));
                    ItemUnitSplit(dataRes[i][idx2],sLow,sUnit);
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("指标上限"));
                    ItemUnitSplit(dataRes[i][idx2],sUpper,sUnit);
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("结论"));
                    if(dMean>=sLow.toDouble()&&dMean<=sUpper.toDouble()){
                        sUnit = "符合指标";
                    }else{
                         sUnit = "不符合指标";
                    }
                    dataRes[i][idx2] = sUnit;
                    ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(sUnit));
                }
                else if(sLabel=="中频带宽转换偏差"){
                    int idx2=getIndexByHeader(sLabel,QStrTranQStrList("测量值"));
                    QString sLow,sUpper,sUnit;
                    //参考点
                    if(i==0){
                        idx2=getIndexByHeader(sLabel,QStrTranQStrList("转换偏差"));
                        dataRes[i][idx2] = "/";
                        ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem("/"));
                        sUnit="参考点";
                    }else{
                        dError = dataRes[i][idx2].toDouble()-dataRes[0][idx2].toDouble();
                        idx2=getIndexByHeader(sLabel,QStrTranQStrList("转换偏差"));
                        newdError = QString::number(dError, 'f', 2);
                        dataRes[i][idx2] = newdError;
                        ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(newdError+"dB"));

                        idx2=getIndexByHeader(sLabel,QStrTranQStrList("指标下限"));
                        ItemUnitSplit(dataRes[i][idx2],sLow,sUnit);
                        idx2=getIndexByHeader(sLabel,QStrTranQStrList("指标上限"));
                        ItemUnitSplit(dataRes[i][idx2],sUpper,sUnit);
                        if(dError>=sLow.toDouble()&&dError<=sUpper.toDouble()){
                            sUnit = "符合指标";
                        }else{
                            sUnit = "不符合指标";
                        }
                    }
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("结论"));
                    dataRes[i][idx2] = sUnit;
                    ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(sUnit));
                }
                else if(sLabel=="参考电平"){
                    int idx2=getIndexByHeader(sLabel,QStrTranQStrList("标准衰减器衰减量"));
                    QString sLow,sUpper,sUnit;
                    //参考点
                    if(i==0){
                        sUnit="参考点";
                    }else{
                        ItemUnitSplit(dataRes[i][idx2],sLow,sUnit);
                        dError = sLow.toDouble();
                        idx2=getIndexByHeader(sLabel,QStrTranQStrList("信号源幅度"));
                        dError = dError+dataRes[0][idx2].toDouble()-dataRes[i][idx2].toDouble();
                        newdError = QString::number(dError, 'f', 2);
                        idx2=getIndexByHeader(sLabel,QStrTranQStrList("测量值"));
                        dataRes[i][idx2] = newdError;
                        ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(newdError+unit));

                        idx2=getIndexByHeader(sLabel,QStrTranQStrList("指标下限"));
                        ItemUnitSplit(dataRes[i][idx2],sLow,sUnit);
                        idx2=getIndexByHeader(sLabel,QStrTranQStrList("指标上限"));
                        ItemUnitSplit(dataRes[i][idx2],sUpper,sUnit);
                        if(dError>=sLow.toDouble()&&dError<=sUpper.toDouble()){
                            sUnit = "符合指标";
                        }else{
                            sUnit = "不符合指标";
                        }
                    }
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("结论"));
                    dataRes[i][idx2] = sUnit;
                    ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(sUnit));
                }
                else if(sLabel=="垂直刻度"){
                    int idx2=getIndexByHeader(sLabel,QStrTranQStrList("标准衰减器衰减量"));
                    QString sLow,sUpper,sUnit;
                    //参考点
                    if(i==0){
                        sUnit="参考点";
                    }else{
                        ItemUnitSplit(dataRes[i][idx2],sUpper,sUnit);
                        ItemUnitSplit(dataRes[0][idx2],sLow,sUnit);
                        dError = sUpper.toDouble()-sLow.toDouble();
                        idx2=getIndexByHeader(sLabel,QStrTranQStrList("信号源幅度"));
                        dError = dError+dataRes[i][idx2].toDouble()-dataRes[0][idx2].toDouble();
                        newdError = QString::number(dError, 'f', 2);
                        idx2=getIndexByHeader(sLabel,QStrTranQStrList("测量值"));
                        dataRes[i][idx2] = newdError;
                        ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(newdError+unit));

                        idx2=getIndexByHeader(sLabel,QStrTranQStrList("指标下限"));
                        ItemUnitSplit(dataRes[i][idx2],sLow,sUnit);
                        idx2=getIndexByHeader(sLabel,QStrTranQStrList("指标上限"));
                        ItemUnitSplit(dataRes[i][idx2],sUpper,sUnit);
                        if(dError>=sLow.toDouble()&&dError<=sUpper.toDouble()){
                            sUnit = "符合指标";
                        }else{
                            sUnit = "不符合指标";
                        }
                    }
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("结论"));
                    dataRes[i][idx2] = sUnit;
                    ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(sUnit));
                }
                else if(sLabel=="平均噪声电平"){
                    int idx2=getIndexByHeader(sLabel,QStrTranQStrList("测量值"));
                    dError = Removeunit(dataRes[i][idx2]).toDouble();
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("修正值"));
                    dError += Removeunit(dataRes[i][idx2]).toDouble();
                    QString sUpper,sUnit;
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("修正后结果"));
                    newdError = QString::number(dError, 'f', 2);
                    dataRes[i][idx2] = newdError;
                    ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(newdError+unit));
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("指标上限"));
                    ItemUnitSplit(dataRes[i][idx2],sUpper,sUnit);
                    if(dError<sUpper.toDouble()){
                        sUnit = "符合指标";
                    }else{
                        sUnit = "不符合指标";
                    }
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("结论"));
                    dataRes[i][idx2] = sUnit;
                    ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(sUnit));
                }
                else if(sLabel=="1db增益压缩点"){
                    int idx2=getIndexByHeader(sLabel,QStrTranQStrList("测量值"));
                    dError = dataRes[i][idx2].toDouble();
                    QString sUpper,sUnit;
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("输入衰减"));
                    ItemUnitSplit(dataRes[i][idx2],sUpper,sUnit);
                    dError -= sUpper.toDouble();
                    newdError = QString::number(dError, 'f', 2);
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("增益压缩"));
                    dataRes[i][idx2] = newdError;
                    ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(newdError+unit));
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("技术指标"));
                    ItemUnitSplit(dataRes[i][idx2],sUpper,sUnit);
                    if(dError>sUpper.toDouble()){
                        sUnit = "符合指标";
                    }else{
                        sUnit = "不符合指标";
                    }
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("结论"));
                    dataRes[i][idx2] = sUnit;
                    ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(sUnit));
                }
                else if(sLabel=="频率显示"||sLabel=="校准信号")
                {
                    double dMean=0;
                    int idx2=getIndexByHeader(sLabel,QStrTranQStrList("测量值1"));
                    dMean += Removeunit(dataRes[i][idx2]).toDouble();
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("测量值2"));
                    dMean += Removeunit(dataRes[i][idx2]).toDouble();
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("测量值3"));
                    dMean += Removeunit(dataRes[i][idx2]).toDouble();
                    dMean/=3;

                    if(sLabel=="频率显示"){
                        newdError=QString::number(dMean, 'f', 6);
                        idx2=getIndexByHeader(sLabel,QStrTranQStrList("均值"));
                    }else{
                        newdError=QString::number(dMean, 'f', 3);
                        idx2=getIndexByHeader(sLabel,QStrTranQStrList("测量结果"));
                    }
                    dataRes[i][idx2] = newdError;
                    ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(newdError+unit));

                    QString sLow,sUpper,sUnit;
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("指标下限"));
                    ItemUnitSplit(dataRes[i][idx2],sLow,sUnit);
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("指标上限"));
                    ItemUnitSplit(dataRes[i][idx2],sUpper,sUnit);
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("结论"));
                    if(dMean>=sLow.toDouble()&&dMean<=sUpper.toDouble()){
                        sUnit = "符合指标";
                    }else{
                         sUnit = "不符合指标";
                    }
                    dataRes[i][idx2] = sUnit;
                    ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(sUnit));
                }
                else if(sLabel=="输入频响"||sLabel=="输入衰减")
                {
                    double dMean=0;
                    int idx2=getIndexByHeader(sLabel,QStrTranQStrList("测量值1"));
                    dMean += Removeunit(dataRes[i][idx2]).toDouble();
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("测量值2"));
                    dMean += Removeunit(dataRes[i][idx2]).toDouble();
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("测量值3"));
                    dMean += Removeunit(dataRes[i][idx2]).toDouble();
                    dMean/=3;

                    newdError=QString::number(dMean, 'f', 2);
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("均值"));
                    dataRes[i][idx2] = newdError;
                    ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(newdError+unit));

                    QString sLow,sUpper,sUnit;
                    //参考点
                    if(i==0){
                        idx2=getIndexByHeader(sLabel,QStrTranQStrList("输入频响"));
                        dataRes[i][idx2] = newdError;
                        ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(newdError+unit));
                        sUnit="参考点";
                    }else{
                        idx2=getIndexByHeader(sLabel,QStrTranQStrList("输入频响"));
                        dError = dataRes[0][idx2].toDouble()-dMean;
                        newdError = QString::number(dError, 'f', 2);
                        dataRes[i][idx2] = newdError;
                        ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(newdError+unit));

                        idx2=getIndexByHeader(sLabel,QStrTranQStrList("指标下限"));
                        ItemUnitSplit(dataRes[i][idx2],sLow,sUnit);
                        idx2=getIndexByHeader(sLabel,QStrTranQStrList("指标上限"));
                        ItemUnitSplit(dataRes[i][idx2],sUpper,sUnit);
                        if(dError>=sLow.toDouble()&&dError<=sUpper.toDouble()){
                            sUnit = "符合指标";
                        }else{
                            sUnit = "不符合指标";
                        }
                    }
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("结论"));
                    dataRes[i][idx2] = sUnit;
                    ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(sUnit));
                }
                else if(sLabel=="谐波失真"){
                    QString sUpper2,sUpper3,sUpper4,sBase,sUnit,sResult;

                    int idx2=getIndexByHeader(sLabel,QStrTranQStrList("二次谐波失真指标上限"));
                    ItemUnitSplit(dataRes[i][idx2],sUpper2,sUnit);
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("三次谐波失真指标上限"));
                    ItemUnitSplit(dataRes[i][idx2],sUpper3,sUnit);
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("四次谐波失真指标上限"));
                    ItemUnitSplit(dataRes[i][idx2],sUpper4,sUnit);

                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("基波幅度"));
                    ItemUnitSplit(dataRes[i][idx2],sBase,sUnit);

                    sResult = "符合指标";
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("二次谐波幅度"));
                    dError = dataRes[i][idx2].toDouble()-sBase.toDouble();
                    newdError=QString::number(dError, 'f', 2);
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("二次谐波失真"));
                    dataRes[i][idx2] = newdError;
                    ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(newdError+"dBc"));
                    if(dError>=sUpper2.toDouble()){
                        sResult = "不符合指标";
                    }
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("三次谐波幅度"));
                    dError = dataRes[i][idx2].toDouble()-sBase.toDouble();
                    newdError=QString::number(dError, 'f', 2);
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("三次谐波失真"));
                    dataRes[i][idx2] = newdError;
                    ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(newdError+"dBc"));
                    if(dError>=sUpper3.toDouble()){
                        sResult = "不符合指标";
                    }
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("四次谐波幅度"));
                    dError = dataRes[i][idx2].toDouble()-sBase.toDouble();
                    newdError=QString::number(dError, 'f', 2);
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("四次谐波失真"));
                    dataRes[i][idx2] = newdError;
                    ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(newdError+"dBc"));
                    if(dError>=sUpper4.toDouble()){
                        sResult = "不符合指标";
                    }
                    idx2=getIndexByHeader(sLabel,QStrTranQStrList("结论"));
                    dataRes[i][idx2] = sResult;
                    ui->tableWidgetCheck->setItem(i,idx2,new QTableWidgetItem(sResult));
                }

            }

        }
    }
    else if(iEquipType==10){
        if(dStdValue==FP_INFINITE){
            dStdValue=InputDialog("请输入被检示值");
        }
        int idx=-1;
        QString data,unit;
        QString ResultCol;
        if(sLabel=="频率" || sLabel=="剩余调频" ||  sLabel=="调相相偏"){
           idx= getIndexByHeader(sLabel,QStrTranQStrList("指标上限"));
           ItemUnitSplit(dataRes[i][idx],data,unit);
           if(sLabel=="剩余调频" ){
                ResultCol=sLabel;
                newdStdValue=QString::number(dStdValue,'f',4);
           }else if(sLabel=="调相相偏"){
               ResultCol="测量值";
               newdStdValue=QString::number(dStdValue,'f',4);
           }else{
               ResultCol="测量值";
               newdStdValue=TranDecimals(sLabel,dataRes[i],dStdValue,"指标上限");
           }
           idx= getIndexByHeader(sLabel,QStrTranQStrList(ResultCol));
           dataRes[i][idx]=newdStdValue+unit;
           ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue+unit)); //测量值
        }else if(sLabel=="调幅深度" || sLabel=="功率" || sLabel=="调频频偏" ){
           idx= getIndexByHeader(sLabel,QStrTranQStrList("标称值"));
           ItemUnitSplit(dataRes[i][idx],data,unit);
           if(sLabel=="调频频偏"){
                newdStdValue=QString::number(dStdValue,'f',4);
            }else{
                newdStdValue=TranDecimals(sLabel,dataRes[i],dStdValue);
           }
           idx= getIndexByHeader(sLabel,QStrTranQStrList("测量值"));
           dataRes[i][idx]=newdStdValue+unit;
           ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue+unit)); //测量值
        }else if(sLabel=="剩余调幅"){
            unit="%";
            newdStdValue=QString::number(dStdValue,'f',2);
            idx= getIndexByHeader(sLabel,QStrTranQStrList(sLabel));
            dataRes[i][idx]=newdStdValue+unit;
            ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue+unit)); //测量值
         }else if (sLabel=="谐波") {
            idx= getIndexByHeader(sLabel,QStrTranQStrList("幅度"));
            ItemUnitSplit(dataRes[i][idx],data,unit);
            newdStdValue=TranDecimals(sLabel,dataRes[i],dStdValue,"幅度");
            idx= getIndexByHeader(sLabel,QStrTranQStrList("基波幅度"));
            ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue+unit)); //测量值
            dataRes[i][idx]=newdStdValue+unit;
            //采集谐波
            //找到频率值
            idx= getIndexByHeader(sLabel,QStrTranQStrList("频率"));
            QString freq,newunit,HarmonicWaveValue;
            double freqtemp;
            ItemUnitSplit(dataRes[i][idx],freq,newunit);
            if(standardEngine.getState()){
                //找到谐波次数值
                idx= getIndexByHeader(sLabel,QStrTranQStrList("谐波次数"));
                if(dataRes[i][idx].contains("/")){
                    freqtemp=freq.toDouble()*(1/dataRes[i][idx].mid(1,2).toDouble());
                }else{
                    freqtemp=freq.toDouble()*dataRes[i][idx].toDouble();
                }
                InstructionLib *instrcutLibstan=getCmdByCheckName(1,sLabel);
                InstructionLib tempstan;
                QString cmd;
                for(QString item:instrcutLibstan->instruct_config.split(";")){
                    if(item.contains("{FREQ}")){
                        cmd=item;
                        cmd.replace("{FREQ}",QString::number(freqtemp,'f',3));
                    }
                }
                tempstan.instruct_config=cmd;
                SendDevice(&tempstan,&standardEngine);
                HarmonicWaveValue= ReadDevice(instrcutLibstan,&standardEngine);
            }else{
                dStdValue=InputDialog("请输入谐波幅度值");
                HarmonicWaveValue=QString::number(dStdValue,'f',2);
            }
            idx= getIndexByHeader(sLabel,QStrTranQStrList("谐波幅度"));
            ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(HarmonicWaveValue+unit)); //测量值
            dataRes[i][idx]=HarmonicWaveValue+unit;
            unit="dBc";
            idx= getIndexByHeader(sLabel,QStrTranQStrList("测量值"));
            QString Res=QString::number(HarmonicWaveValue.toDouble()-newdStdValue.toDouble());
            ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(Res+unit)); //测量值
            dataRes[i][idx]=(newdStdValue.toDouble()-HarmonicWaveValue.toDouble())+unit;


         }else if (sLabel=="非谐波") {
             unit ="dBc";
             newdStdValue=QString::number(dStdValue,'f',1);
             idx= getIndexByHeader(sLabel,QStrTranQStrList("测量值"));
             ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue+unit)); //测量值
             dataRes[i][idx]=newdStdValue+unit;
         }

        idx= getIndexByHeader(sLabel,QStrTranQStrList(("相对误差")));
        if(idx!=-1)
        {
            int idx2=getIndexByHeader(sLabel,QStrTranQStrList(("标准值")));
            if(idx2==-1)
                idx2=getIndexByHeader(sLabel,QStrTranQStrList("标称值"));
            if(idx2!=-1){
               double dTmp = dataRes[i][idx2].toDouble();
               dError = (dStdValue-dTmp);
               newdError=TranDecimals(sLabel,dataRes[i],dError);
               ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdError));
               dataRes[i][idx]=newdError;
            }

        }
        BaseCommonApi::SaveLogInfo(1,QString("转换前 测试值 %1").arg(dStdValue));
        BaseCommonApi::SaveLogInfo(1,QString("转换后 测试值 %1").arg(newdStdValue));
        if(sLabel=="调频频偏" || sLabel=="调相相偏"|| sLabel=="调幅深度"){
            //获取THD值
            unit="%";
            InstructionLib* instrcutLibTHD= getCmdByCheckName(0,"THD");
            if(instrcutLibTHD==nullptr)
            {
                dStdValue= dStdValue=InputDialog("请输入THD值");
                if(sLabel=="调相相偏"){
                   newdStdValue=QString::number(dStdValue,'f',3);
                }else{
                   newdStdValue=QString::number(dStdValue,'f',2);
                }
            }else{
                QString ResultThd=ReadDevice(instrcutLibTHD,&deviceEngine);
                dStdValue = ResultThd.toDouble();
                if(sLabel=="调相相偏"){
                   newdStdValue=QString::number(dStdValue,'f',3);
                }else{
                   newdStdValue=QString::number(dStdValue,'f',2);
                }

            }
            idx= getIndexByHeader(sLabel,QStrTranQStrList("THD"));
            dataRes[i][idx]=newdStdValue+unit;
            ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue+unit));
            BaseCommonApi::SaveLogInfo(1,QString("转换前 THD 测试值 %1").arg(dStdValue));
            BaseCommonApi::SaveLogInfo(1,QString("转换后 THD 测试值 %1").arg(newdStdValue));
        }
    }
    else if(iEquipType==11){
        bool State=true;
        if(dStdValue==FP_INFINITE){
            dStdValue=InputDialog("请输入被测量值");
            State=false;
        }
        int idx=-1;
        int idx2=-1;
        double Upp,low;
        QString Resdata,data,unit;
        unit="dBm";
        newdStdValue=QString::number(dStdValue,'f',2);
        idx= getIndexByHeader(sLabel,QStrTranQStrList("直通测量值"));
        dataRes[i][idx]=newdStdValue+unit;
        ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue+unit)); //测量值
        if(State){
            idx= getIndexByHeader(sLabel,QStrTranQStrList("组合方式"));
            if(!this->showDialog("加入衰减测量值提醒",QString("组合方式 %1").arg(dataRes[i][idx])))
            {
                return;
            }
            BaseCommonApi::SaveLogInfo(1,QString("测量接收机"));
            VisaCommonEngine engineRole=getVisaCommbyRoleName("测量接收机"); //角色
            InstructionLib *instrcutLibRole=getCmdByRoleName("测量接收机",constsLable);
            SendDevice(instrcutLibRole,&engineRole);
            dStdValue=ReadDevice(instrcutLibRole,&engineRole).toDouble();
        }else{
            dStdValue=InputDialog("请输入加入衰减测量值");
        }
        Resdata=QString::number(newdStdValue.toDouble()-dStdValue);
        newdStdValue=QString::number(dStdValue,'f',2);
        idx= getIndexByHeader(sLabel,QStrTranQStrList("加入衰减测量值"));
        dataRes[i][idx]=newdStdValue+unit;
        ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue+unit)); //测量值
        idx= getIndexByHeader(sLabel,QStrTranQStrList("标称值"));
        ItemUnitSplit(dataRes[i][idx],data,unit);
        idx= getIndexByHeader(sLabel,QStrTranQStrList("衰减量"));
        dataRes[i][idx]=Resdata+unit;
         ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(Resdata+unit)); //衰减量
        idx= getIndexByHeader(sLabel,QStrTranQStrList("指标上限"));
        idx2= getIndexByHeader(sLabel,QStrTranQStrList("指标下限"));

        Upp=Removeunit(dataRes[i][idx]).toDouble();
        low=Removeunit(dataRes[i][idx2]).toDouble();
        idx= getIndexByHeader(sLabel,QStrTranQStrList("结论"));
        if(Resdata.toDouble()>low && Resdata.toDouble()<Upp){
             dataRes[i][idx]="符合指标";
             ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem("符合指标"));
        }else{
            dataRes[i][idx]="不符合";
            ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem("不符合"));
        }

    }
    else if(iEquipType==16){
        if(dStdValue==FP_INFINITE){
            dStdValue=InputDialog("请输入被检示值");
        }
        int idx=-1;
        int idx2=-1;
        QString data,unit,dataTemp;
        bool datum=false;
       // QString Resultdata,Resiltunit;
        if(sLabel=="频率" || sLabel=="交流电压" || sLabel=="直流电压"){
             idx= getIndexByHeader(sLabel,QStrTranQStrList("标称值"));
             ItemUnitSplit(dataRes[i][idx],data,unit);
             if(sLabel=="交流电压" || sLabel=="直流电压"){
                newdStdValue=QString::number(dStdValue,'f',3);
             }else{
                idx= getIndexByHeader(sLabel,QStrTranQStrList("有效位数"));
                newdStdValue=QString::number(dStdValue,'f',dataRes[i][idx].toInt());
             }

             idx= getIndexByHeader(sLabel,QStrTranQStrList("测量值"));
             dataRes[i][idx]=newdStdValue+unit;
             ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue+unit)); //测量值
        }else if(sLabel=="上升时间"){
            //寻找单位
            idx= getIndexByHeader(sLabel,QStrTranQStrList("技术指标"));
            int lastDigitIndex = dataRes[i][idx].lastIndexOf(QRegExp("[0-9]"));
            if(lastDigitIndex!=-1){
                //寻找单位
                unit=dataRes[i][idx].mid(lastDigitIndex + 1);
            }else{
                unit="";
            }
            newdStdValue=QString::number(dStdValue,'f',2);
            idx= getIndexByHeader(sLabel,QStrTranQStrList(sLabel));
            dataRes[i][idx]=newdStdValue+unit;
            ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue+unit)); //上升时间
        }else if(sLabel=="正弦信号平坦度"){
            idx= getIndexByHeader(sLabel,QStrTranQStrList("电压"));
            ItemUnitSplit(dataRes[i][idx],data,unit);
            newdStdValue=QString::number(dStdValue,'f',3);
            idx= getIndexByHeader(sLabel,QStrTranQStrList("测量值"));
            dataRes[i][idx]=newdStdValue+unit;
            ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue+unit)); //测量值

            idx= getIndexByHeader(sLabel,QStrTranQStrList("基准点"));
            if(dataRes[i][idx]=="1"){
            idx= getIndexByHeader(sLabel,QStrTranQStrList("平坦度"));
            dataRes[i][idx]=newdStdValue;
            ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue+unit)); //平坦度
            }else{
                idx2= getIndexByHeader(sLabel,QStrTranQStrList("测量值"));
                //寻找基准点测量值
                for(QStringList item:dataRes){
                    if(item[idx]=="1"){
                        data=item[idx2];
                        datum=true;
                    }
                }
                idx= getIndexByHeader(sLabel,QStrTranQStrList("平坦度"));
                dataRes[i][idx]=QString::number((dataRes[i][idx].toDouble()-data.toDouble())/data.toDouble());
            }
            dataTemp="不符合";
            idx=getIndexByHeader(sLabel,QStrTranQStrList("技术指标"));
            idx2= getIndexByHeader(sLabel,QStrTranQStrList("平坦度"));
            data=dataRes[i][idx];
            if(datum){
                dataTemp="基准点";
            }else{
                if(data.contains("<")){
                    QRegularExpression re("[^\\d]+");
                    data=data.replace(re, "");
                    if(dataRes[i][idx2].toDouble()<data.toDouble()){
                      dataTemp="符合指标";
                    }else{
                      dataTemp="不符合";
                    }
                }
            }
            idx=getIndexByHeader(sLabel,QStrTranQStrList("结论"));
            dataRes[i][idx]=dataTemp;
            ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(dataTemp)); //结论
        }
         else if(sLabel=="总谐波失真"){
            unit = "%";
            idx2= getIndexByHeader(sLabel,QStrTranQStrList("测量值"));
            newdStdValue=QString::number(dStdValue,'f',3);
            ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(newdStdValue+unit)); //测量值

            dataTemp="不符合";
            idx=getIndexByHeader(sLabel,QStrTranQStrList("技术指标"));
            data=dataRes[i][idx];
            if(data.contains("<")){
                QRegularExpression re("[^\\d]+");
                data=data.replace(re, "");
                if(dataRes[i][idx2].toDouble()<dStdValue){
                  dataTemp="符合指标";
                }else{
                  dataTemp="不符合";
                }
            }

            idx=getIndexByHeader(sLabel,QStrTranQStrList("结论"));
            ui->tableWidgetCheck->setItem(i,idx,new QTableWidgetItem(dataTemp)); //结论
        }
    }
}
void CheckWindow::Oscilloscopetextflow()
{
    connectDevice();

    for (int _ch=0;_ch<chList.count();_ch++)
    {
      if(!this->showDialog("通道切换提示",QString("请切换 %1 通道").arg(chList[_ch]))){continue;}
      for(int i=0;i<headTableList->keys().length();i++)
      {
             BaseCommonApi::SaveLogInfo(1,QString("---------------------------------------------------"));
          //首先检索当前检定项目的通道
          initCheckTable(i);
          constsLable = headTableList->keys()[i];
          QList<QStringList> dataRes = caliDataList->value(constsLable);
          BaseCommonApi::SaveLogInfo(1,QString("开始 %1 %2 测试").arg(chList[_ch]).arg(constsLable));
          QString sParam;
          QString ValueUnit;
          double TopValue,BottomValue;
          InstructionLib *instrcutLibOhm;
          for (int i = 0; i < dataRes.count(); ++i) {
               TextCount=1;
               constDataRes=dataRes[i];
               InItcmd();
               int _dex= getIndexByHeader(constsLable,QStrTranQStrList("通道"));
               if(_dex==-1){sParam=chList[_ch];}else{sParam=dataRes[i][_dex];}
               if(sParam!=chList[_ch]){continue;};
               CheckBackColorUpdate(true,i);
               QString sRet="";
               QString newCmd;
               double dStdValue=FP_INFINITE;
               QString newdStdValue;
               int idx= getIndexByHeader(constsLable,QStrTranQStrList("检定项目"));
               QString sParam;
               if(idx!=-1)
                   sParam = dataRes[i][idx];
               InstructionLib *instrcutLibbiz=getCmdByCheckName(0,sParam);
               InstructionLib *instrcutLibstan=getCmdByCheckName(1,sParam);
               int idxitem= getIndexByHeader(constsLable,QStrTranQStrList("项目"));
               //INIT 被检设备 - 标准设备
               instrcutLibOhm= getCmdByCheckName(0,"电阻");
               if(deviceEngine.getState())
                  {
                      //设置电阻值+
                      instrcutLibOhm= getCmdByCheckName(0,"电阻");
                      if(instrcutLibOhm!=nullptr){
                        QStringList ohmList = instrcutLibOhm->instruct_config.split(";");
                        int idxohm= getIndexByHeader(constsLable,QStrTranQStrList("阻抗"));
                        if(sParam=="输入电阻"){
                            if(!this->showDialog("输入电阻","请接入电阻测试线")){continue;}
                            idxohm= getIndexByHeader(constsLable,QStrTranQStrList("标称值"));
                        }

                        if(idxohm!=-1)
                        {
                             QString onmname="50Ω";
                             QString CmdOhm;
                             onmname=dataRes[i][idxohm];
                             if(onmname=="50Ω"){CmdOhm=ohmList[0];}
                             else if(onmname=="75Ω"){CmdOhm=ohmList[1];}
                             else{
                                 if(ohmList.count()>2){CmdOhm=ohmList[2];}else{CmdOhm=ohmList[1];};
                             }
                             newCmd = transCmd(CmdOhm,constsLable,constDataRes,true);
                             deviceEngine.sendData(newCmd);
                        }
                      }
                  }

               if(sParam=="频带宽度"){TextCount=2;};
               for (int d=0;d<TextCount;d++) {
               //标准器输出
               BaseCommonApi::SaveLogInfo(1,QString("标准器输出"));
               if(d==1 && sParam=="频带宽度"){
                   QStringList sList = instrcutLibstan->instruct_config.split(";");
                   for(QString &sCmd:sList){
                       sCmd.replace("{BW}","50000");
                       break;
                   }
               }
               QString LEVEL;
               QString STVAL;
               if(d==1 && sParam.contains("触发灵敏度")){
                   QStringList sList = instrcutLibstan->instruct_config.split(";");
                   for(QString &sCmd:sList){
                       if(sCmd.contains("{LEVEL}")){
                             LEVEL=sCmd;
                             sCmd.replace("{LEVEL}","0");
                       }
                       else if(sCmd.contains("{STVAL}"))
                       {
                             STVAL=sCmd;
                             sCmd.replace("{STVAL}","1");
                       }

                   }
               }
               if(sParam!="校准信号")
               {
               SendDevice(instrcutLibstan,&standardEngine);
               //被检设备读数
               BaseCommonApi::SaveLogInfo(1,QString("被检设备输出"));
               SendDevice(instrcutLibbiz,&deviceEngine);
               sRet=ReadDevice(instrcutLibbiz,&deviceEngine);
               }
               if(sParam=="输入电阻"){
                   BaseCommonApi::SaveLogInfo(1,QString("数字多用表控制"));
                   VisaCommonEngine tempEngine=getVisaCommbyRoleName("数字多用表"); //角色
                   InstructionLib *instrcutLibNum=getCmdByRoleName("数字多用表",sParam);
                   sRet=ReadDevice(instrcutLibNum,&tempEngine);
               }
               if(sParam=="校准信号"){
                   if(dataRes[i][idxitem]=="频率"){
                       if(!this->showDialog("输入电阻","请接入频率测试线")){continue;}
                       BaseCommonApi::SaveLogInfo(1,QString("通用计数器控制"));
                       VisaCommonEngine tempEngine=getVisaCommbyRoleName("通用计数器"); //角色
                       InstructionLib *instrcutLibNum=getCmdByRoleName("通用计数器",sParam);
                       //SendDevice(instrcutLibbiz,&deviceEngine);
                       sRet=ReadDevice(instrcutLibNum,&tempEngine);
                   }else if(dataRes[i][idxitem]=="幅度"){
                        if(!this->showDialog("校准信号幅度","请接入幅度测试线")){continue;}
                       instrcutLibbiz=getCmdByCheckName(0,"校准信号幅度1");
                       //初始化被检
                       SendDevice(instrcutLibbiz,&deviceEngine);
                       //获取高值
                       TopValue=ReadDevice(instrcutLibbiz,&deviceEngine).toDouble();
                       //获取低值
                       instrcutLibbiz=getCmdByCheckName(0,"校准信号幅度2");
                       SendDevice(instrcutLibbiz,&deviceEngine);
                       BottomValue=ReadDevice(instrcutLibbiz,&deviceEngine).toDouble();
                       //控制标准器
                       if(!this->showDialog("校准信号幅度","请接入9500测试线")){continue;}
                       //QString config=instrcutLibstan->instruct_config;
                       InstructionLib instrcutLibtest;
                       for(int d=0;d<=1;d++){
                           instrcutLibtest.instruct_config=instrcutLibstan->instruct_config;
                           QString StanValue;
                           double proc;
                           double values;
                           for (QString value:instrcutLibtest.instruct_config.split(';')) {
                               if(value.contains("{STVAL}")){
                                   StanValue=value;
                                   STVAL=value;
                                   break;
                               }
                           }
                           if(d==0){
                              instrcutLibtest.instruct_config=
                                      instrcutLibtest.instruct_config.replace("{STVAL}",QString::number(TopValue,'f',4));
                               proc=TopValue;
                               values=TopValue;
                           }else{
                               instrcutLibtest.instruct_config=
                                       instrcutLibtest.instruct_config.replace("{STVAL}",QString::number(BottomValue,'f',4));
                               proc=BottomValue;
                               values=BottomValue;
                           }
                           //测量值测试
                           int count=0;
                           double lastvlaue;
                           double Sub=0.001;
                           SendDevice(&instrcutLibtest,&standardEngine);
                           //获取幅度值
                           instrcutLibbiz=getCmdByCheckName(0,"校准信号幅度3");
                           SendDevice(instrcutLibbiz,&deviceEngine);
                           Delay_MSec(2000);
                           double tempvalue=ReadDevice(instrcutLibbiz,&deviceEngine).toDouble();
                           QString num;

                           while(tempvalue<values-0.005){
                                 proc+=Sub;
                                 num=QString::number(proc,'f',3);
                                 StanValue.replace("{STVAL}",QString::number(proc));
                                 instrcutLibtest.instruct_config=StanValue;
                                 StanValue.replace(QString::number(proc),"{STVAL}");
                                 SendDevice(&instrcutLibtest,&standardEngine);
                                 Delay_MSec(500);
                                 lastvlaue=ReadDevice(instrcutLibbiz,&deviceEngine).toDouble();
                                 if(lastvlaue>values || count>50){
                                     break;
                                     values=proc;
                                 }
                                 tempvalue=lastvlaue;
                                 count++;
                           }
                           if(d==0){TopValue=values;}else{BottomValue=values;}

                       }
                       int topidx= getIndexByHeader(constsLable,QStrTranQStrList("顶部测量值"));
                       int bomidx= getIndexByHeader(constsLable,QStrTranQStrList("底部测量值"));
                       dataRes[i][topidx]=QString::number(TopValue);
                       dataRes[i][bomidx]=QString::number(BottomValue);
                       sRet=QString::number(TopValue-BottomValue);
                   }
               }
               if(sParam.contains("触发灵敏度")){
                   int count=0;
                  idx= getIndexByHeader(constsLable,QStrTranQStrList("触发类型"));
                  if(dataRes[i][idx]=="内触发"){
                      idx= getIndexByHeader(constsLable,QStrTranQStrList("标称值"));
                      QString value=dataRes[i][idx];
                      QString data,unit;
                      ItemUnitSplit(value,data,unit);
                      double step=0.01;
                      double textvalue=1*data.toDouble();
                      QString tempSTVAL;
                      tempSTVAL=STVAL.replace("{STVAL}",QString::number(textvalue));
                      instrcutLibstan->instruct_config=tempSTVAL;
                      SendDevice(instrcutLibstan,&standardEngine);
                      //是否调节电平--
                      //
                      while(sRet=="TRIGGER" && count<20){
                         textvalue-=step;
                         tempSTVAL=STVAL.replace("{STVAL}",QString::number(textvalue));
                         instrcutLibstan->instruct_config=tempSTVAL;
                         SendDevice(instrcutLibstan,&standardEngine);
                         sRet=ReadDevice(instrcutLibbiz,&deviceEngine);
                         //是否调节电平-- 或延迟
                         //
                         count++;
                      }
                      sRet=textvalue;
                  }else{
                      QString data,unit;
                      double textvalue;
                      QString TempSTVAL;
                      idx= getIndexByHeader(constsLable,QStrTranQStrList("标称值"));
                      double Step=0.005;
                      ItemUnitSplit(dataRes[i][idx],data,unit);
                      data=transUnit(data,unit,true);
                      textvalue=data.toDouble();
                      for(int i=0;i<5;i++){
                          sRet=ReadDevice(instrcutLibbiz,&deviceEngine);
                          if(sRet=="TRIGGER"){break;};
                      }
                      if(sRet=="TRIGGER"){
                          sRet=QString::number(textvalue);
                      }else{
                          count=0;
                          while(sRet!="TRIGGER" && count<20){
                              textvalue+=Step;
                              TempSTVAL=STVAL.replace("{STVAL}",QString::number(textvalue));
                              instrcutLibstan->instruct_config=TempSTVAL;
                              SendDevice(instrcutLibstan,&standardEngine);
                              sRet=ReadDevice(instrcutLibbiz,&deviceEngine);
                              //是否调节电平-- 或延迟
                              //
                              count++;
                          }
                      }
                  }

               }
               //测试结果单位转换
               if(sRet!=""){
                   idx= getIndexByHeader(constsLable,QStrTranQStrList("单位"));
                   //ValueUnit
                   if(idx!=-1)
                   {
                      ValueUnit=dataRes[i][idx];
                   }else
                   {
                      QStringList temp;
                      temp.append("标准值");
                      temp.append("标称值");
                      int idx= getIndexByHeader(constsLable,temp);
                      if(idx!=-1)
                           ItemUnitSplit(dataRes[i][idx],ValueUnit,ValueUnit);

                   }
                   dStdValue = transUnit(sRet,ValueUnit);
               }

               autoAdujstData(constsLable,i,dStdValue,dataRes);
               CheckBackColorUpdate(false,i);
               closeCmd();
               Delay_MSec(2000);

               }
               //CLOSE 被检设备 - 标准设备



          }

          caliDataList->insert(constsLable,dataRes);

      }
      closeDevice();
  }
}
void CheckWindow::Delay_MSec(unsigned int msec)
{
    QTime _Timer = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < _Timer )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
void CheckWindow::SignalGeneratortextflow(){
    Teststate=SyncRunStatus::Running;
    if(headTableList->keys().count()==0)
        return;
    BaseCommonApi::SaveLogInfo(1,QString("-----------------------------------"));
    connectDevice();
    for(int i=0;i<headTableList->keys().length();i++)
    {
        if(Teststate==SyncRunStatus::Stopped){break;};
        //每次循环按钮变化
        initCheckTable(i);
        constsLable= headTableList->keys()[i];
        QList<QStringList> dataRes = caliDataList->value(constsLable);
        BaseCommonApi::SaveLogInfo(1,QString("开始 %1 测试").arg(constsLable));
        for (int i = 0; i < dataRes.count(); ++i) {
            if(Teststate==SyncRunStatus::Stopped){break;};
            constDataRes=dataRes[i];
            InItcmd();
            QString sRet="";
            QString newCmd;
            double dStdValue=FP_INFINITE,dError=0;
            QString newdStdValue,newdError;
            //获取检定项目
            BaseCommonApi::SaveLogInfo(1,QString("被检设备设置"));
            QString sParam,TempValue;
            TempValue.append("检定项目");
            int idx= getIndexByHeader(constsLable,QStrTranQStrList(TempValue));
            if(idx!=-1)
                 sParam = dataRes[i][idx];
            //背景颜色显示
            CheckBackColorUpdate(true,i);
            //首先设置被检设备输出

            InstructionLib *instrcutLib = getCmdByCheckName(0,sParam);
            SendDevice(instrcutLib,&deviceEngine,true);
            //设备标准仪器
            BaseCommonApi::SaveLogInfo(1,QString("标准仪器输出"));
            if(iEquipType==16){
                InstructionLib *instReceiver=getCmdByRoleName("测量接收机",constsLable);
                VisaCommonEngine EngineRec=getVisaCommbyRoleName("测量接收机");
                InstructionLib *instVoltmeter=getCmdByRoleName("电压表",constsLable);
                VisaCommonEngine EngineVolt=getVisaCommbyRoleName("电压表");
                InstructionLib *instOSC=getCmdByRoleName("示波器",constsLable);
                VisaCommonEngine EngineOSC=getVisaCommbyRoleName("示波器");
                InstructionLib *instUPV=getCmdByRoleName("UPV",constsLable);
                VisaCommonEngine EngineUPV=getVisaCommbyRoleName("UPV");
                if(sParam=="频率"){
                    SendDevice(instReceiver,&EngineRec);
                    sRet=ReadDevice(instReceiver,&EngineRec);
                }else if(sParam=="交流电压" || sParam=="直流电压"){
                    SendDevice(instOSC,&EngineVolt);
                    sRet=ReadDevice(instOSC,&EngineVolt);
                }else if(sParam=="上升时间"){
                    SendDevice(instVoltmeter,&EngineOSC);
                    sRet=ReadDevice(instVoltmeter,&EngineOSC);
                }else if(sParam=="谐波失真"){
                    SendDevice(instUPV,&EngineUPV);
                    sRet=ReadDevice(instUPV,&EngineUPV);
                }
            }else{
                InstructionLib *instrcutLibstan=getCmdByCheckName(1,sParam);
                SendDevice(instrcutLibstan,&standardEngine);
                if(sParam=="功率"){
                    bool IsCli=true;
                    if(i!=0){
                        idx = getIndexByHeader(constsLable,QStrTranQStrList("频率"));
                        QString FreqValue,PageFreqValue;
                        FreqValue=Removeunit(dataRes[i][idx]);
                        PageFreqValue=Removeunit(dataRes[i-1][idx]);
                        if(PageFreqValue==FreqValue){
                            IsCli=false;
                        }
                    }
                    if(IsCli){
                        //&& 只在频率切换时进行校准
                        //功率校准
                        InstructionLib *PowerPage=getCmdByCheckName(1,"界面切换");
                        InstructionLib *PowerCali=getCmdByCheckName(0,"校准");
                        InstructionLib *PowerCaliSt=getCmdByCheckName(1,"校准");
                        if(PowerCali!=nullptr && PowerCaliSt!=nullptr && PowerCaliSt!=nullptr){
                            InstructionLib Temp;
                            InstructionLib TempSt;
                            for(QString item:PowerCali->instruct_read.split(';'))
                            {
                                Temp.instruct_config=item;
                                SendDevice(&Temp,&deviceEngine);
                                if(item.contains("20")){
                                    TempSt.instruct_config=PowerPage->instruct_config.split(";")[0];
                                    SendDevice(&TempSt,&standardEngine);
                                    TempSt.instruct_config=PowerCaliSt->instruct_config.split(";")[0];
                                    SendDevice(&TempSt,&standardEngine);
                                }else{
                                    TempSt.instruct_config=PowerPage->instruct_config.split(";")[1];
                                    SendDevice(&TempSt,&standardEngine);
                                    TempSt.instruct_config=PowerCaliSt->instruct_config.split(";")[1];
                                    SendDevice(&TempSt,&standardEngine);
                                }
                            }
                        }
                    }
                }

                sRet=ReadDevice(instrcutLib,&standardEngine);
            }

            if(sRet!=""){
                TempValue.clear();
                TempValue.append("单位");
                idx= getIndexByHeader(constsLable,QStrTranQStrList(TempValue));
                dStdValue = transUnit(sRet,dataRes[i][idx]);
            }

            autoAdujstData(constsLable,i,dStdValue,dataRes);
            CheckBackColorUpdate(false,i);
            closeCmd();
            Delay_MSec(2000);
        }
        caliDataList->insert(constsLable,dataRes);
    }
    closeDevice();
}
void CheckWindow::on_pushButton_pause_clicked()
{

}

void CheckWindow::on_pushButton_stop_clicked()
{
    Teststate=SyncRunStatus::Stopped;

    if(iEquipType==5){
        if(timerChart->isActive())
            timerChart->stop();
    }

}

void CheckWindow::on_pushButton_clicked()
{
    getCheckDevice(false);
}

void CheckWindow::on_pushButton_2_clicked()
{
    ui->comboBoxName->setCurrentIndex(0);
    ui->comboBoxModal->setCurrentIndex(0);
    ui->comboBoxCustomer->setCurrentIndex(0);
    ui->lineEditNo->setText("");
    getCheckDevice(false);
}

void CheckWindow::on_pushButton_10_clicked()
{
    getCheckMethod();
}

void CheckWindow::on_pushButton_11_clicked()
{
    ui->lineEditName->setText("");
    ui->lineEditCreator->setText("");
    getCheckMethod();
}

void CheckWindow::on_pushButton_4_clicked()
{
    updateStandardTable();
}

void CheckWindow::on_pushButton_5_clicked()
{
    ui->comboBoxName_2->setCurrentIndex(0);
    ui->comboBoxModal_2->setCurrentIndex(0);
    ui->lineEditNo_2->setText("");
    ui->lineEdit_4->setText("");
    updateStandardTable();
}

void CheckWindow::on_pushButton_6_clicked()
{
    showDevice();
}

void CheckWindow::on_pushButton_7_clicked()
{
    FormCheckPoint *checkWindow = new FormCheckPoint(nullptr,selectTaskList[0].sample_id);
    checkWindow->setWindowState(Qt::WindowMaximized);
    checkWindow->setWindowModality(Qt::ApplicationModal);
    checkWindow->show();
}

void CheckWindow::on_pushButton_8_clicked()
{
    ConnectInfo& item = connectList[currConnectIdx];
    if(item.programInterface.id==""){
        BaseCommonApi baseApi;
        qint64 id = baseApi.generateId();
        item.programInterface.id = QString::number(id);
        item.programInterface.device_id = item.id;
        if(ui->radioButton_serial->isChecked())
            item.programInterface.communication_type = "SerialPort";
        else
            item.programInterface.communication_type = "Visa";
        item.programInterface.serial_port = QString("%1;%2;%3;%4;%5").arg(ui->comboBox_port->currentText()).arg(ui->comboBox_baurt->currentText()).arg(ui->comboBox_check->currentText())
                .arg(ui->comboBox_data->currentText()).arg(ui->comboBox_stop->currentText());
        item.programInterface.visa = ui->comboBox_visa->currentText();
        if(BaseCommonApi::InsertVerificationProgramConfigInterface(item.programInterface)){
            QMessageBox::information(this, "提示", "保存成功！");
        }
        else
        {
            QMessageBox::warning(this, "警告", "保存失败！");
        }
    }else{
        if(ui->radioButton_serial->isChecked())
            item.programInterface.communication_type = "SerialPort";
        else
            item.programInterface.communication_type = "Visa";
        item.programInterface.serial_port = QString("%1;%2;%3;%4;%5").arg(ui->comboBox_port->currentText()).arg(ui->comboBox_baurt->currentText()).arg(ui->comboBox_check->currentText())
                .arg(ui->comboBox_data->currentText()).arg(ui->comboBox_stop->currentText());
        item.programInterface.visa = ui->comboBox_visa->currentText();
        if(BaseCommonApi::UpdateVerificationProgramConfigInterface(item.programInterface)){
             QMessageBox::information(this, "提示", "保存成功！");
        }else{
            QMessageBox::warning(this, "警告", "保存失败！");
        }
    }
}


void CheckWindow::on_comboBox_visa_OnCurrentTextChanged(QString arg1)
{
     //lab_deviceinfo
    getJoinState(arg1);

}
bool CheckWindow::getJoinState(QString arg1)
{
    bool IsSucc=false;
    QString visaAdd=arg1;
    QStringList parts = engine.getdeviceIdninfo(visaAdd).split(",");


    if(parts.contains("false"))
    {
        ui->lab_deviceinfo->setText("无连接");
    }else{
        if(parts.count()>2)
        {
            ui->lab_deviceinfo->setText(parts[0]+"-"+parts[1]);

        }
        IsSucc=true;
    }
    return IsSucc;
}
void CheckWindow::ItemUnitSplit(QString Value,QString &data,QString &unit)
{
    QString sData2=Value;
    QString  sNewData,sUnit;
    // 正则表达式匹配数字
    int lastDigitIndex = sData2.lastIndexOf(QRegExp("[0-9]"));
    if(lastDigitIndex!=-1){
        data=sData2.mid(0,lastDigitIndex+1);
        unit=sData2.mid(lastDigitIndex+1);
    }else{

        QRegularExpression reLetters("[a-zA-ZΩΜΩ%]");
        QRegularExpression reLetters1("[^a-zA-ZΩΜΩ%]");
        data=sData2.replace(reLetters,"");
        unit= Value.replace(reLetters1,"");

    }


}
void CheckWindow::CheckBackColorUpdate(bool State,int row)
{
    QBrush colorBrush(Qt::white);
    if(State)
    {
        QColor lightGreenColor(173, 216, 230);
        colorBrush.setColor(lightGreenColor);
    }
    for (int col = 0; col < ui->tableWidgetCheck->columnCount(); ++col) {
        QTableWidgetItem *item = ui->tableWidgetCheck->item(row, col);
        if (item) {
            item->setBackground(colorBrush);
        }
    }
}
QString CheckWindow::Removeunit(QString data)
{
    QRegularExpression re("[a-zA-ZΩΜΩ%]");
    return data.replace(re,"");
}
bool CheckWindow::showDialog(QString title,QString mess)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, title, mess,
                                   QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        return  true;
    } else {
        return  false;
    }
}
QMap<QStringList, QStringList> CheckWindow::readDataFromFile(QString filePath) {
    // 获取应用程序的根目录
    QString path = QCoreApplication::applicationDirPath()+"\\File\\config\\"+filePath;
    QMap<QStringList, QStringList> dataMap;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // 错误处理
        qDebug() << "无法打开文件：" << file.errorString();
        QString str=file.errorString();
        return dataMap;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString line;
    bool isFirstLine = true; // 跳过第一行

    while (!in.atEnd()) {
        line = in.readLine();
        if (isFirstLine) {
            isFirstLine = false; // 第一行已读，接下来读取数据
            continue;
        }

        // 使用'|'分割键和值
        QStringList keyValuePairs = line.split(QRegExp("\\s*\\|\\s*"), QString::SkipEmptyParts);
        if (keyValuePairs.size() != 2) {
            continue; // 跳过不符合格式的行
        }

        QStringList keys, values;

        // 分割键部分，并替换空格为空字符串
        QString keyPart = keyValuePairs.at(0);
        keys = keyPart.split(',', QString::SkipEmptyParts);
        for (int i = 0; i < keys.size(); ++i) {
            keys[i].replace(" ", "");
        }

        // 分割值部分，并替换空格为空字符串
        QString valuePart = keyValuePairs.at(1);
        values = valuePart.split(',', QString::SkipEmptyParts);
        for (int i = 0; i < values.size(); ++i) {
            values[i].replace(" ", "");
            values[i].replace("\t", "");
        }

        // 将键和值的QStringList插入到QMap中
        dataMap.insert(keys, values);
    }

    file.close();
    return dataMap;
}
QStringList CheckWindow::QStrTranQStrList(QString value)
{
    QStringList valuelist;
    valuelist.append(value);
    return valuelist;
}


void CheckWindow::keyPressEvent(QKeyEvent *ev)
{
    if(ui->stackedWidget_2->currentIndex()==1&&iEquipType==4){
        if(ev->key() == Qt::Key_Right)
        {
            int idx = curDevciceIdx+1;
            on_deviceButton_clicked(idx);
            //return;
        }else if(ev->key() == Qt::Key_Left)
        {
            int idx = curDevciceIdx-1;
            on_deviceButton_clicked(idx);
            //return;
        }
    }
    //QWidget::keyPressEvent(ev);
}
void CheckWindow::SendDevice(InstructionLib* Inst,VisaCommonEngine* Visa,bool IsUnitTran){
    //发送仪器 进一步封装
    QString newCmd="";
    if(Visa->getState()){
       if(Inst!=nullptr){
           QStringList sList = Inst->instruct_config.split(";");
           if(sList.length()>0){
               for (QString& sCmd : sList){
                   newCmd = transCmd(sCmd,constsLable,constDataRes,IsUnitTran);
                   if(newCmd!=""){
                       QString str=Visa->commType;
                       Visa->sendData(newCmd);
                       qDebug() << "config cmds:" << newCmd;
                   }
               }
           }
       }
}
}
QString CheckWindow::ReadDevice(InstructionLib* Inst,VisaCommonEngine* Visa,bool IsUnitTran){
    //读取仪器 进一步封装
    QString newCmd="";
    QString result="";
    if(Visa->getState()){
        if(Inst!=nullptr){
            newCmd = transCmd(Inst->instruct_read,constsLable,constDataRes,IsUnitTran);
            if(newCmd!=""){
                qDebug() << "query cmds:" << newCmd;
                result = Visa->queryData(newCmd);
                }
            }
    }
    return  result;
}



void CheckWindow::CreateCharts(){
    deleteItem(ui->horizontalLayoutChart);
    QChart *qchart = new QChart;
    chartView = new QChartView(qchart,this);
    chartView->setRenderHint(QPainter::Antialiasing);
    //把chart放到容器里
    //ui->graphicsView->setChart(qchart);
    //设置抗锯齿
    //ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    //创建两条线
    QLineSeries *ser0 = new QLineSeries;
    //QLineSeries *ser1 = new QLineSeries;
    //设置名字
    ser0->setName("变化趋势 单位:℃");
    //ser1->setName("ser1");
    //放入charts里
    qchart->addSeries(ser0);
    //qchart->addSeries(ser1);

    //创建x坐标
    QDateTimeAxis *QaX = new QDateTimeAxis;
    //格式
    QaX->setFormat("hh:mm:ss");
    QaX->setTickCount(5);
    //QaX->setTitleText("time");

    //创建y坐标
    QValueAxis *QaY = new QValueAxis;
    //设置范围
    //QaY->setRange(-1,1);
    QaY->setTickCount(6);
    //QaY->setTitleText("单位:℃");

    //将线条放入表中
    qchart->setAxisX(QaX,ser0);
    qchart->setAxisY(QaY,ser0);
    //qchart->setAxisX(QaX,ser1);
    //qchart->setAxisY(QaY,ser1);
    ui->horizontalLayoutChart->addWidget(chartView);
    //QLayoutItem *child = ui->horizontalLayoutChart->itemAt(0);
    //QChartView *chartView = qobject_cast<QChartView *>(child->widget());
    //QChart *qchart =(QChart *)chartView->chart();
    //获取之前的ser
    //QLineSeries *ser0 = (QLineSeries *)qchart->series().at(0);
    //ser0->clear();

}

void CheckWindow::RefreshTime_Slot(){
    QString sLabel = headTableList->keys()[curHeadIdx];
    QString sRet ;

    VisaCommonEngine tempEngine=getVisaCommbyRoleName("堆栈式测温仪"); //角色
    int ch;
    if(tempEngine.getState()){
        BaseCommonApi::SaveLogInfo(1,QString("堆栈式测温仪控制"));
        InstructionLib *instrcutLib=getCmdByRoleName("堆栈式测温仪",sLabel);
        if(sLabel=="波动性测试"){
            QString newCmd;
            SendDevice(instrcutLib,&tempEngine);
            sRet=ReadDevice(instrcutLib,&tempEngine);
        }else if(sLabel=="均匀性测试"){

            InstructionLib instrcutLibTemp;
            QStringList CmdList=instrcutLib->instruct_read.split(";");
            /*if(NumberCount%2==0){
                ch=0;
            }else{
                ch=1;
            }*/
            //默认只读移动的温度计
            instrcutLibTemp.instruct_read=CmdList[1];
            SendDevice(instrcutLib,&tempEngine);
            sRet=ReadDevice(&instrcutLibTemp,&tempEngine);
            //sRet="通道"+QString::number(ch)+":"+ReadDevice(&instrcutLibTemp,&deviceEngine);
            NumberCount++;
        }
    }
    //获取当前时间
    QDateTime currentTime = QDateTime::currentDateTime();
    //获取随机数
    //qsrand(QTime::currentTime().second());
    //int rand  = qrand()%100;//获取0~10之间的数
    //获取初始化的qchart
    QChart *qchart =(QChart *)chartView->chart();

    if(qchart!=nullptr){
        //获取之前的ser
        QLineSeries *ser0 = (QLineSeries *)qchart->series().at(0);
        //更新数据
        double data = sRet.toDouble();
        ser0->append(currentTime.toMSecsSinceEpoch(),data);
        qchart->axisX()->setMin(QDateTime::currentDateTime().addSecs(-1*30));
        qchart->axisX()->setMax(QDateTime::currentDateTime().addSecs(1*30));

        ui->lineEditTemp->setText(sRet);
    }
}

void CheckWindow::on_pushButtonGetTemp_clicked()
{
    QString sLabel = headTableList->keys()[curHeadIdx];
    QString sData = ui->lineEditTemp->text();
    if(sData.contains("通道"))
        sData=sData.mid(4,sData.count());

    VisaCommonEngine tempEngine=getVisaCommbyRoleName("堆栈式测温仪"); //角色
    InstructionLib *instrcutLib=getCmdByRoleName("堆栈式测温仪",sLabel);

    QString sRet;
    QList<QStringList> dataRes = caliDataList->value(sLabel);
    if(sLabel=="波动性测试"){
        QTableWidgetItem *currentItem=ui->tableWidgetCheck->currentItem();
        if(currentItem==nullptr||currentItem->column()<4||currentItem->column()>18){
            ui->tableWidgetCheck->setCurrentCell(0,4);
        }
        currentItem=ui->tableWidgetCheck->currentItem();
        int row = currentItem->row();
        int col = currentItem->column();
        if(row>=0 && row <=7){
          for(int i=0;i<=7;i++){
              for(int j=4;j<=18;j++){
                  if(Teststate==SyncRunStatus::Stopped){return;};
                  //每次读取值间隔一秒时间
                  Delay_MSec(1000);
                  SendDevice(instrcutLib,&tempEngine);
                  sRet=ReadDevice(instrcutLib,&tempEngine);

                  ui->tableWidgetCheck->setItem(i,j,new QTableWidgetItem(sRet));
                   dataRes[i][j] = sRet;

              }
          }
        }
        if(row>=8 && row <=15){
            for(int i=8;i<=15;i++){
                for(int j=4;j<=18;j++){
                    //每次读取值间隔一秒时间
                    if(Teststate==SyncRunStatus::Stopped){return;};
                    Delay_MSec(1000);
                    SendDevice(instrcutLib,&tempEngine);
                    sRet=ReadDevice(instrcutLib,&tempEngine);
                    ui->tableWidgetCheck->setItem(i,j,new QTableWidgetItem(sRet));
                    dataRes[i][j] = sRet;
                }
            }
          }




        for(int i=0;i<ui->tableWidgetCheck->rowCount()/8;i++){
            double dMin=0;
            double dMax=0;
            for(int k=i*8;k<i*8+8;k++){
                for(int j=4;j<19;j++){
                    QString sTmp = ui->tableWidgetCheck->item(k,j)->text();
                    if(sTmp!=""){
                        dMin = qMin(dMin,sTmp.toDouble());
                        dMax = qMax(dMax,sTmp.toDouble());
                    }
                }
            }
            int idx =getIndexByHeader(sLabel,QStrTranQStrList("最大值"));
            ui->tableWidgetCheck->item(i*8,idx)->setText(QString::number(dMax));
            dataRes[i*8][idx] = QString::number(dMax);
            idx =getIndexByHeader(sLabel,QStrTranQStrList("最小值"));
            ui->tableWidgetCheck->item(i*8,idx)->setText(QString::number(dMin));
            dataRes[i*8][idx] = QString::number(dMin);
            idx =getIndexByHeader(sLabel,QStrTranQStrList("波动性"));
            QString sErr = QString::number(dMax-dMin);
            if(idx>=0&&idx<dataRes[i*8].length())
                dataRes[i*8][idx] = sErr;
            if(ui->tableWidgetCheck->item(i*8,idx)!=nullptr)
                ui->tableWidgetCheck->item(i*8,idx)->setText(sErr);
            else
                ui->tableWidgetCheck->setItem(i*8,idx,new QTableWidgetItem(sErr));
        }
        caliDataList->insert(sLabel,dataRes);
    }else if(sLabel=="均匀性测试"){
        QStringList Head=headTableList->value(sLabel);
        QTableWidgetItem *currentItem=ui->tableWidgetCheck->currentItem();
        InstructionLib instrcutLibTemp;
        QStringList CmdList=instrcutLib->instruct_read.split(";");
        if(currentItem==nullptr||currentItem->column()<5||currentItem->column()>14){
            ui->tableWidgetCheck->setCurrentCell(0,5);
        }
        currentItem=ui->tableWidgetCheck->currentItem();
        int row = currentItem->row();
        //int col = currentItem->column();
        if(row>=0){
            for(int j=5;j<=12;j++){
                if(Teststate==SyncRunStatus::Stopped){break;};
                Delay_MSec(1000);
                QString CurrentColName=Head[j];
                //int rand =0;
                if(CurrentColName.contains("O")){
                    instrcutLibTemp.instruct_read=CmdList[0];
                    SendDevice(instrcutLib,&tempEngine);
                    sRet=ReadDevice(&instrcutLibTemp,&tempEngine);
                }else{
                    instrcutLibTemp.instruct_read=CmdList[1];
                    SendDevice(instrcutLib,&tempEngine);
                    sRet=ReadDevice(&instrcutLibTemp,&tempEngine);
                }

                ui->tableWidgetCheck->setItem(row,j,new QTableWidgetItem(sRet));
                dataRes[row][j] = sRet;
            }
        }
        //currentItem->setText(sData);


        for(int i=0;i<ui->tableWidgetCheck->rowCount()/8;i++){
            double dMin=0;
            double dMax=0;
            double dUpErr=0;
            double dDownErr=0;
            for(int k=i*8;k<i*8+8;k++){
                double dMean1=0;
                double dMean2=0;
                int idx =getIndexByHeader(sLabel,QStrTranQStrList("示值ROA1"));
                if(dataRes[k][idx]!="")
                    dMean1 += dataRes[k][idx].toDouble();
                idx =getIndexByHeader(sLabel,QStrTranQStrList("ROA2"));
                if(dataRes[k][idx]!="")
                    dMean1 += dataRes[k][idx].toDouble();
                idx =getIndexByHeader(sLabel,QStrTranQStrList("ROA3"));
                if(dataRes[k][idx]!="")
                    dMean1 += dataRes[k][idx].toDouble();
                idx =getIndexByHeader(sLabel,QStrTranQStrList("ROA4"));
                if(dataRes[k][idx]!="")
                    dMean1 += dataRes[k][idx].toDouble();
                dMean1/=4;

                idx =getIndexByHeader(sLabel,QStrTranQStrList("示值RA1"));
                if(dataRes[k][idx]!="")
                    dMean2 += dataRes[k][idx].toDouble();
                idx =getIndexByHeader(sLabel,QStrTranQStrList("RA2"));
                if(dataRes[k][idx]!="")
                    dMean2 += dataRes[k][idx].toDouble();
                idx =getIndexByHeader(sLabel,QStrTranQStrList("RA3"));
                if(dataRes[k][idx]!="")
                    dMean2 += dataRes[k][idx].toDouble();
                idx =getIndexByHeader(sLabel,QStrTranQStrList("RA4"));
                if(dataRes[k][idx]!="")
                    dMean2 += dataRes[k][idx].toDouble();
                dMean2/=4;

                if(k<i*8+4){
                    dUpErr = qMax(dMean2-dMean1,dUpErr);
                }else{
                    dDownErr = qMax(dMean2-dMean1,dDownErr);
                }
                idx =getIndexByHeader(sLabel,QStrTranQStrList("ROA均值"));
                ui->tableWidgetCheck->item(k,idx)->setText(QString::number(dMean1));
                dataRes[k][idx] = QString::number(dMean1);

                idx =getIndexByHeader(sLabel,QStrTranQStrList("RA均值"));
                ui->tableWidgetCheck->item(k,idx)->setText(QString::number(dMean2));
                dataRes[k][idx] = QString::number(dMean2);
                for(int j=5;j<13;j++){
                    QString sTmp = ui->tableWidgetCheck->item(k,j)->text();
                    if(sTmp!=""){
                        dMin = qMin(dMin,sTmp.toDouble());
                        dMax = qMax(dMax,sTmp.toDouble());
                    }
                }
            }
            int idx =getIndexByHeader(sLabel,QStrTranQStrList("上水平面温差"));
            ui->tableWidgetCheck->item(i*8,idx)->setText(QString::number(dUpErr));
            dataRes[i*8][idx] = QString::number(dUpErr);
            idx =getIndexByHeader(sLabel,QStrTranQStrList("下水平面温差"));
            ui->tableWidgetCheck->item(i*8,idx)->setText(QString::number(dDownErr));
            dataRes[i*8][idx] = QString::number(dDownErr);
            idx =getIndexByHeader(sLabel,QStrTranQStrList("水平最大温差"));
            ui->tableWidgetCheck->item(i*8,idx)->setText(QString::number(dUpErr-dDownErr));
            dataRes[i*8][idx] = QString::number(dUpErr-dDownErr);

            idx =getIndexByHeader(sLabel,QStrTranQStrList("工作区域最大温差"));
            QString sErr = QString::number(dMax-dMin);
            if(idx>=0&&idx<dataRes[i*8].length())
                dataRes[i*8][idx] = sErr;
            if(ui->tableWidgetCheck->item(i*8,idx)!=nullptr)
                ui->tableWidgetCheck->item(i*8,idx)->setText(sErr);
            else
                ui->tableWidgetCheck->setItem(i*8,idx,new QTableWidgetItem(sErr));
        }

        caliDataList->insert(sLabel,dataRes);
    }
}

void CheckWindow::readDCStabData(QString sLabel){
    QString sRet;
    if(iEquipType==2){
        qDebug() << sLabel;
        QString sRole = "电压表";
        if(sLabel=="纹波电压")
            sRole="示波器";
        else if(sLabel=="电流示值误差(直接测量)")
            sRole ="电流表";
        BaseCommonApi::SaveLogInfo(1,QString("可编程交流电源读取数值"));
        VisaCommonEngine engineRole=getVisaCommbyRoleName(sRole); //角色
        InstructionLib *instrcutLibRole=getCmdByRoleName(sRole,sLabel);
        //先发送配置指令
        SendDevice(instrcutLibRole,&engineRole);
        sRet = ReadDevice(instrcutLibRole,&engineRole);
    }else if(iEquipType==9){
        constsLable=sLabel;
        InstructionLib *instSpect = getCmdByCheckName(0,sLabel); //频谱仪
        InstructionLib *instSoure=getCmdByRoleName("合成信号源",sLabel);
        VisaCommonEngine EngineSoure=getVisaCommbyRoleName("合成信号源");
        InstructionLib *instRace=getCmdByRoleName("测量接收机",sLabel);
        VisaCommonEngine EngineRace=getVisaCommbyRoleName("测量接收机");
        if(sLabel=="频率显示" || sLabel=="扫频宽度" || sLabel=="中频带宽" || sLabel=="输入频响"
             ||  sLabel=="1db增益压缩点" ||sLabel=="校准信号" ){
           sRet=ReadDevice(instRace,&EngineRace);
        }else if(sLabel=="中频带宽转换偏差" || sLabel=="谐波失真"
             ||  sLabel=="输入衰减" || sLabel=="平均噪声电平"){
           sRet=ReadDevice(instSpect,&deviceEngine);
        }else if(sLabel=="参考电平" || sLabel=="垂直刻度"){
           sRet=ReadDevice(instSoure,&EngineSoure);
        }
        else{
           sRet=ReadDevice(instSpect,&deviceEngine);
        }
    }
    else{
        BaseCommonApi::SaveLogInfo(1,QString("被检设备输出"));
        int idx= getIndexByHeader(sLabel,QStrTranQStrList("检定项目"));
        QString sParam;
        if(idx!=-1)
            sParam = stabInputdlg->dataRes->at(idx);
        InstructionLib *instrcutLib = getCmdByCheckName(0,sParam);
        SendDevice(instrcutLib,&deviceEngine);
        sRet=ReadDevice(instrcutLib,&deviceEngine);
        //针对不同项目进行单位转换
    }
    //sRet="0.005";
    if(sRet!=""){
        //QStringList TempValue;
        //TempValue.append("单位");
        //int idx= getIndexByHeader(sLabel,TempValue);
        //double dStdValue = transUnit(sRet,stabInputdlg->dataRes->at(idx));
        double dStdValue=sRet.toDouble();
        stabInputdlg->setInputData(dStdValue);
    }
}
void CheckWindow::sendDCStabData(QString sLabel){
    if(iEquipType==9){
        constsLable=sLabel;
        InstructionLib *instSpect = getCmdByCheckName(0,constsLable); //频谱仪
        InstructionLib *instSoure=getCmdByRoleName("合成信号源",constsLable);
        VisaCommonEngine EngineSoure=getVisaCommbyRoleName("合成信号源");
        InstructionLib *instRace=getCmdByRoleName("测量接收机",constsLable);
        VisaCommonEngine EngineRace=getVisaCommbyRoleName("测量接收机");
        InstructionLib *instMic=getCmdByRoleName("程控衰减器",constsLable);
        VisaCommonEngine EngineMic=getVisaCommbyRoleName("程控衰减器");
        if(sLabel=="频率显示" || sLabel=="扫频宽度" || sLabel=="中频带宽" || sLabel=="输入频响"
             ||  sLabel=="1db增益压缩点" ){
            SendDevice(instSoure,&EngineSoure);
            SendDevice(instRace,&EngineRace);
            SendDevice(instSpect,&deviceEngine);
        }else if(sLabel=="中频带宽转换偏差" || sLabel=="谐波失真" || sLabel=="校准信号"){
            SendDevice(instSoure,&EngineSoure);
            SendDevice(instSpect,&deviceEngine);
        }else if(sLabel=="输入衰减" || sLabel=="参考电平" || sLabel=="垂直刻度"){
            SendDevice(instMic,&EngineMic);
            SendDevice(instSoure,&EngineSoure);
            SendDevice(instSpect,&deviceEngine);
        }else if(sLabel=="平均噪声电平"){
            SendDevice(instSpect,&deviceEngine);
        }
    }
}
void CheckWindow::on_pushButton_Hidd_clicked()
{
    if(ui->pushButton_Hidd->text()=="隐藏"){
        ui->pushButton_Hidd->setText("显示");
        ui->widget_3->hide();
        ui->widget_4->hide();
        ui->widget_5->hide();
        ui->groupBox_5->hide();
        foreach(QWidget *widget, ui->verticalLayout_6->findChildren<QWidget *>()) {
            widget->hide();
        }
        //ui->groupBoxChart->hide();
        //ui->groupBox_3->hide();
        //ui->groupBox_7->hide();
    }else{
        ui->pushButton_Hidd->setText("隐藏");
        ui->widget_3->show();
        ui->widget_4->show();
        ui->widget_5->show();
        ui->groupBox_5->show();
        foreach(QWidget *widget, ui->verticalLayout_6->findChildren<QWidget *>()) {
            widget->show();
        }
        //ui->groupBoxChart->show();
        //ui->groupBox_3->show();
        //ui->groupBox_7->show();
    }

}
double CheckWindow::InputDialog(QString Content)
{
    double value;
    bool bOk = false;
    value = QInputDialog::getDouble(this,"手动测试",Content,0,-100000,	100000,5,&bOk);
    if (!bOk)
        value=0;
    return value;
}
