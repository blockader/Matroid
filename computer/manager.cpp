#include "manager.h"
#include "hidapi.h"
#include <QDateTime>
#include <QDebug>
#include <QTimer>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

using namespace std;

template <class T> void log(const T &m) {
    cerr << QDateTime::currentDateTime().toString().toStdString() + " " << m
         << endl;
}

void notify(const string &m) {
#ifdef __MACH__
    std::system(("osascript -e 'display notification \"" + m +
                 "\" with title \"Matroid.Client\"'")
                    .c_str());
#else
    log("This is not a supported OS.");
    exit(-1);
#endif
    log(m);
}

struct Message {
    int this_id, parent_id;
    string command;
    vector<string> arguments;
};

struct Keyboard {
    Keyboard() : interface(0) {}
    string name;
    int vendor_id, product_id;
    hid_device *interface;
    deque<Message> in_queue, out_queue;
    QMutex in_queue_mutex, out_queue_mutex;
};

vector<Keyboard> keyboards(1);

Manager::Manager() {
#ifdef __MACH__
#else
    log("This is not a supported OS.");
    exit(-1);
#endif
    keyboards[0].name = "Matrix Noah";
    keyboards[0].vendor_id = 0x4D58;
    keyboards[0].product_id = 0x0065;
    QTimer *timer = new QTimer(0);
    connect(timer, &QTimer::timeout, this, &Manager::send_messages);
    timer->start(100);
    timer = new QTimer(0);
    connect(timer, &QTimer::timeout, this, &Manager::receive_messages);
    timer->start(100);
    timer = new QTimer(0);
    connect(timer, &QTimer::timeout, this, &Manager::review);
    timer->start(1000);
}

Manager::~Manager() {}

void Manager::send_messages() {
    for (Keyboard &k : keyboards) {
        if (k.interface) {
            QMutexLocker locker(&k.out_queue_mutex);
            for (const auto &m : k.out_queue) {
                auto buff = to_string(m.this_id) + " " +
                            to_string(m.parent_id) + " " + m.command;
                for (const auto &a : m.arguments)
                    buff += " " + a;
                log(k.name + ": \"" + buff + "\" is being sent");
                if (buff.size() > 32) {
                    log("This message is too large.");
                    exit(-1);
                }
                buff += string(32 - buff.size(), 0);
                buff.insert(buff.begin(), char(0));
                if (hid_write(k.interface, (const unsigned char *)buff.c_str(),
                              33) < 0) {
                    log(k.name + ": This message can't be sent.");
                }
            }
            k.out_queue.clear();
        }
    }
}

void Manager::receive_messages() {
    for (Keyboard &k : keyboards) {
        if (k.interface) {
            QMutexLocker locker(&k.in_queue_mutex);
            while (1) {
                unsigned char buff[33] = {0};
                int l = hid_read(k.interface, buff, 32);
                if (l < 0) {
                    log(k.name + ": Messages may not be received");
                    break;
                }
                if (l == 0)
                    break;
                stringstream ss;
                ss << string((char *)buff);
                log(k.name + ": " + (char *)buff + " is being received.");
                Message m;
                if (!(ss >> m.this_id >> m.parent_id >> m.command)) {
                    log(k.name + ": This messages is corrupted.");
                    break;
                }
                for (string a; ss >> a; m.arguments.push_back(a))
                    ;
                k.in_queue.push_back(m);
            }
        }
    }
}

void Manager::review() {
    for (Keyboard &k : keyboards) {
        log(k.name + ": The review of " + k.name + " is starting.");
        if (k.interface) {
            QMutexLocker locker(&k.in_queue_mutex);
            auto i = find_if(
                begin(k.in_queue), end(k.in_queue),
                [](const Message &m) { return m.command == "heartbeat"; });
            if (i == k.in_queue.end()) {
                notify(k.name + ": " + k.name + " is lost.");
                hid_close(k.interface);
                k.interface = 0;
            } else {
                k.in_queue.erase(i);
                log(k.name + ": " + k.name + " is alive.");
            }
        } else {
            for (int i = 0; i < 16; ++i) {
                if ((k.interface = hid_open(k.vendor_id, k.product_id, 0))) {
                    hid_set_nonblocking(k.interface, 1);
                    notify(k.name + ": " + k.name + " is detected.");
                    break;
                }
            }
        }
        if (k.interface) {
            QMutexLocker locker(&k.out_queue_mutex);
            k.out_queue.push_back({int(time(0)), -1, "heartbeat", {}});
        }
        log(k.name + ": The review of " + k.name + " is ending.");
    }
}
