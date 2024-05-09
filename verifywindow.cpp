#include "verifywindow.h"
#include "dialogverifyresult.h"
#include "global.h"
#include "newverifymethod.h"
#include "ui_verifywindow.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QtMath>

VerifyWindow::VerifyWindow(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::VerifyWindow)
{
    ui->setupUi(this);
    ui->tableWidgetMethod->verticalHeader()->setParent(nullptr); // 隐藏行头
    for (int i = 1; i < 5; i++)
        ui->tableWidgetMethod->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    ui->tableWidgetMethod->setColumnWidth(0, 60);
    ui->tableWidgetMethod->setColumnWidth(5, 100);
    ui->tableWidget_3->verticalHeader()->setParent(nullptr);
    for (int i = 0; i < 2; i++)
        ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    ui->tableWidget_3->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_Standard->verticalHeader()->setParent(nullptr);
    for (int i = 0; i < 2; i++)
        ui->tableWidget_Standard->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    for (int i = 1; i < 6; i++) {
        ui->tableWidgetStd->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }
    for (int i = 1; i < 5; i++) {
        ui->tableWidgetBiz->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }
    ui->tableWidgetStd->setColumnWidth(0, 60);
    ui->tableWidgetBiz->setColumnWidth(0, 60);
    ui->tableWidgetCheck->verticalHeader()->setParent(nullptr);
    for (int i = 0; i < 3; i++)
        ui->tableWidget_Point->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    // 统一定义单位转换系数
    unitTransList = readDataFromFile("unitTrans.txt");
    // 统一定义命令占位符
    cmdPlaceHolderList = readDataFromFile("cmdPlaceHolder.txt");
    // 单位值特殊转换
    unitTypeTransList = readDataFromFile("unitTypeTrans.txt");
    // 测试结果值填充列
    resultfillingList = readDataFromFile("resultfilling.txt");

    // 新打开时指定默认显示选择核查程序的page页
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_2->setCurrentIndex(0);
    getVerifyMethod();
}

VerifyWindow::~VerifyWindow()
{
    programList.clear();
    delete ui;
}

QMap<QStringList, QStringList> VerifyWindow::readDataFromFile(QString filePath)
{
    // 获取应用程序的根目录
    QString path = QCoreApplication::applicationDirPath() + "\\File\\config\\" + filePath;
    QMap<QStringList, QStringList> dataMap;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // 错误处理
        qDebug() << "无法打开文件：" << file.errorString();
        QString str = file.errorString();
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

void VerifyWindow::getVerifyMethod()
{
    programList.clear();
    programList = BaseCommonApi::getCheckProgramManage();
    ui->tableWidgetMethod->clearContents();
    ui->tableWidgetMethod->setRowCount(0);
    int rowCount = 0;
    int row = 0;
    QString sFilterName = ui->lineEdit_9->text();
    QString sFilterUser = ui->lineEdit_10->text();
    for (int i = programList.length() - 1; i >= 0; i--) {
        bool bAdd = true;
        if (sFilterName != "" && !programList[i].check_name.contains(sFilterName)) {
            bAdd = false;
        }
        if (sFilterUser != "" && !programList[i].create_name.contains(sFilterUser)) {
            bAdd = false;
        }
        if (!bAdd) {
            programList.removeAt(i);
        }
    }
    for (const CheckProgramManage& program : programList) {
        rowCount = ui->tableWidgetMethod->rowCount();

        ui->tableWidgetMethod->insertRow(rowCount);
        QTableWidgetItem* p_check = new QTableWidgetItem();
        p_check->setCheckState(Qt::Unchecked); // 设置首列为 CheckBox 控件
        ui->tableWidgetMethod->setItem(row, 0, p_check);
        ui->tableWidgetMethod->setItem(row, 1, new QTableWidgetItem(program.check_name)); // 设置数据
        ui->tableWidgetMethod->setItem(row, 2, new QTableWidgetItem(program.create_name)); // 设置数据
        ui->tableWidgetMethod->setItem(row, 3, new QTableWidgetItem(program.create_time.toString("yyyy-MM-dd"))); // 设置数据
        ui->tableWidgetMethod->setItem(row, 4, new QTableWidgetItem(program.remark)); // 设置数据
        QPushButton* btn_1 = new QPushButton();
        btn_1->setText(tr("详情"));
        btn_1->setStyleSheet("QPushButton{"
                             "background-color:rgba(255,255,255,0);"
                             "color:rgba(0,0,255,100);"
                             "text-decoration:underline;"
                             "}");
        btn_1->setCursor(Qt::PointingHandCursor);
        connect(btn_1, SIGNAL(clicked()), this, SLOT(on_editButton_clicked()));

        btn_1->setIconSize(QSize(16, 16));
        btn_1->setIcon(QIcon(":/image/Index/u2324.svg"));
        QWidget* tmp_widget = new QWidget();
        QHBoxLayout* tmp_layout = new QHBoxLayout(tmp_widget);
        tmp_layout->addWidget(btn_1);
        tmp_layout->setMargin(0);
        ui->tableWidgetMethod->setCellWidget(row, 5, tmp_widget);
        row++;
    }
}

void VerifyWindow::showDevice()
{
    QString sId = "";
    for (int i = 0; i < ui->tableWidgetMethod->rowCount(); i++) {
        Qt::CheckState state = ui->tableWidgetMethod->item(i, 0)->checkState();
        if (state == Qt::Checked) {
            sId = programList[i].id;
            break;
        }
    }
    if (sId == "") {
        QMessageBox::warning(this, "提示", "请选择检定程序！");
    } else {
        loginNowVerifyProgramId = sId;
        programInfo = BaseCommonApi::getCheckProgramManageById(sId);
        // initDeviceInfo();
        selectStandInfo();
        selectBizInfo();
        ui->stackedWidget->setCurrentIndex(1);
    }
}

void VerifyWindow::selectStandInfo()
{
    ui->tableWidgetStd->clearContents();
    ui->tableWidgetStd->setRowCount(0);
    QStringList idList;
    int rowCount = 0;
    int row = 0;
    if (programInfo.standard_device_id != "") {
        idList = programInfo.standard_device_id.split(",");

        DeviceMeasureEquipmentCatorycatory = BaseCommonApi::getDeviceMeasureEquipmentCatory(idList[0]);
        if (DeviceMeasureEquipmentCatorycatory.length() > 0)
            RoleList = BaseCommonApi::getDeviceRoleBydeviceId(DeviceMeasureEquipmentCatorycatory[0].item_category_id);

        QStringList ValueList;
        ValueList.append("请选择角色");
        for (int i = 0; i < RoleList.count(); i++) {
            ValueList.append(RoleList[i].role);
        }

        stdIdList.clear();
        stdList.clear();
        for (const QString& id : idList) {
            QList<CatDeviceInfor> result = BaseCommonApi::getCatDeviceInfor(id);
            if (result.length() > 0) {

                CatDeviceInfor item = result[0];
                QString sFilterName = ui->lineEdit_7->text();
                QString sFilterModal = ui->lineEdit_8->text();
                QString sFilterNo = ui->lineEdit_11->text();
                bool bAdd = true;
                if (sFilterName != "" && !item.equipment_name.contains(sFilterName)) {
                    bAdd = false;
                }
                if (sFilterModal != "" && !item.model.contains(sFilterModal)) {
                    bAdd = false;
                }
                if (sFilterNo != "" && !item.manufacture_no.contains(sFilterNo)) {
                    bAdd = false;
                }
                if (bAdd) {
                    stdIdList.append(id);
                    stdList.append(item);
                }
                result.clear();
            }
        }
        idList.clear();
        for (const CatDeviceInfor& item : stdList) {
            rowCount = ui->tableWidgetStd->rowCount();

            ui->tableWidgetStd->insertRow(rowCount);
            QTableWidgetItem* p_check = new QTableWidgetItem();
            p_check->setCheckState(Qt::Unchecked); // 设置首列为 CheckBox 控件
            ui->tableWidgetStd->setItem(row, 0, p_check);
            ui->tableWidgetStd->setItem(row, 1, new QTableWidgetItem(item.equipment_name)); // 设置数据
            ui->tableWidgetStd->setItem(row, 2, new QTableWidgetItem(item.model)); // 设置数据
            ui->tableWidgetStd->setItem(row, 3, new QTableWidgetItem(item.manufacture_no)); // 设置数据
            ui->tableWidgetStd->setItem(row, 4, new QTableWidgetItem(item.manufacturer));
            // ui->tableWidgetStd->setItem(row, 5, new QTableWidgetItem(""));
            if (RoleList.count() != 0) {

                QComboBox* comboBox = new QComboBox();
                comboBox->addItems(ValueList);
                ui->tableWidgetStd->setCellWidget(0, 5, comboBox);

                //                readraleName=GetdeviceidByRole(id);
                //                if(readraleName!=""){
                //                    comboBox->setCurrentText(readraleName);
                //                }
                // 连接下拉框的currentIndexChanged信号到槽函数
                connect(comboBox, &QComboBox::currentTextChanged, this, [this]() {
                    // onComboBoxCurrTextChanged(comboBox->currentText(),id,i);
                });
            } else {
                ui->tableWidgetStd->setItem(row, 5, new QTableWidgetItem("标准设备"));
            }
            row++;
        }
    }
}

void VerifyWindow::selectBizInfo()
{
    ui->tableWidgetBiz->clearContents();
    ui->tableWidgetBiz->setRowCount(0);
    QStringList idList;
    int rowCount = 0;
    int row = 0;
    if (programInfo.verification_documents_id != "") {
        idList = programInfo.verification_documents_id.split(",");
        bizIdList.clear();
        bizList.clear();
        for (const QString& id : idList) {
            QList<CatDeviceInfor> result = BaseCommonApi::getCatDeviceInfor(id);
            if (result.length() > 0) {

                CatDeviceInfor item = result[0];
                QString sFilterName = ui->lineEdit_12->text();
                QString sFilterModal = ui->lineEdit_13->text();
                QString sFilterNo = ui->lineEdit_14->text();
                bool bAdd = true;
                if (sFilterName != "" && !item.equipment_name.contains(sFilterName)) {
                    bAdd = false;
                }
                if (sFilterModal != "" && !item.model.contains(sFilterModal)) {
                    bAdd = false;
                }
                if (sFilterNo != "" && !item.manufacture_no.contains(sFilterNo)) {
                    bAdd = false;
                }
                if (bAdd) {
                    bizIdList.append(id);
                    bizList.append(item);
                }
                result.clear();
            }
        }
        idList.clear();
        for (const CatDeviceInfor& item : bizList) {
            rowCount = ui->tableWidgetBiz->rowCount();

            ui->tableWidgetBiz->insertRow(rowCount);
            QTableWidgetItem* p_check = new QTableWidgetItem();
            p_check->setCheckState(Qt::Unchecked); // 设置首列为 CheckBox 控件
            ui->tableWidgetBiz->setItem(row, 0, p_check);
            ui->tableWidgetBiz->setItem(row, 1, new QTableWidgetItem(item.equipment_name)); // 设置数据
            ui->tableWidgetBiz->setItem(row, 2, new QTableWidgetItem(item.model)); // 设置数据
            ui->tableWidgetBiz->setItem(row, 3, new QTableWidgetItem(item.manufacture_no)); // 设置数据
            ui->tableWidgetBiz->setItem(row, 4, new QTableWidgetItem(item.manufacturer));
            // ui->tableWidgetBiz->setItem(row, 5, new QTableWidgetItem(""));
            row++;
        }
    }
}

void VerifyWindow::initDeviceInfo(QString stdId, QString bizId)
{
    QString strStyle = QString("QLabel{image:url(%1);}").arg(programInfo.img_path);
    qDebug() << "strStyle:" << strStyle;
    ui->label_8->setStyleSheet(strStyle);

    ui->tableWidget_3->clearContents();
    ui->tableWidget_3->setRowCount(0);
    ui->tableWidget_Standard->clearContents();
    ui->tableWidget_Standard->setRowCount(0);
    QStringList idList;
    int rowCount = 0;
    int row = 0;
    connectList.clear();

    if (bizId != "") {
        selectBizIds = bizId;
        idList = bizId.split(",");
        for (const QString& id : idList) {
            ConnectInfo info;
            info.state = "未连接";
            info.type = 0;
            info.id = id;
            QList<CatDeviceInfor> result = BaseCommonApi::getCatDeviceInfor(id);
            if (result.length() > 0) {
                info.deviceName = result[0].equipment_name;
            }
            //            CheckAndTestedDevice result = BaseCommonApi::getCheckAndTestedDeviceById(id);

            //            info.deviceName = result.equipment_name;

            QList<VerificationProgramConfigInterface> addrList = BaseCommonApi::getVerificationProgramConfigInterface(id);
            if (addrList.length() > 0) {
                info.programInterface = addrList[0];
            }
            info.cmdList = BaseCommonApi::getInstructionLib(id);
            connectList.append(info);
            addrList.clear();
        }
        idList.clear();
    }

    if (stdId != "") {
        selectStdIds = stdId;
        idList = stdId.split(",");
        for (const QString& id : idList) {
            ConnectInfo info;
            info.id = id;
            info.state = "未连接";
            info.type = 1;
            QList<CatDeviceInfor> result = BaseCommonApi::getCatDeviceInfor(id);
            if (result.length() > 0) {
                info.deviceName = result[0].equipment_name;
            }
            result.clear();
            QList<VerificationProgramConfigInterface> addrList = BaseCommonApi::getVerificationProgramConfigInterface(id);
            if (addrList.length() > 0) {
                info.programInterface = addrList[0];
            }
            info.cmdList = BaseCommonApi::getInstructionLib(id);
            connectList.append(info);
            addrList.clear();
        }
        idList.clear();
    }
    for (const ConnectInfo& info : connectList) {
        rowCount = ui->tableWidget_3->rowCount();
        ui->tableWidget_3->insertRow(rowCount);
        ui->tableWidget_3->setItem(row, 0, new QTableWidgetItem(info.deviceName));
        ui->tableWidget_3->setItem(row, 1, new QTableWidgetItem(info.state)); // 设置数据

        ui->tableWidget_Standard->insertRow(rowCount);
        ui->tableWidget_Standard->setItem(row, 0, new QTableWidgetItem(info.deviceName));
        ui->tableWidget_Standard->setItem(row, 1, new QTableWidgetItem(info.state)); // 设置数据
        row++;
    }
    // 搜索仪器地址放入下拉框
    ui->comboBox_visa->clear();

    VisaCommonEngine engine;
    QStringList visaAddrs = engine.getVisaAddrs();
    ui->comboBox_visa->addItems(visaAddrs);
    ui->tableWidget_3->selectRow(0);
    connect(ui->comboBox_visa, &QComboBox::currentTextChanged, this, &VerifyWindow::on_comboBox_visa_OnCurrentTextChanged);
}

void VerifyWindow::on_comboBox_visa_OnCurrentTextChanged(QString arg1)
{
    // lab_deviceinfo
    getJoinState(arg1);
}

bool VerifyWindow::getJoinState(QString arg1)
{
    bool IsSucc = false;
    QString visaAdd = arg1;
    VisaCommonEngine engine;
    QStringList parts = engine.getdeviceIdninfo(visaAdd).split(",");
    if (parts.count() > 2) {
        ui->lab_deviceinfo->setText(parts[0] + "-" + parts[1]);
        IsSucc = true;
    }

    if (parts.contains("false")) {
        ui->lab_deviceinfo->setText("无连接");
    }
    return IsSucc;
}

void VerifyWindow::on_editButton_clicked()
{
    QPushButton* pushButton_ = dynamic_cast<QPushButton*>(this->sender());
    if (NULL == pushButton_) {
        return;
    }
    // 获取按钮的x坐标和y坐标
    int x = pushButton_->parentWidget()->frameGeometry().x();
    int y = pushButton_->parentWidget()->frameGeometry().y();
    // 根据按钮的x和y坐标来定位对应的单元格
    QModelIndex index = ui->tableWidgetMethod->indexAt(QPoint(x, y));
    // 获取该按钮所在表格的行号和列号
    int editIdx = index.row();
    // int column = index.column();
    QString sId = programList[editIdx].id;
    NewVerifyMethod* checkWindow = new NewVerifyMethod(nullptr, sId, false);
    // checkWindow->verifyManageWnd=nullptr;
    checkWindow->setWindowState(Qt::WindowMaximized);
    checkWindow->setWindowModality(Qt::ApplicationModal);
    checkWindow->show();
}

void VerifyWindow::on_tableWidgetMethod_cellChanged(int row, int column)
{
    Qt::CheckState state = ui->tableWidgetMethod->item(row, 0)->checkState();
    if (state == Qt::Checked) {
        for (int i = 0; i < ui->tableWidgetMethod->rowCount(); i++) {
            if (i != row) {
                ui->tableWidgetMethod->item(i, 0)->setCheckState(Qt::Unchecked);
            }
        }
    }
}

void VerifyWindow::on_tableWidget_3_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if (currentRow >= 0 && currentRow < connectList.length()) {
        currConnectIdx = currentRow;
        ConnectInfo info = connectList[currentRow];
        VerificationProgramConfigInterface addr = info.programInterface;
        if (addr.communication_type == "SerialPort")
            ui->radioButton_serial->setChecked(true);
        else
            ui->radioButton_visa->setChecked(true);
        ui->comboBox_visa->setCurrentText(addr.visa);
        if (addr.serial_port != "") {
            QStringList sList = addr.serial_port.split(";");
            if (sList.length() == 5) {
                ui->comboBox_port->setCurrentText(sList[0]);
                ui->comboBox_baurt->setCurrentText(sList[1]);
                ui->comboBox_check->setCurrentText(sList[2]);
                ui->comboBox_data->setCurrentText(sList[3]);
                ui->comboBox_stop->setCurrentText(sList[4]);
            }
        }
        // 修改连接状态
        if (getJoinState(addr.visa)) {
            ui->tableWidget_3->setItem(currentRow, 1, new QTableWidgetItem("已连接"));
        } else {
            ui->tableWidget_3->setItem(currentRow, 1, new QTableWidgetItem("未连接"));
        }
    }
}

