#include "newverifymethod.h"
#include "formaddrsetting.h"
#include "formbizdeviceselect.h"
#include "formcommsetting.h"
#include "formverypoint.h"
#include "global.h"
#include "qfiledialog.h"
#include "ui_newverifymethod.h"
#include "verifydevice.h"
#include <QMessageBox>

NewVerifyMethod::NewVerifyMethod(QWidget* parent, QString sId, bool bEdit)
    : QWidget(parent)
    , methodId(sId)
    , bEditMode(bEdit)
    , ui(new Ui::NewVerifyMethod)
{
    ui->setupUi(this);
    ui->tableWidget->verticalHeader()->setParent(nullptr);
    for (int i = 1; i < 4; i++) {
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
        ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }
    ui->tableWidget->setColumnWidth(0, 60);
    ui->tableWidget->setColumnWidth(5, 200);
    ui->tableWidget_2->setColumnWidth(0, 60);
    ui->tableWidget_2->setColumnWidth(5, 200);
    ui->pushButton_9->setEnabled(bEditMode);

    ui->comboBoxEquipment->addItems(equipNameList);
    ui->dateEditTime->setDate(QDate::currentDate());
    getMethodInfo();
}

void NewVerifyMethod::getMethodInfo()
{
    if (methodId != "") {
        programInfo = BaseCommonApi::getCheckProgramManageById(methodId);

        ui->comboBoxEquipment->setCurrentIndex(equipIdList.indexOf(programInfo.belong_standard_equipment));
        for (int i = 0; i < catList.length(); ++i) {
            if (catList[i].id == programInfo.equipment_type) {
                ui->comboBoxCategory->setCurrentIndex(i); // 找到匹配项，返回 true
                break;
            }
        }
        ui->lineEditName->setText(programInfo.check_name);
        ui->lineEditUser->setText(programInfo.create_name);
        ui->dateEditTime->setDateTime(programInfo.create_time);
        ui->lineEditMark->setText(programInfo.remark);
        if (programInfo.img_path != "") {
            // QImage img(programInfo.img_path);
            // QImage scalimg = img.scaled(ui->label_11->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            // ui->label_11->setPixmap(QPixmap::fromImage(scalimg));
            QString strStyle = "QLabel{"
                               "image:url("
                + programInfo.img_path + ");"
                                         "}";
            qDebug() << "strStyle:" << strStyle;
            ui->labelImage->setStyleSheet(strStyle);
        }
        // chooseDevices(programInfo.standard_device_id,programInfo.verification_documents_id);
        selectStandard("");
        QStringList list;
        selectBizDevice(list);
    }
}

NewVerifyMethod::~NewVerifyMethod()
{
    delete ui;
}

void NewVerifyMethod::showVerifyDevice()
{
    VerifyDevice* checkWindow = new VerifyDevice;
    // checkWindow->newVerifyWnd=this;
    // checkWindow->setWindowState(Qt::WindowMaximized);
    connect(checkWindow, SIGNAL(selectStandard(QString)), this, SLOT(selectStandard(QString)));
    checkWindow->setWindowModality(Qt::ApplicationModal);
    checkWindow->show();
}

void NewVerifyMethod::showVeryPoint()
{
    FormVeryPoint* checkWindow = new FormVeryPoint(nullptr, checkId);
    // checkWindow.setWindowState(Qt::WindowMaximized);
    checkWindow->setWindowModality(Qt::ApplicationModal);
    checkWindow->show();
}

void NewVerifyMethod::on_cmdButton_clicked(int type, QString id)
{
    // qDebug() << "cmd click:" << type << id ;
    FormAddrSetting* form = new FormAddrSetting(nullptr, type, id, checkId, false);
    form->setWindowModality(Qt::ApplicationModal);
    form->show();
}

void NewVerifyMethod::on_commButton_clicked(int type, QString id)
{
    // qDebug() << "comm click:" << type << id ;
    FormCommSetting* form = new FormCommSetting(nullptr, type, id);
    form->setWindowModality(Qt::ApplicationModal);
    form->show();
}

