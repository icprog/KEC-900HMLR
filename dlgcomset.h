#ifndef DLGCOMSET_H
#define DLGCOMSET_H

#include <QDialog>
#include <QString>
#include <QAbstractButton>

namespace Ui {
class DlgComSet;
}

class DlgComSet : public QDialog
{
    Q_OBJECT

public:
    explicit DlgComSet(QWidget *parent = 0);
    ~DlgComSet();

public:
    QString strPort;
    int nBaud;
    int nSampleInterval;
    int nSaveInterval;

signals:
    void SetCom(QString Port,qint32 Baud);
    void SetInterval(qint32 Sample,qint32 Save);

private slots:
    void on_btnOK_clicked();
    void on_btnCancel_clicked();

private:
    Ui::DlgComSet *ui;
};

#endif // DLGCOMSET_H