void VerifyWindow::deleteItem(QLayout* layout)
{
    QLayoutItem* child;
    while ((child = layout->takeAt(0)) != nullptr) {
        // setParent为NULL，防止删除之后界面不消失
        if (child->widget()) {
            child->widget()->setParent(nullptr);
            delete child->widget();
        } else if (child->layout()) {
            deleteItem(child->layout());
            child->layout()->deleteLater();
        }
        delete child;
    }
}
void VerifyWindow::on_deviceButton_clicked(CatDeviceInfor info, int idx)
{
    for (int i = 0; i < catDeviceList.length(); i++) {
        QLayoutItem* child = ui->horizontalLayout_14->itemAt(i);
        QPushButton* btn_1 = qobject_cast<QPushButton*>(child->widget());
        if (i == idx) {
            btn_1->setStyleSheet("QPushButton{"
                                 "background-color:rgb(55,83,123);"
                                 "color:rgb(255,255,255);"
                                 "}");
        } else {
            btn_1->setStyleSheet("QPushButton{"
                                 "background-color:rgb(240,240,240);"
                                 "color:rgb(51,51,51);"
                                 "}");
        }
    }
    ui->label_21->setText(QString("设备名称: %1").arg(info.equipment_name));
    ui->label_22->setText(QString("型号规格: %1").arg(info.model));
    ui->label_23->setText(QString("出厂编号: %1").arg(info.manufacture_no));
    ui->label_24->setText(QString("生产厂家: %1").arg(info.manufacturer));
}

void VerifyWindow::on_pushButton_13_clicked()
{
    Teststate = SyncRunStatus::Stopped;
    deleteItem(ui->horizontalLayout_14);
    int idx = 0;
    catDeviceList.clear();
    QString standardId = connectList[0].id;
    QList<CatDeviceInfor> result = BaseCommonApi::getCatDeviceInfor(standardId);
    if (result.length() > 0) {
        CatDeviceInfor info = result[0];
        catDeviceList.append(info);
        QPushButton* btn_1 = new QPushButton();
        btn_1->setText(info.equipment_name);
        btn_1->setStyleSheet("QPushButton{"
                             "background-color:rgb(240,240,240);"
                             "color:rgb(51,51,51);"
                             "}");
        // btn_1->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
        QFont font;
        font.setPointSize(12); // 字体大小
        font.setBold(true);
        btn_1->setFont(font);
        btn_1->setCursor(Qt::PointingHandCursor);
        connect(btn_1, &QPushButton::clicked, this, [this, info, idx]() {
            on_deviceButton_clicked(info, idx);
        });
        ui->horizontalLayout_14->addWidget(btn_1);
        idx++;
    }
    result.clear();
    on_deviceButton_clicked(catDeviceList[0], 0);
    ui->horizontalLayout_14->addStretch();

    getCheckItemData();
    deleteItem(ui->horizontalLayoutMain);
    mainTabIdx = -1;
    int index = 0;
    int rowCount = 0;
    int row = 0;
    ui->tableWidget_Point->clearContents();
    ui->tableWidget_Point->setRowCount(0);
    for (const auto& item : veriHeadList.keys()) {
        const auto keys = veriHeadList.value(item);
        const auto veriData = veriDataMapList.value(item);
        int start = row;
        for (const auto& key : keys.keys()) {
            const auto dataRes = veriData.value(key);
            for (int i = 0; i < dataRes.count(); ++i) {
                rowCount = ui->tableWidget_Point->rowCount();
                ui->tableWidget_Point->insertRow(rowCount);
                int idx = getIndexByHeader(item, key, QStrTranQStrList("核查项目"));
                if (idx >= 0 && idx < dataRes[i].count())
                    ui->tableWidget_Point->setItem(row, 0, new QTableWidgetItem(dataRes[i][idx]));
                ui->tableWidget_Point->setItem(row, 1, new QTableWidgetItem(key));
                idx = getIndexByHeader(item, key, QStrTranQStrList("核查点"));
                if (idx >= 0 && idx < dataRes[i].count())
                    ui->tableWidget_Point->setItem(row, 2, new QTableWidgetItem(dataRes[i][idx]));
                row++;
            }
        }
        ui->tableWidget_Point->setSpan(start, 0, row - start, 1);
        // ========================================================================================
        // 按钮设置显示
        // ====================================================================================
        QPushButton* button = new QPushButton(item, this);
        // 设置样式
        //  设置按钮大小为 56x20
        // 固定大小文字显示不全
        // button->setFixedSize(85, 40);
        button->setCheckable(true);
        // button->setAutoExclusive(true);
        // 设置第一个按钮选中状态
        if (index == 0) {
            button->setChecked(true);
        }
        // button->setc(true);
        //  设置按钮样式表
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

        ui->horizontalLayoutMain->addWidget(button);
        connect(button, &QPushButton::clicked, this, [this, index]() {
            // tackedWidget->setCurrentIndex(index);
            initCheckTable(index);
        });
        // pageButtons.append(button);
        ++index;
    }
    ui->horizontalLayoutMain->addStretch();
    initCheckTable(0);
    //    if(catDeviceList.length()>0)
    //    {
    //        headList.clear();
    //        calibrationList.clear();
    //        QList<JudgeCalibrationTable> CliTable = BaseCommonApi::getJudgeCalibrationTable(standardId);
    //        if(CliTable.length()>0){
    //            iEquipType = CliTable[0].belong_standard_equipment.toInt();
    //        }
    //        veriDataMapList = BaseCommonApi::getStandardCheckItemDataCalibrator(standardId,&veriHeadList);
    ////        calibrationList =BaseCommonApi::Search_VerifyTable(standardId,&headList);

    //        //tableWidgetItem
    //        ui->tableWidget_Point->clearContents();
    //        ui->tableWidget_Point->setRowCount(0);
    //        //循环数据
    //        int rowCount=0;
    //        int row=0;
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
    //    }

    ui->stackedWidget_2->setCurrentIndex(1);
}

