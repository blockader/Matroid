#!/bin/bash -xe
os=`uname`
if [ $os == "Darwin" ]; then
else
    echo $os is not a supported OS.
fi
