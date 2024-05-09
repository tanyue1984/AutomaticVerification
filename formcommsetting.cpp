#include "formcommsetting.h"
#include "ui_formcommsetting.h"
#include "visacommonengine.h"

FormCommSetting::FormCommSetting(QWidget *parent,int type,QString id) :
    QWidget(parent),
    deviceId(id),
    ui(new Ui::FormCommSetting)
{
    ui->setupUi(this);

    VisaCommonEngine engine;
    QStringList visaAddrs = engine.getVisaAddrs();
    ui->comboBox_9->addItems(visaAddrs);
    visaAddrs.clear();
    QStringList serialAddrs = engine.getSerialAddrs();
    ui->comboBox->addItems(serialAddrs);
    serialAddrs.clear();
    if(deviceId!=""){
        addrList = BaseCommonApi::getVerificationProgramConfigInterface(deviceId);
        if(addrList.length()>0){
            VerificationProgramConfigInterface addr = addrList[0];
            if(addr.communication_type == "SerialPort")
                ui->radioButton->setChecked(true);
            else
                ui->radioButton_2->setChecked(true);
            ui->comboBox_9->setCurrentText(addr.visa);
            if(addr.serial_port!=""){
                QStringList sList = addr.serial_port.split(";");
                if(sList.length()==5){
                    ui->comboBox->setCurrentText(sList[0]);
                    ui->comboBox_3->setCurrentText(sList[1]);
                    ui->comboBox_4->setCurrentText(sList[2]);
                    ui->comboBox_7->setCurrentText(sList[3]);
                    ui->comboBox_8->setCurrentText(sList[4]);
                }
            }
        }
    }
}

FormCommSetting::~FormCommSetting()
{
    delete ui;
}

void FormCommSetting::on_pushButton_14_clicked()
{
    if(addrList.length()>0){
        VerificationProgramConfigInterface addr = addrList[0];
        if(ui->radioButton->isChecked())
            addr.communication_type = "SerialPort";
        else
            addr.communication_type = "Visa";
        addr.serial_port = QString("%1;%2;%3;%4;%5").arg(ui->comboBox->currentText()).arg(ui->comboBox_3->currentText()).arg(ui->comboBox_4->currentText())
                .arg(ui->comboBox_7->currentText()).arg(ui->comboBox_8->currentText());
        addr.visa = ui->comboBox_9->currentText();
        BaseCommonApi::UpdateVerificationProgramConfigInterface(addr);
    }else{
        VerificationProgramConfigInterface addr;
        BaseCommonApi baseApi;
        qint64 id = baseApi.generateId();
        addr.id = QString::number(id);
        addr.device_id = deviceId;
        if(ui->radioButton->isChecked())
            addr.communication_type = "SerialPort";
        else
            addr.communication_type = "Visa";
        addr.serial_port = QString("%1;%2;%3;%4;%5").arg(ui->comboBox->currentText()).arg(ui->comboBox_3->currentText()).arg(ui->comboBox_4->currentText())
                .arg(ui->comboBox_7->currentText()).arg(ui->comboBox_8->currentText());
        addr.visa = ui->comboBox_9->currentText();
        BaseCommonApi::InsertVerificationProgramConfigInterface(addr);
    }
    this->close();
}

void FormCommSetting::on_radioButton_toggled(bool checked)
{
    ui->radioButton_2->setChecked(!ui->radioButton->isChecked());
}

void FormCommSetting::on_radioButton_2_toggled(bool checked)
{
     ui->radioButton->setChecked(!ui->radioButton_2->isChecked());
}
