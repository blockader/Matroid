import easydict
from PyQt5 import QtCore


keyboards = [
    easydict.EasyDict(
        name='Matrix Noah',
        layout_traits=['split_left_shift'],
        standard_keymap=None,
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
    ),
    easydict.EasyDict(
        name='New Poker',
        layout_traits=[],
        standard_keymap=None,
        vendor_id=0x445A,
        product_id=0x2260,
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
