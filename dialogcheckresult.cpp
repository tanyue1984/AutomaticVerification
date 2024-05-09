#include "dialogcheckresult.h"
#include "global.h"
#include "ui_dialogcheckresult.h"
#include <QDesktopServices>
#include <QMessageBox>

DialogCheckResult::DialogCheckResult(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DialogCheckResult)
{
    ui->setupUi(this);
    ui->tableWidget->verticalHeader()->setParent(nullptr);
    ui->dateEditCheck->setDate(QDate::currentDate());
    ui->dateEdit_Valid->setDate(QDate::currentDate().addYears(1));
    setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
}

DialogCheckResult::~DialogCheckResult()
{
    delete ui;
}

void DialogCheckResult::deleteItem(QLayout* layout)
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
void DialogCheckResult::initCalDataTable(int idx)
{
    if (curHeadIdx == idx || headTableList->keys().count() == 0)
        return;
    curHeadIdx = idx;
    QString sLabel = headTableList->keys()[curHeadIdx];
    const auto& dataRes = caliDataList->value(sLabel);
    const auto& headrs = headTableList->value(sLabel);
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->setColumnCount(headrs.count());
    ui->tableWidget->setHorizontalHeaderLabels(headrs);
    for (int i = 0; i < headrs.count(); i++) {
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }
    // tableWidget->setRowCount(data.count());
    int rowCount = 0;
    for (int i = 0; i < dataRes.count(); ++i) {
        // 获取行号 并进行添加行
        rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(rowCount);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        for (int j = 1; j < dataRes[i].count(); j++) {
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(dataRes[i][j]));
        }
    }
}

void DialogCheckResult::initCheckTable(QList<QMap<QString, QList<QStringList>>> measList, QList<SequentialMap<QString, QStringList>> headList, QList<QStringList> StanInfo)
{
    caliDataListAll = measList;
    headTableListAll = headList;
    stdId = StanInfo[2];
    StanDeviceInfo = StanInfo;
    deleteItem(ui->horizontalLayoutDevice);
    int idx = 0;
    curDevciceIdx = -1;
    for (const MyTask& task : taskList) {
        QPushButton* btn_1 = new QPushButton();
        btn_1->setText(task.sampleName);
        btn_1->setStyleSheet("QPushButton{"
                             "background-color:rgb(240,240,240);"
                             "color:rgb(51,51,51);"
                             "}");
        // btn_1->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
        QFont font;
        // font.setPointSize(12);//字体大小
        font.setBold(true);
        btn_1->setFont(font);
        btn_1->setCursor(Qt::PointingHandCursor);
        connect(btn_1, &QPushButton::clicked, this, [this, idx]() {
            on_deviceButton_clicked(idx);
        });
        ui->horizontalLayoutDevice->addWidget(btn_1);
        idx++;
    }
    ui->horizontalLayoutDevice->addStretch();
    on_deviceButton_clicked(0);
}

void DialogCheckResult::on_deviceButton_clicked(int idx)
{
    QStringList item;
    item << "测试"
         << "校准"
         << "检定"
         << "校验";
    ui->comboBoxCategory->addItems(item);
    ui->comboBoxCategory->setCurrentIndex(0);

    QStringList conclusion;
    conclusion << "所检项目合格"
               << "不合格"
               << "除*外所检项目合格";
    ui->comboBoxConclusion->addItems(conclusion);
    ui->comboBoxConclusion->setCurrentIndex(0);

    QStringList identify;
    identify << "合格"
             << "限用"
             << "测试"
             << "校准"
             << "校验"
             << "停用"
             << "禁用"
             << "封存";
    ui->comboBoxIdentify->addItems(identify);
    ui->comboBoxIdentify->setCurrentIndex(0);

    if (curDevciceIdx == idx || idx < 0 || idx >= taskList.length())
        return;
    curDevciceIdx = idx;
    headTableList = &headTableListAll[curDevciceIdx];
    caliDataList = &caliDataListAll[curDevciceIdx];
    deviceId = taskList[curDevciceIdx].sample_id;
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    deleteItem(ui->HeadListLayout);
    int index = 0;
    curHeadIdx = -1;
    for (const auto& item : *headTableList) {
        // ========================================================================================
        // 数据循环显示
        // ===================================================================================
        QString sLabel = headTableList->keys()[index];
        // ========================================================================================
        // 按钮设置显示
        // ====================================================================================
        QPushButton* button = new QPushButton(sLabel, this);
        // 设置样式
        //  设置按钮大小为 56x20
        // button->setFixedSize(85, 40);
        button->setCheckable(true);
        button->setAutoExclusive(true);
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
                              "}"
                              "QPushButton:hover {"
                              "background-color: rgba(215, 215, 215, 1);;" // 鼠标悬停时变为深灰色背景
                              "}"
                              "QPushButton:checked {"
                              "color: white;"
                              "background-color: rgb(74, 102, 186);" // 选中时变为蓝色背景
                              "}");

        ui->HeadListLayout->addWidget(button);
        connect(button, &QPushButton::clicked, this, [this, index]() {
            // tackedWidget->setCurrentIndex(index);
            initCalDataTable(index);
        });
        ++index;
    }
    ui->HeadListLayout->addStretch();
    initCalDataTable(0);

    QList<JudgeCalibrationTable> CliTable = BaseCommonApi::getJudgeCalibrationTable(deviceId);
    if (CliTable.length() > 0) {
        iEquipType = CliTable[0].belong_standard_equipment.toInt();
    }
    catoryList = BaseCommonApi::getDeviceMeasureEquipmentCatory(deviceId);
}

QString DialogCheckResult::getValueByHeader(int idx, QString sLabel, QString sName)
{
    QList<QStringList> dataRes = caliDataList->value(sLabel);
    const auto& headrs = headTableList->value(sLabel);
    for (int i = 0; i < headrs.length(); i++) {
        if (headrs[i] == sName)
            return dataRes[idx][i];
    }
    return "";
}

QString DialogCheckResult::getDictebyItemName(QString Name)
{
    QString NewName = "0";
    if (checkitemlist.keys().contains(Name)) {
        NewName = checkitemlist.value(Name);
    }
    return NewName;
}
void DialogCheckResult::ItemUnitSplit(QString Value, QString& data, QString& unit)
{
    QString sData2 = Value;
    QString sNewData, sUnit;
    // 正则表达式匹配数字
    QRegularExpression reLetters("[a-zA-ZΩΜΩ]");
    QRegularExpression reLetters1("[^a-zA-ZΩΜΩ]");
    data = sData2.replace(reLetters, "");
    unit = Value.replace(reLetters1, "");
}

