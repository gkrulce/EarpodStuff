#!/usr/bin/python
import os

def createDir(dirName):
    if not os.path.exists(dirName):
            os.makedirs(dirName)
kSampleRate = 44100 # 44100 samples per second
kSampleWidth = 2 # 2 bytes per sample
kFrameSize = 8820
print("Execute this script in the same folder as events.csv & recording.pcm")
createDir("VolumeUp")
createDir("VolumeDown")
createDir("Noise")
volUpCnt = 0
volDownCnt = 0
with open("events.csv", "r") as e:
    with open("recording.pcm", "r") as r:
        audio = r.read()
        print("Audio length: {0}".format(len(audio)))
        assert(len(audio) % kSampleWidth == 0)
        currIdx = 0 # In samples
        for l in e:
            vals = l.split("\t")
            assert(len(vals) == 2)
            startIdx = int(float(vals[1]) * kSampleRate * kSampleWidth)
            if(startIdx % 2 == 1):
                startIdx -= 1
            sample = audio[startIdx:startIdx+kFrameSize]
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
