#!/usr/bin/python3

import hid
import easydict
import sys
import IPython
import time
import subprocess
import os
import datetime
import pyperclip
from PyQt5 import QtCore, QtGui, QtWidgets


if '--debug' not in sys.argv:
    sys.stdout = open('/tmp/matroid_computer.log', 'w')
stdout_mutex = QtCore.QMutex()


def log(*m):
    with QtCore.QMutexLocker(stdout_mutex):
        print(datetime.datetime.now().strftime("%Y %m %d %H:%M:%S:%f"), *m)
        sys.stdout.flush()


if sys.platform == 'darwin':
    import pync
else:
    log('%s is not a supported OS.' % sys.platform)
    exit(-1)


def notify(*m):
    if sys.platform == 'darwin':
        pync.notify(' '.join(m))
    else:
        log('%s is not a supported OS.' % sys.platform)
    log(*m)


path = os.path.dirname(sys.argv[0])


keyboards = [
    easydict.EasyDict(
        name='Matrix Noah',
        vendor_id=0x4D58,
        product_id=0x0065,
        interface=None,
        interface_mutex=QtCore.QMutex(QtCore.QMutex.Recursive),
        heartbeat_session_id=-1,
        heartbeat_session_id_mutex=QtCore.QMutex(),
        handness_session_id=-1,
        handness_session_id_mutex=QtCore.QMutex(),
        backlight_session_id=-1,
        backlight_session_id_mutex=QtCore.QMutex(),
        menu=None,
        actions=easydict.EasyDict(),
        actions_mutex=QtCore.QMutex()
    )
]


app = QtWidgets.QApplication(sys.argv)
app.setQuitOnLastWindowClosed(False)


tray = QtWidgets.QSystemTrayIcon()
tray.setIcon(QtGui.QIcon(path + '/icon.png'))
tray.setVisible(True)
tray_menu = QtWidgets.QMenu()
for k in keyboards:
    keyboard_menu = QtWidgets.QMenu(k.name)
    keyboard_menu.setEnabled(False)
    k.menu = keyboard_menu
    keyboard_handness_action = QtWidgets.QAction('Handedness', checkable=True)
    keyboard_handness_action.setChecked(True)

    def toggle_handness(checked):
        with QtCore.QMutexLocker(k.handness_session_id_mutex):
            k.handness_session_id = int(time.time())
            send_message(k, easydict.EasyDict(
                session_id=k.handness_session_id, command='handness', arguments=[int(checked)]))
    keyboard_handness_action.toggled.connect(toggle_handness)
    keyboard_menu.addAction(keyboard_handness_action)
    k.actions.handness_action = keyboard_handness_action
    keyboard_backlight_action = QtWidgets.QAction('Backlight', checkable=True)

    def toggle_backlight(checked):
        with QtCore.QMutexLocker(k.backlight_session_id_mutex):
            k.backlight_session_id = int(time.time())
            send_message(k, easydict.EasyDict(
                session_id=k.backlight_session_id, command='backlight', arguments=[int(checked)]))
    keyboard_backlight_action.toggled.connect(toggle_backlight)
    keyboard_menu.addAction(keyboard_backlight_action)
    k.actions.backlight_action = keyboard_backlight_action
    tray_menu.addMenu(keyboard_menu)
tray_quit_action = QtWidgets.QAction('Quit')
tray_quit_action.triggered.connect(app.quit)
tray_menu.addAction(tray_quit_action)
tray.setContextMenu(tray_menu)


