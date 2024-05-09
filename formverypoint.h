#ifndef FORMVERYPOINT_H
#define FORMVERYPOINT_H

#include <QWidget>

namespace Ui {
class FormVeryPoint;
}

class FormVeryPoint : public QWidget
{
    Q_OBJECT

public:
    explicit FormVeryPoint(QWidget *parent = nullptr,QString sId="");
    ~FormVeryPoint();
private slots:
    void on_pushButton_14_clicked();

private:
    void updateTable(QString id);
private:
    Ui::FormVeryPoint *ui;
    QString checkId="";
};

#endif // FORMVERYPOINT_H
