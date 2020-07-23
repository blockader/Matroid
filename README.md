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
# My Favourite Keyboards
## New Poker
![New Poker](keyboard/poker/keyboard.jpg)
There are several versions of Poker. My one is New Poker and the picture above is New Poker II. The New Poker seems to have the most freedom in customizing the keymap. However, I replaced its PCB with a DZ60 with QMK support in the end.
## Matrix Noah
![Matrix Noah](keyboard/matrix_noah/keyboard.jpg)
I am very lucky to have a Matrix Noah with an old version of PCB which supports split spacebar. I tried different keymaps to make my thumbs more useful in typing with split spacebar, although I am not sure if they actually worked. I am showing a white one but I have a black one.
## Canoe Gen2
![Canoe Gen2](keyboard/canoe_gen2/keyboard.jpg)
I am still waiting for the shipment. It's hard to modify it for split spacebar, which is the reason I am not using split spacebar for other functions anymore.
