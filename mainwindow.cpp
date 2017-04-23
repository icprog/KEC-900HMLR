#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dlgcomset.h"
#include <QMessageBox>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bSampling = false;

    //设置动作的可用状态
    SetActionState();

    //信号和槽
//    connect(this,&MainWindow::OnSampling,&mySerialPort,&MySerialPort::OpenSerial);


//            void OnSampling();
//            void OnStoping();
//            void OnSendData(QString& strSend);
//            void OnReceiveData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetActionState()
{
    ui->action_Connect->setEnabled(!mySerialPort.isOpen());
    ui->action_Off->setEnabled(mySerialPort.isOpen());

    ui->action_SetParam->setEnabled(!mySerialPort.isOpen() && !bSampling);

    ui->action_Start->setEnabled(mySerialPort.isOpen() && !bSampling);
    ui->action_Stop->setEnabled(mySerialPort.isOpen() && bSampling);
    ui->action_Switch->setEnabled(mySerialPort.isOpen() && !bSampling);
    ui->action_Zero->setEnabled(mySerialPort.isOpen() && !bSampling);
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

    SetActionState();
}

void MainWindow::on_action_SetParam_triggered()
{
    //通讯设置对话框
    DlgComSet dlgComSet;
    //信号和槽
    connect(&dlgComSet,&DlgComSet::SetCom,&mySerialPort,&MySerialPort::SetConfig);
//    connect(&dlgComSet,&DlgComSet::SetInterval,)

    //运行对话框
    if ( QDialog::Accepted == dlgComSet.exec()) {

    }

    SetActionState();
}

void MainWindow::on_action_Start_triggered()
{
    //开始采集和保存线程

    mySerialPort.SendBuf(tr("123456"));


    bSampling = true;
    SetActionState();
}

void MainWindow::on_action_Stop_triggered()
{
    //停止采集和保存线程


    SetActionState();
}

void MainWindow::on_action_Switch_triggered()
{
    SetActionState();
}

void MainWindow::on_action_Zero_triggered()
{
    SetActionState();
}

void MainWindow::on_action_Help_triggered()
{
    SetActionState();
}