void VerifyWindow::initCheckTable(int idx)
{
    if (mainTabIdx == idx || veriHeadList.keys().count() == 0)
        return;
    mainTabIdx = idx;
    for (int i = 0; i < veriHeadList.keys().count(); i++) {
        QLayoutItem* child = ui->horizontalLayoutMain->itemAt(i);
        QPushButton* btn_1 = qobject_cast<QPushButton*>(child->widget());
        if (i == mainTabIdx) {
            btn_1->setChecked(true);
        } else {
            btn_1->setChecked(false);
        }
    }
    deleteItem(ui->horizontalLayoutItem);

    itemTabIdx = -1;
    int index = 0;
    const auto items = veriHeadList.values().at(mainTabIdx);
    for (const auto& item : items.keys()) {
        QPushButton* button = new QPushButton(item, this);
        // 设置样式
        //  设置按钮大小为 56x20
        // 固定大小文字显示不全
        // button->setFixedSize(85, 40);
        button->setCheckable(true);
        // button->setAutoExclusive(true);
        // 设置第一个按钮选中状态
        if (index == 0) {
            button->setChecked(true);
        }
        // button->setc(true);
        //  设置按钮样式表
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

        ui->horizontalLayoutItem->addWidget(button);
        connect(button, &QPushButton::clicked, this, [this, index]() {
            // tackedWidget->setCurrentIndex(index);
            initCheckItemData(index);
        });
        // pageButtons.append(button);
        ++index;
    }
    ui->horizontalLayoutItem->addStretch();
    if (iEquipType == 1) {
        QComboBox* comboBox = new QComboBox();
        comboBox->addItems({ "ALL", "最佳点", "最差点", "典型点" });
        // 连接下拉框的currentIndexChanged信号到槽函数
        connect(comboBox, &QComboBox::currentTextChanged, this, [this, comboBox]() {
            initCheckItemData(itemTabIdx, comboBox->currentText(), true);
            // onComboBoxCurrTextChanged(comboBox->currentText(),id,i);
        });
        ui->horizontalLayoutItem->addWidget(comboBox);
    }
    initCheckItemData(0);
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
    //                    if(k<item.length())
    //                        ui->tableWidgetCheck->setItem(row, k, new QTableWidgetItem(item[k]));
    //                }
    //                StandCheckDataCalibrator measItem;
    ////                measItem.item_data_id = item[1];
    ////                measItem.maximum_error_formula = item[10];//最大允许误差(公式)
    ////                measItem.error_param_a = item[11];//误差参数a
    ////                measItem.error_param_b = item[12];//误差参数b
    ////                measItem.params = item[2];//参数
    ////                measItem.capacity=item[3];//量
    ////                measItem.unit=item[4];//单位
    ////                measItem.frequency=item[5];//频率
    ////                measItem.frequency_unit=item[6];//频率单位
    ////                measItem.range_range=item[7];//量程
    ////                measItem.standard_value=item[8];//标准值(输入字段,特殊:指针式万用表输出字段)
    ////                measItem.indicating_value=item[15];//示值(输出字段,特殊:指针式万用表输入字段)
    ////                measItem.maximum_error=item[10];//最大允许误差(值)
    ////                //measItem.absolute_error=item[11];//绝对误差
    ////                measItem.resolution=item[9];//分辨力
    ////                measItem.measure_indication_value=item[19];//被检表示值
    //                measureCalList.append(measItem);
    //                row++;
    //            }
    //        }
    //    }
    //    if(connectList.length()>0&&connectList[0].type==1)
    //        standardEngine.openDevice(connectList[0].programInterface.visa,"Visa");
    //    if(connectList.length()>1&&connectList[1].type==0)
    //        deviceEngine.openDevice(connectList[1].programInterface.visa,"Visa");
}

void VerifyWindow::initCheckItemData(int idx, QString filter, bool bForce)
{
    if (itemTabIdx == idx && !bForce)
        return;
    itemTabIdx = idx;
    QString sLabel = veriHeadList.keys()[mainTabIdx];
    const auto items = veriHeadList.value(sLabel);

    for (int i = 0; i < items.keys().count(); i++) {
        QLayoutItem* child = ui->horizontalLayoutItem->itemAt(i);
        QPushButton* btn_1 = qobject_cast<QPushButton*>(child->widget());
        if (i == itemTabIdx) {
            btn_1->setChecked(true);
        } else {
            btn_1->setChecked(false);
        }
    }
    QString sLabel2 = items.keys()[itemTabIdx];
    const auto headrs = items.value(sLabel2);

    ui->tableWidgetCheck->clearContents();
    ui->tableWidgetCheck->setRowCount(0);
    ui->tableWidgetCheck->setColumnCount(0);
    ui->tableWidgetCheck->setColumnCount(headrs.count());
    ui->tableWidgetCheck->setHorizontalHeaderLabels(headrs);
    for (int i = 0; i < headrs.count(); i++) {
        ui->tableWidgetCheck->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
    }
    int rowCount = 0;
    const auto items2 = veriDataMapList.value(sLabel);
    const auto dataRes = items2.value(sLabel2);
    int typeIdx = getIndexByHeader(sLabel, sLabel2, QStrTranQStrList("核查点类型"));
    for (int i = 0; i < dataRes.count(); ++i) {
        // 获取行号 并进行添加行
        if (iEquipType == 1 && typeIdx != -1 && filter != "ALL" && filter != dataRes[i][typeIdx])
            continue;
        rowCount = ui->tableWidgetCheck->rowCount();
        ui->tableWidgetCheck->insertRow(rowCount);
        ui->tableWidgetCheck->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        for (int j = 1; j < dataRes[i].count(); j++) {
            ui->tableWidgetCheck->setItem(i, j, new QTableWidgetItem(dataRes[i][j]));
        }
    }
}

InstructionLib* VerifyWindow::getCmdByCheckName(int idx, QString name)
{
    if (idx >= 0 && idx < connectList.length()) {
        for (InstructionLib& item : connectList[idx].cmdList) {
            // 核查项目belong_checkname
            if (item.belong_checkname == name)
                return &item;
        }
    }
    return nullptr;
}

VisaCommonEngine* VerifyWindow::getVisaEngineByIdx(int idx)
{
    if (idx >= 0 && idx < connectList.length()) {
        return connectList[idx].visaEngine;
    }
    return nullptr;
}

void VerifyWindow::Delay_MSec(unsigned int msec)
{
    QTime _Timer = QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() < _Timer)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void VerifyWindow::on_pushButton_start_clicked()
{
    startVerify();
    // initCheckTable();
    //    for(int i=0;i<measureCalList.length();i++)
    //    {
    //        StandCheckDataCalibrator& item = measureCalList[i];
    //        QString sRet="";
    //        //标准器输出
    //        if(standardEngine.getState()){
    //            InstructionLib *instrcutLib = getCmdByCheckName(1,item.params);
    //            if(instrcutLib!=nullptr){
    //                QStringList sList = instrcutLib->instruct_config.split(";");
    //                if(sList.length()>0){
    //                    for (QString& sCmd : sList){
    //                        standardEngine.sendData(sCmd);
    //                    }
    //                }
    //            }
    //        }
    //        //被检设备读数
    //        if(deviceEngine.getState()){
    //            InstructionLib *instrcutLib = getCmdByCheckName(0,item.params);
    //            if(instrcutLib!=nullptr){
    //                QStringList sList = instrcutLib->instruct_config.split(";");
    //                if(sList.length()>0){
    //                    for (QString& sCmd : sList){
    //                        deviceEngine.sendData(sCmd);
    //                    }
    //                }
    //                Delay_MSec(2000);
    //                sRet = deviceEngine.queryData(instrcutLib->instruct_read);
    //            }
    //        }
    //        //
    //        if(sRet==""){
    //            bool bOk = false;
    //            double dRet = QInputDialog::getDouble(this,"手动测试","请输入被检示值",0,-100000,	100000,1,&bOk);

    //            if (bOk) {
    //                sRet = QString::number(dRet);
    //            }
    //        }
    //        item.test_value_one = sRet;
    //        ui->tableWidgetCheck->setItem(i,19,new QTableWidgetItem(sRet));
    //    }
}

int VerifyWindow::getIndexByHeader(QString sLabel, QString sLabel2, QStringList sName)
{
    const auto& headrs = veriHeadList.value(sLabel).value(sLabel2);
    for (int j = 0; j < sName.count(); j++) {
        for (int i = 0; i < headrs.length(); i++) {
            if (headrs[i] == sName[j])
                return i;
        }
    }
    return -1;
}

