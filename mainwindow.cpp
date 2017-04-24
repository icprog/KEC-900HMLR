#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dlgcomset.h"
#include <QMessageBox>
#include <QSettings>
#include <QDebug>
#include <QtCore/qmath.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bSampling = false;
    vectorIon.empty();
    nMaxIon = 0;
    nMinIon = 0;
    nAvgIon = 0;
    fStdIon = 0.0;
    nTotleIon = 0;
    //初始化量程
    m_nRange = 2;
    on_action_Switch_triggered();

    //设置动作的可用状态
    SetActionState();

    //信号和槽
    connect(this,&MainWindow::Send,&mySerialPort,&MySerialPort::SendBuf);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetActionState()
{
    //更新菜单动作的可用状态
    ui->action_Connect->setEnabled(!mySerialPort.isOpen());
    ui->action_Off->setEnabled(mySerialPort.isOpen());

    ui->action_SetParam->setEnabled(!mySerialPort.isOpen() && !bSampling);

    ui->action_Start->setEnabled(mySerialPort.isOpen() && !bSampling);
    ui->action_Stop->setEnabled(mySerialPort.isOpen() && bSampling);
    ui->action_Switch->setEnabled(mySerialPort.isOpen() && !bSampling);
    ui->action_Zero->setEnabled(mySerialPort.isOpen() && !bSampling);
}

void MainWindow::ResolveData(QByteArray& buf)
{
    //解析数据
    qint8 nAddr= buf[0] & 0xFF;//地址9
    qint8 nType = buf[1] & 0xFF;//功能码
    qint8 nLen = buf[2] & 0xFF;//数据长度14
    //操作命令
    if (nType == 6) {
        qDebug() << tr("Successful operation");
        return;
    }
    //采集返回数据
    if ((nAddr != 9) || (nLen != 14)) {
        qDebug() << tr("Resolve error");
        return;
    }
    //风扇是否关闭  0,1
    qint16 nOnorOff = buf[4] & 0x00FF;
    nOnorOff |= ((buf[3] << 8) & 0xFF00);
    //量程范围   0,1,2
    qint16 nRange = buf[6] & 0x00FF;
    nRange |= ((buf[5] << 8) & 0xFF00);
    //离子正负   0,1
    qint16 nIon = buf[10] & 0x00FF;
    nIon |= ((buf[9] << 8) & 0xFF00);
    //离子数值
    qint16 nIonValue = buf[12] & 0x00FF;
    nIonValue |= ((buf[11] << 8) & 0xFF00);
    //温度数值
    qint16 nTempValue = buf[14] & 0x00FF;
    nTempValue |= ((buf[13] << 8) & 0xFF00);
    float fTempValue = ((float)nTempValue)/10;
    //湿度数值
    qint16 nHumiValue = buf[16] & 0x00FF;
    nHumiValue |= ((buf[15] << 8) & 0xFF00);
    //记录离子值
    qint32 nTotleTemp = 0;
    qint32 nTemp = qAbs(nIonValue*(qPow(10,nRange + 1)));
    if (vectorIon.isEmpty()) {
        nMaxIon = nTemp;
        nMinIon = nTemp;
    }
    vectorIon.push_back(nTemp);//添加到数组
    nMaxIon = qMax(nMaxIon,nTemp);
    nMinIon = qMin(nMinIon,nTemp);
    nTotleIon += nTemp;
    nAvgIon = nTotleIon/vectorIon.size();
    for (int i = 0; i < vectorIon.size(); ++i) {
        nTotleTemp += qPow((vectorIon.at(i) - nAvgIon),2);
     }
    fStdIon = qSqrt(((qreal)nTotleTemp)/vectorIon.size());
    //显示到界面
    ui->RangeValue->setText(QString((nRange == 0) ? tr("R1") : (nRange == 1) ? tr("R2") : tr("R3")));
    ui->Ion_Value->setText(QString((nIon == 0) ? tr("-ION") : tr("+ION")));
    ui->IonValue->setText(QString::number(nTemp));
    ui->TempValue->setText(QString::number(fTempValue,'f',1) + tr(" ℃"));
    ui->HumiValue->setText(QString::number(nHumiValue) + tr(" %"));
    ui->MaxValue->setText(QString::number(nMaxIon));
    ui->MinValue->setText(QString::number(nMinIon));
    ui->AvgValue->setText(QString::number(nAvgIon));
    ui->StdValue->setText(QString::number(fStdIon,'f',2));
    //保存数据
    QDateTime CurrentDT = QDateTime::currentDateTime();//当前系统时间
    if (mySample.CanSave(CurrentDT)) {
        //满足条件，则保存数据
        QString strTime = CurrentDT.toString(tr("yyyy/MM/dd hh:mm:ss"));
        emit SaveHistory(strTime,fTempValue,nHumiValue,nIonValue);
    }
}


