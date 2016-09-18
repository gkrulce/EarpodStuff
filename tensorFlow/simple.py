#!/usr/bin/python

import tensorflow as tf
import csv
from random import shuffle
import numpy
import sys
import random

# Should make a python dictionary here, shuffle it, and take the first X number
# Input: csv filename
# Output: Data structure that can be used in other functions
def data_read(fileName):
    data = {}
    with open(fileName) as f:
        data["VolumeUp"] = []
        data["VolumeDown"] = []
        data["Noise"] = []
        arrSize = None
        for line in f:
            csv = [val.rstrip("\n") for val in line.split(",")]
            assert(arrSize == None or arrSize == len(csv))
            arrSize = len(csv)
            if csv[0] == "VolUp":
                data["VolumeUp"].append(csv[1:])
            elif csv[0] == "VolDown":
                data["VolumeDown"].append(csv[1:])
            elif csv[0] == "Noise":
                data["Noise"].append(csv[1:])
            else:
                assert(false)
        
        data["FeaturesDim"] = arrSize-1
        data["ClassesDim"] = 3
        print "Read {0}".format(fileName)
        print "Number of output classes {0}".format(data["ClassesDim"])
        print "Number of features {0}".format(data["FeaturesDim"])
        print "Data size (VolUp, VolDown, Noise): ({0},{1},{2})".format(len(data["VolumeUp"]), len(data["VolumeDown"]), len(data["Noise"]))
    return data

# Input: data and number of samples
# Output: tuple of random number of samples (input values, 1-hot output values)
def data_next(data, n): 
    volUp = random.sample(data["VolumeUp"], n)
    volDown = random.sample(data["VolumeDown"], n)
    noise  = random.sample(data["Noise"], n)
    assert(len(volUp) == len(volDown) == len(noise))
    l = len(volUp)
    toReturn = volUp + volDown + noise, [[1,0,0]]*l + [[0,1,0]]*l + [[0,0,1]]*l
    return toReturn

def data_all(data):
    volUp = data["VolumeUp"]
    volDown = data["VolumeDown"]
    noise  = data["Noise"]
    toReturn = volUp + volDown + noise, [[1,0,0]]*len(volUp) + [[0,1,0]]*len(volDown) + [[0,0,1]]*len(noise)

    return toReturn

# name: Name of the matrix
# npArr: The numpy array of this matrix
# The file name we are APPENDING to
def dumpToFile(name, npArr, fileName):
    npShape = numpy.shape(npArr)
    rows = numpy.shape(npArr)[0]
    cols = numpy.shape(npArr)[1]

    with open(fileName, "a") as oF:
        oF.write("#define {0}_ROWS {1}\n".format(name, rows))
        oF.write("#define {0}_COLS {1}\n".format(name, cols))
        oF.write("static float {0}_DATA[{1}][{2}] = {{ \n".format(name, rows, cols))
        for l in npArr:
            oF.write("{{{0}}}, \n".format(",".join([str(val) for val in l])))
        oF.write("}; \n");

def main(argv):
    assert(len(argv) == 1)
    checkpointDir = argv[0]
    iosTrainingData = "IosTrainingData.csv"
    iosTestingData = "IosTestingData.csv"
    #androidTrainingData = ""
    #androidTestingData = ""

    # Import data
    print("Reading IOS training data from {0}, testing data from {1} and writing a checkout files to {2}".format(iosTrainingData, iosTestingData, checkpointDir))
    training_data = data_read(iosTrainingData)
    testing_data = data_read(iosTestingData)

    inputDim = training_data["FeaturesDim"]
    outputDim = training_data["ClassesDim"]

    # Model
    x = tf.placeholder(tf.float32, [None, inputDim])
    W = tf.Variable(tf.zeros([inputDim, outputDim]))
    b = tf.Variable(tf.zeros([outputDim]))
    y = tf.nn.softmax(tf.matmul(x, W) + b)

    # Loss function
    y_ = tf.placeholder(tf.float32, [None, outputDim])
    cross_entropy = tf.reduce_mean(-tf.reduce_sum(y_ * tf.log(y), reduction_indices=[1]))

    # Traning
    train_step = tf.train.GradientDescentOptimizer(0.0001).minimize(cross_entropy)
    init = tf.initialize_all_variables()
    saver = tf.train.Saver()
    with tf.Session() as sess:
        sess.run(init)
        for i in range(5000):
            batch_xs, batch_ys = data_next(training_data,200)
            sess.run(train_step, feed_dict={x: batch_xs, y_: batch_ys})

            if i % 500 == 0:
                saver.save(sess, checkpointDir + "/model-{0}.ckpt".format(i))
                correct_prediction = tf.equal(tf.argmax(y,1), tf.argmax(y_,1))
                accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
                test_xs, test_ys = data_all(testing_data)
                testing_accuracy = sess.run(accuracy, feed_dict={x: test_xs, y_: test_ys})
                print("{0}\tACCURACY\t{1}".format(i, testing_accuracy))

if __name__ == "__main__":
    main(sys.argv[1:])
