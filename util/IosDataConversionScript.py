#!/usr/bin/python
import os

kSampleRate = 44100 # 44100 samples per second
kSampleWidth = 2 # 2 bytes per sample
kFrameSize = 2205 # In samples
kShift = 0 # In samples

def createDir(dirName):
    if not os.path.exists(dirName):
            os.makedirs(dirName)

def toBytes(idx):
    return idx * kSampleWidth

print("Execute this script in the same folder as events.csv & recording.pcm")
createDir("VolumeUp")
createDir("VolumeDown")
createDir("Noise")
volUpCnt = 0
volDownCnt = 0
noiseCnt = 0
noiseIdx = 0 # In samples
with open("events.csv", "r") as e:
    with open("recording.pcm", "r") as r:
        audio = r.read()
        print("Audio length: {0}".format(len(audio)))
        assert(len(audio) % kSampleWidth == 0)
        for l in e:
            vals = l.split("\t")
            assert(len(vals) == 2)
            if vals[0] == "Shift":
                kShift = int(vals[1])
                continue
            if kShift == 0:
               print ("Warning: must set the shift of this data set!")
               assert(false)

            startIdx = int(float(vals[1]) * kSampleRate) + kShift # In samples
            while(noiseIdx + kFrameSize < startIdx):
                with open("Noise/Audio-{0}.pcm".format(noiseCnt), "w") as f:
                    f.write(audio[toBytes(noiseIdx):toBytes(noiseIdx + kFrameSize)])
                noiseCnt += 1
                noiseIdx += kFrameSize

            noiseIdx = startIdx + kFrameSize
            sample = audio[toBytes(startIdx):toBytes(startIdx + kFrameSize)]
            if(vals[0] == "VolUp"):
                with open("VolumeUp/Audio-{0}.pcm".format(volUpCnt), "w") as f:
                    f.write(sample)
                    volUpCnt += 1
            elif(vals[0] == "VolDown"):
                with open("VolumeDown/Audio-{0}.pcm".format(volDownCnt), "w") as f:
                    f.write(sample)
                    volDownCnt += 1
            else:
                print(vals[0])
                assert(False)

print("Wrote {0} volume up samples".format(volUpCnt))
print("Wrote {0} volume down samples".format(volDownCnt))
print("Wrote {0} noise samples".format(noiseCnt))
