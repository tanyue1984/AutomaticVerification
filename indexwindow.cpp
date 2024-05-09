#include "indexwindow.h"
#include "QFile"
#include "QMessageBox"
#include "QTextStream"
#include "SequentialMap.h"
#include "checkmethodmanage.h"
#include "checkwindow.h"
#include "ui_indexwindow.h"
#include "verifymethodmanage.h"
#include "verifywindow.h"
// QMessageBox
// 数据连接类
#include "basecommonapi.h"
#include "databaseconnection.h"

IndexWindow::IndexWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::IndexWindow)
{
    ui->setupUi(this);
    this->initForm();
    /*
           //主要作用 当页面关闭后 回到indexwindow页面 并 通过~IndexWindow 释放此页面指针所占用内存
           // 假设 ShowDataSyncView 是一个指向 DataSyncView 实例的指针
           // 当 ShowDataSyncView 关闭时，它的 closed() 信号会被发出
           // 连接这个信号到 IndexWindow 的 onNewWindowClosed() 槽函数
       if(ShowDataSyncView) {
           //数据同步页面 信号槽
           connect(ShowDataSyncView, &DataSyncView::close, this, &IndexWindow::onNewWindowClosed);
       }
       if(ShowInfoManageView) {
            //信息管理 信号槽
           connect(ShowInfoManageView, &InfoManageView::close, this, &IndexWindow::onNewWindowClosed);
       }
       */
}

IndexWindow::~IndexWindow()
{
    delete ui;
    // 释放页面内存
    /*
    if (ShowDataSyncView) {
         ShowDataSyncView->close();
         delete ShowDataSyncView;
    }
    else if(ShowInfoManageView)
    {
        ShowInfoManageView->close();
        delete ShowInfoManageView;
    }
    */
}
void IndexWindow::initForm()
{
    // this->setWindowFlags(Qt::FramelessWindowHint);
    // 调用样式
    loadStyle(":/qss/Index.css");

    SequentialMap<int, int> map1;
    SequentialMap<int, int>* map;
    map = &map1;
    map->insert(4, 6);
    map->insert(4, 2);
    map->insert(4, 1);
    map->insert(4, 2);
    map->insert(5, 5);
    map->insert(6, 3);
    map->insert(1, 3);
    map->insert(2, 2);
    map->insert(3, 1);

    for (int item : map->keys()) {
        qDebug() << QString::number(map->value(item));
        qDebug() << QString::number(map->value(7));
    }

    //    DatabaseConnection *Conn=DatabaseConnection::getInstance();
    //    //数据库连接  目前先放这里 后续需要放到登录界面
    //    bool IsOpen=Conn->open();
    //    if(IsOpen)
    //        ShowMessageBox("数据库连接","连接成功");
    //    else
    //        ShowMessageBox("数据库连接","连接失败");
    QMap<QString, QMap<QString, QList<QStringList>>> a;
    QMap<QString, QMap<QString, QStringList>> headList;
    //    a = BaseCommonApi::getStandardCheckItemDataSignalGenerator("1752496970477416449",&headList);
    //    a = BaseCommonApi::getStandardCheckItemDataCalibrator("1692808528303013889",&headList);
    //       a  = BaseCommonApi::getStandardCheckItemDataDcPower("1693435753272557570",&headList);
    //        qDebug()<<">"<<a;
    //    QMap<QString,QMap<QString, QStringList>> headList;
    //    BaseCommonApi::getStandardCheckItemDataCalibrator("1748244089305845762",&headList);
    //    BaseCommonApi::getStandardCheckItemDataResistanceThermometer("1692807873677017090",&headList);
}

void IndexWindow::loadStyle(const QString& qssFile)
{
    // 加载样式表
    QString qss;

    QFile file(qssFile);

    if (file.open(QFile::ReadOnly)) {
        // 用QTextStream读取样式文件不用区分文件编码 带bom也行
        QStringList list;
        QTextStream in(&file);
        // in.setCodec("utf-8");
        while (!in.atEnd()) {
            QString line;
            in >> line;
            list << line;
        }

        file.close();
        qss = list.join("\n");
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(paletteColor));
        // 用时主要在下面这句
        qApp->setStyleSheet(qss);
    }
}

void IndexWindow::ShowMessageBox(QString title, QString value)
{
    // 创建一个QMessageBox对象
    QMessageBox msgBox;

    // 设置消息框的标题
    msgBox.setWindowTitle(title);

    // 设置消息框的文本内容
    msgBox.setText(value);

    // 设置消息框的标准按钮，这里是“确定”按钮
    msgBox.setStandardButtons(QMessageBox::Ok);

    // 设置样式表以改变文本颜色
    QString styleSheet = "QLabel { color: black; }"; // 这里将文本颜色设置为红色
    msgBox.setStyleSheet(styleSheet);
    // 显示消息框
    msgBox.exec();
}
void IndexWindow::onNewWindowClosed()
{
    // 新窗体关闭后，设置 IndexWindow 为活动窗口
    this->activateWindow();
    // 也可以设置焦点到 MainWindow 中的某个控件
    // ui->someWidget->setFocus();
}

void IndexWindow::showCheckWindow()
{
    CheckWindow* checkWindow = new CheckWindow;
    checkWindow->setWindowState(Qt::WindowMaximized);
    checkWindow->setWindowModality(Qt::ApplicationModal);
    checkWindow->show();
}

// 信息管理按钮
void IndexWindow::on_tb3_clicked()
{
    InfoManageView* ShowInfoManageView = new InfoManageView();
    ShowInfoManageView->setWindowState(Qt::WindowMaximized);
    ShowInfoManageView->setWindowModality(Qt::ApplicationModal);
    ShowInfoManageView->show();
}
// 数据同步按钮
void IndexWindow::on_tb6_clicked()
{

    // 创建新窗体实例并显示
    DataSyncView* ShowDataSyncView = new DataSyncView();
    ShowDataSyncView->setWindowState(Qt::WindowMaximized);
    ShowDataSyncView->setWindowModality(Qt::ApplicationModal);
    ShowDataSyncView->show();
}

void IndexWindow::showCheckMethod()
{
    CheckMethodManage* checkWindow = new CheckMethodManage;
    checkWindow->setWindowState(Qt::WindowMaximized);
    checkWindow->setWindowModality(Qt::ApplicationModal);
    checkWindow->show();
}

void IndexWindow::showVerifyWindow()
{
    VerifyWindow* checkWindow = new VerifyWindow;
    checkWindow->setWindowState(Qt::WindowMaximized);
    checkWindow->setWindowModality(Qt::ApplicationModal);
    checkWindow->show();
}

void IndexWindow::showVerifyMethod()
{
    VerifyMethodManage* checkWindow = new VerifyMethodManage;
    checkWindow->setWindowState(Qt::WindowMaximized);
    checkWindow->setWindowModality(Qt::ApplicationModal);
    checkWindow->show();
}
