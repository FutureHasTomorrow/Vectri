#include "MainWindow.h"
#include "DataBase.h"

#include <QApplication>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    // MainProc p(&w);
    // QThread procThread;
    // p.moveToThread(&procThread);
    // QObject::connect(&procThread, &QThread::started, &procThread, &MainProc::proc);
    // QObject::connect(&procThread, &QThread::finished, &procThread, &QObject::deleteLater);
    // procThread.start();

    return a.exec();
}
