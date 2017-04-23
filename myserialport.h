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
    bool isOpen();
    qint64 SendBuf(QString str);
    void ReceiveBuf();

signals:
    void Resolve(QString& strRev);//解析数据信号

public slots:
    void SetConfig(QString Port,qint32 Baud);

private:
    QString strPort;
    qint32 nBaud;

    QSerialPort* mySerial;
};

#endif // MYSERIALPORT_H
