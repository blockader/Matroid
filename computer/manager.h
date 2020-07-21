#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>

struct Manager : QObject {
    Q_OBJECT
  public:
    Manager();
    ~Manager();
  public slots:
    void send_messages();
    void receive_messages();
    void review();
};

#endif // MANAGER_H