void VerifyWindow::startVerify()
{
    if(Teststate==SyncRunStatus::Running)
        return;
    Teststate=SyncRunStatus::Running;
    connectDevice();

    for (int i = 0; i < veriHeadList.keys().length(); i++) {  // 遍历每一个检定项
        if (Teststate == SyncRunStatus::Stopped) {
            break;
        };

        // 每次循环按钮变化
        initCheckTable(i);   // 切换对应的检定项 表格
        constsLable = veriHeadList.keys()[i];  // 当前检定项名称
        if(!veriDataMapList.keys().contains(constsLable))
            continue;
        setReadItemHeader();
        auto veriData = veriDataMapList.value(constsLable);  // 检定项对应的表格数据，相当于该检定项的每一个核查点，
        QString labelItem;
        if (veriData.keys().contains("重复性")) {
            labelItem = "重复性";
            constsLableItem = labelItem;
            int itemIdx = veriData.keys().indexOf(labelItem);
            initCheckItemData(itemIdx, "ALL", true);
        } else {
            constsLableItem = labelItem = veriData.keys().at(0);
        }
        QList<QStringList> dataRes = veriData.value(labelItem);

        BaseCommonApi::SaveLogInfo(1, QString("开始 %1 测试").arg(constsLable));

        if (iEquipType == 4){
            if (iEquipType == 4 and veriHeadList.keys().contains("压力值")) {
                // 压力值 为人工读数并记录
                for(QString head : readItems) {
                    for (int i = 0; i < dataRes.count(); i++) {
                        double dStdValue = FP_INFINITE;
                        autoAdujstData(constsLable, labelItem, head + readItemsSecond.at(0), i, dStdValue, dataRes, veriData);
                        Delay_MSec(1000);
                    }

                    for (int i = 0; i < dataRes.count(); i++) {
                        double dStdValue = FP_INFINITE;
                        autoAdujstData(constsLable, labelItem, head + readItemsSecond.at(1), dataRes.count() - i - 1, dStdValue, dataRes, veriData);
                        Delay_MSec(1000);
                    }
                }
            }
            else {
                for (int i = 0; i < dataRes.count(); ++i) {
                    if(Teststate==SyncRunStatus::Stopped){break;};
                    constDataRes=dataRes[i]; // 表格指定行数据
                    //InItcmd();
                    QString sRet="";
                    QString newCmd;
                    double dStdValue=FP_INFINITE,dError=0;
                    QString newdStdValue,newdError;

                    //标准器输出
                    BaseCommonApi::SaveLogInfo(1,QString("标准器输出"));
                    QString sParam;
                    QStringList TempValue;
                    TempValue.append("核查项目");
                    int idx= getIndexByHeader(constsLable,labelItem,TempValue);
                    if(idx!=-1)
                        sParam = dataRes[i][idx];
                    CheckBackColorUpdate(true,i);

                    //暂时加到这个地方 后面统一编写
                    if(sParam=="DCI"||sParam=="ACI")
                    {
                        int idx2= getIndexByHeader(constsLable,labelItem,QStrTranQStrList("单位"));
                        if(idx2!=-1)
                        {
                            QString ConstUnit="";
                            QString v=dataRes[i][idx2].toUpper();
                            if(i!=0)
                                ConstUnit=dataRes[i-1][idx2].toUpper();

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
                    //标准器输出
                    InstructionLib *instrcutLibstan = getCmdByCheckName(0,sParam);
                    VisaCommonEngine *standardEngine = getVisaEngineByIdx(0);
                    SendDevice(instrcutLibstan,standardEngine,true);
                    //核查件输出
                    BaseCommonApi::SaveLogInfo(1,QString("核查件配置"));
                    InstructionLib *instrcutLib = getCmdByCheckName(1,sParam);
                    VisaCommonEngine *deviceEngine = getVisaEngineByIdx(1);
                    SendDevice(instrcutLib,deviceEngine);
                    //核查件读取6次测试值
                    qDebug()<<"readItems" << readItems;
                    for(QString head : readItems){
                        sRet=ReadDevice(instrcutLib,deviceEngine); // 程控读取返回值
                        if(sRet!=""){
                            TempValue.clear();
                            TempValue.append("单位");
                            idx= getIndexByHeader(constsLable,labelItem,TempValue);
                            dStdValue = transUnit(sRet,dataRes[i][idx]);
                        }
                        autoAdujstData(constsLable,labelItem,head,i,dStdValue,dataRes,veriData);
                        //CheckBackColorUpdate(false,i);
                        Delay_MSec(500);
                    }
                    closeCmd();
                    CheckBackColorUpdate(false,i);
                    Delay_MSec(2000);
                }
            }
        }
        else{
            for (int i = 0; i < dataRes.count(); ++i) {
                        if (Teststate == SyncRunStatus::Stopped) {
                            break;
                        };
                        constDataRes = dataRes[i];
                        // InItcmd();
                        QString sRet = "";
                        QString newCmd;
                        double dStdValue = FP_INFINITE, dError = 0;
                        QString newdStdValue, newdError;

                        QString sParam;
                        QStringList TempValue;
                        TempValue.append("核查项目");
                        int idx = getIndexByHeader(constsLable, labelItem, TempValue);
                        if (idx != -1)
                            sParam = dataRes[i][idx];
                        CheckBackColorUpdate(true, i);

                        // 暂时加到这个地方 后面统一编写
                        if (sParam == "DCI" || sParam == "ACI") {
                            int idx2 = getIndexByHeader(constsLable, labelItem, QStrTranQStrList("单位"));
                            if (idx2 != -1) {
                                QString ConstUnit = "";
                                QString v = dataRes[i][idx2].toUpper();
                                if (i != 0)
                                    ConstUnit = dataRes[i - 1][idx2].toUpper();

                                BaseCommonApi::SaveLogInfo(1, ConstUnit + "-" + v);
                                if ((ConstUnit == "MA" && v == "A") || (ConstUnit == "A" && v == "MA")) {
                                    if (!this->showDialog("电流换线提醒", QString("换线提醒\n %1 -> %2").arg(ConstUnit).arg(v))) {
                                        continue;
                                    }
                                }
                            }
                        }
                        InstructionLib* instrcutLibstan;
                        VisaCommonEngine* standardEngine;
                        InstructionLib* instrcutLib;
                        VisaCommonEngine* deviceEngine;
                        if(bizOutMode){
                            instrcutLibstan = getCmdByCheckName(0, sParam);
                            standardEngine = getVisaEngineByIdx(0);
                            instrcutLib = getCmdByCheckName(1, sParam);
                            deviceEngine = getVisaEngineByIdx(1);
                        }else{
                            instrcutLibstan = getCmdByCheckName(1, sParam);
                            standardEngine = getVisaEngineByIdx(1);
                            instrcutLib = getCmdByCheckName(0, sParam);
                            deviceEngine = getVisaEngineByIdx(0);
                        }
                        // 被核查件输出
                        BaseCommonApi::SaveLogInfo(1, QString("源输出设备配置"));
                        SendDevice(instrcutLibstan, standardEngine, true);
                        // 标准器输出
                        BaseCommonApi::SaveLogInfo(1, QString("读数设备配置"));
                        SendDevice(instrcutLib, deviceEngine);
                        // 循环读取N次测试值
                        for (QString head : readItems) {
                            if (Teststate == SyncRunStatus::Stopped) {
                                break;
                            };
                            sRet = ReadDevice(instrcutLib, deviceEngine);
                            if (sRet != "") {
                                TempValue.clear();
                                TempValue.append("单位");
                                idx = getIndexByHeader(constsLable, labelItem, TempValue);
                                dStdValue = transUnit(sRet, dataRes[i][idx]);
                            }
                            autoAdujstData(constsLable, labelItem, head, i, dStdValue, dataRes, veriData);
                            // CheckBackColorUpdate(false,i);
                            Delay_MSec(1000);
                        }
                        closeCmd();
                        CheckBackColorUpdate(false, i);
                        Delay_MSec(2000);
                    }

        }
        veriData.insert(labelItem, dataRes);
        veriDataMapList.insert(constsLable, veriData);
    }
    closeDevice();
    Teststate = SyncRunStatus::Stopped;
}

void VerifyWindow::setReadItemHeader(){
    if (iEquipType == 5) {
        readItems.clear();
        if (constsLable == "温度值")
            readItems << "测量标准示值1"
                      << "测量标准示值2"
                      << "测量标准示值3"
                      << "测量标准示值4"
                      << "测量标准示值5"
                      << "测量标准示值6";
        else if (constsLable == "RTP值")
            readItems << "被校示值(Ω)1"
                      << "被校示值(Ω)2"
                      << "被校示值(Ω)3"
                      << "被校示值(Ω)4"
                      << "被校示值(Ω)5"
                      << "被校示值(Ω)6";
        else if (constsLable == "绝缘电阻表校准")
            readItems << "测量值";
        else if (constsLable == "测温仪电阻核查")
            readItems << "测温仪示值1"
                      << "测温仪示值2"
                      << "测温仪示值3"
                      << "测温仪示值4"
                      << "测温仪示值5"
                      << "测温仪示值6";
    }else if (iEquipType == 13) {
        readItems.clear();
        if (constsLable == "稳幅正弦信号电压平坦度")
            readItems  << "测量值X1"
                       << "测量值X2"
                       << "测量值X3"
                       << "测量值X4"
                       << "测量值X5"
                       << "测量值X6"
                       << "测量值X7"
                       << "测量值X8"
                       << "测量值X9"
                       << "测量值X10";
        else if (constsLable == "上升时间")
            readItems << "测量值";
        else {
            readItems  << "测量值*X1"
                       << "测量值*X2"
                       << "测量值*X3"
                       << "测量值*X4"
                       << "测量值*X5"
                       << "测量值*X6"
                       << "测量值*X7"
                       << "测量值*X8"
                       << "测量值*X9"
                       << "测量值*X10";
        }
    }
}

void VerifyWindow::CheckBackColorUpdate(bool State, int row)
{
    QBrush colorBrush(Qt::white);
    if (State) {
        QColor lightGreenColor(173, 216, 230);
        colorBrush.setColor(lightGreenColor);
    }
    for (int col = 0; col < ui->tableWidgetCheck->columnCount(); ++col) {
        QTableWidgetItem* item = ui->tableWidgetCheck->item(row, col);
        if (item) {
            item->setBackground(colorBrush);
        }
    }
}

QString VerifyWindow::FormatDecimals(double dVal, int pos)
{
    if (dVal == 0)
        return QString::number(dVal);
    if (pos != -1)
        return QString::number(dVal, 'f', pos);
    if (abs(dVal) >= 1)
        return QString::number(dVal, 'f', 2);
    else {
        //        QString sVal =  QString::number(dVal);
        //        int idx=0;
        //        for(int i=0;i<sVal.length();i++){
        //            if(sVal[i]!='0'&&sVal[i]!="."){
        //                idx = i;
        //                break;
        //            }
        //        }
        //        idx = idx-sVal.indexOf('.')+2;
        //        QString dTmp = QString::number(dVal,'f',idx);
        return QString::number(dVal, 'E', 2);
    }
}

void VerifyWindow::autoAdujstData(QString sLabel, QString labelItem, QString header, int i, double dStdValue, QList<QStringList>& dataRes, QMap<QString, QList<QStringList>>& veriData)
{
    // double dError;
    QString newdStdValue, newdError, Value;
    if (dStdValue == FP_INFINITE) {
        bool bOk = false;
        dStdValue = QInputDialog::getDouble(this, "手动测试", "请输入被检示值", 0, -100000, 100000, 5, &bOk);

        if (bOk) {
            // sRet = QString::number(dStdValue);
        } else {
            dStdValue = 0;
        }
    }
    //  ItemUnitSplit
    int dotPos =-1;
    if(iEquipType == 11)
        dotPos=3;
    else if(iEquipType==13){
        dotPos=-2;
        int idx = getIndexByHeader(sLabel, labelItem, QStrTranQStrList("核查点单位"));
        if(idx!=-1){
            QString veriUnit= dataRes[i][idx].toUpper();
            if(veriUnit=="GHZ"||veriUnit=="MHZ"||veriUnit=="KHZ"||veriUnit=="HZ")
                dotPos=6;
            else if(veriUnit=="S"||veriUnit=="MS"||veriUnit=="US"||veriUnit=="NS"||veriUnit=="PS")
                dotPos=9;
        }
    }
    // newdStdValue= TranDecimals(sLabel, dataRes[i],dStdValue);
    newdStdValue = FormatDecimals(dStdValue,dotPos);
    // QStringList ResultName=getsParambyHeader(sParam);
    int idx = getIndexByHeader(sLabel, labelItem, QStrTranQStrList(header));
    if (idx != -1) {
        dataRes[i][idx] = newdStdValue;
        ui->tableWidgetCheck->setItem(i, idx, new QTableWidgetItem(newdStdValue)); // 示值
    }
    if (iEquipType == 1 || iEquipType == 2 || iEquipType == 10|| iEquipType == 11|| iEquipType == 18) {
        if (header == readItems.last()) {
            // 计算平均值
            double dMean = 0;
            for (QString item : readItems) {
                idx = getIndexByHeader(sLabel, labelItem, QStrTranQStrList(item));
                dMean += dataRes[i][idx].toDouble();
            }
            dMean /= readItems.count();
            newdStdValue = FormatDecimals(dMean,dotPos);
            idx = getIndexByHeader(sLabel, labelItem, { "算数平均值", "平均值", "平均值x" });
            if (idx != -1) {
                dataRes[i][idx] = newdStdValue;
                ui->tableWidgetCheck->setItem(i, idx, new QTableWidgetItem(newdStdValue));
            }
            // 计算标准偏差S
            double dStdErr = 0;
            for (QString item : readItems) {
                idx = getIndexByHeader(sLabel, labelItem, QStrTranQStrList(item));
                dStdErr += pow(dataRes[i][idx].toDouble() - dMean, 2);
            }
            dStdErr = sqrt(dStdErr / (readItems.count() - 1));
            newdStdValue = FormatDecimals(dStdErr,dotPos);
            idx = getIndexByHeader(sLabel, labelItem, { "标准偏差S(X)", "标准偏差", "重复性S(x)/x","重复性S(x)"});
            if (idx != -1) {
                dataRes[i][idx] = newdStdValue;
                ui->tableWidgetCheck->setItem(i, idx, new QTableWidgetItem(newdStdValue));
            }
            // 计算相对重复性
            newdStdValue = FormatDecimals(dStdErr / dMean);
            idx = getIndexByHeader(sLabel, labelItem, { "相对重复性", "相对标准偏差" });
            if (idx != -1) {
                dataRes[i][idx] = newdStdValue;
                ui->tableWidgetCheck->setItem(i, idx, new QTableWidgetItem(newdStdValue));
            }

            // 同步计算稳定性数据
            QString labelStable = "稳定性";
            // auto veriData = veriDataMapList.value(constsLable);
            QList<QStringList> dataStable = veriData.value(labelStable);
            idx = getIndexByHeader(sLabel, labelStable, { "算数平均值", "平均值" });
            newdStdValue = FormatDecimals(dMean,dotPos);
            if (idx != -1 && i<dataStable.count()) {
                //本次核查数值
                dataStable[i][idx - 1] = newdStdValue;
                dStdErr = getRepeatData(dataStable[i], idx, dMean);
                //稳定性平均值
                newdStdValue = FormatDecimals(dMean,dotPos);
                dataStable[i][idx] = newdStdValue;
                if (dStdErr != FP_INFINITE) {
                    newdStdValue = FormatDecimals(dStdErr);
                    idx = getIndexByHeader(sLabel, labelStable, { "标准偏差S(X)", "标准偏差", "稳定性" });
                    if (idx != -1) {
                        dataStable[i][idx] = newdStdValue;
                    }
                    // 计算相对重复性

                    idx = getIndexByHeader(sLabel, labelStable, { "相对重复性", "相对标准偏差" });
                    if (idx != -1) {
                        dStdErr /= dMean;
                        newdStdValue = FormatDecimals(dStdErr);
                        dataStable[i][idx] = newdStdValue;
                    }
                    idx = getIndexByHeader(sLabel, labelStable, { "Urel", "相对扩展不确定度Urel", "U(k=2)" });
                    QString sRet = "";
                    if (idx != -1) {
                        if(iEquipType == 11||iEquipType == 18){
                            if (dStdErr < dataStable[i][idx].toDouble())
                                sRet = "合格";
                            else
                                sRet = "不合格";
                        }else{
                            if (dStdErr < dataStable[i][idx].toDouble())
                                sRet = "是";
                            else
                                sRet = "否";
                        }
                    }
                    idx = getIndexByHeader(sLabel, labelStable, { "相对重复性是否小于相对扩展不确定度Urel", "相对标准偏差是否小于相对扩展不确定度Urel", "结论" });
                    if (idx != -1)
                        dataStable[i][idx] = sRet;
                }
                veriData.insert(labelStable, dataStable);
            }
        }
        // BaseCommonApi::SaveLogInfo(1,QString("转换前 测试值 %1").arg(dStdValue));
        // BaseCommonApi::SaveLogInfo(1,QString("转换后 测试值 %1").arg(newdStdValue));
        // Delay_MSec(2000);
    } else if (iEquipType == 5) {
        if (header == readItems.last()) {
            double dMean = 0;
            double dStdErr = 0;
            if (constsLable == "温度值") {
                QMap<QString, QString> stdItems;
                stdItems.insert("核查标准示值1", "差值1");
                stdItems.insert("核查标准示值2", "差值2");
                stdItems.insert("核查标准示值3", "差值3");
                stdItems.insert("核查标准示值4", "差值4");
                stdItems.insert("核查标准示值5", "差值5");
                stdItems.insert("核查标准示值6", "差值6");
                int idx2;
                QList<double> errList;
                for (int k = 0; k < stdItems.count(); k++) {
                    QString key = stdItems.keys().at(k);
                    idx = getIndexByHeader(sLabel, labelItem, QStrTranQStrList(readItems[k]));
                    idx2 = getIndexByHeader(sLabel, labelItem, QStrTranQStrList(key));
                    dStdErr = dataRes[i][idx2].toDouble() - dataRes[i][idx].toDouble();
                    errList.append(dStdErr);
                    dMean += dStdErr;
                    newdStdValue = FormatDecimals(dStdErr);
                    idx2 = getIndexByHeader(sLabel, labelItem, QStrTranQStrList(stdItems.value(key)));
                    if (idx2 != -1) {
                        dataRes[i][idx2] = newdStdValue;
                        ui->tableWidgetCheck->setItem(i, idx2, new QTableWidgetItem(newdStdValue));
                    }
                }
                dMean /= stdItems.count();
                newdStdValue = FormatDecimals(dMean);
                idx = getIndexByHeader(sLabel, labelItem, QStrTranQStrList("平均值(差值)"));
                if (idx != -1) {
                    dataRes[i][idx] = newdStdValue;
                    ui->tableWidgetCheck->setItem(i, idx, new QTableWidgetItem(newdStdValue));
                }
                // 计算重复性S(x)
                dStdErr = 0;
                for (double err : errList) {
                    dStdErr += pow(err - dMean, 2);
                }
                dStdErr = sqrt(dStdErr / (errList.count() - 1));
                errList.clear();
                newdStdValue = FormatDecimals(dStdErr);
                idx = getIndexByHeader(sLabel, labelItem, QStrTranQStrList("重复性S(x)"));
                if (idx != -1) {
                    dataRes[i][idx] = newdStdValue;
                    ui->tableWidgetCheck->setItem(i, idx, new QTableWidgetItem(newdStdValue));
                }
                // 同步计算稳定性数据
                QString labelStable = "稳定性";
                // auto veriData = veriDataMapList.value(constsLable);
                QList<QStringList> dataStable = veriData.value(labelStable);
                idx = getIndexByHeader(sLabel, labelStable, QStrTranQStrList("平均值"));
                newdStdValue = FormatDecimals(dMean);
                if (idx != -1&& i<dataStable.count()) {
                    if(idx-1<dataStable[i].count())
                        dataStable[i][idx - 1] = newdStdValue;
                    dStdErr = getRepeatData(dataStable[i], idx, dMean);
                    newdStdValue = FormatDecimals(dMean);
                    if(idx<dataStable[i].count())
                        dataStable[i][idx] = newdStdValue;
                    if (dStdErr != FP_INFINITE) {
                        newdStdValue = FormatDecimals(dStdErr);
                        idx = getIndexByHeader(sLabel, labelStable, QStrTranQStrList("Sm"));
                        if (idx != -1&&idx<dataStable[i].count()) {
                            dataStable[i][idx] = newdStdValue;
                        }

                        idx = getIndexByHeader(sLabel, labelStable, QStrTranQStrList("扩展不确定度U"));
                        if (idx != -1&&idx<dataStable[i].count()){
                            QString sRet = "";
                            if (dStdErr <= dataStable[i][idx].toDouble())
                                sRet = "是";
                            else
                                sRet = "否";
                            idx = getIndexByHeader(sLabel, labelStable, QStrTranQStrList("是否Sm≤U"));
                            if (idx != -1&&idx<dataStable[i].count())
                                dataStable[i][idx] = sRet;
                        }
                    }
                    veriData.insert(labelStable, dataStable);
                }
            } else if (constsLable == "RTP值" || constsLable == "测温仪电阻核查") {
                // 计算平均值
                double dMean = 0, dMax = FP_INFINITE, dMin = FP_INFINITE;
                for (QString item : readItems) {
                    idx = getIndexByHeader(sLabel, labelItem, QStrTranQStrList(item));
                    if (dataRes[i][idx] != "") {
                        dMean += dataRes[i][idx].toDouble();
                        if (dMax == FP_INFINITE)
                            dMin = dMax = dataRes[i][idx].toDouble();
                        else {
                            dMax = qMax(dMax, dataRes[i][idx].toDouble());
                            dMin = qMin(dMin, dataRes[i][idx].toDouble());
                        }
                    }
                }
                dMean /= readItems.count();
                newdStdValue = FormatDecimals(dMean);
                idx = getIndexByHeader(sLabel, labelItem, { "平均值" });
                if (idx != -1) {
                    dataRes[i][idx] = newdStdValue;
                    ui->tableWidgetCheck->setItem(i, idx, new QTableWidgetItem(newdStdValue));
                }
                if (constsLable == "RTP值") {
                    QString labelStable = "稳定性";
                    QList<QStringList> dataStable = veriData.value(labelStable);
                    idx = getIndexByHeader(sLabel, labelStable, QStrTranQStrList("测量极差值"));
                    newdStdValue = FormatDecimals(dMean);
                    if (idx != -1&& i<dataStable.count()) {
                        if(idx-1<dataStable[i].count())
                            dataStable[i][idx - 1] = newdStdValue;
                        int cnt = 0;
                        double dMax, dMin;
                        for (int k = idx - 6; k < idx; k++) {
                            if (k>=0&&k<dataStable[i].count()&&dataStable[i][k] != "") {
                                if (cnt == 0) {
                                    dMax = dMin = dataStable[i][k].toDouble();
                                } else {
                                    dMax = qMax(dMax, dataStable[i][k].toDouble());
                                    dMin = qMin(dMin, dataStable[i][k].toDouble());
                                }
                                cnt++;
                            }
                        }
                        if (cnt >= 4) {
                            dStdErr = dMax - dMin;
                            newdStdValue = FormatDecimals(dStdErr);
                            if(idx<dataStable[i].count())
                                dataStable[i][idx] = newdStdValue;
                            idx = getIndexByHeader(sLabel, labelStable, QStrTranQStrList("允许值"));
                            if(idx!=-1&&idx<dataStable[i].count()){
                                QString sRet = "";
                                if (dStdErr <= dataStable[i][idx].toDouble())
                                    sRet = "合格";
                                else
                                    sRet = "不合格";
                                idx = getIndexByHeader(sLabel, labelStable, QStrTranQStrList("结论"));
                                if (idx != -1&&idx<dataStable[i].count())
                                    dataStable[i][idx] = sRet;
                            }
                        }
                        veriData.insert(labelStable, dataStable);
                    }
                } else if (constsLable == "测温仪电阻核查") {
                    idx = getIndexByHeader(sLabel, labelItem, { "核查点" });
                    dMax = dMax - dataRes[i][idx].toDouble();
                    newdStdValue = FormatDecimals(dMax);
                    idx = getIndexByHeader(sLabel, labelItem, { "最大偏差" });
                    if (idx != -1) {
                        dataRes[i][idx] = newdStdValue;
                        ui->tableWidgetCheck->setItem(i, idx, new QTableWidgetItem(newdStdValue));
                    }
                }
            } else if (constsLable == "绝缘电阻表校准") {
                idx = getIndexByHeader(sLabel, labelItem, { "确认点" });
                if (idx != -1) {
                    dStdErr = dStdValue - dataRes[i][idx].toDouble();
                    newdStdValue = FormatDecimals(dStdErr);
                    idx = getIndexByHeader(sLabel, labelItem, { "示值误差" });
                    dataRes[i][idx] = newdStdValue;
                    ui->tableWidgetCheck->setItem(i, idx, new QTableWidgetItem(newdStdValue));
                }
            }
        }
    } else if (iEquipType == 9) {
        if (header == readItems.last()) {
            // 计算平均值
            double dMean = 0;
            for (QString item : readItems) {
                idx = getIndexByHeader(sLabel, labelItem, QStrTranQStrList(item));
                dMean += dataRes[i][idx].toDouble();
            }
            dMean /= readItems.count();
            newdStdValue = FormatDecimals(dMean);
            idx = getIndexByHeader(sLabel, labelItem, { "平均值x" });
            if (idx != -1) {
                dataRes[i][idx] = newdStdValue;
                ui->tableWidgetCheck->setItem(i, idx, new QTableWidgetItem(newdStdValue));
            }
            // 计算标准偏差S
            double dStdErr = 0;
            for (QString item : readItems) {
                idx = getIndexByHeader(sLabel, labelItem, QStrTranQStrList(item));
                dStdErr += pow(dataRes[i][idx].toDouble() - dMean, 2);
            }
            dStdErr = sqrt(dStdErr / (readItems.count() - 1));
            // 计算相对重复性
            newdStdValue = FormatDecimals(dStdErr / dMean);
            idx = getIndexByHeader(sLabel, labelItem, { "重复性S(x)/x" });
            if (idx != -1) {
                dataRes[i][idx] = newdStdValue;
                ui->tableWidgetCheck->setItem(i, idx, new QTableWidgetItem(newdStdValue));
            }

            // 同步计算稳定性数据
            QString labelStable = "稳定性";
            // auto veriData = veriDataMapList.value(constsLable);
            QList<QStringList> dataStable = veriData.value(labelStable);
            idx = getIndexByHeader(sLabel, labelStable, { "平均值" });
            newdStdValue = FormatDecimals(dMean);
            if (idx != -1) {
                dataStable[i][idx - 1] = newdStdValue;
                dStdErr = getRepeatData(dataStable[i], idx, dMean);
                newdStdValue = FormatDecimals(dMean);
                dataStable[i][idx] = newdStdValue;
                if (dStdErr != FP_INFINITE) {
                    newdStdValue = FormatDecimals(dStdErr);
                    idx = getIndexByHeader(sLabel, labelStable, { "稳定性" });
                    if (idx != -1) {
                        dStdErr /= dMean;
                        newdStdValue = FormatDecimals(dStdErr);
                        dataStable[i][idx] = newdStdValue;
                    }

                    idx = getIndexByHeader(sLabel, labelStable, { "U(k=2)" });
                    QString sRet = "";
                    if (idx != -1) {
                        if (dStdErr < dataStable[i][idx].toDouble())
                            sRet = "合格";
                        else
                            sRet = "不合格";
                    }
                    idx = getIndexByHeader(sLabel, labelStable, { "结论" });
                    if (idx != -1)
                        dataStable[i][idx] = sRet;
                }
                veriData.insert(labelStable, dataStable);
            }
        }
    }
    else if (iEquipType == 13) {
        if (header == readItems.last()) {
            // 计算平均值
            double dMean = 0;
            // 计算标准偏差S
            double dStdErr = 0;
            if (constsLable == "稳幅正弦信号电压平坦度")
            {
                QMap<QString, QString> stdItems;
                stdItems.insert("转换为电压值X1", "平坦度/%X1");
                stdItems.insert("转换为电压值X2", "平坦度/%X2");
                stdItems.insert("转换为电压值X3", "平坦度/%X3");
                stdItems.insert("转换为电压值X4", "平坦度/%X4");
                stdItems.insert("转换为电压值X5", "平坦度/%X5");
                stdItems.insert("转换为电压值X6", "平坦度/%X6");
                stdItems.insert("转换为电压值X7", "平坦度/%X7");
                stdItems.insert("转换为电压值X8", "平坦度/%X8");
                stdItems.insert("转换为电压值X9", "平坦度/%X9");
                stdItems.insert("转换为电压值X10", "平坦度/%X10");
                int idx2,idx3;
                QList<double> errList;
                double voltU0 = 1;
                for (int k = 0; k < stdItems.count(); k++) {
                    QString key = stdItems.keys().at(k);
                    idx = getIndexByHeader(sLabel, labelItem, QStrTranQStrList(readItems[k]));
                    idx2 = getIndexByHeader(sLabel, labelItem, QStrTranQStrList(key));
                    //转换为电压值
                    dStdErr = sqrt(50*dataRes[i][idx].toDouble());
                    newdStdValue = FormatDecimals(dStdErr,4);
                    dataRes[i][idx2] = newdStdValue;
                    ui->tableWidgetCheck->setItem(i, idx2, new QTableWidgetItem(newdStdValue));
                    //平坦度/%
                    idx2 = getIndexByHeader(sLabel, labelItem, QStrTranQStrList(stdItems.value(key)));
                    if(i==0){
                        dataRes[i][idx2] = "/";
                        ui->tableWidgetCheck->setItem(i, idx2, new QTableWidgetItem("/"));
                    }else{
                        dStdErr = (dStdErr - voltU0)/voltU0;
                        newdStdValue = FormatDecimals(dStdErr,3);
                        dataRes[i][idx2] = newdStdValue;
                        ui->tableWidgetCheck->setItem(i, idx2, new QTableWidgetItem(newdStdValue));

                        errList.append(dStdErr);
                        dMean += dStdErr;
                    }
                }
                dMean /= stdItems.count();
                // 计算重复性S(x)
                if(i==0){
                    newdStdValue = "/";
                    idx = getIndexByHeader(sLabel, labelItem, {"平均值x"});
                    if (idx != -1) {
                        dataRes[i][idx] = newdStdValue;
                        ui->tableWidgetCheck->setItem(i, idx, new QTableWidgetItem(newdStdValue));
                    }
                    idx = getIndexByHeader(sLabel, labelItem, QStrTranQStrList("重复性S(x)"));
                    if (idx != -1) {
                        dataRes[i][idx] = newdStdValue;
                        ui->tableWidgetCheck->setItem(i, idx, new QTableWidgetItem(newdStdValue));
                    }
                }else{
                    newdStdValue = FormatDecimals(dMean);
                    idx = getIndexByHeader(sLabel, labelItem, {"平均值x"});
                    if (idx != -1) {
                        dataRes[i][idx] = newdStdValue;
                        ui->tableWidgetCheck->setItem(i, idx, new QTableWidgetItem(newdStdValue));
                    }
                    dStdErr = 0;
                    for (double err : errList) {
                        dStdErr += pow(err - dMean, 2);
                    }
                    dStdErr = sqrt(dStdErr / (errList.count() - 1));
                    errList.clear();
                    newdStdValue = FormatDecimals(dStdErr);
                    idx = getIndexByHeader(sLabel, labelItem, QStrTranQStrList("重复性S(x)"));
                    if (idx != -1) {
                        dataRes[i][idx] = newdStdValue;
                        ui->tableWidgetCheck->setItem(i, idx, new QTableWidgetItem(newdStdValue));
                    }
                }

            }else{
                for (QString item : readItems) {
                    idx = getIndexByHeader(sLabel, labelItem, QStrTranQStrList(item));
                    dMean += dataRes[i][idx].toDouble();
                }
                dMean /= readItems.count();
                newdStdValue = FormatDecimals(dMean,dotPos);
                idx = getIndexByHeader(sLabel, labelItem, {"平均值x"});
                if (idx != -1) {
                    dataRes[i][idx] = newdStdValue;
                    ui->tableWidgetCheck->setItem(i, idx, new QTableWidgetItem(newdStdValue));
                }

                for (QString item : readItems) {
                    idx = getIndexByHeader(sLabel, labelItem, QStrTranQStrList(item));
                    dStdErr += pow(dataRes[i][idx].toDouble() - dMean, 2);
                }
                dStdErr = sqrt(dStdErr / (readItems.count() - 1));
                newdStdValue = FormatDecimals(dStdErr,dotPos);
                idx = getIndexByHeader(sLabel, labelItem, {"重复性S(x)"});
                if (idx != -1) {
                    dataRes[i][idx] = newdStdValue;
                    ui->tableWidgetCheck->setItem(i, idx, new QTableWidgetItem(newdStdValue));
                }
            }

            // 同步计算稳定性数据
            QString labelStable = "稳定性";
            // auto veriData = veriDataMapList.value(constsLable);
            QList<QStringList> dataStable = veriData.value(labelStable);
            if(constsLable == "上升时间"){
                if (i<dataStable.count()){
                    newdStdValue = FormatDecimals(dStdValue);
                    int idx = getIndexByHeader(sLabel, labelStable, QStrTranQStrList(header));
                    if (idx != -1) {
                        dataStable[i][idx] = newdStdValue;
                    }
                    idx = getIndexByHeader(sLabel, labelStable, { "技术指标"});
                    QString sRet = "";
                    if (idx != -1) {
                        if (dStdValue < dataStable[i][idx].toDouble())
                            sRet = "合格";
                        else
                            sRet = "不合格";
                    }
                    idx = getIndexByHeader(sLabel, labelStable, {"结论" });
                    if (idx != -1)
                        dataStable[i][idx] = sRet;
                    veriData.insert(labelStable, dataStable);
                }
            }else{
                idx = getIndexByHeader(sLabel, labelStable, { "算数平均值", "平均值" });
                newdStdValue = FormatDecimals(dMean,dotPos);
                if (idx != -1 && i<dataStable.count()) {
                    //本次核查数值
                    dataStable[i][idx - 1] = newdStdValue;
                    dStdErr = getRepeatData(dataStable[i], idx, dMean);
                    //稳定性平均值
                    newdStdValue = FormatDecimals(dMean,dotPos);
                    dataStable[i][idx] = newdStdValue;
                    if (dStdErr != FP_INFINITE) {
                        newdStdValue = FormatDecimals(dStdErr);
                        idx = getIndexByHeader(sLabel, labelStable, {"稳定性" });
                        if (idx != -1) {
                            dataStable[i][idx] = newdStdValue;
                        }
                        idx = getIndexByHeader(sLabel, labelStable, { "最大允许误差的绝对值"});
                        QString sRet = "";
                        if (idx != -1) {
                            if (dStdErr < dataStable[i][idx].toDouble())
                                sRet = "合格";
                            else
                                sRet = "不合格";
                        }
                        idx = getIndexByHeader(sLabel, labelStable, {"结论" });
                        if (idx != -1)
                            dataStable[i][idx] = sRet;
                    }
                    veriData.insert(labelStable, dataStable);
                }
            }
        }
        // BaseCommonApi::SaveLogInfo(1,QString("转换前 测试值 %1").arg(dStdValue));
        // BaseCommonApi::SaveLogInfo(1,QString("转换后 测试值 %1").arg(newdStdValue));
        // Delay_MSec(2000);
    }
    else if (iEquipType == 4) {
            // 压力值:自动计算均值、最大误差、最大回程误差等
            if (sLabel == "压力值"){autoCalculateGaugeCheckData(i, idx, dataRes);}
            else{
                autoCalculateGaugeElecSignalCheckData(i, idx, dataRes);
            }

        }
}

void VerifyWindow::autoCalculateGaugeCheckData(int row, int col, QList<QStringList>& dataRes)
{
    QTableWidgetItem * item = ui->tableWidgetCheck->item(row, col);
    QString cellStr = item->text();

    // 只有第4列 - 第9列 数据值变化时要进行操作
    // 按行操作
    if (col >= 3 && col <= 8) {
        double checkVal = ui->tableWidgetCheck->item(row, 2)->text().toDouble(); // 核查点的值
        double avg = 0.0; // 均值计算结果
        double maxErr = 0.0; // 最大示值误差
        double maxReturnErr = 0.0; // 最大回程误差

        double sum = 0.0;
        int count = 0;

        double delta = 0.0;
        double deltaRet = 0.0;

        for (int i = 3; i < 9; i++) {
            QTableWidgetItem * cell = ui->tableWidgetCheck->item(row, i);
            if (cell != nullptr) {
                QString valCellStr = cell->text();

                if (valCellStr != "") {
                    // 计算均值
                    sum += valCellStr.toDouble();
                    count++;

                    // 计算最大示值误差
                    delta = qAbs(valCellStr.toDouble() - checkVal);
                    maxErr = qMax(delta, maxErr);

                    // 计算最大回程误差
                    // 列序号为偶数表示为回程数值 配对取前一个；列序号为奇数表示为去程数 配对去后一个
                    int indexRet = i % 2 == 0 ? (i - 1) : (i + 1);
                    QTableWidgetItem * pair = ui->tableWidgetCheck->item(row, indexRet);
                    if (pair != nullptr && pair->text() != "") {
                        deltaRet = qAbs(pair->text().toDouble() - valCellStr.toDouble());
                        maxReturnErr = qMax(deltaRet, maxReturnErr);
                    }
                }
            }
        }

        if (count > 0) {
            avg = sum / count * 1.0;
        }

        // 更新均值
        if (ui->tableWidgetCheck->item(row, 9) != nullptr) {
            ui->tableWidgetCheck->item(row, 9)->setText(QString::number(avg, 'f', 3));

            dataRes[row][9] = QString::number(avg, 'f', 3);
        }

        // 更新最大示值误差
        if (ui->tableWidgetCheck->item(row, 10) != nullptr) {
            ui->tableWidgetCheck->item(row, 10)->setText(QString::number(maxErr, 'f', 3));

            dataRes[row][10] = QString::number(maxErr, 'f', 3);
        }

        // 更新最大回程误差
        if (ui->tableWidgetCheck->item(row, 11) != nullptr) {
            ui->tableWidgetCheck->item(row, 11)->setText(QString::number(maxReturnErr, 'f', 3));

            dataRes[row][11] = QString::number(maxReturnErr, 'f', 3);
        }
    }
}
void VerifyWindow::autoCalculateGaugeElecSignalCheckData(int row, int col, QList<QStringList> &dataRes){
    //0.02活塞-电信号，核查，公式自动计算，更改dataRes
    // row,col:当前执行到单元格的行列索引，dataRes，表格数据
    if(col == 15){ // 当这些列变化时，计算第17列的平均值，第18列的最大示值误差

        double sum = 0.0;
        double avg = 0.0;
        double max = 0.0;
        for (int col_pointer =5; col_pointer <= 15; col_pointer+=2) { // 5,7,9,11,13
            sum += ui->tableWidgetCheck->item(row, col_pointer)->text().toDouble();
            double sub = ui->tableWidgetCheck->item(row, col_pointer)->text().toDouble() -
                    ui->tableWidgetCheck->item(row, col_pointer-1)->text().toDouble(); // col_pointer这一列减前一列的值
            if (abs(sub)>abs(max)){
                max = abs(sub);
            }
        }
        avg = sum/6;
        dataRes[row][17] = QString::number(avg, 'f', 3);
        dataRes[row][18] = QString::number(max, 'f', 3);
        ui->tableWidgetCheck->setItem(row,17,new QTableWidgetItem(QString::number(avg, 'f', 3)));
        ui->tableWidgetCheck->setItem(row,18,new QTableWidgetItem(QString::number(max, 'f', 3)));
    }

}
double VerifyWindow::getRepeatData(QStringList dataStable, int idx, double& dMean)
{
    int cnt = 0;
    double dRepeat = FP_INFINITE, dMax, dMin;
    dMean = 0;
    for (int k = idx - 6; k < idx; k++) {
        if (dataStable[k] != "") {
            dMean += dataStable[k].toDouble();
            if (cnt == 0) {
                dMax = dMin = dataStable[k].toDouble();
            } else {
                dMax = qMax(dMax, dataStable[k].toDouble());
                dMin = qMin(dMin, dataStable[k].toDouble());
            }
            cnt++;
        }
    }
    dMean /= cnt;
    if (cnt >= 4) {
        dRepeat = 0;
        if (cnt == 4) {
            dRepeat = (dMax - dMin) / 2.06;
        } else if (cnt == 5) {
            dRepeat = (dMax - dMin) / 2.33;
        } else if (cnt == 6) {
            for (int k = idx - 6; k < idx; k++) {
                dRepeat += pow(dataStable[k].toDouble() - dMean, 2);
            }
            dRepeat = sqrt(dRepeat / 5);
        }
    }
    return dRepeat;
}

double VerifyWindow::transUnit(QString sData, QString sUnit, bool reverse)
{
    double dTmp = sData.toDouble();
    QString unit = sUnit.toUpper();
    QStringList TempValue;
    for (const QStringList& key : unitTransList.keys()) {
        for (int i = 0; i < key.count(); i++) {
            if (key[i] == unit) {
                if (reverse) {
                    dTmp /= unitTransList.value(key)[0].toDouble();
                    return dTmp;
                } else {
                    dTmp *= unitTransList.value(key)[0].toDouble();
                    return dTmp;
                }
            }
        }
    }
    return dTmp;
}

QStringList VerifyWindow::QStrTranQStrList(QString value)
{
    QStringList valuelist;
    valuelist.append(value);
    return valuelist;
}

bool VerifyWindow::showDialog(QString title, QString mess)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, title, mess,
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        return true;
    } else {
        return false;
    }
}