void NewVerifyMethod::selectStandard(QString ids)
{
    if (ids != "") {
        if (programInfo.standard_device_id == "")
            programInfo.standard_device_id = ids;
        else
            programInfo.standard_device_id = QString("%1,%2").arg(programInfo.standard_device_id).arg(ids);
    }
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    QStringList idList;
    int rowCount = 0;
    int row = 0;
    checkId = "";
    if (programInfo.standard_device_id != "") {
        idList = programInfo.standard_device_id.split(",");
        for (const QString& id : idList) {
            checkId = id;
            QList<CatDeviceInfor> result = BaseCommonApi::getCatDeviceInfor(id);
            if (result.length() > 0) {
                rowCount = ui->tableWidget->rowCount();

                ui->tableWidget->insertRow(rowCount);
                QTableWidgetItem* p_check = new QTableWidgetItem();
                p_check->setCheckState(Qt::Unchecked); // 设置首列为 CheckBox 控件
                ui->tableWidget->setItem(row, 0, p_check);
                ui->tableWidget->setItem(row, 1, new QTableWidgetItem(result[0].equipment_name)); // 设置数据
                ui->tableWidget->setItem(row, 2, new QTableWidgetItem(result[0].model)); // 设置数据
                ui->tableWidget->setItem(row, 3, new QTableWidgetItem(result[0].manufacture_no)); // 设置数据
                QPushButton* btn_1 = new QPushButton();
                btn_1->setText(tr("设置命令"));
                btn_1->setStyleSheet("QPushButton{"
                                     "background-color:rgba(255,255,255,0);"
                                     "color:rgba(92,170,54,100);"
                                     "text-decoration:underline;"
                                     "}");
                btn_1->setCursor(Qt::PointingHandCursor);
                int type = 1;
                // connect(btn_1,SIGNAL(clicked()),this,SLOT(on_cmdButton_clicked(1,id)));
                connect(btn_1, &QPushButton::clicked, this, [this, type, id]() {
                    on_cmdButton_clicked(type, id);
                });
                btn_1->setIconSize(QSize(16, 16));
                btn_1->setIcon(QIcon(":/image/Index/u2324.svg"));
                QPushButton* btn_2 = new QPushButton();
                btn_2->setText(tr("设置设备接口"));
                btn_2->setStyleSheet("QPushButton{"
                                     "background-color:rgba(255,255,255,0);"
                                     "color:rgba(92,170,54,100);"
                                     "text-decoration:underline;"
                                     "}");
                btn_2->setCursor(Qt::PointingHandCursor);
                // connect(btn_2,SIGNAL(clicked()),this,SLOT(on_commButton_clicked(1,id)));
                connect(btn_2, &QPushButton::clicked, this, [this, type, id]() {
                    on_commButton_clicked(type, id);
                });
                btn_2->setIconSize(QSize(16, 16));
                btn_2->setIcon(QIcon(":/image/Index/u2324.svg"));
                QWidget* tmp_widget = new QWidget();
                QHBoxLayout* tmp_layout = new QHBoxLayout(tmp_widget);
                tmp_layout->addWidget(btn_1);
                tmp_layout->addWidget(btn_2);
                tmp_layout->setMargin(0);
                ui->tableWidget->setCellWidget(row, 4, tmp_widget);
                row++;
            }
        }
        idList.clear();
    }
}

void NewVerifyMethod::selectBizDevice(QList<QString> ids)
{
    if (ids.count() > 2) {
        QString bizIds = ids[2];
        if (programInfo.verification_documents_id == "")
            programInfo.verification_documents_id = bizIds;
        else
            programInfo.verification_documents_id = QString("%1,%2").arg(programInfo.verification_documents_id).arg(bizIds);
    }
    ui->tableWidget_2->clearContents();
    ui->tableWidget_2->setRowCount(0);
    QStringList idList;
    int rowCount = 0;
    int row = 0;
    if (programInfo.verification_documents_id != "") {
        idList = programInfo.verification_documents_id.split(",");
        for (const QString& id : idList) {
            QList<CatDeviceInfor> result = BaseCommonApi::getCatDeviceInfor(id);
            if (result.length() > 0) {
                rowCount = ui->tableWidget_2->rowCount();

                ui->tableWidget_2->insertRow(rowCount);
                QTableWidgetItem* p_check = new QTableWidgetItem();
                p_check->setCheckState(Qt::Unchecked); // 设置首列为 CheckBox 控件
                ui->tableWidget_2->setItem(row, 0, p_check);
                ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(result[0].equipment_name)); // 设置数据
                ui->tableWidget_2->setItem(row, 2, new QTableWidgetItem(result[0].model)); // 设置数据
                ui->tableWidget_2->setItem(row, 3, new QTableWidgetItem(result[0].manufacture_no)); // 设置数据

                QPushButton* btn_1 = new QPushButton();
                btn_1->setText(tr("设置命令"));
                btn_1->setStyleSheet("QPushButton{"
                                     "background-color:rgba(255,255,255,0);"
                                     "color:rgba(92,170,54,100);"
                                     "text-decoration:underline;"
                                     "}");
                btn_1->setCursor(Qt::PointingHandCursor);
                int type = 0;
                connect(btn_1, &QPushButton::clicked, this, [this, type, id]() {
                    on_cmdButton_clicked(type, id);
                });

                btn_1->setIconSize(QSize(16, 16));
                btn_1->setIcon(QIcon(":/image/Index/u2324.svg"));
                QPushButton* btn_2 = new QPushButton();
                btn_2->setText(tr("设置设备接口"));
                btn_2->setStyleSheet("QPushButton{"
                                     "background-color:rgba(255,255,255,0);"
                                     "color:rgba(92,170,54,100);"
                                     "text-decoration:underline;"
                                     "}");
                btn_2->setCursor(Qt::PointingHandCursor);
                connect(btn_2, &QPushButton::clicked, this, [this, type, id]() {
                    on_commButton_clicked(type, id);
                });
                btn_2->setIconSize(QSize(16, 16));
                btn_2->setIcon(QIcon(":/image/Index/u2324.svg"));
                QWidget* tmp_widget = new QWidget();
                QHBoxLayout* tmp_layout = new QHBoxLayout(tmp_widget);
                tmp_layout->addWidget(btn_1);
                tmp_layout->addWidget(btn_2);
                tmp_layout->setMargin(0);
                ui->tableWidget_2->setCellWidget(row, 4, tmp_widget);
                row++;
            }
        }
        idList.clear();
    }
}

