cd util/
make clean
make
cd ..
rm AllData.csv
rm Validation.csv
TRAINING_SETS="MondayNight WednesdayMorning"
ANDROID_SETS="Android"
for t in $TRAINING_SETS
do
util/ConversionScript $t.csv data/$t/VolumeUp/ data/$t/VolumeDown/ data/$t/Noise/
done

cat `for t in $TRAINING_SETS; do echo $t.csv; done` | shuf > AllData.csv
TOTAL_LINES=`wc -l AllData.csv | cut -f1 -d ' '`
SPLIT_POINT=$(($TOTAL_LINES/10))
head -n $SPLIT_POINT AllData.csv > IosTestingData.csv
tail -n +$(($SPLIT_POINT+1)) AllData.csv > IosTrainingData.csv

for t in $ANDROID_SETS
do
util/ConversionScript $t.csv data/$t/VolumeUp/ data/$t/VolumeDown/ data/$t/Noise/
done

cat `for t in $ANDROID_SETS; do echo $t.csv; done` | shuf > AllAndroid.csv
TOTAL_LINES=`wc -l AllAndroid.csv | cut -f1 -d ' '`
SPLIT_POINT=$(($TOTAL_LINES/10))
head -n $SPLIT_POINT AllAndroid.csv > AndroidTestingData.csv
tail -n +$(($SPLIT_POINT+1)) AllAndroid.csv > AndroidTrainingData.csv