void VerifyWindow::SendDevice(InstructionLib* Inst, VisaCommonEngine* Visa, bool IsUnitTran)
{
    // 发送仪器 进一步封装
    QString newCmd = "";
    if (Visa != nullptr && Visa->getState()) {
        if (Inst != nullptr) {
            QStringList sList = Inst->instruct_config.split(";");
            if (sList.length() > 0) {
                for (QString& sCmd : sList) {
                    newCmd = transCmd(sCmd, constsLable, constsLableItem, constDataRes, IsUnitTran);
                    if (newCmd != "") {
                        QString str = Visa->commType;
                        Visa->sendData(newCmd);
                        qDebug() << "config cmds:" << newCmd;
                    }
                }
            }
        }
    }
}
QString VerifyWindow::ReadDevice(InstructionLib* Inst, VisaCommonEngine* Visa, bool IsUnitTran)
{
    // 读取仪器 进一步封装
    QString newCmd = "";
    QString result = "";
    if (Visa != nullptr && Visa->getState()) {
        if (Inst != nullptr) {
            newCmd = transCmd(Inst->instruct_read, constsLable, constsLableItem, constDataRes, IsUnitTran);
            if (newCmd != "") {
                qDebug() << "query cmds:" << newCmd;
                result = Visa->queryData(newCmd);
            }
        }
    }
    return result;
}