void NewVerifyMethod::chooseDevices(QString bizIds, QString checkIds)
{
    programInfo.standard_device_id = bizIds;
    programInfo.verification_documents_id = checkIds;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    QStringList idList;
    int rowCount = 0;
    int row = 0;
    checkId = "";
    if (bizIds != "") {
        idList = bizIds.split(",");
        for (const QString& id : idList) {
            checkId = id;
            QList<CatDeviceInfor> result = BaseCommonApi::getCatDeviceInfor(id);
            if (result.length() > 0) {
                rowCount = ui->tableWidget->rowCount();

                ui->tableWidget->insertRow(rowCount);
                ui->tableWidget->setItem(row, 0, new QTableWidgetItem("标准设备"));
                ui->tableWidget->setItem(row, 1, new QTableWidgetItem(result[0].equipment_name)); // 设置数据
                ui->tableWidget->setItem(row, 2, new QTableWidgetItem(result[0].model)); // 设置数据
                ui->tableWidget->setItem(row, 3, new QTableWidgetItem(result[0].manufacture_no)); // 设置数据
                ui->tableWidget->setItem(row, 4, new QTableWidgetItem(result[0].manufacturer));
                QPushButton* btn_1 = new QPushButton();
                btn_1->setText(tr("设置命令"));
                btn_1->setStyleSheet("QPushButton{"
                                     "background-color:rgba(255,255,255,0);"
                                     "color:rgba(92,170,54,100);"
                                     "text-decoration:underline;"
                                     "}");
                btn_1->setCursor(Qt::PointingHandCursor);
                int type = 1;
                // connect(btn_1,SIGNAL(clicked()),this,SLOT(on_cmdButton_clicked(1,id)));
                connect(btn_1, &QPushButton::clicked, this, [this, type, id]() {
                    on_cmdButton_clicked(type, id);
                });
                btn_1->setIconSize(QSize(16, 16));
                btn_1->setIcon(QIcon(":/image/Index/u2324.svg"));
                QPushButton* btn_2 = new QPushButton();
                btn_2->setText(tr("设置设备接口"));
                btn_2->setStyleSheet("QPushButton{"
                                     "background-color:rgba(255,255,255,0);"
                                     "color:rgba(92,170,54,100);"
                                     "text-decoration:underline;"
                                     "}");
                btn_2->setCursor(Qt::PointingHandCursor);
                // connect(btn_2,SIGNAL(clicked()),this,SLOT(on_commButton_clicked(1,id)));
                connect(btn_2, &QPushButton::clicked, this, [this, type, id]() {
                    on_commButton_clicked(type, id);
                });
                btn_2->setIconSize(QSize(16, 16));
                btn_2->setIcon(QIcon(":/image/Index/u2324.svg"));
                QWidget* tmp_widget = new QWidget();
                QHBoxLayout* tmp_layout = new QHBoxLayout(tmp_widget);
                tmp_layout->addWidget(btn_1);
                tmp_layout->addWidget(btn_2);
                tmp_layout->setMargin(0);
                ui->tableWidget->setCellWidget(row, 5, tmp_widget);
                row++;
            }
        }
        idList.clear();
    }

    if (checkIds != "") {
        idList = checkIds.split(",");
        for (const QString& id : idList) {
            //            CheckAndTestedDevice result = BaseCommonApi::getCheckAndTestedDeviceById(id);
            // QList<TestedCatDevice> result = BaseCommonApi::getTestedCatDevice(id);
            //            if(result.id!="")
            //            {
            //                rowCount = ui->tableWidget->rowCount();

            //                ui->tableWidget->insertRow(rowCount);
            //                ui->tableWidget->setItem(row, 0, new QTableWidgetItem("核查件"));
            //                ui->tableWidget->setItem(row, 1, new QTableWidgetItem(result.equipment_name)); //设置数据
            //                ui->tableWidget->setItem(row, 2, new QTableWidgetItem(result.model)); //设置数据
            //                ui->tableWidget->setItem(row, 3, new QTableWidgetItem(result.manufacture_no)); //设置数据
            //                ui->tableWidget->setItem(row, 4, new QTableWidgetItem(result.manufacturer));
            //                QPushButton *btn_1 = new QPushButton();
            //                btn_1->setText(tr("设置命令"));
            //                btn_1->setStyleSheet("QPushButton{"
            //                                     "background-color:rgba(255,255,255,0);"
            //                                     "color:rgba(92,170,54,100);"
            //                                     "text-decoration:underline;"
            //                                     "}");
            //                btn_1->setCursor(Qt::PointingHandCursor);
            //                int type=0;
            //                connect(btn_1, &QPushButton::clicked, this, [this,type, id]() {
            //                    on_cmdButton_clicked(type,id);
            //                });

            //                btn_1->setIconSize(QSize(16,16));
            //                btn_1->setIcon(QIcon(":/image/Index/u2324.svg"));
            //                QPushButton *btn_2 = new QPushButton();
            //                btn_2->setText(tr("设置设备接口"));
            //                btn_2->setStyleSheet("QPushButton{"
            //                                     "background-color:rgba(255,255,255,0);"
            //                                     "color:rgba(92,170,54,100);"
            //                                     "text-decoration:underline;"
            //                                     "}");
            //                btn_2->setCursor(Qt::PointingHandCursor);
            //                connect(btn_2, &QPushButton::clicked, this, [this,type, id]() {
            //                    on_commButton_clicked(type,id);
            //                });
            //                btn_2->setIconSize(QSize(16,16));
            //                btn_2->setIcon(QIcon(":/image/Index/u2324.svg"));
            //                QWidget *tmp_widget = new QWidget();
            //                QHBoxLayout *tmp_layout = new QHBoxLayout(tmp_widget);
            //                tmp_layout->addWidget(btn_1);
            //                tmp_layout->addWidget(btn_2);
            //                tmp_layout->setMargin(0);
            //                ui->tableWidget->setCellWidget(row,5,tmp_widget);
            //                row++;
            //            }
        }
        idList.clear();
    }
}

