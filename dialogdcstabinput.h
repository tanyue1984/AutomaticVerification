#ifndef DIALOGDCSTABINPUT_H
#define DIALOGDCSTABINPUT_H

#include <QDialog>

namespace Ui {
class DialogDCStabInput;
}

class DialogDCStabInput : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDCStabInput(QWidget *parent = nullptr,QString label="",QString sTips="");
    ~DialogDCStabInput();

   double getInputData();
   double setInputData(double dVal);

signals:
    void readData(QString label);
    void sendData(QString label);

private slots:
    void on_pushButtonRead_clicked();

    void on_pushButtonOK_clicked();

    void on_pushButtonCancel_clicked();

    void on_pushButtonSend_clicked();

private:
    Ui::DialogDCStabInput *ui;
    QString sLabel="";
public:
    QStringList* dataRes;
};

#endif // DIALOGDCSTABINPUT_H
