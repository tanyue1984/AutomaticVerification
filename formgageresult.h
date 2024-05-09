#ifndef FORMGAGERESULT_H
#define FORMGAGERESULT_H

#include <QWidget>

namespace Ui {
class FormGageResult;
}

class FormGageResult : public QWidget
{
    Q_OBJECT

public:
    explicit FormGageResult(QWidget *parent = nullptr,QString deviceId="");
    ~FormGageResult();

private:
    Ui::FormGageResult *ui;
    QString sDeviceId;
    //QMap<QString, QList<QStringList>> caliDataList;
    //QMap<QString, QStringList> headTableList;
};

#endif // FORMGAGERESULT_H
