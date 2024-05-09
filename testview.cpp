#include "testview.h"
#include "ui_testview.h"
#include "QDialog"



TestView::TestView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestView)
{
    ui->setupUi(this);
    this->initForm();

    ui->textEdit->installEventFilter(this); //它负责在相应部件上安装事件过滤器

    thread=new mythread; //定义⼀个线程对象
    thread->setObjectName(QString("okk"));//设置线程对象的名字，注意在启动前设置
    connect(thread,&mythread::over,this,&TestView::start_two_tree);//连接槽函数

    //调色板
    //获取 pushButton 的调⾊板
    QPalette palette1 = ui->pushButton->palette();
    //设置按钮⽂本颜⾊为红⾊
    palette1.setColor(QPalette::ButtonText,Qt::red);
    //设置按钮背景⾊为绿⾊
    palette1.setColor(QPalette::Button,Qt::green);
    //pushButton 使⽤修改后的调⾊板
    ui->pushButton->setPalette(palette1);
    //设置 lineEdit 不可⽤
    ui->lineEdit->setDisabled(true);
    QPalette palette2 = ui->lineEdit->palette();
    //设置⾏编辑器不可⽤时的背景颜⾊为蓝⾊
    palette2.setColor(QPalette::Disabled,QPalette::Base,Qt::red);
    ui->lineEdit->setPalette(palette2);

    //设置背景图片
    //设置背景图标，但是不能让背景图⽚⾃适应
    QPalette pt=this->palette();
    QImage img(":/image/index/u15.png");
    QImage myimg=img.scaled(this->width(),this->height()); //设置⽐例
    pt.setBrush(QPalette::Window,QBrush(myimg)); //设置画笔
    this->setPalette(pt);

}

TestView::~TestView()
{
    delete ui;
}
void TestView::initForm()
{
    QPushButton *button1 = new QPushButton("Button 1");
    QPushButton *button2 = new QPushButton("Button 2");
    ui->horizontalLayout_2->addWidget(button1);
    ui->horizontalLayout_2->addWidget(button2);
    ui->horizontalLayout_2->setStretchFactor(button1,1);
   ui->horizontalLayout_2->setStretchFactor(button2,2);

   this->setWindowTitle("软件标题");
   this->setWindowFlags(Qt::WindowMaximizeButtonHint|Qt::WindowCloseButtonHint);
   this->setWindowFlag(Qt::CustomizeWindowHint,true);
   this->setWindowFlag(Qt::WindowSystemMenuHint,true);
}

void TestView::on_pushButton_clicked()
{
    QDialog dialog(this);
    QLabel* lb1 =new QLabel("kkkk",&dialog); //向对话框中添加内容
    dialog.exec();
}

void TestView::on_pushButton_6_clicked()
{
    QDialog *dialog =new QDialog(this); //在构造⽅法中创建⼀个⾮模态对话框
        dialog->show();
}


void TestView::keyPressEvent(QKeyEvent *ev)
{
    //重写键盘按下事件
     ui->label_5->clear();
     if(ev->key() == Qt::Key_0)
     {
         ui->label_5->setText("1被按下了");
         return;
     }else if(ev->key() == Qt::Key_Q)
         ui->label_5->setText("Q被按下了");

     QWidget::keyPressEvent(ev);

}
bool TestView::eventFilter(QObject *obj, QEvent *event) //此函数中实现事件过滤器。
{
    if(obj==ui->textEdit) //textEdit控件
    {
        if(event->type()==QEvent::Wheel)
        {
            QWheelEvent * w=static_cast<QWheelEvent*>(event);
            if(w->delta()>0)
                ui->label_5->setText(QString::number(count++));
            else
                  ui->label_5->setText(QString::number(count--));
            return true;
        }
        else
            return false;
    }
    else
        return QWidget::eventFilter(obj, event);
}
//主线程要调⽤的任务函数
void TestView::start_two_tree()
{
    //ui->label_5->setText(QString::number(QThread::currentThreadId()));

    for(int i=0; i<5;i++)
    {
        ui->label_5->setText("开始采摘第"+QString::number(i+1)+"苹果");
       // qDebug()<<"$$$$$$开始采摘第"<<i+1<<"苹果"<<endl;
        QThread::sleep(1);
    }
      ui->label_5->setText("采摘完成");
      //ui->label_5->setText(thread->objectName());

}

//⼦线程的任务函数 通过重新run虚函数实现
void mythread::run ()
{

  //  qDebug()<<"开始采摘苹果"<<QThread::currentThreadId()<<endl;
    for(int i=0; i<5;i++)
    {
      //  qDebug()<<"####开始采第"<<i+1<<"个苹果"<<endl;
        QThread::sleep(1);
    }
   // qDebug()<<"####采摘完成"<<endl;
    emit over(); //⼦线程向主线程发送⼀个信号
}
