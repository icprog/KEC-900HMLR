#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>

class MySerialPort : public QObject
{
    Q_OBJECT
public:
    explicit MySerialPort(QObject *parent = 0);
    ~MySerialPort();

public:
    bool OpenSerial();
    void CloseSerial();
    void ClearSerial();
    bool isOpen();
    void ReceiveBuf();

signals:
    void Resolve(QByteArray buf);//解析数据信号

public slots:
    void SetConfig(QString Port,qint32 Baud);
    void SendBuf(char* buf,qint64 len);

private:
    QString strPort;
    qint32 nBaud;

    QSerialPort* mySerial;
};

#endif // MYSERIALPORT_H
