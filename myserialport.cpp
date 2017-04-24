#include "myserialport.h"
#include <QSettings>
#include <QDebug>
#include <synchapi.h>

MySerialPort::MySerialPort(QObject *parent) : QObject(parent)
{
    //构建串口通讯对象
    mySerial = new QSerialPort;

    // 当前目录的INI文件,初始化
    QSettings settings(tr("config.ini"), QSettings::IniFormat);
    strPort = settings.value(tr("config/Port"),tr("COM1")).toString();
    nBaud = settings.value(tr("config/Baud"),9600).toInt();
}

MySerialPort::~MySerialPort()
{
    mySerial->deleteLater();
}

bool MySerialPort::OpenSerial()
{
    //串口没打开，则打开
    if (!mySerial->isOpen()) {
        //设置串口名
        mySerial->setPortName(strPort);
        //打开串口
        mySerial->open(QIODevice::ReadWrite);
        //设置波特率
        mySerial->setBaudRate(nBaud);
        //设置数据位数
        mySerial->setDataBits(QSerialPort::Data8);
        //设置奇偶校验
        mySerial->setParity(QSerialPort::NoParity);
        //设置停止位
        mySerial->setStopBits(QSerialPort::OneStop);
        //设置流控制
        mySerial->setFlowControl(QSerialPort::NoFlowControl);

        //连接信号槽
       connect(mySerial, &QSerialPort::readyRead, this, &MySerialPort::ReceiveBuf);
    }

    return mySerial->isOpen();
}

void MySerialPort::CloseSerial()
{
    //关闭串口
    if (mySerial->isOpen()) {
        mySerial->clear();
        mySerial->close();
    }
}

void MySerialPort::ClearSerial()
{
    //清空缓冲区
    mySerial->clear();
}

bool MySerialPort::isOpen()
{
    return mySerial->isOpen();
}

void MySerialPort::SendBuf(char* buf,qint64 len)
{
    //写命令
    if (mySerial->write(buf,len) != len) {
            qDebug() << tr("Write Error");
    }
    else {
        qDebug() << tr("Write Success");
    }
}

void MySerialPort::ReceiveBuf()
{
    QByteArray buf;
    buf = mySerial->readAll();
    if(!buf.isEmpty())
    {
        emit Resolve(buf);
        qDebug() << tr("Receive Success");
    }
    buf.clear();
}

void MySerialPort::SetConfig(QString Port, qint32 Baud)
{
    strPort = Port;
    nBaud = Baud;
}

