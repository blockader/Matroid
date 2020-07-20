#!/bin/bash -xe
os=`uname`
if [ $os == "Darwin" ]; then
    if [ ! -x `which realpath|cut -d' ' -f1` ]; then
        brew install coreutils
    fi
else
    echo $os is not a supported OS.
    exit
fi
path=`realpath $0`
path=`dirname $0`
if [ $os == "Darwin" ]; then
    if [ ! -f ~/Library/Application\ Support/qmk/qmk.ini ]; then
        echo QMK\'s config can\'t be detected. Are you sure that you have installed it?
        exit
    fi
    cat ~/Library/Application\ Support/qmk/qmk.ini | awk '{if($1=="qmk_home")print$1" "$2" '"$path/qmk_firmware"'";else print$0}' > /tmp/qmk.ini
    cp /tmp/qmk.ini ~/Library/Application\ Support/qmk/qmk.ini
fi
qmk compile -k $1 -m blockader
mv $path/qmk_firmware/$1_blockader.hex $path/$1