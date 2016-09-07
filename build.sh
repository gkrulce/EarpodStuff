#!/bin/bash
set -e 
set -o pipefail

export HOME_DIR=`pwd`

# Build fft library
if [ "--full" = "$1" ]
then
    ## Setup fftw
    cd $HOME_DIR/fftLib
    ./configure --prefix=$HOME_DIR/fftLib/local/
    make
    make install

    # Setup tensorflow
    cd $HOME_DIR/tensorFlow
    virtualenv --system-site-packages tensorflowEnv
    source tensorflowEnv/bin/activate
    export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/mac/cpu/tensorflow-0.10.0-py2-none-any.whl
    pip install --upgrade $TF_BINARY_URL
    deactivate
fi

cd $HOME_DIR/util
make
DATA_DIR="/Users/gkrulce/Desktop/Audio Samples"
./ConversionScript "$HOME_DIR/small.csv" "$DATA_DIR/VolumeUp" "$DATA_DIR/VolumeDown" "$DATA_DIR/Noise"
