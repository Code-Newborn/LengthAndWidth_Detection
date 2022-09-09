#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // 解决预览与显示不一致【分辨率高的屏幕适应性问题】
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
