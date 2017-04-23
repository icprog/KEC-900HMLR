#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QObject>

class MyDataBase : public QObject
{
    Q_OBJECT
public:
    explicit MyDataBase(QObject *parent = 0);
    ~MyDataBase();

public:
    bool IsConnectDB();

signals:

public slots:
    void BeginTransaction();
    void CommitTransaction();

protected:
    void ConnectDB();
    bool CreateTable();
    void CloseDB();

private:
    bool m_bConnect;
};

#endif // MYDATABASE_H
