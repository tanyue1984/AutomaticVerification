#include "visacommonengine.h"
#include <QDebug>


VisaCommonEngine::VisaCommonEngine() : serial(new QSerialPort)
{

}
VisaCommonEngine::~VisaCommonEngine()
{
}

/*VisaCommonEngine::~VisaCommonEngine()
{
    closeDevice();
}*/
QStringList VisaCommonEngine::getVisaAddrs()
{
    QStringList addrList;
    //addrList.append("TCPIP0::10.129.41.111::inst0::INSTR");
    //addrList.append("GPIB1::INSTR");
    char instrDescriptor[VI_FIND_BUFLEN];
    ViUInt32 numInstrs;
    ViFindList findList;

    status = viOpenDefaultRM(&defaultRM);
    if ( status < VI_SUCCESS ) {
        qDebug() << "Could not open a session to the VISA Resource Manager!\n";
        return addrList;
    }

    status = viFindRsrc( defaultRM, "?*INSTR", &findList, &numInstrs, instrDescriptor );
    if ( status < VI_SUCCESS ) {
        qDebug() << "An error occurred while finding resources.";
        return addrList;
    }
    //openDevice("*IDN?")
    if ( numInstrs > 0 ) {
        addrList.append(instrDescriptor);
    }

    //status = viOpen( defaultRM, instrDescriptor, VI_NULL, VI_NULL, &instr );

    while (--numInstrs ) {
        status = viFindNext( findList, instrDescriptor );
        if ( status < VI_SUCCESS ) {
            qDebug() <<"An error occurred finding the next resource.";
            //return status;
        }else{
            addrList.append(instrDescriptor);
        }
        //这个时候的instrDescriptor就是你电脑上找到的资源，并且可用，如果为空就说明你的连线有问题
    }

    //viClose(instr);
    //viClose(findList);
    viClose(defaultRM);

    return addrList;
}
QString VisaCommonEngine::getdeviceIdninfo(QString sAddr)
{
   QString info="false";
   if(this->openDevice(sAddr,"Visa"))
   {
        info=this->queryData("*IDN?");
        this->closeDevice();
   }
   return info;
}
QStringList VisaCommonEngine::getSerialAddrs()
{
    QStringList addrList;
    //addrList.append("COM1");
    //addrList.append("COM2");
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        addrList.append(info.portName());
    }
    return addrList;
}

