# Matroid
This repo includes keyboard firmware code to largely enhance QMK-based customized keyboards and GUI software to manage them. The configuration is very personal but the code can easily be reused for those who are familiar with QMK. All code is designed to work on MacOS, Linux and Windows without modification. This is an ongoing personal project out of pure interest. I am also planning to work on keyboard hardware in the future.
# Structure
## keyboard/*/keymap.c
This code implements different keymaps for each keyboard as they may vary in the number and layout of keys. They are all using Colemak layout.
## keyboard/common/matroid.h
This code implements Vim-like operations in the keyboard firmware. It also accepts various commands from the computer via a USB connection, which enables keyboard configuration on the computer.
## keyboard/install.h
This code compiles and installs the firmware, handling all details.
## computer/computer.py
This code implements a GUI keyboard manager using PyQt.
## computer/install.h
This code installs the keyboard manager, handling all details.
