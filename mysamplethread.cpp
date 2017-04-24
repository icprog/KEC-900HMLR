#include "mysamplethread.h"
#include <QDebug>
#include <QSettings>

MySampleThread::MySampleThread()
{
    //停止线程变量赋值为false
    bStoped = false;
    LastSaveDT = QDateTime(QDate(2016,1,1));

    // 当前目录的INI文件,初始化
    QSettings settings(tr("config.ini"), QSettings::IniFormat);
    nSampleInterval = settings.value(tr("config/Sample"),3).toInt();
    nSaveInterval = settings.value(tr("config/Save"),6).toInt();
}

void MySampleThread::StopThread()
{
    bStoped = true;
}

bool MySampleThread::CanSave(QDateTime time)
{
    if (LastSaveDT.secsTo(time) >= nSaveInterval) {
        LastSaveDT = time;
        return true;
    }
    return false;
}

void MySampleThread::SetConfig(qint32 Sample,qint32 Save)
{
    nSampleInterval = Sample;
    nSaveInterval = Save;
}

void MySampleThread::run()
{
    bStoped = false;
    //循环
    while (!bStoped) {
        //发送信号
        emit Send((char*)SendBuf,8);
        //线程等待,单位秒
        sleep(nSampleInterval);
    }
}

