#include "newcheckmethod.h"
#include "ui_newcheckmethod.h"
#include "checkdevice.h"
#include "qfiledialog.h"
#include "formcheckpoint.h"
#include <QMessageBox>
#include <QDebug>
#include "formcommsetting.h"
#include "formaddrsetting.h"
#include "formbizdeviceselect.h"
#include "global.h"

NewCheckMethod::NewCheckMethod(QWidget *parent,QString sId,bool bEdit) :
    QWidget(parent),
    methodId(sId),
    bEditMode(bEdit),
    ui(new Ui::NewCheckMethod)
{
    ui->setupUi(this);
    ui->tableWidget->verticalHeader()->setParent(nullptr);
    for(int i=1;i<7;i++)
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Stretch);
    ui->pushButton_3->setEnabled(bEditMode);
    ui->comboBoxEquipment->addItems(equipNameList);
    ui->dateEditTime->setDate(QDate::currentDate());
    getMethodInfo();
}


NewCheckMethod::~NewCheckMethod()
{
    delete ui;
}

void NewCheckMethod::getMethodInfo()
{
    QList<QString> list;
    if(methodId!="")
    {
        programInfo = BaseCommonApi::getVerificationProgramInfo(methodId);
        ui->comboBoxEquipment->setCurrentIndex(equipIdList.indexOf(programInfo.belong_standard_equipment));

        for (int i = 0; i < catList.length(); ++i) {
            if (catList[i].id == programInfo.equipment_item_category) {
                ui->comboBoxCategory->setCurrentIndex(i); // 找到匹配项，返回 true
                break;
            }
        }
        ui->lineEdit_2->setText(programInfo.equipment_name );
        ui->lineEdit_3->setText(programInfo.model);
        ui->lineEdit_4->setText(programInfo.help_instruction);
        ui->lineEditName->setText(programInfo.program_name);
        ui->lineEditUser->setText(programInfo.create_name);
        ui->dateEditTime->setDateTime(programInfo.create_time);
        ui->lineEditMark->setText(programInfo.remark);
        if(programInfo.img_path!="")
        {
            //QImage img(programInfo.img_path);
            //QImage scalimg = img.scaled(ui->label_11->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            //ui->label_11->setPixmap(QPixmap::fromImage(scalimg));
            QString strStyle = "QLabel{"
                               "image:url("+programInfo.img_path+");"
                               "}";
            qDebug() << "strStyle:" << strStyle ;
            ui->label_11->setStyleSheet(strStyle);
        }
        chooseDevices(list);
    }else{
        BaseCommonApi baseApi;
        qint64 id = baseApi.generateId();
        methodId = QString::number(id);
    }
}

void NewCheckMethod::showEvent(QShowEvent *event)
{
}

void NewCheckMethod::showCheckDevice()
{
//    CheckDevice *checkWindow = new CheckDevice;
//    checkWindow->newCheckWnd = this;
//    checkWindow->setWindowState(Qt::WindowMaximized);
//    checkWindow->setWindowModality(Qt::ApplicationModal);
//    checkWindow->show();
    FormBizDeviceSelect *checkWindow = new FormBizDeviceSelect;
    //checkWindow->newVerifyWnd=this;
    checkWindow->setWindowState(Qt::WindowMaximized);
    connect(checkWindow, SIGNAL(selectBizDevice(QList<QString>)), this,SLOT(chooseDevices(QList<QString>)));
    checkWindow->setWindowModality(Qt::ApplicationModal);
    checkWindow->show();
}

void NewCheckMethod::choosePicture()
{
    QString sName = QFileDialog::getOpenFileName(this,"选择图片","","Image File(*.bmp *.jpg *.jpeg *.png)");
    //第一个参数：标准文件对话框的父窗口；第二个参数：标准文件对话框的标题；第三个参数：指定默认的目录；第四个参数：文件过滤器
    //QImage img(sName);
    //int WindowW = this->width();
    //int WindowH = this->height();
    //ui->label->setGeometry(0, 0, WindowW, WindowH);
    //QImage scalimg = img.scaled(ui->label_11->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //ui->label_11->setPixmap(QPixmap::fromImage(scalimg));
    //ui->label_11->setPixmap(QPixmap(sName));
    QString strStyle = "QLabel{"
                       "image:url("+sName+");"
                       "}";
    //qDebug() << "strStyle:" << strStyle ;
    ui->label_11->setStyleSheet(strStyle);
    programInfo.img_path = sName;
}

