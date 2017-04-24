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

signals:

public slots:
    void SaveHistory(QString strTime,float Temp,qint32 Humi,qint32 Ion);

protected:
    void ConnectDB();
    bool CreateTable();
    void CloseDB();

private:
    bool m_bConnect;
};

#endif // MYDATABASE_H
