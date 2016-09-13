#!/bin/bash
set -e 
set -o pipefail

export HOME_DIR=`pwd`

# Build fft library
if [ "--full" = "$1" ]
then
    # Setup tensorflow
    cd $HOME_DIR/tensorFlow
    virtualenv --system-site-packages tensorflowEnv
    source tensorflowEnv/bin/activate
    export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/mac/cpu/tensorflow-0.10.0-py2-none-any.whl
    pip install --upgrade $TF_BINARY_URL
    deactivate
fi

# CPP util
cd $HOME_DIR/util
make clean
make
DATA_DIR="$HOME_DIR/data/MondayNight"
./ConversionScript "$HOME_DIR/small.csv" "$DATA_DIR/VolumeUp" "$DATA_DIR/VolumeDown" "$DATA_DIR/Noise"

# Model training
cd $HOME_DIR/tensorFlow
source tensorflowEnv/bin/activate
python simple.py $HOME_DIR/small.csv $HOME_DIR/util/nn.hpp
deactivate

cd $HOME_DIR
util/EarpodModelTest