void NewCheckMethod::deletePicture()
{
    ui->label_11->setStyleSheet("");
    programInfo.img_path = "";
}

void NewCheckMethod::showCheckPoint()
{
//    FormCheckPoint *checkWindow = new FormCheckPoint(nullptr,checkId);
//    //checkWindow.setWindowState(Qt::WindowMaximized);
//    checkWindow->setWindowModality(Qt::ApplicationModal);
//    checkWindow->show();
    QString equipId =equipIdList[ui->comboBoxEquipment->currentIndex()];
    QString catId;
    int idx = ui->comboBoxCategory->currentIndex();
    if(idx>=0&&idx<catList.length())
        catId = catList[idx].id;
    FormAddrSetting *form = new FormAddrSetting(nullptr,0,methodId,checkId,true,equipId,catId);
    form->setWindowModality(Qt::ApplicationModal);
    form->show();
}

void NewCheckMethod::on_pushButton_3_clicked()
{
    if(!bEditMode)
        return;
    if(programInfo.id=="")
    {
        //BaseCommonApi baseApi;
        //qint64 id = baseApi.generateId();
        programInfo.id = methodId;
        programInfo.belong_standard_equipment = equipIdList[ui->comboBoxEquipment->currentIndex()];
        int idx = ui->comboBoxCategory->currentIndex();
        if(idx>=0&&idx<catList.length())
            programInfo.equipment_item_category = catList[idx].id;
        programInfo.equipment_name = ui->lineEdit_2->text();
        programInfo.model = ui->lineEdit_3->text();
        programInfo.help_instruction = ui->lineEdit_4->text();
        programInfo.program_name = ui->lineEditName->text();
        programInfo.create_name = ui->lineEditUser->text();
        programInfo.create_time = ui->dateEditTime->dateTime();
        programInfo.remark = ui->lineEditMark->text();
        //qDebug() << "programInfo.id" << programInfo.id ;
        if(BaseCommonApi::InsertVerificationProgramInfo(programInfo))
        {
            checkManageWnd->updateTable(true,programInfo);
            this->close();
        }else{
            QMessageBox::warning(this, "警告", "保存失败！");
        }
    }else{
        programInfo.belong_standard_equipment = equipIdList[ui->comboBoxEquipment->currentIndex()];
        int idx = ui->comboBoxCategory->currentIndex();
        if(idx>=0&&idx<catList.length())
            programInfo.equipment_item_category = catList[idx].id;
        programInfo.equipment_name = ui->lineEdit_2->text();
        programInfo.model = ui->lineEdit_3->text();
        programInfo.help_instruction = ui->lineEdit_4->text();
        programInfo.program_name = ui->lineEditName->text();
        programInfo.create_name = ui->lineEditUser->text();
        programInfo.create_time = ui->dateEditTime->dateTime();
        programInfo.update_time = QDateTime::currentDateTime();
        programInfo.remark = ui->lineEditMark->text();
        if(BaseCommonApi::UpdateVerificationProgramInfo(programInfo))
        {
            checkManageWnd->updateTable(false,programInfo);
            this->close();
        }else{
            QMessageBox::warning(this, "警告", "保存失败！");
        }
    }
}

void NewCheckMethod::on_pushButton_4_clicked()
{
    this->close();
}
void NewCheckMethod::on_cmdButton_clicked(int type,QString id)
{
    //qDebug() << "cmd click:" << type << id ;
    QString equipId = equipIdList[ui->comboBoxEquipment->currentIndex()];
    QString catId;
    int idx = ui->comboBoxCategory->currentIndex();
    if(idx>=0&&idx<catList.length())
        catId = catList[idx].id;
    FormAddrSetting *form = new FormAddrSetting(nullptr,type,id,checkId,true,equipId,catId);
    form->setWindowModality(Qt::ApplicationModal);
    form->show();
}

