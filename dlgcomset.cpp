#include "dlgcomset.h"
#include "ui_dlgcomset.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QString>
#include <QSettings>

DlgComSet::DlgComSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgComSet)
{
    ui->setupUi(this);

    // 当前目录的INI文件,初始化
    QSettings settings(tr("config.ini"), QSettings::IniFormat);
    strPort = settings.value(tr("config/Port"),tr("COM1")).toString();
    nBaud = settings.value(tr("config/Baud"),9600).toInt();
    nSampleInterval = settings.value(tr("config/Sample"),3).toInt();
    nSaveInterval = settings.value(tr("config/Save"),6).toInt();

    //查找可用的串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->cobPort->addItem(serial.portName());
            serial.close();
        }
    }
    //默认填写第一个
    ui->cobPort->setCurrentText(strPort);

    //波特率默认设置为列表中的3号位的9600
    ui->cobBoud->setCurrentText(QString::number(nBaud));

    ui->edSampleInterval->setText(QString::number(nSampleInterval));
    ui->edSaveInterval->setText(QString::number(nSaveInterval));
}

DlgComSet::~DlgComSet()
{
    delete ui;
}

void DlgComSet::on_btnOK_clicked()
{
    //获取控件的值
    strPort = ui->cobPort->currentText();
    nBaud = ui->cobBoud->currentText().toInt();
    nSampleInterval = ui->edSampleInterval->text().toInt();
    nSaveInterval = ui->edSaveInterval->text().toInt();

    // 当前目录的INI文件,参数写入文件
    QSettings settings(tr("config.ini"), QSettings::IniFormat);
    settings.beginGroup(tr("config"));
    settings.setValue(tr("Port"), strPort);
    settings.setValue(tr("Baud"), nBaud);
    settings.setValue(tr("Sample"), nSampleInterval);
    settings.setValue(tr("Save"), nSaveInterval);
    settings.endGroup();

    //发送信号
    emit SetCom(strPort,nBaud);
//        emit SetInterval(nSampleInterval,nSaveInterval);

    //退出对话框
    accept();
}

void DlgComSet::on_btnCancel_clicked()
{
    //退出对话框
    reject();
}