void DialogCheckResult::on_pushButtonPreview_clicked()
{
    QString strPath = QCoreApplication::applicationDirPath() + "//checkTemplate.doc";
    if (QFile::exists(strPath)) {
        WordEngine engine;
        engine.open(strPath);
        QStringList itemList;
        QStringList conentList;
        itemList << "unit"
                 << "address"
                 << "jianAddr"
                 << "time2"
                 << "temperature"
                 << "humidity";
        conentList << ""
                   << "" << ui->comboBoxPlace->currentText() << ui->dateEditCheck->text() << ui->lineEditTemp->text() << ui->lineEdit_Hum->text();
        engine.setMarks("unit", taskList[curDevciceIdx].customer_name);
        engine.setMarks("address", "");
        engine.setMarks("name", taskList[curDevciceIdx].sampleName);
        engine.setMarks("type", taskList[curDevciceIdx].sampleModel);
        engine.setMarks("serial", taskList[curDevciceIdx].manufactureNo);
        engine.setMarks("factory", taskList[curDevciceIdx].manufacturer);
        engine.setMarks("jianAddr", ui->comboBoxPlace->currentText());
        engine.setMarks("time2", ui->dateEditCheck->text());
        engine.setMarks("temperature", ui->lineEditTemp->text());
        engine.setMarks("humidity", ui->lineEdit_Hum->text());
        QList<CatDeviceInfor> result = BaseCommonApi::getCatDeviceInfor(stdId[0]);
        if (result.length() > 0) {
            engine.setMarks("stdname", result[0].equipment_name);
            engine.setMarks("stdtype", result[0].model);
            engine.setMarks("stdno", result[0].manufacture_no);
            engine.setMarks("stduncert", "");
            engine.setMarks("stdvalid", result[0].measure_valid_date);
        }

        // engine.setBatchMarks(itemList,conentLis;t)
        QString strLabel = "result";
        // engine.setMarks(strLabel,"1.表格1");

        for (int k = headTableList->keys().length() - 1; k >= 0; k--) {
            QString sLabel = headTableList->keys()[k];
            QString strTitle = QString("%1.%2").arg(k + 2).arg(sLabel);
            const auto& headrs = headTableList->value(sLabel);
            const auto& dataRes = caliDataList->value(sLabel);
            int rows = dataRes.count() + 1;
            int cols = headrs.length();
            QAxObject* pTable = engine.insertTable(strLabel, rows, cols, strTitle);
            for (int i = 0; i < cols; i++) {
                engine.setCellString(pTable, 1, i + 1, headrs[i]);
            }
            // tableWidget->setRowCount(data.count());
            for (int i = 0; i < dataRes.count(); ++i) {
                engine.setCellString(pTable, i + 2, 1, QString::number(i + 1));
                for (int j = 1; j < dataRes[i].count(); j++) {
                    engine.setCellString(pTable, i + 2, j + 1, dataRes[i][j]);
                }
            }
        }
        //        QString strTitle = "1.表格1";
        //        int rows = measureCalList.length()+1;
        //        int cols = measHeadList.length();
        //        QAxObject* pTable = engine.insertTable(strLabel,rows,cols,strTitle);
        //        for (int i=0;i<cols;i++){
        //            engine.setCellString(pTable,1,i,measHeadList[i]);
        //        }
        //        for(int j=2;j<=rows;j++){
        //            SaveDeviceMeasureCalibrator measItem =measureCalList[j-2];
        //            engine.setCellString(pTable,j, 2, measItem.params);
        //            engine.setCellString(pTable,j, 3, measItem.capacity);
        //            engine.setCellString(pTable,j, 4, measItem.unit);
        //            engine.setCellString(pTable,j, 5, measItem.frequency);
        //            engine.setCellString(pTable,j, 6, measItem.frequency_unit);
        //            engine.setCellString(pTable,j, 7, measItem.range_range);
        //            engine.setCellString(pTable,j, 8, measItem.standard_value);
        //            engine.setCellString(pTable,j, 9, measItem.resolution);
        //            engine.setCellString(pTable,j, 10, measItem.maximum_error);
        //            engine.setCellString(pTable,j, 11, measItem.error_param_a);
        //            engine.setCellString(pTable,j, 12, measItem.error_param_b);
        //            engine.setCellString(pTable,j, 15, measItem.indicating_value);
        //            engine.setCellString(pTable,j, 19, measItem.measure_indication_value);
        //        }
        QString savePath = QCoreApplication::applicationDirPath() + "//检定证书预览.doc";
        engine.saveAs(savePath);
        QUrl fileUrl = QUrl::fromLocalFile(savePath);
        QDesktopServices::openUrl(fileUrl);
    } else {
        QMessageBox::warning(this, "警告", strPath + "文件不存在！");
    }
}

