#include "mainwindow.h"
#include <QApplication>
#include "AfCd.h"

int main(int argc, char *argv[])
{
    //设置当前路径
    AfCd::cd();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