class timer(QtCore.QThread):
    triggered = QtCore.pyqtSignal()

    def __init__(self, interval, job):
        super(timer, self).__init__()
        self.interval = interval
        self.job = job

    def run(self):
        while True:
            now = time.perf_counter()
            self.job()
            self.triggered.emit()
            time.sleep(max(self.interval - (time.perf_counter() - now), 0))


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
            if subprocess.call([path+'/swim/.build/release/swim',
                                'use', languages[m.arguments[0]]]):
                notify('%s: The language switching failed due to failure on calling swim.' %
                       k.name)
            else:
                notify('%s: The language is now %s.' %
                       (k.name, m.arguments[0]))
    elif m.command == 'handness':
        if len(m.arguments) != 1:
            log('%s: This handness is corrupted.' % k.name)
        elif m.arguments[0].isdigit():
            with QtCore.QMutexLocker(k.actions_mutex):
                k.actions.handness_action.setChecked(int(m.arguments[0]))
                notify('%s: Handness is now %s.' %
                       (k.name, 'on' if k.actions.handness_action.isChecked() else 'off'))
        else:
            with QtCore.QMutexLocker(k.handness_session_id_mutex):
                if k.handness_session_id == m.session_id:
                    if m.arguments[0] != 'success':
                        k.handness_session_id = -2
                        log('%s: This handness isn\'t successful.' % k.name)
                    else:
                        k.handness_session_id = -1
                        with QtCore.QMutexLocker(k.actions_mutex):
                            notify('%s: Handness is now %s.' %
                                   (k.name, 'on' if k.actions.handness_action.isChecked() else 'off'))
                else:
                    log('%s: This handness isn\'t expected.' % k.name)
    elif m.command == 'backlight':
        if len(m.arguments) != 1:
            log('%s: This backlight is corrupted.' % k.name)
        elif m.arguments[0].isdigit():
            with QtCore.QMutexLocker(k.actions_mutex):
                k.actions.backlight_action.setChecked(int(m.arguments[0]))
                notify('%s: Backlight is now %s.' %
                       (k.name, 'on' if k.actions.backlight_action.isChecked() else 'off'))
        else:
            with QtCore.QMutexLocker(k.backlight_session_id_mutex):
                if k.backlight_session_id == m.session_id:
                    if m.arguments[0] != 'success':
                        k.backlight_session_id = -2
                        log('%s: This backlight isn\'t successful.' % k.name)
                    else:
                        k.backlight_session_id = -1
                        with QtCore.QMutexLocker(k.actions_mutex):
                            notify('%s: Backlight is now %s.' %
                                   (k.name, 'on' if k.actions.backlight_action.isChecked() else 'off'))

                else:
                    log('%s: This backlight isn\'t expected.' % k.name)
    elif m.command == 'time':
        if len(m.arguments) != 1 or not m.arguments[0].isdigit():
            log('%s: This time is corrupted.' % k.name)
        elif time.perf_counter()*1000 > int(m.arguments[0]):
            notify('%s: The config is being uploading to the keyboard.' % k.name)
            with QtCore.QMutexLocker(k.actions_mutex):
                send_message(k, easydict.EasyDict(
                    session_id=int(time.time()), command='handness', arguments=[1 if k.actions.handness_action.isChecked() else 0]))
                send_message(k, easydict.EasyDict(
                    session_id=int(time.time()), command='backlight', arguments=[1 if k.actions.backlight_action.isChecked() else 0]))
        else:
            notify('%s: The config is being downloading from the keyboard.' % k.name)
            with QtCore.QMutexLocker(k.actions_mutex):
                send_message(k, easydict.EasyDict(
                    session_id=int(time.time()), command='handness', arguments=[-1]))
                send_message(k, easydict.EasyDict(
                    session_id=int(time.time()), command='backlight', arguments=[-1]))
    elif m.command == 'notify':
        if len(m.arguments) != 1 or not m.arguments[0].isdigit():
            log('%s: This notify is corrupted.' % k.name)
        else:
            notify('%s: %s' % (k.name, m.arguments[0]))
    else:
        return False
    return True


def receive_messages():
    for k in keyboards:
        if k.interface:
            while True:
                try:
                    with QtCore.QMutexLocker(k.interface_mutex):
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


receive_messages_timer = timer(0.005, receive_messages)


