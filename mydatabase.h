#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QObject>

class MyDataBase : public QObject
{
    Q_OBJECT
public:
    explicit MyDataBase(QObject *parent = 0);

public:
    bool ConnectDB();
    void CloseDB();
    bool CreateTable();
    void BeginTransaction();
    void CommitTransaction();

signals:

public slots:

private:
    bool m_bConnect;
};

#endif // MYDATABASE_H
