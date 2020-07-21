#!/usr/bin/python3

import hid
import IPython


d = hid.device()
while True:
    try:
        d.open(0x4D58, 0x0065)
        break
    except:
        continue


def send(m):
    m = [ord(c) for c in m]
    assert len(m) <= 32
    m = [0]+m+[0]*(32-len(m))
    d.write(m)


def receive():
    m = d.read(32)
    while not m[-1]:
        m.pop(-1)
    return ''.join([chr(c)for c in m])


IPython.embed()