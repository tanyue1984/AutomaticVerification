#include "dialogverifyresult.h"
#include "basecommonapi.h"
#include "global.h"
#include "ui_dialogverifyresult.h"
#include <QMessageBox>

DialogVerifyResult::DialogVerifyResult(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DialogVerifyResult)
{
    ui->setupUi(this);
    QStringList headers;
    headers << "序号"
            << "设备名称"
            << "型号规格"
            << "出厂编号"
            << "制造厂"
            << "证书有效期"
            << "备注";
    ;
    ui->tableWidgetStd->setColumnCount(headers.count());
    ui->tableWidgetStd->setHorizontalHeaderLabels(headers);
    ui->tableWidgetChecker->setColumnCount(headers.count());
    ui->tableWidgetChecker->setHorizontalHeaderLabels(headers);
    for (int i = 1; i < headers.count(); i++) {
        ui->tableWidgetStd->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
        ui->tableWidgetChecker->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }
    ui->tableWidgetChecker->setFixedHeight(80);
    ui->tableWidgetStd->verticalHeader()->setParent(nullptr);
    ui->tableWidgetChecker->verticalHeader()->setParent(nullptr);
    ui->tableWidgetCheck->verticalHeader()->setParent(nullptr);
    ui->tableWidgetCheckStable->verticalHeader()->setParent(nullptr);
    ui->dateEditTime->setDate(QDate::currentDate());
    setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
}

DialogVerifyResult::~DialogVerifyResult()
{
    delete ui;
}

void DialogVerifyResult::deleteItem(QLayout* layout)
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

