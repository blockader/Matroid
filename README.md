# Introduction
This repository includes keyboard firmware code (C and C++) to largely enhance QMK-based (an open-source keyboard firmware project) custom keyboards and GUI software (Python and PyQt) to manage them. In a word, it adds several useful features to a keyboard. The configuration is personal but the code can easily be reused for those who are familiar with QMK. This is an ongoing project out of interest.
# Motivation
## The Drawbacks of Normal Keyboards
1. The positions of alphabet keys are not optimized for high-speed typing.
2. (, -, +, Enter, ↑, ←, ↓, → and Backspace are among the most used keys for programmers, but reaching them requires large hand movement from the home row.
3. The workload on each finger is not balanced. The two thumbs are only responsible for one spacebar, while the right pinky finger has to press too many frequent keys. This could potentially cause errors.
4. Keys for moving to the previous or next word, moving to the beginning or end of the line or erasing the current word don't even exist. These keys can be useful for fast coding. I know some shortcuts may work, but they vary in different OSs or even applications or require too many key presses. It would be good to provide a unified and easy solution.
5. Some useful features like double tapping are missing. It would be good if I can go to the end of the document by double tap the page down key. It's much more intuitive than shortcuts. It would be even better if we can have marcos, command repeating and other advanced features.
6. The staggered alignment came from old typewriters. This design no longer makes senses on modern keyboards and it makes some keys hard to reach.
7. The two hands are too close which is an unnatural posture. 
## Existing Solutions
1. Vim, Emacs and other similar software can solve some of the above problems. Although with the help of plugins, Vim can be used in other applications, they are still available in limited applications. I want to have the same typing experience when I am writing an email, using the terminal and even taking notes with keyboard connecting to an iPad.
2. Karabiner-Elements and other similar software remaps the keys for the entire operating system. Still, they can't provide unified support across different operating systems. Besides, some of them are not flexible enough to run custom code.
3. Custom keyboards with QMK-supported PCBs seem to be a better solution. All the implementation is in the keyborad firmware, so it works everywhere. Some custom keyboards, such as ErgoDox EZ, are split and ortholinear, which also solves the last two drawbacks. My code is based on the QMK project and it adds some advanced features. I also have pull requests that are merged to QMK.
# Features
1. The keymaps are optimized based on Colemak to support fast typing for programming. The location of most common symbols for programming are placed closer to the center. The symbols are actually sharing keys with letters with support from two additional modifiers.
2. Additional usesful keys are added. For example, use one key to create a new next line and move to its beginning when your cursor is in the middle of a line or to delete a word or the entire line.
2. It includes multiple layers to handle different situations, for example, a layer that works almost the same as Vim in its normal mode. For example, one can type 2, d and d to delete two lines in Vim. With this, one can do it anywhere! No software is needed.
3. The keyboard will constantly communicate with the OS to perform more user-friendly operations. For example, the keyboard will behave differently based on the current focused application and OS. If you press the moving-back-a-word key, the keyboard will translate it to a shortcut that the current software can recognize. The OS will also change its state, for example, switching the input source, based on the current layer. Please note the the main purpose of this repository is to provide a hardware independent solution, but a GUI manager runs on the OS can also provide additional support when possible.
4. Multiple keyboards can share internal states (current layer, current modifieres, all settings, etc.). This can occasionaly be useful when using multiple keyboards.
6. Some features such as key repeating are now handled by the keyboard instead of the OS to provide more customized experience.
7. It also have some details to help improve typing. For example, some keys will be disabled based on the position of the previously pressed modifier key to guide the user to efficiently use both hands. For a keyboard with a split spacebar, one of the space key may be disabled.
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
All the above layout designs are empirical. I am planning to use data to automatically discover layouts that are best for computer programmers. I am also planning to work on keyboard hardware in the future.
# Introduction to Custom Keyboards
These keyboards are generally more carefully designed. You usually have to buy several parts and put them together yourself, which means you can choose your own keycaps and switches. They usually come with a QMK-supported PCB, and you can write your own C code to make them do things you want. The below picture is a new custom keyboard called Canoe Gen2 produced by Percent Studio. ![Canoe Gen2](https://cdn.shopify.com/s/files/1/0162/8117/4080/products/canoe_gen2_panda_1080x.png?v=1596003849) Some other keyboards are more unique in their designs. They try to solve the drawbacks by completely changing how a keyboard looks like. The below picture is a new ergonomic keyboard called Moonlander Mark 1 produced by ZSA Technology Labs. ![Moonlander Mark 1](https://www.zsa.io/static/b42dcd715c76bd330737b385b2a218a6/0838a/1-hero-white.webp) You can find lots of them on [GeekHack](https://geekhack.org/index.php?board=70.0).
