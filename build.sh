#!/bin/bash
set -e 
set -o pipefail

export HOME_DIR=`pwd`

# Build fft library
if [ "--full" = "$1" ]
then
    cd $HOME_DIR/fftLib
    ./configure --prefix=$HOME_DIR/fftLib/local/
    make
    make install
    cd $HOME_DIR
fi

cd $HOME_DIR/util
make
