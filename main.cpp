#include "loginwindow.h"
#include "mainwindow.h"
#include <QApplication>
#include "indexWindow.h"
#include "testview.h"
#include "basecommonapi.h"
#include "infomanageview.h"
#include "datasyncview.h"
#include "QMessageBox"
#include "QDebug"

//#include <openssl/md5.h>

//#include <QByteArray>

//#include <QString>

//#include <QDebug>

//QString md5HashBySlat(const QString &input) {
//    unsigned char result[MD5_DIGEST_LENGTH];
//    MD5((unsigned char*)input.toUtf8().constData(), input.toUtf8().length(), result);

//    // 将结果转换为 32 位小写格式的字符串
//    QString hash;
//    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
//        // 直接转换为小写十六进制字符串并添加到哈希中
//        hash.append(QString::number(result[i], 16).toLower());
//    }

//    // 确保哈希是 32 位长，如果不是，则前面补 0
////    while (hash.length() < 32) {
////        hash.prepend("0");
////    }

//    return hash;
//}
//// 辅助函数：将字节数组转换为十六进制字符串
//QString byteArrayToHex(const QByteArray &ba) {
//    QString hex;
//    for (char c : ba) {
//        hex += QString::number(static_cast<unsigned char>(c), 16).toUpper().rightJustified(2, '0');
//    }
//    return hex;
//}

//// 迭代 MD5 哈希函数
//QString iterativeMd5Hash(const QString &password, const QString &salt, int iterations) {
//    QByteArray passwordBytes = password.toUtf8();
//    QByteArray saltBytes = salt.toUtf8();
//    QByteArray combinedBytes = passwordBytes + saltBytes;

//    for (int i = 0; i < iterations; ++i) {
//        unsigned char md5Digest[MD5_DIGEST_LENGTH];
//        MD5((unsigned char*)combinedBytes.data(), combinedBytes.size(), md5Digest);
//        combinedBytes = QByteArray((char*)md5Digest, MD5_DIGEST_LENGTH);
//    }

//    return byteArrayToHex(combinedBytes);
//}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 加载外部的CSS文件
    //a.setStyleSheet("file:///path/to/styles.css");
    //设置字体
    QFont font;
    font.setFamily("Microsoft Yahei");
    font.setPixelSize(13);
    a.setFont(font);

    //DataSyncView w;
    //IndexWindow w;
    //TestView w;
    //w.thread->start();
//    QString input = "gph2o";
//    QString md5Hash = md5HashBySlat(input);
//    qDebug() << "32-bit lowercase MD5 Hash of" << input << "is" << md5Hash;

//    QString password1 = "1234567";
//    QString md5HashBypassword = md5HashBySlat(password1);
//    qDebug() << "md5HashBypassword:" << password1 << "is" << md5HashBypassword;


//    QString password = "1234567";
//    QString salt = md5Hash;
//    int iterations = 1000;
//    QString hashedPassword = iterativeMd5Hash(password, salt, iterations);
//    qDebug() << "Hashed Password:" << hashedPassword;


    loginWindow w;
    w.show();
    return a.exec();
}
