#!/bin/bash -e
os=`uname`
if [ $os == "Darwin" ]; then
    if [ ! -x `which realpath|cut -d' ' -f1` ]; then
        brew install coreutils
    fi
else
    echo $os is not a supported OS.
    exit 1
fi
path=`realpath "$0"`
path=`dirname "$path"`
if [ ! -d "$path"/python ]; then
    python3 -m pip install virtualenv --user 
    python3 -m virtualenv -p python3 "$path"/python
    source "$path"/python/bin/activate
    pip install hidapi easydict pyperclip
    if [ $os == "Darwin" ]; then
        if [ -z "`pip show py2app`" ]; then  
            pip install py2app
        fi
    else
        echo $os is not a supported OS.
        exit 1
    fi
fi
source "$path"/python/bin/activate
pushd $path
rm -rf build dist
set +e
pkill "Matroid Computer"
python setup.py py2app -A
set -e
defaults write org.pythonmac.unspecified.MatroidComputer NSAppSleepDisabled -bool YES
cd swim
swift build -c release
cd ..
open dist/Matroid\ Computer.app
popd $path