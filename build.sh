#!/bin/bash
set -e 
set -o pipefail

HOME=`pwd`
cd $HOME/fftLib
./configure
make