void VerifyWindow::InItcmd()
{
    // 发送INIt相关指令
    int count = 0;
    for (const ConnectInfo& visa : connectList) {
        VisaCommonEngine* Engine = visa.visaEngine;
        InstructionLib* instrcutLib = getCmdByCheckName(count, "INIT");
        if (Engine != nullptr && instrcutLib != nullptr)
            SendDevice(instrcutLib, Engine);
        count++;
    }
}

void VerifyWindow::connectDevice()
{
    QString rolename;
    int count = 0;
    for (ConnectInfo& visa : connectList) {
        if (visa.programInterface.visa != "") {
            if (visa.visaEngine == nullptr)
                visa.visaEngine = new VisaCommonEngine;
            visa.visaEngine->openDevice(visa.programInterface.visa, "Visa");
            if (visa.visaEngine->getState()) {
                visa.state = "已连接";
                ui->tableWidget_Standard->setItem(count, 1, new QTableWidgetItem("ON"));
            }
        }
        // visa.state="已连接";
        // ui->tableWidget_Standard->setItem(count,1,new QTableWidgetItem("ON"));
        count++;
    }
}

void VerifyWindow::closeCmd()
{
    // 发送Close相关指令
    int count = 0;
    for (const ConnectInfo& visa : connectList) {
        VisaCommonEngine* Engine = visa.visaEngine;
        InstructionLib* instrcutLib = getCmdByCheckName(count, "CLOSE");
        if (Engine != nullptr && instrcutLib != nullptr)
            SendDevice(instrcutLib, Engine);
        count++;
    }
}

