#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("TKA soft");
     w.resize(425,325);
    w.show();
    return a.exec();
}
