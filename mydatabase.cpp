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

    QSqlDatabase::database().transaction();

    //创建表
    if (!CreateTable()) {
        QMessageBox::critical(0,tr("创建表失败"),QSqlDatabase::database().lastError().text());
    }

    QSqlDatabase::database().commit();
    return;
}

void MyDataBase::SaveHistory(QString strTime, float Temp, qint32 Humi, qint32 Ion)
{
    QSqlQuery query;
    if (query.prepare(QString("insert into history (time,temp,humi,ion) values(?,?,?,?)"))) {
        query.addBindValue(strTime);
        query.addBindValue(Temp);
        query.addBindValue(Humi);
        query.addBindValue(Ion);
        query.exec();
        QSqlDatabase::database().commit();
        qDebug() << tr("insert Success");
    }
    else {
        qDebug() << tr("insert Error");
    }
}

bool MyDataBase::CreateTable()
{
    QSqlQuery query;
    //创建表
    return query.exec(QString("create table if not exists history (time varchar(14) primary key,temp float,humi int,ion int)"));
}

void MyDataBase::CloseDB()
{
    QSqlDatabase::database().commit();
    QSqlDatabase::database().close();
    m_bConnect = false;
}