void NewCheckMethod::on_commButton_clicked(int type,QString id)
{
    //qDebug() << "comm click:" << type << id ;
    FormCommSetting *form = new FormCommSetting(nullptr,type,id);
    form->setWindowModality(Qt::ApplicationModal);
    form->show();
}

void NewCheckMethod::chooseDevices(QList<QString> biziInfo)
{
    if(biziInfo.count()>2){
        QString bizIds=biziInfo[2];
        //programInfo.tested_device_id=checkIds;
        if(bizIds!=""){
            if(programInfo.standard_device_id=="")
                programInfo.standard_device_id = bizIds;
            else
                programInfo.standard_device_id =QString("%1,%2").arg(programInfo.standard_device_id).arg(bizIds);
        }
        if(biziInfo[0]!=""){
            if(programInfo. biz_standard_id=="")
                programInfo. biz_standard_id=biziInfo[0];
            else
                programInfo. biz_standard_id=QString("%1,%2").arg(programInfo.biz_standard_id).arg(biziInfo[0]);
        }
        if(biziInfo[1]!=""){
            if(programInfo.biz_standard_name=="")
                programInfo.biz_standard_name=biziInfo[1];
            else
                programInfo.biz_standard_name =QString("%1,%2").arg(programInfo.biz_standard_name).arg(biziInfo[1]);
        }
    }

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    QStringList idList;
    int rowCount = 0;
    int row=0;
    checkId="";
//    if(checkIds!="")
//    {
//        idList = checkIds.split(",");
//        for(const QString& id : idList)
//        {
//            checkId = id;
//            QList<TestedCatDevice> result = BaseCommonApi::getTestedCatDevice(id);
//            if(result.length()>0){
//                rowCount = ui->tableWidget->rowCount();

//                ui->tableWidget->insertRow(rowCount);
//                ui->tableWidget->setItem(row, 0, new QTableWidgetItem("被检设备"));
//                ui->tableWidget->setItem(row, 1, new QTableWidgetItem(result[0].equipment_name)); //设置数据
//                ui->tableWidget->setItem(row, 2, new QTableWidgetItem(result[0].model)); //设置数据
//                ui->tableWidget->setItem(row, 3, new QTableWidgetItem(result[0].help_instruction)); //设置数据
//                ui->tableWidget->setItem(row, 4, new QTableWidgetItem(result[0].manufacture_no)); //设置数据
//                ui->tableWidget->setItem(row, 5, new QTableWidgetItem(result[0].manufacturer));
//                QPushButton *btn_1 = new QPushButton();
//                btn_1->setText(tr("设置命令"));
//                btn_1->setStyleSheet("QPushButton{"
//                                     "background-color:rgba(255,255,255,0);"
//                                     "color:rgba(92,170,54,100);"
//                                     "text-decoration:underline;"
//                                     "}");
//                btn_1->setCursor(Qt::PointingHandCursor);
//                int type=0;
//                //connect(btn_1,SIGNAL(clicked()),this,SLOT(on_cmdButton_clicked(1,id)));
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
//                //connect(btn_2,SIGNAL(clicked()),this,SLOT(on_commButton_clicked(1,id)));
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
//                ui->tableWidget->setCellWidget(row,6,tmp_widget);
//                row++;
//            }
//        }
//        idList.clear();
//    }

    if(programInfo.standard_device_id!="")
    {
        idList = programInfo.standard_device_id.split(",");
        for(const QString& id : idList)
        {
            QList<CatDeviceInfor> result = BaseCommonApi::getCatDeviceInfor(id);
            if(result.length()>0){
                rowCount = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(rowCount);
                QTableWidgetItem* p_check = new QTableWidgetItem();
                p_check->setCheckState(Qt::Unchecked);        //设置首列为 CheckBox 控件
                ui->tableWidget->setItem(row,0,p_check);
                ui->tableWidget->setItem(row, 1, new QTableWidgetItem(result[0].equipment_name)); //设置数据
                ui->tableWidget->setItem(row, 2, new QTableWidgetItem(result[0].model)); //设置数据
                ui->tableWidget->setItem(row, 3, new QTableWidgetItem(result[0].manufacture_no)); //设置数据
                ui->tableWidget->setItem(row, 4, new QTableWidgetItem(result[0].manufacturer));
                ui->tableWidget->setItem(row, 5, new QTableWidgetItem(result[0].type));
                QPushButton *btn_1 = new QPushButton();
                btn_1->setText(tr("设置命令"));
                btn_1->setStyleSheet("QPushButton{"
                                     "background-color:rgba(255,255,255,0);"
                                     "color:rgba(92,170,54,100);"
                                     "text-decoration:underline;"
                                     "}");
                btn_1->setCursor(Qt::PointingHandCursor);
                int type=1;
                connect(btn_1, &QPushButton::clicked, this, [this,type, id]() {
                    on_cmdButton_clicked(type,id);
                });

                btn_1->setIconSize(QSize(16,16));
                btn_1->setIcon(QIcon(":/image/Index/u2324.svg"));
                QPushButton *btn_2 = new QPushButton();
                btn_2->setText(tr("设置设备接口"));
                btn_2->setStyleSheet("QPushButton{"
                                     "background-color:rgba(255,255,255,0);"
                                     "color:rgba(92,170,54,100);"
                                     "text-decoration:underline;"
                                     "}");
                btn_2->setCursor(Qt::PointingHandCursor);
                connect(btn_2, &QPushButton::clicked, this, [this,type, id]() {
                    on_commButton_clicked(type,id);
                });
                btn_2->setIconSize(QSize(16,16));
                btn_2->setIcon(QIcon(":/image/Index/u2324.svg"));
                QWidget *tmp_widget = new QWidget();
                QHBoxLayout *tmp_layout = new QHBoxLayout(tmp_widget);
                tmp_layout->addWidget(btn_1);
                tmp_layout->addWidget(btn_2);
                tmp_layout->setMargin(0);
                ui->tableWidget->setCellWidget(row,6,tmp_widget);
                row++;
            }
        }
        idList.clear();
    }
}

