#!/usr/bin/python

import argparse
import glob

# Input string
# Output object usable from tensorflow
def decode(fileName):
    pass

# Input: directory holding .pcm files
# Output: Array of byte strings
def _readAudioFiles(directoryName):
    fileNames = glob.glob(directoryName + "/*.pcm")
    print "Discovered {0} pcm files in {1}".format(len(fileNames), directoryName)
    binaryStrings = []
    for f in fileNames:
        with open(f, 'r') as myfile:
            binaryStrings.append(myfile.read())
    return binaryStrings
    

# Input: byte string
# Output: array of features
def _extractFeatures(audio):
    pass

if __name__ == '__main__':
    print "This script assumes 16-bit big endian raw PCM"
    parser = argparse.ArgumentParser()
    parser.add_argument('-out', required=True)
    parser.add_argument('-volUp', required=True)
    parser.add_argument('-volDown', required=True)
    parser.add_argument('-noise', required=True)
    args = parser.parse_args()

    binStrs = _readAudioFiles(args.noise)
    for s in binStrs:
        feats = _extractFeatures(s)
        print feats


