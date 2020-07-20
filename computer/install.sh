#!/bin/bash -xe
os=`uname`
if [ $os == "Darwin" ]; then
    brew install hidapi
else
    echo $os is not a supported OS.
fi
