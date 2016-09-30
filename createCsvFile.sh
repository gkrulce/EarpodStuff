#!/bin/bash

set -e
set -o pipefail

HOME_DIR=`pwd`

cd util/
make clean
make
cd $HOME_DIR

rm -f AllData.csv
rm -f Validation.csv
TRAINING_SETS="MondayNight WednesdayMorning MondayMorning"
ANDROID_SETS="Android"

for t in $TRAINING_SETS $ANDROID_SETS
do
cd $HOME_DIR/data/$t
rm -rf VolumeUp
rm -rf VolumeDown
rm -rf Noise
python $HOME_DIR/util/IosDataConversionScript.py
$HOME_DIR/util/ConversionScript $HOME_DIR/$t.csv VolumeUp/ VolumeDown/ Noise/
done

cd $HOME_DIR

cat `for t in $TRAINING_SETS; do echo $t.csv; done` | shuf > AllData.csv
TOTAL_LINES=`wc -l AllData.csv | cut -f1 -d ' '`
SPLIT_POINT=$(($TOTAL_LINES/10))
head -n $SPLIT_POINT AllData.csv > IosTestingData.csv
tail -n +$(($SPLIT_POINT+1)) AllData.csv > IosTrainingData.csv

cat `for t in $ANDROID_SETS; do echo $t.csv; done` | shuf > AllAndroid.csv
TOTAL_LINES=`wc -l AllAndroid.csv | cut -f1 -d ' '`
SPLIT_POINT=$(($TOTAL_LINES/10))
head -n $SPLIT_POINT AllAndroid.csv > AndroidTestingData.csv
tail -n +$(($SPLIT_POINT+1)) AllAndroid.csv > AndroidTrainingData.csv

