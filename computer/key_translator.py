import os
import sys
import keyboards
import IPython

path = os.path.dirname(sys.argv[0])

for k in keyboards.keyboards:
    t = open((path or './')+'/../keyboard/' +
             k.name.lower().replace(' ', '_')+'/keymap.c').read()
    t = t.replace('\\', ' ').replace(',', ' ').split()
    i = 0
    keymap_permute = [[], []]
    keymap = []
    while i < len(t):
        if t[i] == '#define' and t[i+1] == 'KEYMAP_PERMUTE(':
            i = i+2
            while t[i] != ')':
                keymap_permute[0].append(t[i])
                i += 1
            i = i+2
            while t[i] != '}':
                if t[i].startswith('{'):
                    keymap_permute[1].append([])
                keymap_permute[1][-1].append(
                    t[i].replace('{', '').replace('}', ''))
                i += 1
        if t[i] == '[LAYER_NORM_BASE]' and t[i+2] == 'KEYMAP_PERMUTE(':
            i = i+3
            while t[i] != ')':
                keymap.append(t[i])
                i += 1
        i += 1
    k.standard_keymap = [[], {}]
    for x, y in zip(keymap_permute[0], keymap):
        for i in range(len(keymap_permute[1])):
            for j in range(len(keymap_permute[1][i])):
                if keymap_permute[1][i][j] == x:
                    keymap_permute[1][i][j] = y
    k.standard_keymap[0] = keymap_permute[1]
    for i in range(len(keymap_permute[1])):
        for j in range(len(keymap_permute[1][i])):
            k.standard_keymap[1][keymap_permute[1][i][j]] = (i, j)


def translate(s, t, p):
    k = s.standard_keymap[0][p[0]][p[1]]
    if 'split_left_shift' in s.layout_traits and 'split_left_shift' not in t.layout_traits:
        if k == 'KC_CAPS':
            k = 'KC_ENT'
        elif k == 'KC_ENT':
            k = 'KC_CAPS'
    elif 'split_left_shift' in t.layout_traits and 'split_left_shift' not in s.layout_traits:
        if k == 'KC_ENT':
            k = 'KC_CAPS'
    return t.standard_keymap[1].get(k, None)