void VerifyWindow::closeDevice()
{
    // 关闭设备连接
    int count = 0;
    for (ConnectInfo& visa : connectList) {
        VisaCommonEngine* Engine = visa.visaEngine;
        if (Engine != nullptr)
            Engine->closeDevice();
        visa.state = "未连接";
        ui->tableWidget_Standard->setItem(count, 1, new QTableWidgetItem("OFF"));
        count++;
    }
}

QString VerifyWindow::TranTypeUnit(QString sUnit)
{
    QString NewsUnit = sUnit;
    for (QStringList keyList : unitTypeTransList.keys()) {
        if(keyList[0].contains(sUnit)){
           NewsUnit = unitTypeTransList.value(keyList)[0];
        }
    }
    return NewsUnit;
}

QString VerifyWindow::transCmd(QString sCmd, QString sLabel, QString sLabelItem, QStringList data, bool devicetype)
{
    QString sNewCmd = sCmd;
    if (sNewCmd.contains("{DELAY:")) {
        QRegExp re("DELAY:(\\d+)");
        int delay = 0;
        if (re.indexIn(sNewCmd) != -1) {
            delay = re.cap(1).toUInt();
        }
        BaseCommonApi::SaveLogInfo(1, QString("延迟 %1 ms").arg(delay));
        qDebug() << "delay time:" << delay;
        Delay_MSec(delay);
        return "";
    }
    QString NewUnit;
    QStringList cmdPlaceHolderValue;
    QStringList TemporaryValue;
    for (QStringList keyList : cmdPlaceHolderList.keys()) {
        if (sCmd.contains(keyList[0])) {
            cmdPlaceHolderValue = cmdPlaceHolderList.value(keyList);

            int idx = getIndexByHeader(sLabel, sLabelItem, cmdPlaceHolderList.value(keyList));
            if (idx != -1) {
                if (devicetype || keyList[0] == "{CH}") {
                    NewUnit = TranTypeUnit(data[idx]); // 转换 Ω->OHM
                    sNewCmd = sNewCmd.replace(keyList[0], NewUnit);
                    continue;
                }
                double dTmp;
                QRegularExpression re("(?=.*[0-9])(?=.*[a-zA-ZΩΜΩ])");
                if (re.match(data[idx]).hasMatch()) {
                    dTmp = tranunitSplit(data[idx], true);
                } else {
                    TemporaryValue.append("单位");
                    int idx2 = getIndexByHeader(sLabel, sLabelItem, TemporaryValue);
                    if (idx2 != -1) {
                        QString unit = data[idx2];
                        dTmp = transUnit(data[idx], unit, true);
                    } else {
                        dTmp = data[idx].toDouble();
                    };
                }
                sNewCmd = sNewCmd.replace(keyList[0], QString::number(dTmp));
            }
        }
    }
    return sNewCmd;
}

