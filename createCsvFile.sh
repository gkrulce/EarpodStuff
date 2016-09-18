cd util/
make clean
make
cd ..
rm AllData.csv
rm Validation.csv
TRAINING_SETS="MondayNight WednesdayMorning"
VALIDATION_SETS="Android"
for t in $TRAINING_SETS
do
util/ConversionScript $t.csv data/$t/VolumeUp/ data/$t/VolumeDown/ data/$t/Noise/
done

cat `for t in $TRAINING_SETS; do echo $t.csv; done` | shuf > AllData.csv
TOTAL_LINES=`wc -l AllData.csv | cut -f1 -d ' '`
SPLIT_POINT=$(($TOTAL_LINES/10))
head -n $SPLIT_POINT AllData.csv > IosTestingData.csv
tail -n +$(($SPLIT_POINT+1)) AllData.csv > IosTrainingData.csv

for t in $VALIDATION_SETS
do
util/ConversionScript $t.csv data/$t/VolumeUp/ data/$t/VolumeDown/ data/$t/Noise/
cat $t.csv >> Validation.csv
done
