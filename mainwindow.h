#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mydatabase.h"
#include "myserialport.h"
#include "mysamplethread.h"
#include <QDateTime>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void SetActionState();
    void ResolveData(QByteArray& buf);

signals:
    void Send(char* buf,qint64 len);
    void SaveHistory(QString strTime,float Temp,qint32 Humi,qint32 Ion);

private slots:
    void on_action_Connect_triggered();
    void on_action_Off_triggered();
    void on_action_SetParam_triggered();
    void on_action_Start_triggered();
    void on_action_Stop_triggered();
    void on_action_Switch_triggered();
    void on_action_Zero_triggered();
    void on_action_Help_triggered();

    void OnResolve(QByteArray buf);//解析获取数据

private:
    Ui::MainWindow *ui;

private:
    MyDataBase myDatabase;//数据库对象
    MySerialPort mySerialPort;//串口通讯对象
    MySampleThread mySample;//采集线程对象

    qint8 m_nRange;

    bool bSampling;
    QByteArray RecvBuf;
    QVector<int> vectorIon;
    qint32 nMaxIon;
    qint32 nMinIon;
    qint32 nAvgIon;
    float  fStdIon;
    qint32 nTotleIon;
};

#endif // MAINWINDOW_H