double VerifyWindow::tranunitSplit(QString sData, bool reverse)
{

    QRegularExpression re("(?=.*[0-9])(?=.*[a-zA-ZΩΜΩ])");
    if (re.match(sData).hasMatch()) {
        QString sData2 = sData;
        QString sNewData, sUnit;
        ItemUnitSplit(sData, sNewData, sUnit);
        return transUnit(sNewData, sUnit, reverse);
    } else {
        return sData.toDouble();
    }
}

void VerifyWindow::ItemUnitSplit(QString Value, QString& data, QString& unit)
{
    QString sData2 = Value;
    QString sNewData, sUnit;
    // 正则表达式匹配数字
    QRegularExpression reLetters("[a-zA-ZΩΜΩ%]");
    QRegularExpression reLetters1("[^a-zA-ZΩΜΩ%]");
    data = sData2.replace(reLetters, "");
    unit = Value.replace(reLetters1, "");
}

void VerifyWindow::on_pushButton_pause_clicked()
{
}

void VerifyWindow::on_pushButton_stop_clicked()
{
    Teststate = SyncRunStatus::Stopped;
}

void VerifyWindow::on_pushButton_save_clicked()
{
    DialogVerifyResult* dlg = new DialogVerifyResult;
    // dlg->taskList =selectTaskList;
    // dlg->deviceId = selectTaskList[0].sample_id;
    dlg->setData(iEquipType, &veriDataMapList, &veriHeadList, &programInfo, selectStdIds, selectBizIds);
    dlg->setWindowState(Qt::WindowMaximized);
    dlg->setModal(true);
    dlg->show();
}

void VerifyWindow::on_pushButton_close_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(0);
    getVerifyMethod();
}

/**
 * @brief VerifyWindow::on_pushButton_12_clicked
 * 选择核查程序页面 点击加载按钮后操作
 * 保存选择的核查程序id 用于后续查询核查项
 */
void VerifyWindow::on_pushButton_12_clicked()
{
    showDevice();
}

void VerifyWindow::on_pushButton_10_clicked()
{
    getVerifyMethod();
}

void VerifyWindow::on_pushButton_11_clicked()
{
    ui->lineEdit_9->setText("");
    ui->lineEdit_10->setText("");
    getVerifyMethod();
}

void VerifyWindow::on_pushButton_14_clicked()
{
    QString sId = "";
    for (int i = 0; i < ui->tableWidgetStd->rowCount(); i++) {
        Qt::CheckState state = ui->tableWidgetStd->item(i, 0)->checkState();
        if (state == Qt::Checked) {
            // sId = stdIdList[i];
            // break;
            if (sId == "")
                sId = stdIdList[i];
            else
                sId = QString("%1,%2").arg(sId).arg(stdIdList[i]);
        }
    }
    QString sBizId = "";
    for (int i = 0; i < ui->tableWidgetBiz->rowCount(); i++) {
        Qt::CheckState state = ui->tableWidgetBiz->item(i, 0)->checkState();
        if (state == Qt::Checked) {
            sBizId = bizIdList[i];
            break;
            //            if(sBizId=="")
            //                sBizId = bizIdList[i];
            //            else
            //                sBizId =QString("%1,%2").arg(sBizId).arg(bizIdList[i]);
        }
    }
    if (sId == "") {
        QMessageBox::warning(this, "提示", "请选择标准设备！");
    } else if (sBizId == "") {
        QMessageBox::warning(this, "提示", "请选择核查件！");
    } else {
        // programInfo = BaseCommonApi::getCheckProgramManageById(sId);
        loginNowVerifyEquimentId = sBizId;
        initDeviceInfo(sId, sBizId);
        ui->stackedWidget->setCurrentIndex(2);
    }
}

void VerifyWindow::on_pushButton_16_clicked()
{
    selectStandInfo();
}

void VerifyWindow::on_pushButton_17_clicked()
{
    ui->lineEdit_7->setText("");
    ui->lineEdit_8->setText("");
    ui->lineEdit_11->setText("");
    selectStandInfo();
}

void VerifyWindow::on_pushButton_18_clicked()
{
    selectBizInfo();
}

void VerifyWindow::on_pushButton_19_clicked()
{
    ui->lineEdit_12->setText("");
    ui->lineEdit_13->setText("");
    ui->lineEdit_14->setText("");
    selectBizInfo();
}

void VerifyWindow::on_tableWidgetStd_cellChanged(int row, int column)
{
    //    Qt::CheckState state = ui->tableWidgetStd->item(row,0)->checkState();
    //    if(state == Qt::Checked)
    //    {
    //        for(int i=0;i<ui->tableWidgetStd->rowCount();i++){
    //            if(i!=row)
    //            {
    //                ui->tableWidgetStd->item(i,0)->setCheckState(Qt::Unchecked);
    //            }
    //        }
    //    }
}

void VerifyWindow::getCheckItemData()
{
    QString standardId = connectList[0].id;
    readItems.clear();

    bizOutMode=true;
    // 从检定程序中获取被核查标准设备的类型
    iEquipType = programInfo.belong_standard_equipment.toInt();

    qDebug() << standardId << iEquipType;

    switch (iEquipType) {
    case 1:
        veriDataMapList = BaseCommonApi::getStandardCheckItemDataCalibrator(standardId, &veriHeadList);
        readItems << "示值1"
                  << "示值2"
                  << "示值3"
                  << "示值4"
                  << "示值5"
                  << "示值6";
        break;
    case 2:
        veriDataMapList = BaseCommonApi::getStandardCheckItemDataDcPower(standardId, &veriHeadList);
        readItems << "示值1"
                  << "示值2"
                  << "示值3"
                  << "示值4"
                  << "示值5"
                  << "示值6";
        bizOutMode=false;
        break;
    case 4:
        veriDataMapList = BaseCommonApi::getStandardCheckItemDataPistonGauge(standardId, &veriHeadList);
        if (veriDataMapList.keys()[0] == "电信号"){
              readItems << "第一次测量标准示值"<< "第二次测量标准示值"
                        << "第三次测量标准示值"<< "第四次测量标准示值"
                        << "第五次测量标准示值"<< "第六次测量标准示值";
        }
        else{
            readItems << "第一次" << "第二次" << "第三次";
            readItemsSecond << "正行程" << "反行程";
        }
        break;
    case 5:
        veriDataMapList = BaseCommonApi::getStandardCheckItemDataResistanceThermometer(standardId, &veriHeadList);
        readItems << "测量标准示值1"
                  << "测量标准示值2"
                  << "测量标准示值3"
                  << "测量标准示值4"
                  << "测量标准示值5"
                  << "测量标准示值6";
        bizOutMode=false;
        break;
    // 频谱分析仪
    case 9:
        veriDataMapList = BaseCommonApi::getStandardCheckItemDataSpectrumAnalyzer(standardId, &veriHeadList);
        readItems << "测量值1"
                  << "测量值2"
                  << "测量值3"
                  << "测量值4"
                  << "测量值5"
                  << "测量值6"
                  << "测量值7"
                  << "测量值8"
                  << "测量值9"
                  << "测量值10";
        break;
    // 信号发生器
    case 10:
        veriDataMapList = BaseCommonApi::getStandardCheckItemDataSignalGenerator(standardId, &veriHeadList);
        readItems << "测量值1"
                  << "测量值2"
                  << "测量值3"
                  << "测量值4"
                  << "测量值5"
                  << "测量值6"
                  << "测量值7"
                  << "测量值8"
                  << "测量值9"
                  << "测量值10";
        break;
        // 微波衰减标准装置
    case 11:
        veriDataMapList = BaseCommonApi::getStandardCheckItemDataMicrowaveAttenuation(standardId, &veriHeadList);
        readItems << "测量值1"
                  << "测量值2"
                  << "测量值3"
                  << "测量值4"
                  << "测量值5"
                  << "测量值6"
                  << "测量值7"
                  << "测量值8"
                  << "测量值9"
                  << "测量值10";
        break;
        //示波器检定装置
    case 13:
        veriDataMapList = BaseCommonApi::getStandardCheckItemDataOscilloscope(standardId, &veriHeadList);
        readItems << "测量值*X1"
                  << "测量值*X2"
                  << "测量值*X3"
                  << "测量值*X4"
                  << "测量值*X5"
                  << "测量值*X6"
                  << "测量值*X7"
                  << "测量值*X8"
                  << "测量值*X9"
                  << "测量值*X10";
        break;
        //失真度标准装置
    case 18:
        qDebug() << "getStandardCheckItemDataDistortionFactor";
        veriDataMapList = BaseCommonApi::getStandardCheckItemDataDistortionFactor(standardId, &veriHeadList);
        readItems << "测量值1"
                  << "测量值2"
                  << "测量值3"
                  << "测量值4"
                  << "测量值5"
                  << "测量值6"
                  << "测量值7"
                  << "测量值8"
                  << "测量值9"
                  << "测量值10";
        break;
    default:
        qDebug() << "missing iEquipType";
        break;
    }
}

void VerifyWindow::on_tableWidgetBiz_cellChanged(int row, int column)
{
    Qt::CheckState state = ui->tableWidgetBiz->item(row, 0)->checkState();
    if (state == Qt::Checked) {
        for (int i = 0; i < ui->tableWidgetBiz->rowCount(); i++) {
            if (i != row) {
                ui->tableWidgetBiz->item(i, 0)->setCheckState(Qt::Unchecked);
            }
        }
    }
}

void VerifyWindow::on_pushButtonSaveAddress_clicked()
{
    if (currConnectIdx >= 0 && currConnectIdx < connectList.length()) {
        ConnectInfo& item = connectList[currConnectIdx];
        if (item.programInterface.id == "") {
            BaseCommonApi baseApi;
            qint64 id = baseApi.generateId();
            item.programInterface.id = QString::number(id);
            item.programInterface.device_id = item.id;
            if (ui->radioButton_serial->isChecked())
                item.programInterface.communication_type = "SerialPort";
            else
                item.programInterface.communication_type = "Visa";
            item.programInterface.serial_port = QString("%1;%2;%3;%4;%5").arg(ui->comboBox_port->currentText()).arg(ui->comboBox_baurt->currentText()).arg(ui->comboBox_check->currentText()).arg(ui->comboBox_data->currentText()).arg(ui->comboBox_stop->currentText());
            item.programInterface.visa = ui->comboBox_visa->currentText();
            if (BaseCommonApi::InsertVerificationProgramConfigInterface(item.programInterface)) {
                QMessageBox::information(this, "提示", "保存成功！");
            } else {
                QMessageBox::warning(this, "警告", "保存失败！");
            }
        } else {
            if (ui->radioButton_serial->isChecked())
                item.programInterface.communication_type = "SerialPort";
            else
                item.programInterface.communication_type = "Visa";
            item.programInterface.serial_port = QString("%1;%2;%3;%4;%5").arg(ui->comboBox_port->currentText()).arg(ui->comboBox_baurt->currentText()).arg(ui->comboBox_check->currentText()).arg(ui->comboBox_data->currentText()).arg(ui->comboBox_stop->currentText());
            item.programInterface.visa = ui->comboBox_visa->currentText();
            if (BaseCommonApi::UpdateVerificationProgramConfigInterface(item.programInterface)) {
                QMessageBox::information(this, "提示", "保存成功！");
            } else {
                QMessageBox::warning(this, "警告", "保存失败！");
            }
        }
    }
}


