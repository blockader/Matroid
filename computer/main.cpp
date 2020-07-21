#include "manager.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(0);
    Manager m;
    return a.exec();
}
