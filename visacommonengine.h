#ifndef VISACOMMONENGINE_H
#define VISACOMMONENGINE_H
#include "visa.h"
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "basecommonApi.h"

class VisaCommonEngine
{
public:
    VisaCommonEngine();
    ~VisaCommonEngine();
    //~VisaCommonEngine();
    QString queryData(QString sCmd);
    bool sendData(QString sCmd);
public:
    bool openDevice(QString sAddr,QString sType);
    void closeDevice();
    bool getState();
    QStringList getVisaAddrs();
    QStringList getSerialAddrs();
    QString getdeviceIdninfo(QString sAddr);
     QString commType="Visa";
     QString role;
private:
    //visa
    ViSession defaultRM;
    ViSession instr;
    ViUInt32 retCount;
    ViUInt32 writeCount;
    ViStatus status;
    unsigned char buffer[100];
    char stringinput[512];
    ViConstRsrc remoteAddr="";

    QByteArray utf8Bytes;
    bool commState=false;
    QSerialPort *serial;

};

#endif // VISACOMMONENGINE_H