void NewVerifyMethod::on_pushButton_11_clicked()
{
    QString sName = QFileDialog::getOpenFileName(this, "选择图片", "", "Image File(*.bmp *.jpg *.jpeg *.png)");
    // 第一个参数：标准文件对话框的父窗口；第二个参数：标准文件对话框的标题；第三个参数：指定默认的目录；第四个参数：文件过滤器
    // QImage img(sName);
    // int WindowW = this->width();
    // int WindowH = this->height();
    // ui->label->setGeometry(0, 0, WindowW, WindowH);
    // QImage scalimg = img.scaled(ui->label_11->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // ui->label_11->setPixmap(QPixmap::fromImage(scalimg));
    // ui->label_11->setPixmap(QPixmap(sName));
    QString strStyle = "QLabel{"
                       "image:url("
        + sName + ");"
                  "}";
    // qDebug() << "strStyle:" << strStyle ;
    ui->labelImage->setStyleSheet(strStyle);
    programInfo.img_path = sName;
}

void NewVerifyMethod::on_pushButton_12_clicked()
{
    ui->labelImage->setStyleSheet("");
    programInfo.img_path = "";
}

void NewVerifyMethod::on_pushButton_9_clicked()
{
    if (!bEditMode) {
        return;
    }
    if (programInfo.id == "") {
        BaseCommonApi baseApi;
        qint64 id = baseApi.generateId();
        programInfo.id = QString::number(id);
        programInfo.check_name = ui->lineEditName->text();
        int idx = ui->comboBoxEquipment->currentIndex();
        programInfo.belong_standard_equipment = equipIdList[idx];
        programInfo.belong_standard_equipment_name = equipNameList[idx];
        idx = ui->comboBoxCategory->currentIndex();
        if (idx >= 0 && idx < catList.length()) {
            programInfo.equipment_type = catList[idx].id;
            programInfo.standard_id = loginNowVerifyDeviceId;
            programInfo.category_name = catList[idx].category_name;
        }
        programInfo.create_name = ui->lineEditUser->text();
        programInfo.create_time = ui->dateEditTime->dateTime();
        programInfo.remark = ui->lineEditMark->text();
        // qDebug() << "programInfo.id" << programInfo.id ;
        if (BaseCommonApi::InsertCheckProgramManage(programInfo)) {
            verifyManageWnd->updateTable(true, programInfo);
            this->close();
        } else {
            QMessageBox::warning(this, "警告", "保存失败！");
        }
    } else {
        programInfo.check_name = ui->lineEditName->text();
        int idx = ui->comboBoxEquipment->currentIndex();
        programInfo.belong_standard_equipment = equipIdList[idx];
        programInfo.belong_standard_equipment_name = equipNameList[idx];
        idx = ui->comboBoxCategory->currentIndex();
        if (idx >= 0 && idx < catList.length()) {
            programInfo.equipment_type = catList[idx].id;
            programInfo.category_name = catList[idx].category_name;
        }
        programInfo.create_name = ui->lineEditUser->text();
        programInfo.create_time = ui->dateEditTime->dateTime();
        programInfo.update_time = QDateTime::currentDateTime();
        programInfo.remark = ui->lineEditMark->text();
        if (BaseCommonApi::UpdateCheckProgramManage(programInfo)) {
            verifyManageWnd->updateTable(false, programInfo);
            this->close();
        } else {
            QMessageBox::warning(this, "警告", "保存失败！");
        }
    }
}