bool VisaCommonEngine::getState()
{
    return commState;
}
bool VisaCommonEngine::openDevice(QString sAddr,QString sType)
{
    if(getState())
    {
        closeDevice();
    }


    commType = sType;
    if(sType=="Visa" && sAddr!=""){
        utf8Bytes = sAddr.toUtf8();
        remoteAddr = reinterpret_cast<ViConstRsrc>(utf8Bytes.constData());
        status = viOpenDefaultRM(&defaultRM);
        if (status < VI_SUCCESS)
        {
            qDebug() << "Could not open a session to the VISA Resource Manager!\n";
            return false;
        }


        status = viOpen(defaultRM, remoteAddr, VI_NULL, VI_NULL, &instr);
        if (status < VI_SUCCESS)
        {
            qDebug() << "Cannot open a session to the device.\n";
            closeDevice();
            return false;
        }

        //viQueryf(instr,"IDN  ?\n","%t",buffer);
        /*status = viQueryf( instr, "*IDN?\n", "%s", buffer);

    if (status < VI_SUCCESS){
        qDebug() << "Error reading a response from the device.\n";
    }else{
        qDebug() << "Data read:" << retCount << QString::fromLocal8Bit((char*)buffer);

        //ui->textBrowser->setText(QString::fromLocal8Bit((char*)buffer,retCount));
    }*/


        /* Set the timeout to 5 seconds (5000 milliseconds). */
        status = viSetAttribute(instr, VI_ATTR_TMO_VALUE, 10000);

        /* Set the baud rate to 4800 (default is 9600). */
        //status = viSetAttribute(instr, VI_ATTR_ASRL_BAUD, 4800);

        /* Set the number of data bits contained in each frame (from 5 to 8).
         * The data bits for  each frame are located in the low-order bits of
         * every byte stored in memory.
         */
        //status = viSetAttribute(instr, VI_ATTR_ASRL_DATA_BITS, 8);

        //status = viSetAttribute(instr, VI_ATTR_ASRL_PARITY, VI_ASRL_PAR_NONE);

        //status = viSetAttribute(instr, VI_ATTR_ASRL_STOP_BITS, VI_ASRL_STOP_ONE);

        //status = viSetAttribute(instr, VI_ATTR_TERMCHAR_EN, VI_TRUE);

        /* Set the termination character to 0xA
         */
        status = viSetAttribute(instr, VI_ATTR_TERMCHAR, 0xA);


        //strcpy(stringinput, ":MEASure:VOLTage:DC?\n");
        //strcpy(stringinput, "*IDN?\n");
        //status = viWrite(instr, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
        //if (status < VI_SUCCESS){
        //    qDebug() << "Error writing to the device.\n";
        //    goto Close;
        //}

        //status = viRead(instr, buffer, 100, &retCount);
        //if (status < VI_SUCCESS){
        //    qDebug() << "Error reading a response from the device.\n";
        //}else{
        //    qDebug() << "Data read:" << retCount << QString::fromLocal8Bit((char*)buffer,retCount);

        //ui->textBrowser->setText(QString::fromLocal8Bit((char*)buffer,retCount));
        //}
        commState = true;
        return true;
    }else{
        if(sAddr!=""){
            QStringList sList = sAddr.split(";");
            if(sList.length()==5){
                //设置串口名
                serial->setPortName(sList[0]);
                //设置波特率
                serial->setBaudRate(sList[1].toInt());
                //设置奇偶校验
                QSerialPort::Parity parity=QSerialPort::NoParity;
                if(sList[2]=="None")
                    parity = QSerialPort::NoParity;
                else if(sList[2]=="Odd")
                    parity = QSerialPort::OddParity;
                else if(sList[2]=="Even")
                    parity = QSerialPort::EvenParity;
                serial->setParity(parity);
                //设置数据位数
                QSerialPort::DataBits bits=QSerialPort::Data7;
                if(sList[3]=="7")
                    bits=QSerialPort::Data7;
                else if(sList[3]=="8")
                    bits=QSerialPort::Data8;
                serial->setDataBits(bits);

                //设置停止位
                QSerialPort::StopBits stop =QSerialPort::OneStop;
                if(sList[4]=="None")
                    stop=QSerialPort::UnknownStopBits;
                else if(sList[4]=="One")
                    stop=QSerialPort::OneStop;
                else if(sList[4]=="Two")
                    stop=QSerialPort::TwoStop;
                else if(sList[4]=="OnePointFive")
                    stop=QSerialPort::OneAndHalfStop;
                serial->setStopBits(stop);
                //设置流控制
                //serial.setFlowControl(QSerialPort::NoFlowControl);
                //打开串口
                if(serial->open(QIODevice::ReadWrite)){
                    commState = true;
                    return true;
                }
            }
        }
    }
}

void VisaCommonEngine::closeDevice()
{
    if(commType=="Visa"){
        status = viClose(instr);
        status = viClose(defaultRM);
        commState = false;
    }else{
        serial->close();
        commState = false;
    }
}

QString VisaCommonEngine::queryData(QString sCmd)
{
    if(commType=="Visa"){
          BaseCommonApi::SaveLogInfo(1,QString("读取指令：%1").arg(sCmd));

           utf8Bytes = (sCmd+"\n").toUtf8();
            ViConstRsrc sQuery = reinterpret_cast<ViConstRsrc>(utf8Bytes.constData());
           status = viQueryf( instr, sQuery, "%s", buffer);

           if (status < VI_SUCCESS){
               qDebug() << "Error reading a response from the device.\n";
               return "";
           }else{
               qDebug() << "Data read:" << retCount << QString::fromLocal8Bit((char*)buffer);
               return QString::fromLocal8Bit((char*)buffer);
               //ui->textBrowser->setText(QString::fromLocal8Bit((char*)buffer,retCount));
           }
           return "";
       }else{
           serial->clear();
           sendData(sCmd);
           QString strRet;
           QByteArray buf;
           buf = serial->readAll();
           if(!buf.isEmpty())
           {
              strRet=QString::fromLatin1(buf);
           }
           buf.clear();
           return strRet;
       }
}

bool VisaCommonEngine::sendData(QString sCmd)
{
    QString srr=commType;
    //strcpy(stringinput, strData.toLatin1().data());
       //strcpy(stringinput, "*IDN?\n");
       if(commType=="Visa"){
           BaseCommonApi::SaveLogInfo(1,QString("配置指令：%1").arg(sCmd));
           utf8Bytes = (sCmd+"\n").toUtf8();   
           ViBuf buffer  = reinterpret_cast<ViBuf>(utf8Bytes.data());
           ViUInt32 bufferSize = static_cast<ViUInt32>(utf8Bytes.size());
           status = viWrite(instr, buffer, bufferSize, &writeCount);
           if (status < VI_SUCCESS){
               return false;
           }
           return true;
       }else{
           serial->write(sCmd.toLatin1());
           return true;
       }
}
