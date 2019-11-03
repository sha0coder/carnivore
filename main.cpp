/*
 * Carnivore v3
 *
 *                                +--- portscan
 *                                |
 * MainWindow -> agents -> agent -+--- dirscan
 *                                |
 *                                +--- crawler
 *                                |
 *                                +--- bruter
 *
 *
 *
 */


#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
