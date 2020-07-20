#include <cstdlib>
#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include "hidapi.h"
#include "mainwindow.h"

void log(const QString& m) { qDebug() << QDateTime::currentDateTime().toString() << ' ' << m; }

void notify(const QString& m) { std::system(("osascript -e 'display notification \"" + m + "\" with title \"Matroid.Client\"'").toStdString().c_str()); }

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow   w;
    w.show();
    notify("1");
    return a.exec();
}
