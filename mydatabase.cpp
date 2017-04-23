#include "mydatabase.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QDebug>

MyDataBase::MyDataBase(QObject *parent) : QObject(parent)
{
    m_bConnect = false;

    //连接数据库并创建表
    ConnectDB();
}

MyDataBase::~MyDataBase()
{
    CloseDB();
}

void MyDataBase::ConnectDB()
{
    if (m_bConnect) {//已经连接数据库，返回true
        return;
    }
    QSqlDatabase db = QSqlDatabase::addDatabase(tr("QSQLITE"));
    db.setDatabaseName(tr("Data.db"));
    if (!db.open()) {//打开数据库文件
        QMessageBox::critical(0,tr("数据库错误"),db.lastError().text());
        return;
    }
    //数据库连接成功
    m_bConnect = true;

    //启动事务
    BeginTransaction();

    //创建表
    if (!CreateTable()) {
        QMessageBox::critical(0,tr("创建表失败"),QSqlDatabase::database().lastError().text());
    }

    //成功后提交事务
    CommitTransaction();

    return;
}

bool MyDataBase::IsConnectDB()
{
    return m_bConnect;
}

void MyDataBase::BeginTransaction()
{
    QSqlDatabase::database().transaction();
}

void MyDataBase::CommitTransaction()
{
    QSqlDatabase::database().commit();
}

bool MyDataBase::CreateTable()
{
    QSqlQuery query;
    //关键的判断,判断指定表是否存在
    bool isTableExist = query.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(tr("Data.db")));
    if(isTableExist)
    {
        qDebug() << "表已存在";
        return true;
    }
    else
    {
        qDebug() << "表不存在";
        //创建表
        return query.exec(QString("create table history (time varchar(14) primary key,temp float,humi float,ion float)"));
    }
}

void MyDataBase::CloseDB()
{
    CommitTransaction();//关闭前提交事务
    QSqlDatabase::database().close();
    m_bConnect = false;
}

