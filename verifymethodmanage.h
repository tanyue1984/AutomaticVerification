#ifndef VERIFYMETHODMANAGE_H
#define VERIFYMETHODMANAGE_H

#include <QWidget>
#include "basecommonapi.h"

namespace Ui {
class VerifyMethodManage;
}

class VerifyMethodManage : public QWidget
{
    Q_OBJECT

public:
    explicit VerifyMethodManage(QWidget *parent = nullptr);
    ~VerifyMethodManage();
    void updateTable(bool insert,CheckProgramManage program);
private:
    void getVerifyMethod();
    void updateRow(int row,CheckProgramManage program);
private slots:
    void showNewMethod();
    void on_editButton_clicked();
    void on_deleteButton_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();
    void onpageChanged(int currPage);

private:
    Ui::VerifyMethodManage *ui;
    QList<CheckProgramManage> programList;
    int editIdx=-1;
    int page;
    int totalcount;
    int pageIndex;
};

#endif // VERIFYMETHODMANAGE_H