def five_secondly_routine():
    application = None
    for k in keyboards:
        with QtCore.QMutexLocker(k.interface_mutex):
            if k.interface:
                if not application:
                    if sys.platform == 'darwin':
                        application = subprocess.run(['osascript', '-e', 'tell application "System Events"', '-e',
                                                      'set t to name of first application process whose frontmost is true', '-e', 'end tell'], stdout=subprocess.PIPE).stdout.strip().decode()
                    else:
                        log('%s is not a supported OS.' % sys.platform)
                    log('The current application is %s.' % application)
                    application = {'Terminal': 1}.get(application, 0)
                send_message(k, easydict.EasyDict(
                    session_id=int(time.time()), command='application', arguments=[application]))


five_secondly_routine_timer = timer(5, five_secondly_routine)


previous_clipboard = ''


def five_minutely_routine():
    global previous_clipboard
    if previous_clipboard == pyperclip.paste() and previous_clipboard:
        pyperclip.copy('')
        log('The clipboard is cleared.')
    else:
        previous_clipboard = pyperclip.paste()


five_minutely_routine_timer = timer(5 * 60, five_minutely_routine)


def hourly_routine():
    pass


break_dialog = QtWidgets.QMessageBox()
break_dialog.setInformativeText(
    'You should take a break to relax your body, clear your mind and schedule things. Remeber laziness is efficiency.')
break_dialog.setWindowFlags(
    QtCore.Qt.WindowStaysOnTopHint)


def show_break_dialog():
    if not break_dialog.isVisible():
        break_dialog.exec_()


hourly_routine_timer = timer(60 * 60, hourly_routine)
hourly_routine_timer.triggered.connect(show_break_dialog)


def daily_routine():
    with QtCore.QMutexLocker(stdout_mutex):
        if '--debug' not in sys.argv:
            sys.stdout.close()
            lines = open('/tmp/matroid_computer.log').readlines()
            lines = lines[-1024:]
            sys.stdout = open('/tmp/matroid_computer.log', 'w')
            for line in lines:
                sys.stdout.write(line)


daily_routine_timer = timer(24 * 60 * 60, daily_routine)


class ReviewTimer(QtCore.QThread):
    connected = QtCore.pyqtSignal(int)
    lost = QtCore.pyqtSignal(int)

    def __init__(self, interval):
        super(ReviewTimer, self).__init__()
        self.interval = interval

    def run(self):
        while True:
            now = time.perf_counter()
            for i, k in enumerate(keyboards):
                log('%s: The review of %s is starting.' % (k.name, k.name))
                if k.interface:
                    with QtCore.QMutexLocker(k.heartbeat_session_id_mutex):
                        if k.heartbeat_session_id != -1:
                            notify('%s: The keyboard is lost.' % k.name)
                            with QtCore.QMutexLocker(k.interface_mutex):
                                k.interface.close()
                                k.interface = None
                            self.lost.emit(i)
                        else:
                            log('%s: The keyboard is alive.' % k.name)
                else:
                    for j in range(16):
                        try:
                            with QtCore.QMutexLocker(k.interface_mutex):
                                d = hid.device()
                                d.open(k.vendor_id, k.product_id)
                                d.set_nonblocking(1)
                                k.interface = d
                            notify('%s: The keyboard is connected.' % k.name)
                            send_message(k, easydict.EasyDict(
                                session_id=int(time.time()), command='time', arguments=[]))
                            self.connected.emit(i)
                            break
                        except OSError:
                            pass
                if k.interface:
                    with QtCore.QMutexLocker(k.heartbeat_session_id_mutex):
                        k.heartbeat_session_id = int(time.time())
                        send_message(k, easydict.EasyDict(
                            session_id=k.heartbeat_session_id, command='heartbeat', arguments=[]))
                log('%s: The review of %s is ending.' % (k.name, k.name))
            time.sleep(max(self.interval - (time.perf_counter() - now), 0))


review_timer = ReviewTimer(1)
review_timer.lost.connect(lambda i: keyboards[i].menu.setEnabled(False))
review_timer.connected.connect(lambda i: keyboards[i].menu.setEnabled(True))


receive_messages_timer.start()
review_timer.start()
daily_routine_timer.start()
hourly_routine_timer.start()
five_minutely_routine_timer.start()
five_secondly_routine_timer.start()


app.exec_()