void MainWindow::on_action_Connect_triggered()
{
    //打开串口
    if (!mySerialPort.OpenSerial()) {
        QMessageBox::critical(0,tr("串口失败"),tr("打开串口失败\n请检查后重试"));
    }
    //菜单项可用状态
    SetActionState();
}

void MainWindow::on_action_Off_triggered()
{
    //关闭串口
    mySerialPort.CloseSerial();
    //菜单项可用状态
    SetActionState();
}

void MainWindow::on_action_SetParam_triggered()
{
    //通讯设置对话框
    DlgComSet dlgComSet;
    //信号和槽
    connect(&dlgComSet,&DlgComSet::SetCom,&mySerialPort,&MySerialPort::SetConfig);
    connect(&dlgComSet,&DlgComSet::SetInterval,&mySample,&MySampleThread::SetConfig);
    //运行对话框
    dlgComSet.exec();
    //菜单项可用状态
    SetActionState();
}

void MainWindow::on_action_Start_triggered()
{
    //信号和槽
    connect(this,&MainWindow::SaveHistory,&myDatabase,MyDataBase::SaveHistory);
    connect(&mySample,&MySampleThread::Send,&mySerialPort,&MySerialPort::SendBuf);
    connect(&mySerialPort,&MySerialPort::Resolve,this,&MainWindow::OnResolve);

    //清空通讯缓冲区
    mySerialPort.ClearSerial();
    //开始线程
    mySample.start();
    //成功
    bSampling = true;
    //菜单项可用状态
    SetActionState();
}

void MainWindow::on_action_Stop_triggered()
{
    //停止采集和保存线程
    mySample.StopThread();
    bSampling = false;
    //清空数据，界面置零
    vectorIon.empty();
    nMaxIon = 0;
    nMinIon = 0;
    nAvgIon = 0;
    fStdIon = 0.0;
    nTotleIon = 0;
    ui->RangeValue->setText(QString(tr("R1")));
    ui->Ion_Value->setText(QString(tr("-ION")));
    ui->IonValue->setText(tr("0"));
    ui->TempValue->setText(tr("0.0 ℃"));
    ui->HumiValue->setText(tr("0 %"));
    ui->MaxValue->setText(tr("0"));
    ui->MinValue->setText(tr("0"));
    ui->AvgValue->setText(tr("0"));
    ui->StdValue->setText(tr("0.00"));
    //菜单项可用状态
    SetActionState();
}

void MainWindow::on_action_Switch_triggered()
{
    switch (m_nRange) {
    case 0:
    {
        const unsigned char SendBuf[8] = {0x09,0x06,0x00,0x01,0x00,0x01,0x18,0x82};
        emit Send((char*)SendBuf,8);
        m_nRange = 1;
        break;
    }
    case 1:
    {
        const unsigned char SendBuf[8] = {0x09,0x06,0x00,0x01,0x00,0x02,0x58,0x83};
        emit Send((char*)SendBuf,8);
        m_nRange = 2;
        break;
    }
    case 2:
    {
        const unsigned char SendBuf[8] = {0x09,0x06,0x00,0x01,0x00,0x00,0xD9,0x42};
        emit Send((char*)SendBuf,8);
        m_nRange = 0;
        break;
    }
    default:
        break;
    }
    ui->RangeValue->setText(QString((m_nRange == 0) ? tr("R1") : (m_nRange == 1) ? tr("R2") : tr("R3")));
    //菜单项可用状态
    SetActionState();
}

void MainWindow::on_action_Zero_triggered()
{
    const unsigned char SendBuf[8]={0x09,0x06,0x00,0x02,0x00,0x01,0xE8,0x82};
    emit Send((char*)SendBuf,8);
    //菜单项可用状态
    SetActionState();
}

void MainWindow::on_action_Help_triggered()
{
    //菜单项可用状态
    SetActionState();
}

void MainWindow::OnResolve(QByteArray buf)
{
    RecvBuf.append(buf);
    if (RecvBuf.size() >= 19) {
        //解析数据
        ResolveData(RecvBuf);
        //调试输出
        qDebug() << RecvBuf.toHex();
        RecvBuf.clear();
    }
}