void NewCheckMethod::on_comboBoxEquipment_currentIndexChanged(int index)
{
    QString id = equipIdList[index];
    catList.clear();
    ui->comboBoxCategory->clear();
    catList = BaseCommonApi::getitemCategoryList(id);
    for(const itemCategory& item : catList){
        ui->comboBoxCategory->addItem(item.category_name);
    }
    setMethodName();
}

void NewCheckMethod::on_comboBoxCategory_currentIndexChanged(int index)
{
    setMethodName();
}

void NewCheckMethod::on_lineEdit_2_textChanged(const QString &arg1)
{
    setMethodName();
}

void NewCheckMethod::on_lineEdit_3_textChanged(const QString &arg1)
{
    setMethodName();
}

void NewCheckMethod::setMethodName(){
    QString sName = ui->comboBoxEquipment->currentText()+"-"+ui->comboBoxCategory->currentText();
    if(ui->lineEdit_2->text()!=""){
        sName+="-"+ui->lineEdit_2->text();
    }
    if(ui->lineEdit_3->text()!=""){
        sName+="-"+ui->lineEdit_3->text();
    }
    ui->lineEditName->setText(sName);
}

void NewCheckMethod::on_pushButton_clicked()
{
    int cnt=0;
    for(int i=ui->tableWidget->rowCount()-1;i>=0;i--){
        if(ui->tableWidget->item(i,0)->checkState()==Qt::Checked){
            cnt=1;
            break;
        }
    }
    if(cnt>0&&QMessageBox::question(this, "确认", "确认删除吗？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes)==QMessageBox::Yes)
    {
        QStringList idList = programInfo.standard_device_id.split(",");
        QStringList StanidList = programInfo.biz_standard_id.split(",");
        QStringList StanNameList = programInfo.biz_standard_name.split(",");
        for(int i=ui->tableWidget->rowCount()-1;i>=0;i--){
            if(ui->tableWidget->item(i,0)->checkState()==Qt::Checked){
                ui->tableWidget->removeRow(i);
                idList.removeAt(i);
                StanidList.removeAt(i);
                StanNameList.removeAt(i);
            }
        }
        programInfo.standard_device_id = idList.join(",");
        programInfo.biz_standard_id=StanidList.join(",");
        programInfo.biz_standard_name=StanNameList.join(",");
        idList.clear();
    }
}
