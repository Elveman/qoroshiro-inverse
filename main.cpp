#include "mainwindow.h"

#include <QApplication>

/* qoroshiro-inverse by Elveman
 * NOTE: this software is distributed under GNU GPLv3. */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("qoroshiro-inverse 0.2.0");
    w.show();
    return a.exec();
}
