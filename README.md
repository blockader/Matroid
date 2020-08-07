# Introduction
This repository includes keyboard firmware code (C and C++) to largely enhance QMK-based (an open-source keyboard firmware project) customized keyboards and GUI software (Python and PyQt) to manage them. In a word, it adds several useful features to a keyboard. The configuration is personal but the code can easily be reused for those who are familiar with QMK. This is an ongoing project out of interest. I am also planning to work on keyboard hardware in the future.

# Motivation
## The Drawbacks of Normal Keyboards
1. The positions of alphabet keys are not optimized for high-speed typing.
2. (, -, +, Enter, ↑, ←, ↓ and → are among the most used keys for programmers, but reaching them requires large hand movement from the home row.
4. The workload on each finger is not balanced. The two thumbs are only responsible for one spacebar, while the right pinky finger has to press too many frequent keys. This could potentially cause errors.
5. The staggered alignment came from old typewriters. This design no longer makes senses on modern keyboards and it makes some keys hard to reach.


2. Some useful keys such as arrow keys are too far to reach. And keys for moving to the previous/next word, moving to the beginning or end of the line and erasing the whole line don't even exist. These keys can be useful for fast coding. I know some shortcuts may work, but they vary in different OSs or even applications and are not handy. It would be good to provide a unified solution. And in many cases like writing email, taking a note or using the terminal, Vim may not be available.
3. Two hands are too close to make long time typing not comfortable.
4. The keys are not aligned. This is not a clever design but merely path dependency.
5. The locations of the keys are designed to slow down typing. See Colemak or Dvorak.
6. Some useful features like double tapping are missing. When I double tapping the page down key in my keyboard, it actually goes to the end of the document. Although this is not necessary, I find it very interesting and user-friendly. If your have a custmized keyboard with QMK support, then you can write you own C code to achieve this and more.
## What should we do?
As a computer programmer, I type a lot every day. So the efficiency matters. There are many ways to solve some of these problems. I am doing things in the firmware. Please check out Karabiner-Elements for hardware independent solution. Vim and Emacs with careful configuration can also help in some cases. The advantage of modifying the firmware is that you don't have to develop and install any software on a new computer/OS to make most things work. I can easily plug my keyboard in an iPad and start to take notes with my keyboard features. Although a few advanced functions do require a keyboard manager I implemented below.

Some problems can only be solved with new hardware design. See ones like ErgoDox and ortholinear keyboards.
# Features
1. The keymaps are optimized based on Colemak to support fast typing for programming. The location of most common symbols for programming are placed closer to the center and additional usesful keys are added. The symbols are actually sharing keys with letters with support from two additional modifiers.
2. It includes multiple layers to handle different situations, for example, a layer that works almost the same as Vim in its normal mode and a layer that is for TypeRacer only. This means that you can enjoy the efficiency of Vim anywhere as long as you have your keyboard. No software is needed.
3. The keyboard will constantly communicate with the OS to perform more user-friendly operations. For example, the keyboard will behave differently based on the current focused application and OS. If you press the moving-back-a-word key, the keyboard will translate it to a shortcut that the current software can recognize. The OS will also change its state, for example, switching the input source, based on the current layer.
4. Multiple keyboards can share internal states (current layer, current modifieres, all settings, etc.). This means using one keyboard each hand will be very enjoyble. You may ask why would anyone want to use two keyboards at the same time. The reason is exactly the same as why we use split keyboards. The positions of the wrists will be more healthy and the typing will be easier.
5. It also have some details to help improve typing. For example, some keys will be disabled based on the position of the previously pressed modifier key to guide the user to efficiently use both hands. For a keyboard with a split spacebar, one of the space key may be disabled.
##
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
# Future Work
I really want to do an automatic keyboard layout search for coding. This requires two things. One is an accurate enough model of hands and their movement. The other is a lot of data. The second one is apparently easy. The first one should not be very hard but I don't have time yet.
# My Favourite Keyboards
## New Poker
![New Poker](keyboard/new_poker/keyboard.jpg)
There are several versions of Poker. My one is New Poker and the picture above is New Poker II. The New Poker seems to have the most freedom in customizing the keymap. However, I replaced its PCB with a DZ60 with QMK support in the end.
## Matrix Noah
![Matrix Noah](keyboard/matrix_noah/keyboard.jpg)
I am very lucky to have a Matrix Noah with an old version of PCB which supports split spacebar. I tried different keymaps to make my thumbs more useful in typing with split spacebar, although I am not sure if they actually worked. I am showing a white one but I have a black one.
## Canoe Gen2
![Canoe Gen2](keyboard/canoe_gen2/keyboard.jpg)
I am still waiting for the shipment. It's hard to modify it for a split spacebar, which is the reason I am not using split spacebars for other functions anymore.
