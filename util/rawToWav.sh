#!/bin/bash

rm -r wavFiles/
mkdir wavFiles
for f in `ls *.raw`
do
    sox -r 44100 -e signed-integer -b 16 -c 1 -B $f wavFiles/$f.wav
done
