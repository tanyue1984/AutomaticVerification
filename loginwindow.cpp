#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include "indexwindow.h"

loginWindow::loginWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::loginWindow)
{
    ui->setupUi(this);
    this->initForm();
    ui->label->styleSheet();
}

loginWindow::~loginWindow()
{
    delete ui;
}
void loginWindow::initForm()
{
    // this->setWindowFlags(Qt::FramelessWindowHint);
    // 调用样式
    setWindowTitle("v5-7-4");
    loadStyle(":/qss/login.css");
    //    ui->lineEdit_pwd->setStyleSheet("QLineEdit { border-bottom: 2px solid gray; font-size: 26px;}");
    ui->lineEdit_pwd->setPlaceholderText("请输入密码");
    ui->lineEdit_pwd->setEchoMode(QLineEdit::Password);
}

void loginWindow::loadStyle(const QString& qssFile)
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

void loginWindow::on_Button_login_clicked()
{

    DatabaseConnection* Conn = DatabaseConnection::getInstance();
    // 数据库连接  目前先放这里 后续需要放到登录界面
    Conn->open();
    // 查询数据库内是否有账号
    QString account = ui->textEdit_acc->toPlainText();
    QString password = ui->lineEdit_pwd->text();
    bool isSuccess = false;
    if (!(account.isEmpty() || password.isEmpty())) {
        isSuccess = BaseCommonApi::SysUserLogin(account, password);
        if (isSuccess) {
            QMessageBox::information(this, "提示", "登录成功！");
            IndexWindow* indexWindow = new IndexWindow;
            indexWindow->setWindowState(Qt::WindowMaximized);
            indexWindow->show();
            this->close();
        } else {
            QMessageBox::critical(this, "提示", "账号或密码错误，请重新输入！");
            //             QMessageBox::information(this, "提示", "登录成功！");
            //             IndexWindow *indexWindow = new IndexWindow;
            //             indexWindow->setWindowState(Qt::WindowMaximized);
            //             indexWindow->show();
            //             this->close();
            ui->textEdit_acc->clear();
            ui->lineEdit_pwd->clear();
            ui->textEdit_acc->setFocus();
        }
    } else {
        QMessageBox::critical(this, "提示", "账号或密码为空！");
    }
}
