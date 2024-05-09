#include "dialogdcstabinput.h"
#include "ui_dialogdcstabinput.h"

DialogDCStabInput::DialogDCStabInput(QWidget *parent,QString label,QString sTips) :
    QDialog(parent),
    sLabel(label),
    ui(new Ui::DialogDCStabInput)
{
    ui->setupUi(this);
    setWindowTitle(label);
    ui->labelTips->setText(sTips);
}

DialogDCStabInput::~DialogDCStabInput()
{
    delete ui;
}

void DialogDCStabInput::on_pushButtonRead_clicked()
{
    //ui->doubleSpinBox->setValue(1);
    emit readData(sLabel);
}

void DialogDCStabInput::on_pushButtonOK_clicked()
{
    this->accept();
}

void DialogDCStabInput::on_pushButtonCancel_clicked()
{
    this->reject();
}

double DialogDCStabInput::getInputData(){
    return ui->doubleSpinBox->value();
}

double DialogDCStabInput::setInputData(double dVal)
{
      ui->doubleSpinBox->setValue(dVal);
      return  0;
}

void DialogDCStabInput::on_pushButtonSend_clicked()
{
    emit sendData(sLabel);
}
