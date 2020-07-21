#!/bin/bash -e
if [ $# -ne 1 ]; then
  echo "One argument, the name of the keyboard, is expected."
  exit 1
fi
keyboard=$1
if [ $1 == dz_60 ]; then
    alias=dz_60
elif [ $1 == matrix_noah ]; then
    alias=matrix/noah
else
    echo $1 is not a supported keyboard.
fi
os=`uname`
if [ $os == "Darwin" ]; then
    if [ ! -x `which realpath|cut -d' ' -f1` ]; then
        brew install coreutils
    fi
else
    echo $os is not a supported OS.
    exit 1
fi
path=`realpath $0`
path=`dirname $path`
if [ $os == Darwin ]; then
    if [ ! -f ~/Library/Application\ Support/qmk/qmk.ini ]; then
        echo QMK\'s config can\'t be detected. Are you sure that you have installed it?
        exit 1
    fi
    cat ~/Library/Application\ Support/qmk/qmk.ini | awk '{if($1=="qmk_home")print$1" "$2" '"$path"/qmk_firmware'";else print$0}' > /tmp/qmk.ini
    cp /tmp/qmk.ini ~/Library/Application\ Support/qmk/qmk.ini
fi
qmk compile -kb $alias -km blockader
if [ $keyboard == matrix_noah ]; then
    echo The firmware for Matrix Noah can\'t be automatically uploaded. You need to write a new bootloader for that.
    for i in {0..9}; do
        echo -ne Please reset your keyboard by pressing the top-left key while plugging in the wire. The firmware uploading will start in $((10-$i)) seconds." \r"
        sleep 1
    done
    echo Please reset your keyboard by pressing the top-left key while plugging in the wire. The firmware uploading will start in 0 seconds.
    cp "$path"/qmk_firmware/`echo $alias|tr / _`_blockader.bin /Volumes/NOAH\ BOOT/FIRMWARE.BIN
fi