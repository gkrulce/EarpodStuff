cd util/
make clean
make
cd ..
util/ConversionScript MondayNight.csv data/MondayNight/VolumeUp/ data/MondayNight/VolumeDown/ data/MondayNight/Noise/
util/ConversionScript WednesdayMorning.csv data/WednesdayMorning/VolumeUp/ data/WednesdayMorning/VolumeDown/ data/WednesdayMorning/Noise/
cat MondayNight.csv WednesdayMorning.csv > AllData.csv