void DialogCheckResult::on_pushButtonSave_clicked()
{
    if (ui->lineEditTemp->text() == "") {
        QMessageBox::warning(this, "提示", "温度不能为空！");
        return;
    }
    if (ui->lineEdit_Hum->text() == "") {
        QMessageBox::warning(this, "提示", "温度不能为空！");
        return;
    }
    int num = ui->comboBoxCategory->currentIndex() + 1;
    ui->comboBox_User->addItem(loginUserName);
    checkitemlist.clear();
    SaveDeviceMeasureInfo measInfo;
    BaseCommonApi api;
    measInfo.id = QString::number(api.generateId());
    // measInfo.data_no;
    measInfo.order_id = taskList[curDevciceIdx].orderId;
    measInfo.sample_id = deviceId;
    measInfo.sample_name = taskList[curDevciceIdx].sampleName;
    measInfo.model = taskList[curDevciceIdx].sampleModel;
    measInfo.manufacturer = taskList[curDevciceIdx].manufacturer;
    measInfo.manufacture_no = taskList[curDevciceIdx].manufactureNo;
    measInfo.measure_category = QString::number(num);
    measInfo.item_id = catoryList[0].id;
    measInfo.temperature = ui->lineEditTemp->text();
    measInfo.humidity = ui->lineEdit_Hum->text();
    measInfo.measure_address = ui->comboBoxPlace->currentText();
    measInfo.trace_date = ui->dateEditCheck->date();
    measInfo.measure_valid_date = ui->dateEdit_Valid->dateTime();
    // measInfo.outcome = ui->lineEdit_Cons->text();
    //    measInfo.create_user_name = ui->comboBox_User->currentText();
    measInfo.create_user_name = loginUserName;
    measInfo.data_source = "自动检定系统";
    measInfo.appearance_function_check = "外观完好，工作正常";
    measInfo.conclusion = ui->comboBoxConclusion->currentText();
    measInfo.meter_identify = ui->comboBoxIdentify->currentText();
    measInfo.create_user_id = loginUserId;
    measInfo.measure_person_id = loginMeasurePersonId;
    //    QString itemId = BaseCommonApi::getStandardItemInfo();
    if (BaseCommonApi::InsertSaveDeviceMeasureInfo(measInfo)) {
        //     loginOrderId = measInfo.order_id;
        QString sqls = "UPDATE biz_business_device_measure_item_info SET data_sync=1,sync_time=:1 WHERE id= " + measInfo.item_id;
        BaseCommonApi::saveLogToSql(sqls, nullptr);
        MeasureModifyRecord record;
        record.id = QString::number(api.generateId());
        record.data_id = measInfo.id;
        record.source_system = "自动检定系统";
        record.modify_content = "新增";
        record.create_user_id = loginUserId;
        record.create_user_name = loginUserName;
        BaseCommonApi::InsertModifyRecord(record);

        QStringList idList = stdId;
        int i = 0;
        for (const QString& id : idList) {
            DeviceMeasureEquipment equipment;
            equipment.id = QString::number(api.generateId());
            equipment.data_id = measInfo.id;
            equipment.equipment_id = id;
            equipment.create_time = equipment.update_time = QDateTime::currentDateTime();
            // 添加标准装置名称和标准装置ID
            if (StanDeviceInfo[0].count() == idList.count() && StanDeviceInfo[1].count() == idList.count()) {
                equipment.standard_id = StanDeviceInfo[0][i];
                equipment.standard_name = StanDeviceInfo[1][i];
            }

            BaseCommonApi::InsertDeviceMeasureEquipment(equipment);
            i++;
        }
        for (int k = 0; k < headTableList->keys().length(); k++) {
            QString sLabel = headTableList->keys()[k];
            const auto& dataRes = caliDataList->value(sLabel);
            // tableWidget->setRowCount(data.count());
            // 多功能校准源
            if (iEquipType == 1) {
                for (int i = 0; i < dataRes.count(); ++i) {
                    SaveDeviceMeasureCalibrator measItem;
                    measItem.id = QString::number(api.generateId());
                    measItem.data_id = measInfo.id;
                    measItem.item_data_id = getValueByHeader(i, sLabel, "序号");
                    measItem.maximum_error_formula = getValueByHeader(i, sLabel, "最大允许误差"); // 最大允许误差(公式)
                    measItem.error_param_a = getValueByHeader(i, sLabel, "a"); // 误差参数a
                    measItem.error_param_b = getValueByHeader(i, sLabel, "b"); // 误差参数b
                    measItem.params = getValueByHeader(i, sLabel, "检定项目"); // 参数
                    measItem.capacity = getValueByHeader(i, sLabel, "量"); // 量
                    measItem.unit = getValueByHeader(i, sLabel, "单位"); // 单位
                    measItem.line_mode = getValueByHeader(i, sLabel, "两线/四线");
                    measItem.frequency = getValueByHeader(i, sLabel, "频率"); // 频率
                    measItem.frequency_unit = getValueByHeader(i, sLabel, "频率单位"); // 频率单位
                    measItem.range_range = getValueByHeader(i, sLabel, "量程"); // 量程
                    measItem.standard_value = getValueByHeader(i, sLabel, "标准值"); // 标准值(输入字段,特殊:指针式万用表输出字段)
                    measItem.indicating_value = getValueByHeader(i, sLabel, "示值"); // 示值(输出字段,特殊:指针式万用表输入字段)
                    measItem.maximum_error = getValueByHeader(i, sLabel, "最大允许误差"); // 最大允许误差(值)
                    measItem.absolute_error = getValueByHeader(i, sLabel, "绝对误差"); // 绝对误差
                    measItem.resolution = getValueByHeader(i, sLabel, "分辨力"); // 分辨力
                    measItem.measure_indication_value = getValueByHeader(i, sLabel, "示值"); // 被检表示值
                    BaseCommonApi::SearchSelectTable(QString::number(iEquipType), &measItem);
                }
            }
            // 直流稳压电源
            else if (iEquipType == 2) {
                for (int i = 0; i < dataRes.count(); ++i) {
                    bizBusinessDeviceMeasureDataDcPower measItem;
                    measItem.id = QString::number(api.generateId());
                    measItem.data_id = measInfo.id;
                    measItem.data_type = getValueByHeader(i, sLabel, "检定项目");
                    measItem.output_channel = getValueByHeader(i, sLabel, "输出通道");
                    measItem.unit = getValueByHeader(i, sLabel, "单位");
                    measItem.voltage_regulator_output_value = getValueByHeader(i, sLabel, "调压器输出值");
                    measItem.standard_value = getValueByHeader(i, sLabel, "标准值");
                    measItem.maximum_error = getValueByHeader(i, sLabel, "最大允许误差");
                    measItem.absolute_error = getValueByHeader(i, sLabel, "绝对误差");
                    measItem.voltage_output_value = getValueByHeader(i, sLabel, "被检电源输出电压值");
                    measItem.voltage_max_difference = getValueByHeader(i, sLabel, "被检电源输出电压最大差值");
                    measItem.technical_index = getValueByHeader(i, sLabel, "被检电源技术指标");
                    measItem.technical_index_symbol = getValueByHeader(i, sLabel, "被检电源技术指标前符号");
                    measItem.voltage_regulation = getValueByHeader(i, sLabel, "电源电压调整率");
                    measItem.load_situation = getValueByHeader(i, sLabel, "负载情况");
                    measItem.voltage_difference = getValueByHeader(i, sLabel, "电压差值");
                    measItem.load_regulation = getValueByHeader(i, sLabel, "负载调整率");
                    measItem.measure_indication_value = getValueByHeader(i, sLabel, "被检表示值");
                    measItem.standard_resistance_value = getValueByHeader(i, sLabel, "标准电阻值");
                    measItem.standard_resistance_value_unit = getValueByHeader(i, sLabel, "标准电阻值单位");
                    measItem.ripple_voltage = getValueByHeader(i, sLabel, "纹波电压");
                    measItem.output_voltage_max_change = getValueByHeader(i, sLabel, "输出电压最大变化值");
                    if (measItem.voltage_output_value.isEmpty()) {
                        measItem.voltage_output_value = getValueByHeader(i, sLabel, "被测电源输出电压");
                    }
                    if (measItem.technical_index.isEmpty()) {
                        measItem.technical_index = getValueByHeader(i, sLabel, "仪器技术指标");
                    }
                    //                   measItem.technical_index_symbol = getValueByHeader(i,sLabel,"仪器技术指标单位");
                    measItem.short_term_stability = getValueByHeader(i, sLabel, "短期稳定性");
                    BaseCommonApi::SearchSelectTable(QString::number(iEquipType), &measItem);
                }
            }
            // 0.02压力活塞计
            else if (iEquipType == 4) {
                for (int i = 0; i < dataRes.count(); ++i) {
                    DeviceMeasureDataPistonGauge measItem;
                    measItem.id = QString::number(api.generateId());
                    measItem.data_id = measInfo.id;
                    if (sLabel == "示值误差" || sLabel == "回程误差" || sLabel == "轻敲位移" || sLabel == "示值误差/回程误差/轻敲位移")
                        measItem.data_type = "1";
                    else
                        measItem.data_type = "2";
                    measItem.data_category = "1";
                    measItem.before_friction_forward_stroke = getValueByHeader(i, sLabel, "轻敲前示值-正");
                    measItem.before_friction_reverse_stroke = getValueByHeader(i, sLabel, "轻敲前示值-反");
                    measItem.after_friction_forward_stroke = getValueByHeader(i, sLabel, "轻敲后示值-正");
                    measItem.after_friction_reverse_stroke = getValueByHeader(i, sLabel, "轻敲后示值-反");
                    measItem.variation_forward_stroke = getValueByHeader(i, sLabel, "轻敲前后变动量-正");
                    measItem.variation_reverse_stroke = getValueByHeader(i, sLabel, "轻敲前后变动量-反");
                    measItem.indicating_value = getValueByHeader(i, sLabel, "标准器示值");
                    // BaseCommonApi::InsertDeviceMeasureDataPistonGauge(measItem);
                    BaseCommonApi::SearchSelectTable(QString::number(iEquipType), &measItem);
                }
            }
            // 二等铂电阻温度计标准装置
            else if (iEquipType == 5) {
                for (int i = 0; i < dataRes.count(); ++i) {
                    DeviceMeasureDataResistanceThermometer measItem;
                    measItem.id = QString::number(api.generateId());
                    measItem.data_id = measInfo.id;
                    measItem.data_category = "1";
                    if (sLabel == "波动性测试") {
                        measItem.data_type = "1";
                        measItem.upper_lower_point = getValueByHeader(i, sLabel, "下限点");
                        if (measItem.upper_lower_point == "")
                            measItem.upper_lower_point = getValueByHeader(i, sLabel, "上限点");
                        measItem.unit = getValueByHeader(i, sLabel, "单位");
                        measItem.one_value = getValueByHeader(i, sLabel, "示值1");
                        measItem.two_value = getValueByHeader(i, sLabel, "示值2");
                        measItem.three_value = getValueByHeader(i, sLabel, "示值3");
                        measItem.four_value = getValueByHeader(i, sLabel, "示值4");
                        measItem.five_value = getValueByHeader(i, sLabel, "示值5");
                        measItem.six_value = getValueByHeader(i, sLabel, "示值6");
                        measItem.seven_value = getValueByHeader(i, sLabel, "示值7");
                        measItem.eight_value = getValueByHeader(i, sLabel, "示值8");
                        measItem.nine_value = getValueByHeader(i, sLabel, "示值9");
                        measItem.ten_value = getValueByHeader(i, sLabel, "示值10");
                        measItem.eleven_value = getValueByHeader(i, sLabel, "示值11");
                        measItem.twelve_value = getValueByHeader(i, sLabel, "示值12");
                        measItem.thirteen_value = getValueByHeader(i, sLabel, "示值13");
                        measItem.fourteen_value = getValueByHeader(i, sLabel, "示值14");
                        measItem.fifteen_value = getValueByHeader(i, sLabel, "示值15");
                        measItem.max_value = getValueByHeader(i, sLabel, "最大值");
                        measItem.min_value = getValueByHeader(i, sLabel, "最小值");
                        measItem.volatility = getValueByHeader(i, sLabel, "波动性");
                    } else if (sLabel == "均匀性测试") {
                        measItem.data_type = "2";
                        measItem.upper_lower_point = getValueByHeader(i, sLabel, "下限点");
                        if (measItem.upper_lower_point == "")
                            measItem.upper_lower_point = getValueByHeader(i, sLabel, "上限点");
                        measItem.unit = getValueByHeader(i, sLabel, "单位");
                        measItem.location = getValueByHeader(i, sLabel, "位置");
                        measItem.one_value = getValueByHeader(i, sLabel, "示值ROA1");
                        measItem.two_value = getValueByHeader(i, sLabel, "示值RA1");
                        measItem.three_value = getValueByHeader(i, sLabel, "RA2");
                        measItem.four_value = getValueByHeader(i, sLabel, "ROA2");
                        measItem.five_value = getValueByHeader(i, sLabel, "ROA3");
                        measItem.six_value = getValueByHeader(i, sLabel, "RA3");
                        measItem.seven_value = getValueByHeader(i, sLabel, "RA4");
                        measItem.eight_value = getValueByHeader(i, sLabel, "ROA4");
                        measItem.nine_value = getValueByHeader(i, sLabel, "ROA均值");
                        measItem.ten_value = getValueByHeader(i, sLabel, "RA均值");
                        measItem.upper_temperature_different = getValueByHeader(i, sLabel, "上水平面温差");
                        measItem.lower_temperature_different = getValueByHeader(i, sLabel, "下水平面温差");
                        measItem.max_horizontal_temperature_different = getValueByHeader(i, sLabel, "水平最大温差");
                        measItem.max_work_temperature_different = getValueByHeader(i, sLabel, "工作区域最大温差");
                    }
                    BaseCommonApi::SearchSelectTable(QString::number(iEquipType), &measItem);
                }
            }
            // 频谱分析仪装置
            else if (iEquipType == 9) {
                for (int i = 0; i < dataRes.count(); ++i) {
                    DeviceMeasureDataSpectrumAnalyzer measItem;
                    measItem.id = QString::number(api.generateId());
                    measItem.data_id = measInfo.id;
                    if (sLabel == "频率显示") {
                        measItem.data_type = "1";
                        measItem.nominal_value = getValueByHeader(i, sLabel, "标称值");
                        measItem.sweep_width = getValueByHeader(i, sLabel, "扫频宽度");
                        measItem.resolution_bandwidth = getValueByHeader(i, sLabel, "分辨力带宽");
                        measItem.horizontal_resolution = getValueByHeader(i, sLabel, "水平分辨率");
                        measItem.one_value = getValueByHeader(i, sLabel, "测量值1");
                        measItem.two_value = getValueByHeader(i, sLabel, "测量值2");
                        measItem.three_value = getValueByHeader(i, sLabel, "测量值3");
                        measItem.lower_index = getValueByHeader(i, sLabel, "指标下限");
                        measItem.average_value = getValueByHeader(i, sLabel, "均值");
                        measItem.upper_index = getValueByHeader(i, sLabel, "指标上限");
                        measItem.conclusion = getValueByHeader(i, sLabel, "结论");
                    } else if (sLabel == "扫频宽度") {
                        measItem.data_type = "2";
                        measItem.nominal_value = getValueByHeader(i, sLabel, "标称值");
                        measItem.horizontal_resolution = getValueByHeader(i, sLabel, "水平分辨率");
                        measItem.one_value = getValueByHeader(i, sLabel, "fl1");
                        measItem.two_value = getValueByHeader(i, sLabel, "fr1");
                        measItem.three_value = getValueByHeader(i, sLabel, "fl2");
                        measItem.four_value = getValueByHeader(i, sLabel, "fr2");
                        measItem.five_value = getValueByHeader(i, sLabel, "fl3");
                        measItem.six_value = getValueByHeader(i, sLabel, "fr3");
                        measItem.lower_index = getValueByHeader(i, sLabel, "指标下限");
                        measItem.average_value = getValueByHeader(i, sLabel, "均值");
                        measItem.upper_index = getValueByHeader(i, sLabel, "指标上限");
                        measItem.conclusion = getValueByHeader(i, sLabel, "结论");
                    } else if (sLabel == "中频带宽") {
                        measItem.data_type = "3";
                        measItem.nominal_value = getValueByHeader(i, sLabel, "标称值");
                        measItem.horizontal_resolution = getValueByHeader(i, sLabel, "水平分辨率");
                        measItem.one_value = getValueByHeader(i, sLabel, "fl1");
                        measItem.two_value = getValueByHeader(i, sLabel, "fr1");
                        measItem.three_value = getValueByHeader(i, sLabel, "fl2");
                        measItem.four_value = getValueByHeader(i, sLabel, "fr2");
                        measItem.five_value = getValueByHeader(i, sLabel, "fl3");
                        measItem.six_value = getValueByHeader(i, sLabel, "fr3");
                        measItem.eight_value = getValueByHeader(i, sLabel, "△f1");
                        measItem.nine_value = getValueByHeader(i, sLabel, "△f2");
                        measItem.ten_value = getValueByHeader(i, sLabel, "△f3");
                        measItem.lower_index = getValueByHeader(i, sLabel, "指标下限");
                        measItem.average_value = getValueByHeader(i, sLabel, "均值");
                        measItem.upper_index = getValueByHeader(i, sLabel, "指标上限");
                        measItem.conclusion = getValueByHeader(i, sLabel, "结论");
                    } else if (sLabel == "中频带宽转换偏差") {
                        measItem.data_type = "4";
                        measItem.nominal_value = getValueByHeader(i, sLabel, "中频带宽");
                        measItem.one_value = getValueByHeader(i, sLabel, "测量值");
                        measItem.lower_convert_deviation = getValueByHeader(i, sLabel, "指标下限");
                        measItem.convert_deviation = getValueByHeader(i, sLabel, "转换偏差");
                        measItem.upper_convert_deviation = getValueByHeader(i, sLabel, "指标上限");
                        measItem.conclusion = getValueByHeader(i, sLabel, "结论");
                    } else if (sLabel == "参考电平") {
                        measItem.data_type = "5";
                        measItem.nominal_value = getValueByHeader(i, sLabel, "标称值");
                        measItem.standard_attenuator_attenuation = getValueByHeader(i, sLabel, "标准衰减器衰减量");
                        measItem.signal_source_amplitude = getValueByHeader(i, sLabel, "信号源幅度");
                        measItem.lower_index = getValueByHeader(i, sLabel, "指标下限");
                        measItem.one_value = getValueByHeader(i, sLabel, "测量值");
                        measItem.upper_index = getValueByHeader(i, sLabel, "指标上限");
                        measItem.conclusion = getValueByHeader(i, sLabel, "结论");
                    } else if (sLabel == "垂直刻度") {
                        measItem.data_type = "6";
                        measItem.nominal_value = getValueByHeader(i, sLabel, "标称值");
                        measItem.vertical_scale = getValueByHeader(i, sLabel, "垂直刻度");
                        measItem.standard_attenuator_attenuation = getValueByHeader(i, sLabel, "标准衰减器衰减量");
                        measItem.signal_source_amplitude = getValueByHeader(i, sLabel, "信号源幅度");
                        measItem.lower_index = getValueByHeader(i, sLabel, "指标下限");
                        measItem.one_value = getValueByHeader(i, sLabel, "测量值");
                        measItem.upper_index = getValueByHeader(i, sLabel, "指标上限");
                        measItem.conclusion = getValueByHeader(i, sLabel, "结论");
                    } else if (sLabel == "输入频响") {
                        measItem.data_type = "7";
                        measItem.frequency = getValueByHeader(i, sLabel, "频率");
                        measItem.one_value = getValueByHeader(i, sLabel, "测量值1");
                        measItem.two_value = getValueByHeader(i, sLabel, "测量值2");
                        measItem.three_value = getValueByHeader(i, sLabel, "测量值3");
                        measItem.average_value = getValueByHeader(i, sLabel, "均值");
                        measItem.lower_index = getValueByHeader(i, sLabel, "指标下限");
                        measItem.input_frequency = getValueByHeader(i, sLabel, "输入频响");
                        measItem.upper_index = getValueByHeader(i, sLabel, "指标上限");
                        measItem.conclusion = getValueByHeader(i, sLabel, "结论");
                    } else if (sLabel == "平均噪声电平") {
                        measItem.data_type = "8";
                        measItem.start_frequency = getValueByHeader(i, sLabel, "起始频率");
                        measItem.end_frequency = getValueByHeader(i, sLabel, "终止频率");
                        measItem.one_value = getValueByHeader(i, sLabel, "测量值");
                        measItem.corrected_value = getValueByHeader(i, sLabel, "修正值");
                        measItem.revised_result = getValueByHeader(i, sLabel, "修正后结果");
                        measItem.lower_index = getValueByHeader(i, sLabel, "指标下限");
                        measItem.upper_index = getValueByHeader(i, sLabel, "指标上限");
                        measItem.conclusion = getValueByHeader(i, sLabel, "结论");
                    } else if (sLabel == "谐波失真") {
                        measItem.data_type = "9";
                        measItem.frequency = getValueByHeader(i, sLabel, "频率");
                        measItem.one_value = getValueByHeader(i, sLabel, "基波幅度");
                        measItem.two_value = getValueByHeader(i, sLabel, "二次谐波幅度");
                        measItem.three_value = getValueByHeader(i, sLabel, "二次谐波失真");
                        measItem.four_value = getValueByHeader(i, sLabel, "三次谐波幅度");
                        measItem.five_value = getValueByHeader(i, sLabel, "三次谐波失真");
                        measItem.six_value = getValueByHeader(i, sLabel, "四次谐波幅度");
                        measItem.seven_value = getValueByHeader(i, sLabel, "四次谐波失真");
                        measItem.upper_index_twice = getValueByHeader(i, sLabel, "二次谐波失真指标上限");
                        measItem.upper_index_thrice = getValueByHeader(i, sLabel, "三次谐波失真指标上限");
                        measItem.upper_index_quartic = getValueByHeader(i, sLabel, "四次谐波失真指标上限");
                        measItem.conclusion = getValueByHeader(i, sLabel, "结论");
                    } else if (sLabel == "输入衰减") {
                        measItem.data_type = "10";
                        measItem.frequency = getValueByHeader(i, sLabel, "频率");
                        measItem.one_value = getValueByHeader(i, sLabel, "测量值1");
                        measItem.two_value = getValueByHeader(i, sLabel, "测量值2");
                        measItem.three_value = getValueByHeader(i, sLabel, "测量值3");
                        measItem.average_value = getValueByHeader(i, sLabel, "均值");
                        measItem.lower_index = getValueByHeader(i, sLabel, "指标下限");
                        measItem.input_frequency = getValueByHeader(i, sLabel, "输入频响");
                        measItem.upper_index = getValueByHeader(i, sLabel, "指标上限");
                        measItem.conclusion = getValueByHeader(i, sLabel, "结论");
                    } else if (sLabel == "1db增益压缩点") {
                        measItem.data_type = "11";
                        measItem.input_attenuator = getValueByHeader(i, sLabel, "输入衰减");
                        measItem.spectrum_level = getValueByHeader(i, sLabel, "参考电平");
                        measItem.one_value = getValueByHeader(i, sLabel, "测量值");
                        measItem.gain_compression = getValueByHeader(i, sLabel, "增益压缩");
                        measItem.technical_index = getValueByHeader(i, sLabel, "技术指标");
                        measItem.conclusion = getValueByHeader(i, sLabel, "结论");
                    } else if (sLabel == "校准信号") {
                        measItem.data_type = "12";
                        measItem.type_value = getValueByHeader(i, sLabel, "类型");
                        measItem.nominal_value = getValueByHeader(i, sLabel, "标称值");
                        measItem.one_value = getValueByHeader(i, sLabel, "测量值1");
                        measItem.two_value = getValueByHeader(i, sLabel, "测量值2");
                        measItem.three_value = getValueByHeader(i, sLabel, "测量值3");
                        measItem.lower_index = getValueByHeader(i, sLabel, "指标下限");
                        measItem.measure_result = getValueByHeader(i, sLabel, "测量结果");
                        measItem.upper_index = getValueByHeader(i, sLabel, "指标上限");
                        measItem.conclusion = getValueByHeader(i, sLabel, "结论");
                    }
                    BaseCommonApi::SearchSelectTable(QString::number(iEquipType), &measItem);
                }
            }
            // 信号装置
            else if (iEquipType == 10) {
                QString Value, Unit, StanValue;
                for (int i = 0; i < dataRes.count(); ++i) {
                    DeviceMeasureDataSignalGenerator measItem;
                    measItem.id = QString::number(api.generateId());
                    measItem.data_id = measInfo.id;
                    if (sLabel == "频率") {
                        measItem.data_type = 1;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "标称值"), StanValue, Unit);
                        measItem.nominal_value = StanValue;
                        measItem.unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "指标下限"), Value, Unit);
                        measItem.lower_index = Value;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "测量值"), Value, Unit);
                        measItem.measure_value = Value;
                        measItem.measure_value_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "指标上限"), Value, Unit);
                        measItem.upper_index = Value;
                        measItem.relative_error = getValueByHeader(i, sLabel, "相对误差");
                        measItem.urel = getValueByHeader(i, sLabel, "U(k=2)");
                    } else if (sLabel == "功率") {
                        measItem.data_type = 2;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "频率"), Value, Unit);
                        measItem.frequency = Value;
                        measItem.frequency_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "标称值"), StanValue, Unit);
                        measItem.nominal_value = StanValue;
                        measItem.unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "指标下限"), Value, Unit);
                        measItem.lower_index = Value;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "测量值"), Value, Unit);
                        measItem.measure_value = Value;
                        measItem.measure_value_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "指标上限"), Value, Unit);
                        measItem.upper_index = Value;
                        measItem.relative_error = getValueByHeader(i, sLabel, "相对误差");
                        measItem.urel = getValueByHeader(i, sLabel, "U(k=2)");
                    } else if (sLabel == "调幅深度") {
                        measItem.data_type = 3;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "频率"), Value, Unit);
                        measItem.frequency = Value;
                        measItem.frequency_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "高通滤波"), Value, Unit);
                        measItem.high_pass_filter = Value;
                        measItem.high_pass_filter_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "低通滤波"), Value, Unit);
                        measItem.lower_pass_filter = Value;
                        measItem.lower_pass_filter_unit = Unit;
                        // lower_pass_filter
                        ItemUnitSplit(getValueByHeader(i, sLabel, "标称值"), StanValue, Unit);
                        measItem.nominal_value = StanValue;
                        measItem.unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "指标下限"), Value, Unit);
                        measItem.lower_index = Value;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "测量值"), Value, Unit);
                        measItem.measure_value = Value;
                        measItem.measure_value_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "指标上限"), Value, Unit);
                        measItem.upper_index = Value;
                        measItem.relative_error = getValueByHeader(i, sLabel, "相对误差");
                        measItem.urel = getValueByHeader(i, sLabel, "U(k=2)");
                        measItem.thd = getValueByHeader(i, sLabel, "THD");
                        Value = getValueByHeader(i, sLabel, "THD上限");
                        int percentSignIndex = Value.lastIndexOf('%');
                        int lessThanSignIndex = Value.indexOf('<');
                        measItem.upper_thd = Value.mid(lessThanSignIndex + 1, percentSignIndex - lessThanSignIndex - 1);
                        measItem.upper_index_symbol = Value.left(lessThanSignIndex + 1);
                        measItem.upper_index_unit = Value.right(1);
                    } else if (sLabel == "调频频偏") {
                        measItem.data_type = 4;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "频率"), Value, Unit);
                        measItem.frequency = Value;
                        measItem.frequency_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "高通滤波"), Value, Unit);
                        measItem.high_pass_filter = Value;
                        measItem.high_pass_filter_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "低通滤波"), Value, Unit);
                        measItem.lower_pass_filter = Value;
                        measItem.lower_pass_filter_unit = Unit;
                        // lower_pass_filter
                        ItemUnitSplit(getValueByHeader(i, sLabel, "标称值"), StanValue, Unit);
                        measItem.nominal_value = StanValue;
                        measItem.unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "指标下限"), Value, Unit);
                        measItem.lower_index = Value;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "测量值"), Value, Unit);
                        measItem.measure_value = Value;
                        measItem.measure_value_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "指标上限"), Value, Unit);
                        measItem.upper_index = Value;
                        measItem.relative_error = getValueByHeader(i, sLabel, "相对误差");
                        measItem.urel = getValueByHeader(i, sLabel, "U(k=2)");
                        measItem.thd = getValueByHeader(i, sLabel, "THD");
                        Value = getValueByHeader(i, sLabel, "THD上限");
                        int percentSignIndex = Value.lastIndexOf('%');
                        int lessThanSignIndex = Value.indexOf('<');
                        measItem.upper_thd = Value.mid(lessThanSignIndex + 1, percentSignIndex - lessThanSignIndex - 1);
                        measItem.upper_index_symbol = Value.left(lessThanSignIndex + 1);
                        measItem.upper_index_unit = Value.right(1);
                    } else if (sLabel == "剩余调幅") {
                        measItem.data_type = 5;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "频率"), Value, Unit);
                        measItem.frequency = Value;
                        measItem.frequency_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "高通滤波"), Value, Unit);
                        measItem.high_pass_filter = Value;
                        measItem.high_pass_filter_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "低通滤波"), Value, Unit);
                        measItem.lower_pass_filter = Value;
                        measItem.lower_pass_filter_unit = Unit;
                        measItem.remain_amplitude_modulation = getValueByHeader(i, sLabel, "剩余调幅");
                        ItemUnitSplit(getValueByHeader(i, sLabel, "指标上限"), Value, Unit);
                        measItem.urel = getValueByHeader(i, sLabel, "U(k=2)");

                    } else if (sLabel == "剩余调频") {
                        measItem.data_type = 6;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "频率"), Value, Unit);
                        measItem.frequency = Value;
                        measItem.frequency_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "高通滤波"), Value, Unit);
                        measItem.high_pass_filter = Value;
                        measItem.high_pass_filter_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "低通滤波"), Value, Unit);
                        measItem.lower_pass_filter = Value;
                        measItem.lower_pass_filter_unit = Unit;
                        measItem.remain_amplitude_modulation = getValueByHeader(i, sLabel, "剩余调幅");

                        measItem.upper_index = getValueByHeader(i, sLabel, "指标上限");
                        measItem.urel = getValueByHeader(i, sLabel, "U(k=2)");

                    } else if (sLabel == "谐波") {
                        measItem.data_type = 7;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "频率"), Value, Unit);
                        measItem.frequency = Value;
                        measItem.frequency_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "幅度"), Value, Unit);
                        measItem.amplitude = Value;
                        measItem.amplitude_unit = Unit;
                        measItem.harmonic_number = getValueByHeader(i, sLabel, "谐波次数");
                        ItemUnitSplit(getValueByHeader(i, sLabel, "基波幅度"), Value, Unit);
                        measItem.fundamental_amplitude = Value;
                        measItem.fundamental_amplitude_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "谐波幅度"), Value, Unit);
                        measItem.harmonic_amplitude = Value;
                        measItem.harmonic_amplitude_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "测量值"), Value, Unit);
                        measItem.measure_value = Value;
                        measItem.measure_value_unit = Unit;
                        measItem.upper_index = getValueByHeader(i, sLabel, "指标上限");
                        measItem.urel = getValueByHeader(i, sLabel, "U(k=2)");
                    } else if (sLabel == "调相相偏") {
                        measItem.data_type = 7;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "频率"), Value, Unit);
                        measItem.frequency = Value;
                        measItem.frequency_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "高通滤波"), Value, Unit);
                        measItem.high_pass_filter = Value;
                        measItem.high_pass_filter_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "低通滤波"), Value, Unit);
                        measItem.lower_pass_filter = Value;
                        measItem.lower_pass_filter_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "调制速率"), Value, Unit);
                        measItem.modulation_rate = Value;
                        measItem.modulation_rate_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "标称值"), StanValue, Unit);
                        measItem.nominal_value = StanValue;
                        measItem.unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "指标下限"), Value, Unit);
                        measItem.lower_index = Value;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "测量值"), Value, Unit);
                        measItem.measure_value = Value;
                        measItem.measure_value_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "指标上限"), Value, Unit);
                        measItem.upper_index = Value;
                        measItem.urel = getValueByHeader(i, sLabel, "U(k=2)");
                        measItem.thd = getValueByHeader(i, sLabel, "THD");
                        Value = getValueByHeader(i, sLabel, "THD上限");
                        int percentSignIndex = Value.lastIndexOf('%');
                        int lessThanSignIndex = Value.indexOf('<');
                        measItem.upper_thd = Value.mid(lessThanSignIndex + 1, percentSignIndex - lessThanSignIndex - 1);
                        measItem.upper_index_symbol = Value.left(lessThanSignIndex + 1);
                        measItem.upper_index_unit = Value.right(1);
                    } else if (sLabel == "非谐波") {
                        measItem.data_type = 6;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "频率"), Value, Unit);
                        measItem.frequency = Value;
                        measItem.frequency_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "功率"), Value, Unit);
                        measItem.power = Value;
                        measItem.power_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "偏移频率"), Value, Unit);
                        measItem.offset_frequency = Value;
                        measItem.offset_frequency_unit = Unit;
                        ItemUnitSplit(getValueByHeader(i, sLabel, "测量值"), Value, Unit);
                        measItem.measure_value = Value;
                        measItem.measure_value_unit = Unit;
                        measItem.remain_amplitude_modulation = getValueByHeader(i, sLabel, "剩余调幅");
                        measItem.upper_index = getValueByHeader(i, sLabel, "指标上限");
                        measItem.urel = getValueByHeader(i, sLabel, "U(k=2)");
                    }
                    BaseCommonApi::SearchSelectTable(QString::number(iEquipType), &measItem);
                }
            }
            // 示波器检定装置
            else if (iEquipType == 13) {
                checkitemlist.insert("时基", "1");
                checkitemlist.insert("直流增益", "2");
                checkitemlist.insert("直流偏置", "3");
                checkitemlist.insert("频带宽度", "4");
                checkitemlist.insert("触发灵敏度", "5");
                checkitemlist.insert("上升时间", "6");
                checkitemlist.insert("输入电阻", "7");
                checkitemlist.insert("校准信号", "8");
                QString Value, Unit;
                // ItemUnitSplit
                for (int i = 0; i < dataRes.count(); ++i) {
                    DeviceOscilloscopeCalibration measItem;
                    measItem.id = QString::number(api.generateId());
                    measItem.data_id = measInfo.id;
                    measItem.data_type = getDictebyItemName(getValueByHeader(i, sLabel, "检定项目"));
                    ItemUnitSplit(getValueByHeader(i, sLabel, "周期时标信号"), Value, Unit);
                    measItem.cycle_signal = Value;
                    measItem.cycle_signal_unit = Unit;
                    measItem.time_base_absolute_error = getValueByHeader(i, sLabel, "时基绝对误差△T/ns");
                    measItem.time_base_relative_error = getValueByHeader(i, sLabel, "时基相对误差△δ");
                    ItemUnitSplit(getValueByHeader(i, sLabel, "技术指标"), Value, Unit);
                    measItem.technical_index = Value;
                    measItem.technical_index_unit = Unit;
                    measItem.conclusion = getValueByHeader(i, sLabel, "结论");
                    measItem.thoroughfare = getValueByHeader(i, sLabel, "通道");
                    ItemUnitSplit(getValueByHeader(i, sLabel, "阻抗"), Value, Unit);
                    measItem.impedance = Value;
                    measItem.impedance_unit = Unit;
                    ItemUnitSplit(getValueByHeader(i, sLabel, "幅度/DIV"), Value, Unit);
                    measItem.vertical_deflect_coefficient = Value;
                    measItem.vertical_deflect_coefficient_unit = Unit;
                    ItemUnitSplit(getValueByHeader(i, sLabel, "标准值"), Value, Unit);
                    measItem.standard_value = Value;
                    measItem.standard_value_unit = Unit;
                    ItemUnitSplit(getValueByHeader(i, sLabel, "测量值"), Value, Unit);
                    measItem.measure_value = Value;
                    measItem.measure_value_unit = Unit;
                    measItem.relative_error = getValueByHeader(i, sLabel, "相对误差");
                    measItem.error_value = getValueByHeader(i, sLabel, "误差");
                    ItemUnitSplit(getValueByHeader(i, sLabel, "频带宽度"), Value, Unit);
                    measItem.band_width = Value;
                    measItem.band_width_unit = Unit;
                    ItemUnitSplit(getValueByHeader(i, sLabel, "参考幅度"), Value, Unit);
                    measItem.reference_amplitude = Value;
                    measItem.reference_amplitude = Unit;
                    ItemUnitSplit(getValueByHeader(i, sLabel, "测量幅度"), Value, Unit);
                    measItem.measure_amplitude = Value;
                    measItem.measure_amplitude_unit = Unit;
                    measItem.decibel_number = getValueByHeader(i, sLabel, "分贝数(dB)");
                    measItem.trigger_type = getValueByHeader(i, sLabel, "触发类型");
                    ItemUnitSplit(getValueByHeader(i, sLabel, "标称值"), Value, Unit);
                    measItem.nominal_value = Value;
                    measItem.nominal_value_unit = Unit;
                    measItem.data_type = getValueByHeader(i, sLabel, "项目");
                    ItemUnitSplit(getValueByHeader(i, sLabel, "顶部测量值"), Value, Unit);
                    measItem.top_measure_value = Value;
                    measItem.top_measure_value_unit = Unit;
                    ItemUnitSplit(getValueByHeader(i, sLabel, "底部测量值"), Value, Unit);
                    measItem.bottom_measure_value = Value;
                    measItem.bottom_measure_value_unit = Unit;
                    // decibel_number
                    BaseCommonApi::SearchSelectTable(QString::number(iEquipType), &measItem);
                }
            }
            // 微波衰减器
            else if (iEquipType == 11) {
                QString Value, Unit;
                // ItemUnitSplit
                for (int i = 0; i < dataRes.count(); ++i) {
                    MicrowaveAttenuationCalibration measItem;
                    measItem.id = QString::number(api.generateId());
                    measItem.data_id = measInfo.id;
                    measItem.data_type = "1";
                    ItemUnitSplit(getValueByHeader(i, sLabel, "频率"), Value, Unit);
                    measItem.frequency = Value;
                    measItem.frequency_unit = Unit;
                    ItemUnitSplit(getValueByHeader(i, sLabel, "标称值"), Value, Unit);
                    measItem.nominal_value = Value;
                    measItem.nominal_value_unit = Unit;
                    measItem.composition = getValueByHeader(i, sLabel, "组合方式");
                    measItem.direct_measure_value = getValueByHeader(i, sLabel, "直接测量值");
                    measItem.add_measure_value = getValueByHeader(i, sLabel, "加入衰减测量值");
                    measItem.attenuation_amount = getValueByHeader(i, sLabel, "衰减量");
                    measItem.lower_index = getValueByHeader(i, sLabel, "指标下限");
                    measItem.upper_index = getValueByHeader(i, sLabel, "指标上限");
                    measItem.urel = getValueByHeader(i, sLabel, "U(k=2)");
                    measItem.conclusion = getValueByHeader(i, sLabel, "结论");
                    // decibel_number
                    BaseCommonApi::SearchSelectTable(QString::number(iEquipType), &measItem);
                }
            } else if (iEquipType == 16) {
                checkitemlist.insert("频率", "1");
                checkitemlist.insert("交流电压", "2");
                checkitemlist.insert("直流电压", "3");
                checkitemlist.insert("上升时间", "4");
                checkitemlist.insert("正弦信号平坦度", "5");
                checkitemlist.insert("正弦信号平坦度", "6");
                QString Value, Unit;
                // ItemUnitSplit
                for (int i = 0; i < dataRes.count(); ++i) {
                    LFSignalGeneratoCalibration measItem;
                    measItem.id = QString::number(api.generateId());
                    measItem.data_id = measInfo.id;
                    measItem.data_type = getDictebyItemName(getValueByHeader(i, sLabel, "检定项目"));
                    ItemUnitSplit(getValueByHeader(i, sLabel, "标称值"), Value, Unit);
                    measItem.nominal_value = Value;
                    measItem.unit = Unit;
                    measItem.lower_index = getValueByHeader(i, sLabel, "指标下限");
                    measItem.upper_index = getValueByHeader(i, sLabel, "指标上限");
                    ItemUnitSplit(getValueByHeader(i, sLabel, "测量值"), Value, Unit);
                    measItem.measure_value = Value;
                    measItem.measure_value_unit = Unit;
                    measItem.conclusion = getValueByHeader(i, sLabel, "结论");
                    // measItem.convert_deviation="";
                    ItemUnitSplit(getValueByHeader(i, sLabel, "频率"), Value, Unit);
                    measItem.frequency = Value;
                    measItem.frequency_unit = Unit;
                    ItemUnitSplit(getValueByHeader(i, sLabel, "电压"), Value, Unit);
                    measItem.voltage = Value;
                    measItem.voltage_unit = Unit;
                    measItem.rise_time = getValueByHeader(i, sLabel, "上升时间");
                    // 是否为基准点 如何判断 需确定
                    measItem.datum_point = "0";
                    ItemUnitSplit(getValueByHeader(i, sLabel, "交流频率"), Value, Unit);
                    measItem.ac_frequency = Value;
                    measItem.ac_frequency_unit = Unit;
                    ItemUnitSplit(getValueByHeader(i, sLabel, "幅度"), Value, Unit);
                    measItem.amplitude = Value;
                    measItem.amplitude_unit = Unit;
                    measItem.flatness = getValueByHeader(i, sLabel, "平坦度");
                    // 超出范围标*(1/0)界面没有
                    Value = getValueByHeader(i, sLabel, "技术指标");
                    if (Value != "") {
                        measItem.technical_index = Value.mid(1, Value.count() - 1);
                        measItem.technical_index_symbol = Value.left(1);
                        measItem.technical_index_unit = Value.right(1);
                    }

                    BaseCommonApi::SearchSelectTable(QString::number(iEquipType), &measItem);
                }
            }
        }
        //        for(SaveDeviceMeasureCalibrator& item:measureCalList){
        //            item.id = QString::number(api.generateId());
        //            item.data_id = measInfo.id;
        //            BaseCommonApi::InsertVerMultiCal(item);
        //        }
        QMessageBox::information(this, "提示", "保存成功！");
    } else {
        QMessageBox::warning(this, "警告", "保存失败！");
    }
}

void DialogCheckResult::on_pushButtonPrint_clicked()
{
    QMessageBox::warning(this, "警告", "打印");
}
