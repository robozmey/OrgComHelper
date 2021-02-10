#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("ISO-8859-1"));
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
