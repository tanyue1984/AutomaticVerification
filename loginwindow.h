#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>



namespace Ui {
class loginWindow;
}

class loginWindow : public QMainWindow
{
    Q_OBJECT

public:
   explicit loginWindow(QWidget *parent = 0);
    ~loginWindow();
private slots:
    void initForm();
    void loadStyle(const QString &qssFile);
    void on_Button_login_clicked();


private:
    Ui::loginWindow *ui;
private:
    QSqlQuery query;
};
#endif // MAINWINDOW_H