void DialogVerifyResult::setData(int iType, QMap<QString, QMap<QString, QList<QStringList>>>* data,
    QMap<QString, QMap<QString, QStringList>>* header, CheckProgramManage* program, QString stdId, QString bizIds)
{
    iEquipType = iType;
    veriDataMapList = data;
    veriHeadList = header;
    programInfo = program;
    selectStdIds = stdId;
    selectBizIds = bizIds;

    QStringList idList = selectStdIds.split(",");
    int rowCount = 0;
    int row = 0;
    for (const QString& id : idList) {
        //<<"序号"<<"设备名称"<<"型号规格"<<"出厂编号"<<"制造厂"<<"证书有效期"<<"备注";
        QList<CatDeviceInfor> result = BaseCommonApi::getCatDeviceInfor(id);
        if (result.length() > 0) {
            CatDeviceInfor item = result[0];
            rowCount = ui->tableWidgetStd->rowCount();
            ui->tableWidgetStd->insertRow(rowCount);
            ui->tableWidgetStd->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
            ui->tableWidgetStd->setItem(row, 1, new QTableWidgetItem(item.equipment_name));
            ui->tableWidgetStd->setItem(row, 2, new QTableWidgetItem(item.model)); // 设置数据
            ui->tableWidgetStd->setItem(row, 3, new QTableWidgetItem(item.manufacture_no));
            ui->tableWidgetStd->setItem(row, 4, new QTableWidgetItem(item.manufacturer));
            ui->tableWidgetStd->setItem(row, 5, new QTableWidgetItem(item.measure_valid_date));
            ui->tableWidgetStd->setItem(row, 6, new QTableWidgetItem(item.remark));
            row++;
        }
        result.clear();
    }
    idList.clear();

    idList = selectBizIds.split(",");
    rowCount = 0;
    row = 0;
    for (const QString& id : idList) {
        QList<CatDeviceInfor> result = BaseCommonApi::getCatDeviceInfor(id);
        if (result.length() > 0) {
            CatDeviceInfor item = result[0];
            rowCount = ui->tableWidgetChecker->rowCount();
            ui->tableWidgetChecker->insertRow(rowCount);
            ui->tableWidgetChecker->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
            ui->tableWidgetChecker->setItem(row, 1, new QTableWidgetItem(item.equipment_name));
            ui->tableWidgetChecker->setItem(row, 2, new QTableWidgetItem(item.model)); // 设置数据
            ui->tableWidgetChecker->setItem(row, 3, new QTableWidgetItem(item.manufacture_no));
            ui->tableWidgetChecker->setItem(row, 4, new QTableWidgetItem(item.manufacturer));
            ui->tableWidgetChecker->setItem(row, 5, new QTableWidgetItem(item.measure_valid_date));
            ui->tableWidgetChecker->setItem(row, 6, new QTableWidgetItem(item.remark));
            row++;
        }
        result.clear();
    }
    idList.clear();

    deleteItem(ui->horizontalLayoutMain);
    mainTabIdx = -1;
    int index = 0;
    for (const auto& item : veriHeadList->keys()) {
        // ========================================================================================
        // 按钮设置显示
        // ====================================================================================
        QPushButton* button = new QPushButton(item, this);
        // 设置样式
        //  设置按钮大小为 56x20
        // 固定大小文字显示不全
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
    if (iEquipType == 1) {
        QComboBox* comboBox = new QComboBox();
        comboBox->addItems({ "ALL", "最佳点", "最差点", "典型点" });
        // 连接下拉框的currentIndexChanged信号到槽函数
        connect(comboBox, &QComboBox::currentTextChanged, this, [this, comboBox]() {
            initCheckTable(mainTabIdx, comboBox->currentText(), true);
            // onComboBoxCurrTextChanged(comboBox->currentText(),id,i);
        });
        ui->horizontalLayoutMain->addWidget(comboBox);
    }
    initCheckTable(0);
}

void DialogVerifyResult::initCheckTable(int idx, QString filter, bool bForce)
{
    if (veriHeadList->keys().count() == 0)
        return;
    if (mainTabIdx == idx && !bForce)
        return;
    mainTabIdx = idx;
    QString sLabel = veriHeadList->keys()[mainTabIdx];
    QString sLabel2 = "重复性";
    const auto veriData = veriDataMapList->value(sLabel);
    auto dataRes = veriData.value(sLabel2);
    const auto headDta = veriHeadList->value(sLabel);
    auto headrs = headDta.value(sLabel2);
    ui->tableWidgetCheck->clearContents();
    ui->tableWidgetCheck->setRowCount(0);
    ui->tableWidgetCheck->setColumnCount(0);
    ui->tableWidgetCheck->setColumnCount(headrs.count());
    ui->tableWidgetCheck->setHorizontalHeaderLabels(headrs);
    for (int i = 0; i < headrs.count(); i++) {
        ui->tableWidgetCheck->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }
    int rowCount = 0;
    for (int i = 0; i < dataRes.count(); ++i) {
        if (iEquipType == 1 && filter != "ALL") {
            QString sType = getValueByHeader(dataRes[i], headrs, "核查点类型");
            if (sType != filter)
                continue;
        }
        // 获取行号 并进行添加行
        rowCount = ui->tableWidgetCheck->rowCount();
        ui->tableWidgetCheck->insertRow(rowCount);
        ui->tableWidgetCheck->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        for (int j = 1; j < dataRes[i].count(); j++) {
            ui->tableWidgetCheck->setItem(i, j, new QTableWidgetItem(dataRes[i][j]));
        }
    }

    sLabel2 = "稳定性";
    dataRes = veriData.value(sLabel2);
    headrs = headDta.value(sLabel2);
    ui->tableWidgetCheckStable->clearContents();
    ui->tableWidgetCheckStable->setRowCount(0);
    ui->tableWidgetCheckStable->setColumnCount(0);
    ui->tableWidgetCheckStable->setColumnCount(headrs.count());
    ui->tableWidgetCheckStable->setHorizontalHeaderLabels(headrs);
    for (int i = 0; i < headrs.count(); i++) {
        ui->tableWidgetCheckStable->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }
    rowCount = 0;
    for (int i = 0; i < dataRes.count(); ++i) {
        if (iEquipType == 1 && filter != "ALL") {
            QString sType = getValueByHeader(dataRes[i], headrs, "核查点类型");
            if (sType != filter)
                continue;
        }
        // 获取行号 并进行添加行
        rowCount = ui->tableWidgetCheckStable->rowCount();
        ui->tableWidgetCheckStable->insertRow(rowCount);
        ui->tableWidgetCheckStable->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        for (int j = 1; j < dataRes[i].count(); j++) {
            ui->tableWidgetCheckStable->setItem(i, j, new QTableWidgetItem(dataRes[i][j]));
        }
    }
}

QString DialogVerifyResult::getValueByHeader(QStringList dataList, QStringList headList, QString sName)
{
    for (int i = 0; i < headList.length(); i++) {
        if (headList[i] == sName)
            return dataList[i];
    }
    return "";
}

void DialogVerifyResult::on_pushButtonSave_2_clicked()
{
    if (ui->lineEditNo->text() == "") {
        QMessageBox::warning(this, "提示", "数据编号不能为空！");
        return;
    }
    if (ui->lineEditTemp->text() == "") {
        QMessageBox::warning(this, "提示", "温度不能为空！");
        return;
    }
    if (ui->lineEdit_Hum->text() == "") {
        QMessageBox::warning(this, "提示", "温度不能为空！");
        return;
    }
    StandardCheckInfo baseInfo;
    BaseCommonApi api;
    baseInfo.id = QString::number(api.generateId());
    baseInfo.data_no = ui->lineEditNo->text(); // 数据编号
    baseInfo.check_date = ui->dateEditTime->text(); // 核查日期
    baseInfo.check_address = ui->comboBoxPlace->currentText(); // 核查地点
    baseInfo.temperature = ui->lineEditTemp->text(); // 温度
    baseInfo.humidity = ui->lineEdit_Hum->text(); // 湿度
    baseInfo.check_accord = ui->lineEditRule->text(); // 核查依据(即标准装置的作业指导书文件minio文件名，多个分号分割)
    baseInfo.stability_examine = ui->comboBoxExam->currentText() == "是" ? "1" : "0"; // 是否用于稳定性考核(1/0)
    baseInfo.standard_equipment_id = programInfo->standard_id; // 被核查标准装置id
    baseInfo.equipment_id = selectBizIds; // 被核查设备id
    baseInfo.item_category_id = programInfo->equipment_type; // 核查项分类id
    baseInfo.conclusion = ui->comboBoxConclusion->currentText(); // 结论(字典value)
    baseInfo.remark = ui->lineEditNote->text(); // 备注
    // QString create_user_id;//核查员id
    baseInfo.create_user_name = ui->comboBox_User->currentText(); // 核查员
    baseInfo.create_time = baseInfo.update_time = QDateTime::currentDateTime();
    baseInfo.create_user_id = loginUserId;
    QString Value, Unit;
    if (BaseCommonApi::InsertStandardCheckInfo(baseInfo)) {
        switch (iEquipType) {
        // 多功能校准源
        case 1:
            for (QString sLabel : veriHeadList->keys()) {
                QMap<QString, QStringList> items = veriHeadList->value(sLabel);
                for (QString sLabel2 : items.keys()) {
                    QList<QStringList> dataRes = veriDataMapList->value(sLabel).value(sLabel2);
                    QStringList headers = items.value(sLabel2);
                    for (int i = 0; i < dataRes.count(); ++i) {
                        StandardCheckDataCalibrator calData;
                        calData.id = QString::number(api.generateId());
                        calData.data_id = baseInfo.id; // 核查数据管理基础信息表id
                        calData.check_type = getValueByHeader(dataRes[i], headers, "核查类型"); // 核查类型(直接存字典value)
                        calData.test_type = getValueByHeader(dataRes[i], headers, "核查点类型");
                        calData.params = getValueByHeader(dataRes[i], headers, "核查项目");
                        calData.unit = getValueByHeader(dataRes[i], headers, "单位");
                        calData.frequency = getValueByHeader(dataRes[i], headers, "频率");
                        calData.frequency_unit = getValueByHeader(dataRes[i], headers, "频率单位");
                        calData.check_point = getValueByHeader(dataRes[i], headers, "核查点");
                        if (sLabel2 == "重复性") {
                            calData.test_value_one = getValueByHeader(dataRes[i], headers, "示值1");
                            calData.test_value_two = getValueByHeader(dataRes[i], headers, "示值2");
                            calData.test_value_three = getValueByHeader(dataRes[i], headers, "示值3");
                            calData.test_value_four = getValueByHeader(dataRes[i], headers, "示值4");
                            calData.test_value_five = getValueByHeader(dataRes[i], headers, "示值5");
                            calData.test_value_six = getValueByHeader(dataRes[i], headers, "示值6");
                            calData.average_value = getValueByHeader(dataRes[i], headers, "算数平均值");
                            calData.standard_deviation = getValueByHeader(dataRes[i], headers, "标准偏差S(X)");
                            calData.relative_repeatability = getValueByHeader(dataRes[i], headers, "相对重复性");
                        } else {
                            for (int k = 0; k < headers.count(); k++) {
                                if (headers[k] == "算数平均值") {
                                    calData.average_value = dataRes[i][k];
                                    calData.test_value_six = dataRes[i][k - 1];
                                    calData.test_value_six_date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
                                    break;
                                }
                            }
                            calData.standard_deviation = getValueByHeader(dataRes[i], headers, "标准偏差S(X)");
                            calData.relative_repeatability = getValueByHeader(dataRes[i], headers, "相对重复性");
                            calData.urel = getValueByHeader(dataRes[i], headers, "Urel");
                            if (calData.relative_repeatability.toDouble() < calData.urel.toDouble())
                                calData.less_than = "1";
                            else
                                calData.less_than = "0";
                        }
                        calData.create_time = calData.update_time = QDateTime::currentDateTime();
                        BaseCommonApi::InsertStandardCheckDataCalibrator(calData);
                    }
                }
            }
            break;
        // 直流稳压电源
        case 2:
            for (QString sLabel : veriHeadList->keys()) {
                QMap<QString, QStringList> items = veriHeadList->value(sLabel);
                for (QString sLabel2 : items.keys()) {
                    QList<QStringList> dataRes = veriDataMapList->value(sLabel).value(sLabel2);
                    QStringList headers = items.value(sLabel2);
                    for (int i = 0; i < dataRes.count(); ++i) {
                        StandardCheckDataDcPower calData;
                        calData.id = QString::number(api.generateId());
                        calData.data_id = baseInfo.id; // 核查数据管理基础信息表id
                        calData.check_type = getValueByHeader(dataRes[i], headers, "核查类型"); // 核查类型(直接存字典value)
                        calData.params = getValueByHeader(dataRes[i], headers, "核查项目");
                        calData.check_param = getValueByHeader(dataRes[i], headers, "核查参数");
                        calData.unit = getValueByHeader(dataRes[i], headers, "单位");
                        calData.check_point = getValueByHeader(dataRes[i], headers, "核查点");
                        if (sLabel2 == "重复性") {
                            calData.test_value_one = getValueByHeader(dataRes[i], headers, "示值1");
                            calData.test_value_two = getValueByHeader(dataRes[i], headers, "示值2");
                            calData.test_value_three = getValueByHeader(dataRes[i], headers, "示值3");
                            calData.test_value_four = getValueByHeader(dataRes[i], headers, "示值4");
                            calData.test_value_five = getValueByHeader(dataRes[i], headers, "示值5");
                            calData.test_value_six = getValueByHeader(dataRes[i], headers, "示值6");
                            calData.average_value = getValueByHeader(dataRes[i], headers, "平均值");
                            calData.standard_deviation = getValueByHeader(dataRes[i], headers, "标准偏差");
                            calData.relative_repeatability = getValueByHeader(dataRes[i], headers, "相对标准偏差");
                        } else {
                            for (int k = 0; k < headers.count(); k++) {
                                if (headers[k] == "平均值") {
                                    calData.average_value = dataRes[i][k];
                                    calData.test_value_six = dataRes[i][k - 1];
                                    calData.test_value_six_date = QDate::currentDate().toString("yyyy-MM-dd");
                                    break;
                                }
                            }
                            calData.standard_deviation = getValueByHeader(dataRes[i], headers, "标准偏差");
                            calData.relative_repeatability = getValueByHeader(dataRes[i], headers, "相对标准偏差");
                            calData.urel = getValueByHeader(dataRes[i], headers, "相对扩展不确定度Urel");
                            if (calData.relative_repeatability.toDouble() < calData.urel.toDouble())
                                calData.less_than = "1";
                            else
                                calData.less_than = "0";
                            // calData.less_than=getValueByHeader(dataRes[i],headers,"相对标准偏差是否小于相对扩展不确定度Urel");
                        }
                        calData.create_time = calData.update_time = QDateTime::currentDateTime();
                        BaseCommonApi::InsertStandardCheckItemDataDcPower(calData);
                    }
                }
            }
            break;
        // 二等铂电阻
        case 5:
            for (QString sLabel : veriHeadList->keys()) {
                QMap<QString, QStringList> items = veriHeadList->value(sLabel);
                for (QString sLabel2 : items.keys()) {
                    QList<QStringList> dataRes = veriDataMapList->value(sLabel).value(sLabel2);
                    QStringList headers = items.value(sLabel2);
                    for (int i = 0; i < dataRes.count(); ++i) {
                        StandardCheckDataResistanceThermometer calData;
                        calData.id = QString::number(api.generateId());
                        calData.data_id = baseInfo.id; // 核查数据管理基础信息表id
                        calData.check_type = getValueByHeader(dataRes[i], headers, "核查类型"); // 核查类型(直接存字典value)
                        calData.this_item_check = "1";
                        calData.params = getValueByHeader(dataRes[i], headers, "核查项目");
                        calData.unit = getValueByHeader(dataRes[i], headers, "单位");
                        calData.check_point = getValueByHeader(dataRes[i], headers, "核查点");
                        calData.thoroughfare = getValueByHeader(dataRes[i], headers, "通道");
                        if (sLabel == "温度值") {
                            if (sLabel2 == "重复性") {
                                calData.test_value_one = getValueByHeader(dataRes[i], headers, "核查标准示值1");
                                calData.measure_value_one = getValueByHeader(dataRes[i], headers, "测量标准示值1");
                                calData.different_value_one = getValueByHeader(dataRes[i], headers, "差值1");
                                calData.test_value_two = getValueByHeader(dataRes[i], headers, "核查标准示值2");
                                calData.measure_value_two = getValueByHeader(dataRes[i], headers, "测量标准示值2");
                                calData.different_value_two = getValueByHeader(dataRes[i], headers, "差值2");
                                calData.test_value_three = getValueByHeader(dataRes[i], headers, "核查标准示值3");
                                calData.measure_value_three = getValueByHeader(dataRes[i], headers, "测量标准示值3");
                                calData.different_value_three = getValueByHeader(dataRes[i], headers, "差值3");
                                calData.test_value_four = getValueByHeader(dataRes[i], headers, "核查标准示值4");
                                calData.measure_value_four = getValueByHeader(dataRes[i], headers, "测量标准示值4");
                                calData.different_value_four = getValueByHeader(dataRes[i], headers, "差值4");
                                calData.test_value_five = getValueByHeader(dataRes[i], headers, "核查标准示值5");
                                calData.measure_value_five = getValueByHeader(dataRes[i], headers, "测量标准示值5");
                                calData.different_value_five = getValueByHeader(dataRes[i], headers, "差值5");
                                calData.test_value_six = getValueByHeader(dataRes[i], headers, "核查标准示值6");
                                calData.measure_value_six = getValueByHeader(dataRes[i], headers, "测量标准示值6");
                                calData.different_value_six = getValueByHeader(dataRes[i], headers, "差值6");
                                calData.average_value = getValueByHeader(dataRes[i], headers, "平均值(差值)");
                                calData.standard_deviation = getValueByHeader(dataRes[i], headers, "重复性S(x)");
                            } else {
                                for (int k = 0; k < headers.count(); k++) {
                                    if (headers[k] == "平均值") {
                                        calData.average_value = dataRes[i][k];
                                        calData.test_value_six = dataRes[i][k - 1];
                                        calData.test_value_six_date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
                                        break;
                                    }
                                }
                                calData.standard_deviation = getValueByHeader(dataRes[i], headers, "Sm");
                                calData.urel = getValueByHeader(dataRes[i], headers, "扩展不确定度U");
                                calData.less_than = getValueByHeader(dataRes[i], headers, "是否Sm≤U");
                            }
                        } else if (sLabel == "RTP值") {
                            if (sLabel2 == "重复性") {
                                calData.test_value_one = getValueByHeader(dataRes[i], headers, "被校示值(Ω)1");
                                calData.test_value_two = getValueByHeader(dataRes[i], headers, "被校示值(Ω)2");
                                calData.test_value_three = getValueByHeader(dataRes[i], headers, "被校示值(Ω)3");
                                calData.test_value_four = getValueByHeader(dataRes[i], headers, "被校示值(Ω)4");
                                calData.test_value_five = getValueByHeader(dataRes[i], headers, "被校示值(Ω)5");
                                calData.test_value_six = getValueByHeader(dataRes[i], headers, "被校示值(Ω)6");
                                calData.average_value = getValueByHeader(dataRes[i], headers, "平均值");
                            } else {
                                for (int k = 0; k < headers.count(); k++) {
                                    if (headers[k] == "测量极差值") {
                                        calData.measure_value = dataRes[i][k];
                                        calData.test_value_six = dataRes[i][k - 1];
                                        calData.test_value_six_date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
                                        break;
                                    }
                                }
                                calData.allow_value = getValueByHeader(dataRes[i], headers, "允许值");
                                calData.illustration = getValueByHeader(dataRes[i], headers, "结论");
                            }
                        } else if (sLabel == "绝缘电阻表校准") {
                            calData.allow_value = getValueByHeader(dataRes[i], headers, "测量值");
                            calData.indicating_error = getValueByHeader(dataRes[i], headers, "示值误差");
                            calData.maximum_error = getValueByHeader(dataRes[i], headers, "最大允许误差");
                        } else if (sLabel == "测温仪电阻核查") {
                            calData.test_value_one = getValueByHeader(dataRes[i], headers, "测温仪示值1");
                            calData.test_value_two = getValueByHeader(dataRes[i], headers, "测温仪示值2");
                            calData.test_value_three = getValueByHeader(dataRes[i], headers, "测温仪示值3");
                            calData.test_value_four = getValueByHeader(dataRes[i], headers, "测温仪示值4");
                            calData.test_value_five = getValueByHeader(dataRes[i], headers, "测温仪示值5");
                            calData.test_value_six = getValueByHeader(dataRes[i], headers, "测温仪示值6");
                            calData.average_value = getValueByHeader(dataRes[i], headers, "平均值");
                            calData.maximum_deviation = getValueByHeader(dataRes[i], headers, "最大偏差");
                            calData.maximum_error = getValueByHeader(dataRes[i], headers, "最大允许误差绝对值|MPE|");
                        }
                        calData.create_time = calData.update_time = QDateTime::currentDateTime().toString("yyyy-MM-dd");
                        BaseCommonApi::InsertStandardCheckItemDataResistanceThermometer(calData);
                    }
                }
            }
            break;
        // 频谱分析仪
        case 9:
            for (QString sLabel : veriHeadList->keys()) {
                QMap<QString, QStringList> items = veriHeadList->value(sLabel);
                for (QString sLabel2 : items.keys()) {
                    QList<QStringList> dataRes = veriDataMapList->value(sLabel).value(sLabel2);
                    QStringList headers = items.value(sLabel2);
                    for (int i = 0; i < dataRes.count(); ++i) {
                        StandardCheckItemDataSpectrumAnalyzer calData;
                        calData.id = QString::number(api.generateId());
                        calData.data_id = baseInfo.id; // 核查数据管理基础信息表id
                        calData.check_type = getValueByHeader(dataRes[i], headers, "核查类型"); // 核查类型(直接存字典value)
                        calData.params = sLabel; // getValueByHeader(dataRes[i],headers,"核查项目");
                        calData.unit = getValueByHeader(dataRes[i], headers, "单位");
                        calData.check_point = getValueByHeader(dataRes[i], headers, "频率点");
                        calData.unit = getValueByHeader(dataRes[i], headers, "频率点单位");
                        // QString sPower = getValueByHeader(dataRes[i],headers,"功率");
                        // ItemUnitSplit(sPower,Value,Unit);
                        calData.power = getValueByHeader(dataRes[i], headers, "功率");
                        calData.power_unit = getValueByHeader(dataRes[i], headers, "功率单位");
                        // sPower = getValueByHeader(dataRes[i],headers,"相对电平");
                        // ItemUnitSplit(sPower,Value,Unit);
                        calData.relative_level = getValueByHeader(dataRes[i], headers, "相对电平");
                        calData.relative_level_unit = getValueByHeader(dataRes[i], headers, "相对电平单位");
                        calData.composition = getValueByHeader(dataRes[i], headers, "组合方式");
                        if (sLabel2 == "重复性") {
                            calData.test_value_one = getValueByHeader(dataRes[i], headers, "测量值1");
                            calData.test_value_two = getValueByHeader(dataRes[i], headers, "测量值2");
                            calData.test_value_three = getValueByHeader(dataRes[i], headers, "测量值3");
                            calData.test_value_four = getValueByHeader(dataRes[i], headers, "测量值4");
                            calData.test_value_five = getValueByHeader(dataRes[i], headers, "测量值5");
                            calData.test_value_six = getValueByHeader(dataRes[i], headers, "测量值6");
                            calData.test_value_seven = getValueByHeader(dataRes[i], headers, "测量值7");
                            calData.test_value_eight = getValueByHeader(dataRes[i], headers, "测量值8");
                            calData.test_value_nine = getValueByHeader(dataRes[i], headers, "测量值9");
                            calData.test_value_ten = getValueByHeader(dataRes[i], headers, "测量值10");
                            calData.average_value = getValueByHeader(dataRes[i], headers, "平均值x");
                            calData.standard_deviation = getValueByHeader(dataRes[i], headers, "重复性S(x)/x");
                        } else {
                            for (int k = 0; k < headers.count(); k++) {
                                if (headers[k] == "平均值") {
                                    calData.average_value = dataRes[i][k];
                                    calData.test_value_six = dataRes[i][k - 1];
                                    calData.test_value_six_date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
                                    break;
                                }
                            }
                            calData.stability = getValueByHeader(dataRes[i], headers, "稳定性");
                            calData.urel = getValueByHeader(dataRes[i], headers, "U(k=2)");
                            if (calData.stability.toDouble() < calData.urel.toDouble())
                                calData.less_than = "1";
                            else
                                calData.less_than = "0";
                            calData.qualified = getValueByHeader(dataRes[i], headers, "结论");
                        }
                        calData.create_time = calData.update_time = QDateTime::currentDateTime();
                        BaseCommonApi::InsertStandardCheckItemDataSpectrumAnalyzer(calData);
                    }
                }
            }
            break;
        // 信号发生器
        case 10:
            for (QString sLabel : veriHeadList->keys()) {
                QMap<QString, QStringList> items = veriHeadList->value(sLabel);
                for (QString sLabel2 : items.keys()) {
                    QList<QStringList> dataRes = veriDataMapList->value(sLabel).value(sLabel2);
                    QStringList headers = items.value(sLabel2);
                    for (int i = 0; i < dataRes.count(); ++i) {
                        StandardCheckItemDataSignalGenerator calData;
                        calData.id = QString::number(api.generateId());
                        calData.data_id = baseInfo.id; // 核查数据管理基础信息表id
                        calData.check_type = getValueByHeader(dataRes[i], headers, "核查类型"); // 核查类型(直接存字典value)
                        calData.params = getValueByHeader(dataRes[i], headers, "核查项目");
                        calData.unit = getValueByHeader(dataRes[i], headers, "单位");
                        calData.check_point = getValueByHeader(dataRes[i], headers, "核查点");
                        calData.unit = getValueByHeader(dataRes[i], headers, "单位");
                        QString sPower = getValueByHeader(dataRes[i], headers, "功率");
                        ItemUnitSplit(sPower, Value, Unit);
                        calData.power = Value;
                        calData.power_unit = Unit;
                        sPower = getValueByHeader(dataRes[i], headers, "相对电平");
                        ItemUnitSplit(sPower, Value, Unit);
                        calData.relative_level = Value;
                        calData.relative_level_unit = Unit;
                        calData.modulation = getValueByHeader(dataRes[i], headers, "调幅度");
                        sPower = getValueByHeader(dataRes[i], headers, "调频频偏");
                        ItemUnitSplit(sPower, Value, Unit);
                        calData.frequency_deviation = Value;
                        calData.phase_deviation_unit = Unit;
                        sPower = getValueByHeader(dataRes[i], headers, "调相相偏");
                        ItemUnitSplit(sPower, Value, Unit);
                        calData.phase_deviation = Value;
                        calData.frequency_deviation_unit = Unit;
                        if (sLabel2 == "重复性") {
                            calData.test_value_one = getValueByHeader(dataRes[i], headers, "测量值1");
                            calData.test_value_two = getValueByHeader(dataRes[i], headers, "测量值2");
                            calData.test_value_three = getValueByHeader(dataRes[i], headers, "测量值3");
                            calData.test_value_four = getValueByHeader(dataRes[i], headers, "测量值4");
                            calData.test_value_five = getValueByHeader(dataRes[i], headers, "测量值5");
                            calData.test_value_six = getValueByHeader(dataRes[i], headers, "测量值6");
                            calData.test_value_seven = getValueByHeader(dataRes[i], headers, "测量值7");
                            calData.test_value_eight = getValueByHeader(dataRes[i], headers, "测量值8");
                            calData.test_value_nine = getValueByHeader(dataRes[i], headers, "测量值9");
                            calData.test_value_ten = getValueByHeader(dataRes[i], headers, "测量值10");
                            calData.average_value = getValueByHeader(dataRes[i], headers, "平均值x");
                            calData.standard_deviation = getValueByHeader(dataRes[i], headers, "重复性S(x)/x");
                        } else {
                            for (int k = 0; k < headers.count(); k++) {
                                if (headers[k] == "平均值") {
                                    calData.average_value = dataRes[i][k];
                                    calData.test_value_six = dataRes[i][k - 1];
                                    calData.test_value_six_date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
                                    break;
                                }
                            }
                            calData.stability = getValueByHeader(dataRes[i], headers, "稳定性");
                            calData.urel = getValueByHeader(dataRes[i], headers, "U(k=2)");
                            if (calData.stability.toDouble() < calData.urel.toDouble())
                                calData.less_than = "1";
                            else
                                calData.less_than = "0";
                            // calData.less_than=getValueByHeader(dataRes[i],headers,"结论");
                        }
                        calData.create_time = calData.update_time = QDateTime::currentDateTime();
                        BaseCommonApi::InsertStandardCheckItemDataSignalGenerator(calData);
                    }
                }
            }
            break;
            // 微波衰减标准装置
        case 11:
            for (QString sLabel : veriHeadList->keys()) {
                QMap<QString, QStringList> items = veriHeadList->value(sLabel);
                for (QString sLabel2 : items.keys()) {
                    QList<QStringList> dataRes = veriDataMapList->value(sLabel).value(sLabel2);
                    QStringList headers = items.value(sLabel2);
                    for (int i = 0; i < dataRes.count(); ++i) {
                        StandardCheckItemDataMicrowaveAttenuation calData;
                        calData.id = QString::number(api.generateId());
                        calData.data_id = baseInfo.id; // 核查数据管理基础信息表id
                        calData.check_type = sLabel2; // getValueByHeader(dataRes[i],headers,"核查类型");//核查类型(直接存字典value)
                        calData.params = sLabel; // getValueByHeader(dataRes[i],headers,"核查项目");
                        calData.frequency = getValueByHeader(dataRes[i], headers, "频率点"); // 频率点
                        calData.frequency_unit = getValueByHeader(dataRes[i], headers, "频率点单位"); // 频率点单位
                        calData.attenuation_amount = getValueByHeader(dataRes[i], headers, "衰减量"); // 衰减量
                        calData.attenuation_amount_unit = getValueByHeader(dataRes[i], headers, "衰减量单位");
                        calData.composition = getValueByHeader(dataRes[i], headers, "组合方式");
                        calData.gear_type = getValueByHeader(dataRes[i], headers, "档位类型");

                        if (sLabel2 == "重复性") {
                            calData.test_value_one = getValueByHeader(dataRes[i], headers, "测量值1");
                            calData.test_value_two = getValueByHeader(dataRes[i], headers, "测量值2");
                            calData.test_value_three = getValueByHeader(dataRes[i], headers, "测量值3");
                            calData.test_value_four = getValueByHeader(dataRes[i], headers, "测量值4");
                            calData.test_value_five = getValueByHeader(dataRes[i], headers, "测量值5");
                            calData.test_value_six = getValueByHeader(dataRes[i], headers, "测量值6");
                            calData.test_value_seven = getValueByHeader(dataRes[i], headers, "测量值7");
                            calData.test_value_eight = getValueByHeader(dataRes[i], headers, "测量值8");
                            calData.test_value_nine = getValueByHeader(dataRes[i], headers, "测量值9");
                            calData.test_value_ten = getValueByHeader(dataRes[i], headers, "测量值10");
                            calData.average_value = getValueByHeader(dataRes[i], headers, "平均值x");
                            calData.standard_deviation = getValueByHeader(dataRes[i], headers, "重复性S(x)");
                        } else {
                            for (int k = 0; k < headers.count(); k++) {
                                if (headers[k] == "平均值") {
                                    calData.average_value = dataRes[i][k];
                                    calData.test_value_six = dataRes[i][k - 1];
                                    calData.test_value_six_date = QDate::currentDate();
                                    break;
                                }
                            }
                            calData.stability = getValueByHeader(dataRes[i], headers, "稳定性");
                            calData.urel = getValueByHeader(dataRes[i], headers, "U(k=2)");
                            if (calData.stability.toDouble() < calData.urel.toDouble())
                                calData.less_than = "1";
                            else
                                calData.less_than = "0";
                            calData.qualified = getValueByHeader(dataRes[i], headers, "结论");
                        }
                        calData.create_time = calData.update_time = QDateTime::currentDateTime();
                        BaseCommonApi::InsertStandardCheckItemDataMicrowaveAttenuation(calData);
                    }
                }
            }
            break;
            // 失真度标准装置
        case 18:
            for (QString sLabel : veriHeadList->keys()) {
                QMap<QString, QStringList> items = veriHeadList->value(sLabel);
                for (QString sLabel2 : items.keys()) {
                    QList<QStringList> dataRes = veriDataMapList->value(sLabel).value(sLabel2);
                    QStringList headers = items.value(sLabel2);
                    for (int i = 0; i < dataRes.count(); ++i) {
                        StandardCheckDataDistortionFactor calData;
                        calData.id = QString::number(api.generateId());
                        calData.data_id = baseInfo.id; // 核查数据管理基础信息表id
                        calData.check_type = sLabel2; // getValueByHeader(dataRes[i],headers,"核查类型");//核查类型(直接存字典value)
                        calData.params = sLabel; // getValueByHeader(dataRes[i],headers,"核查项目");
                        calData.frequency = getValueByHeader(dataRes[i], headers, "频率点"); // 频率点
                        calData.frequency_unit = getValueByHeader(dataRes[i], headers, "频率点单位"); // 频率点单位
                        calData.nominal_value = getValueByHeader(dataRes[i], headers, "标称值/%"); // 标称值

                        if (sLabel2 == "重复性") {
                            calData.test_value_one = getValueByHeader(dataRes[i], headers, "测量值1");
                            calData.test_value_two = getValueByHeader(dataRes[i], headers, "测量值2");
                            calData.test_value_three = getValueByHeader(dataRes[i], headers, "测量值3");
                            calData.test_value_four = getValueByHeader(dataRes[i], headers, "测量值4");
                            calData.test_value_five = getValueByHeader(dataRes[i], headers, "测量值5");
                            calData.test_value_six = getValueByHeader(dataRes[i], headers, "测量值6");
                            calData.test_value_seven = getValueByHeader(dataRes[i], headers, "测量值7");
                            calData.test_value_eight = getValueByHeader(dataRes[i], headers, "测量值8");
                            calData.test_value_nine = getValueByHeader(dataRes[i], headers, "测量值9");
                            calData.test_value_ten = getValueByHeader(dataRes[i], headers, "测量值10");
                            calData.average_value = getValueByHeader(dataRes[i], headers, "平均值x");
                            calData.standard_deviation = getValueByHeader(dataRes[i], headers, "重复性S(x)");
                        } else {
                            for (int k = 0; k < headers.count(); k++) {
                                if (headers[k] == "平均值") {
                                    calData.average_value = dataRes[i][k];
                                    calData.test_value_six = dataRes[i][k - 1];
                                    calData.test_value_six_date = QDate::currentDate();
                                    break;
                                }
                            }
                            calData.stability = getValueByHeader(dataRes[i], headers, "稳定性");
                            calData.urel = getValueByHeader(dataRes[i], headers, "U(k=2)");
                            if (calData.stability.toDouble() < calData.urel.toDouble())
                                calData.qualified = calData.less_than = 1;
                            else
                                calData.qualified = calData.less_than = 0;
                            // calData.qualified=getValueByHeader(dataRes[i],headers,"结论");
                        }
                        calData.create_time = calData.update_time = QDateTime::currentDateTime();
                        BaseCommonApi::InsertStandardCheckItemDataDistortionFactor(calData);
                    }
                }
            }
            break;
        }
        QMessageBox::information(this, "提示", "保存成功！");
    } else {
        QMessageBox::warning(this, "警告", "保存失败！");
    }
}

void DialogVerifyResult::ItemUnitSplit(QString Value, QString& data, QString& unit)
{
    QString sData2 = Value;
    QString sNewData, sUnit;
    // 正则表达式匹配数字
    QRegularExpression reLetters("[a-zA-ZΩΜΩ]");
    QRegularExpression reLetters1("[^a-zA-ZΩΜΩ]");
    data = sData2.replace(reLetters, "");
    unit = Value.replace(reLetters1, "");
}