void NewVerifyMethod::on_pushButton_10_clicked()
{
    this->close();
}

void NewVerifyMethod::on_comboBoxEquipment_currentIndexChanged(int index)
{
    QString id = equipIdList[index];
    catList.clear();
    ui->comboBoxCategory->clear();
    catList = BaseCommonApi::getStandardCheckItemCategory(id);
    for (const StandardCheckItemCategory& item : catList) {
        ui->comboBoxCategory->addItem(item.category_name);
    }
}

void NewVerifyMethod::on_pushButton_clicked()
{
    FormBizDeviceSelect* checkWindow = new FormBizDeviceSelect;
    // checkWindow->newVerifyWnd=this;
    checkWindow->setWindowState(Qt::WindowMaximized);
    connect(checkWindow, SIGNAL(selectBizDevice(QList<QString>)), this, SLOT(selectBizDevice(QList<QString>)));
    checkWindow->setWindowModality(Qt::ApplicationModal);
    checkWindow->show();
}

void NewVerifyMethod::on_pushButton_3_clicked()
{
    int cnt = 0;
    for (int i = ui->tableWidget->rowCount() - 1; i >= 0; i--) {
        if (ui->tableWidget->item(i, 0)->checkState() == Qt::Checked) {
            cnt = 1;
            break;
        }
    }
    if (cnt > 0 && QMessageBox::question(this, "确认", "确认删除吗？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes) {
        QStringList idList = programInfo.standard_device_id.split(",");
        for (int i = ui->tableWidget->rowCount() - 1; i >= 0; i--) {
            if (ui->tableWidget->item(i, 0)->checkState() == Qt::Checked) {
                ui->tableWidget->removeRow(i);
                idList.removeAt(i);
            }
        }
        programInfo.standard_device_id = idList.join(",");
        idList.clear();
    }
}

void NewVerifyMethod::on_pushButton_2_clicked()
{
    int cnt = 0;
    for (int i = ui->tableWidget_2->rowCount() - 1; i >= 0; i--) {
        if (ui->tableWidget_2->item(i, 0)->checkState() == Qt::Checked) {
            cnt = 1;
            break;
        }
    }
    if (cnt > 0 && QMessageBox::question(this, "确认", "确认删除吗？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes) {
        QStringList idList = programInfo.verification_documents_id.split(",");
        for (int i = ui->tableWidget_2->rowCount() - 1; i >= 0; i--) {
            if (ui->tableWidget_2->item(i, 0)->checkState() == Qt::Checked) {
                ui->tableWidget_2->removeRow(i);
                idList.removeAt(i);
            }
        }
        programInfo.verification_documents_id = idList.join(",");
        idList.clear();
    }
}
