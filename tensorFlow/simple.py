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
    return data_all(data)[:n]

def data_all(data):
    volUp = data["VolumeUp"]
    volDown = data["VolumeDown"]
    noise  = data["Noise"]
    toReturn = volUp + volDown + noise, [[1,0,0]]*len(volUp) + [[0,1,0]]*len(volDown) + [[0,0,1]]*len(noise)

    return toReturn

def data_priors(data):
    total = float(len(data["VolumeUp"]) + len(data["VolumeDown"]) + len(data["Noise"]))
    volUp = total/len(data["VolumeUp"])
    volDown = total/len(data["VolumeDown"])
    noise = total/len(data["Noise"])
    arr = [[volUp, 0.0, 0.0],[0.0, volDown, 0.0],[0.0, 0.0, noise]]
    print arr
    priors = tf.constant(arr)
    return priors

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

def init_weights(shape):
    return tf.Variable(tf.random_normal(shape, stddev=0.01))

def loss(calc, ref, priors):
    return tf.reduce_mean(-tf.reduce_sum(ref * tf.log(tf.matmul(calc, priors)), reduction_indices=[1]))

def main(argv):
    assert(len(argv) == 1)
    checkpointDir = argv[0]
    iosTrainingData = "IosTrainingData.csv"
    iosTestingData = "IosTestingData.csv"
    androidTrainingData = "AndroidTrainingData.csv"
    androidTestingData = "AndroidTestingData.csv"
    outputModelFile = "nn.hpp"

    # Import data
    print("Reading IOS training data from {0}, testing data from {1} and writing a checkout files to {2}".format(iosTrainingData, iosTestingData, checkpointDir))
    training_data = data_read(iosTrainingData)
    testing_data = data_read(iosTestingData)
    android_training_data = data_read(androidTrainingData)
    android_testing_data = data_read(androidTestingData)

    inputDim = training_data["FeaturesDim"]
    outputDim = training_data["ClassesDim"]
    hiddenDim = (inputDim + outputDim)/2
    print "Hidden layer dimensions {0}".format(hiddenDim)

    # Main model
    x = tf.placeholder(tf.float32, [None, inputDim])
    w_h = init_weights([inputDim, hiddenDim])
    w_o = init_weights([hiddenDim, outputDim])
    h = tf.sigmoid(tf.matmul(x, w_h))
    y = tf.nn.softmax(tf.matmul(h, w_o))

    # Loss function
    y_ = tf.placeholder(tf.float32, [None, outputDim])
    main_cross_entropy = loss(y, y_, data_priors(training_data))

    # Traning
    main_train_step = tf.train.GradientDescentOptimizer(0.5).minimize(main_cross_entropy)
    init = tf.initialize_all_variables()
    saver = tf.train.Saver()
    with tf.Session() as sess:
        sess.run(init)
        for i in range(5000):
            batch_xs, batch_ys = data_next(training_data, 2000)
            sess.run(main_train_step, feed_dict={x: batch_xs, y_: batch_ys})

            if i % 10 == 0:
                saver.save(sess, checkpointDir + "/main-model-{0}.ckpt".format(i))
                correct_prediction = tf.equal(tf.argmax(y,1), tf.argmax(y_,1))
                accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
                test_xs, test_ys = data_all(testing_data)
                android_xs, android_ys = data_all(android_testing_data)
                testing_accuracy = sess.run(accuracy, feed_dict={x: test_xs, y_: test_ys})
                android_accuracy = sess.run(accuracy, feed_dict={x: android_xs, y_: android_ys})
                print("{0}\tACCURACY\t{1}\tANDROID_ACCURACY\t{2}".format(i, testing_accuracy, android_accuracy))

if __name__ == "__main__":
    main(sys.argv[1:])
