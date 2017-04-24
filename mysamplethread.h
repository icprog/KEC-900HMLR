#ifndef MYSAMPLETHREAD_H
#define MYSAMPLETHREAD_H

#include <QThread>
#include <QString>
#include <QDateTime>

class MySampleThread : public QThread
{
    Q_OBJECT
public:
    MySampleThread();
    void StopThread();
    bool CanSave(QDateTime time);

signals:
    void Send(char* buf,qint64 len);

public slots:
    void SetConfig(qint32 Sample,qint32 Save);

protected:
    void run();

private:
    qint32 nSampleInterval;
    qint32 nSaveInterval;
    const unsigned char SendBuf[8]={0x09,0x03,0x00,0x00,0x00,0x07,0x05,0x40};

    QDateTime LastSaveDT;
    volatile bool bStoped;
};

#endif // MYSAMPLETHREAD_H
