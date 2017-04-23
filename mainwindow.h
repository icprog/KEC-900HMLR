#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mydatabase.h"
#include "myserialport.h"

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

signals:
//    void OnSampling();
//    void OnStoping();
//    void OnSendData(QString& strSend);
//    void OnReceiveData();

private slots:
    void on_action_Connect_triggered();
    void on_action_Off_triggered();
    void on_action_SetParam_triggered();
    void on_action_Start_triggered();
    void on_action_Stop_triggered();
    void on_action_Switch_triggered();
    void on_action_Zero_triggered();
    void on_action_Help_triggered();

private:
    Ui::MainWindow *ui;

private:
    MyDataBase myDatabase;//数据库对象
    MySerialPort mySerialPort;//串口通讯对象

    bool bSampling;
};

#endif // MAINWINDOW_H
