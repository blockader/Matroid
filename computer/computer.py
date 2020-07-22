#!/usr/bin/python3

import hid
import easydict
import sys
import IPython
import time
import subprocess
import os
import datetime
from PyQt5 import QtCore, QtGui, QtWidgets
if sys.platform == 'darwin':
    import pync


if sys.platform not in ['darwin']:
    print('%s is not a supported OS.' % sys.platform)


def log(*m):
    print(datetime.datetime.now().strftime("%Y %m %d %H:%M:%S:%f"), *m)


def notify(*m):
    if sys.platform == 'darwin':
        pync.notify(' '.join(m))
    log(*m)


keyboards = [
    easydict.EasyDict(
        name='Matrix Noah',
        vendor_id=0x4D58,
        product_id=0x0065,
        interface=None,
        interface_mutex=QtCore.QMutex(),
        heartbeat_session_id=-1,
        heartbeat_session_id_mutex=QtCore.QMutex(),
        handness_session_id=-1,
        handness_session_id_mutex=QtCore.QMutex()
    )
]


app = QtWidgets.QApplication(sys.argv)
app.setQuitOnLastWindowClosed(False)


def send_message(k, m):
    with QtCore.QMutexLocker(k.interface_mutex):
        if k.interface:
            m = ' '.join(
                map(str, [m.session_id, m.command] + m.arguments))
            log('%s: "%s" is being sent.' % (k.name, m))
            if (len(m) > 32):
                notify('%s: This message is too large.' % k.name)
                exit(-1)
            m = [ord(c)for c in m]
            m = m + [0] * (32 - len(m))
            try:
                k.interface.write(m)
            except:
                log('%s: This message can\'t be sent.' % k.name)
        else:
            log('%s: This message can\'t be sent because there is no connection.' % k.name)


def handle_message(k, m):
    if m.command == 'confusion':
        log('%s: A message with session id %d can\'t be processed by %d.' %
            (k.name, m.session_id, k.name))
    elif m.command == 'heartbeat':
        with QtCore.QMutexLocker(k.heartbeat_session_id_mutex):
            if k.heartbeat_session_id == m.session_id:
                k.heartbeat_session_id = -1
            else:
                log('%s: This heartbeat isn\'t expected.' % k.name)
    elif m.command == 'language':
        languages = {'english': 'com.apple.keylayout.ABC',
                     'chinese': 'com.apple.inputmethod.SCIM.ITABC'}
        if len(m.arguments) != 1 or m.arguments[0] not in languages.keys():
            notify('%s: The language switching failed due to an illegal message format.' %
                   k.name)
        else:
            path = os.path.dirname(sys.argv[0])
            if subprocess.call([path+'/swim/.build/release/swim',
                                'use', languages[m.arguments[0]]]):
                notify('%s: The language switching failed due to failure on calling swim.' %
                       k.name)
    elif m.command == 'handness':
        with QtCore.QMutexLocker(k.handness_session_id_mutex):
            if k.handness_session_id == m.session_id:
                if len(m.arguments) != 1 or m.arguments[0] != 'success':
                    k.handness_session_id = -2
                    log('%s: This handness isn\'t successful.' % k.name)
                else:
                    k.handness_session_id = -1
            else:
                log('%s: This handness isn\'t expected.' % k.name)
    else:
        return False
    return True


def receive_messages():
    for k in keyboards:
        with QtCore.QMutexLocker(k.interface_mutex):
            if k.interface:
                while True:
                    try:
                        m = k.interface.read(32)
                    except:
                        log('%s: Messages may not be received.' % k.name)
                        m = []
                    if not m:
                        break
                    while m and m[-1] == 0:
                        m.pop(-1)
                    if m:
                        m = ''.join([chr(c)for c in m])
                        log('%s: "%s" is being received.' % (k.name, m))
                        m = m.split()
                        try:
                            m = easydict.EasyDict(
                                session_id=int(m[0]), command=m[1], arguments=m[2:])
                        except:
                            log('%s: This messages is corrupted.' % k.name)
                            continue
                        if not handle_message(k, m):
                            log('%s: This messages can\'t be recognized.' % k.name)


receive_messages_timer = QtCore.QTimer()
receive_messages_timer.timeout.connect(receive_messages)
receive_messages_timer.start(5)


def review():
    for k in keyboards:
        log('%s: The review of %s is starting.' % (k.name, k.name))
        if k.interface:
            with QtCore.QMutexLocker(k.heartbeat_session_id_mutex):
                if k.heartbeat_session_id != -1:
                    notify('%s: %s is lost.' % (k.name, k.name))
                    with QtCore.QMutexLocker(k.interface_mutex):
                        k.interface.close()
                        k.interface = None
                else:
                    log('%s: %s is alive.' % (k.name, k.name))
        else:
            for j in range(16):
                try:
                    with QtCore.QMutexLocker(k.interface_mutex):
                        d = hid.device()
                        d.open(k.vendor_id, k.product_id)
                        d.set_nonblocking(1)
                        k.interface = d
                    notify('%s: %s is detected.' % (k.name, k.name))
                    break
                except OSError:
                    pass
        if k.interface:
            with QtCore.QMutexLocker(k.heartbeat_session_id_mutex):
                k.heartbeat_session_id = int(time.time())
                send_message(k, easydict.EasyDict(
                    session_id=k.heartbeat_session_id, command='heartbeat', arguments=[]))
        log('%s: The review of %s is ending.' % (k.name, k.name))


review_timer = QtCore.QTimer()
review_timer.timeout.connect(review)
review_timer.start(1000)


app.exec_()
