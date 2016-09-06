#!/bin/bash
set -e 
set -o pipefail

export HOME_DIR=`pwd`

# Build fft library
if [ "--full" = "$1" ]
then
    cd $HOME_DIR/fftLib
    ./configure
    make
    cd $HOME_DIR
fi

cd $HOME_DIR/util
make
