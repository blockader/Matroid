#include "manager.h"
#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(0);
    Manager m;
    return a.exec();
}
