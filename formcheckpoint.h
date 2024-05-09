#ifndef FORMCHECKPOINT_H
#define FORMCHECKPOINT_H

#include <QWidget>

namespace Ui {
class FormCheckPoint;
}

class FormCheckPoint : public QWidget
{
    Q_OBJECT

public:
    explicit FormCheckPoint(QWidget *parent = nullptr,QString sId="");
    ~FormCheckPoint();
private slots:
    void on_pushButton_14_clicked();

private:
    void updateTable(QString id);
private:
    Ui::FormCheckPoint *ui;
    QString checkId="";
};

#endif // FORMCHECKPOINT_H
