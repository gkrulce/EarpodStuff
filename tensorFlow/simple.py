#!/usr/bin/python

import tensorflow as tf
import csv
from random import shuffle
import numpy
import sys

def one_hot(nameList, name):
    assert(nameList.count(name) == 1)
    l = [0] * len(nameList)
    for idx, val in enumerate(nameList):
        if name == val:
            l[idx] = 1
            return l
    assert(false)

# Should make a python dictionary here, shuffle it, and take the first X number
# Input: csv filename
# Output: Data structure that can be used in other functions
def data_read(fileName):
    
    with open(fileName) as f:
        arr = []
        data = {}
        arrSize = None
        for line in f:
            csv = [val.rstrip("\n") for val in line.split(",")]
            assert(arrSize == None or arrSize == len(csv))
            arrSize = len(csv)
            arr.append({"class":csv[0], "features":csv[1:]})
        
        outputClasses = ["VolUp", "VolDown", "Noise"]
        print "Output classes: {0}".format(outputClasses)
        for idx, val in enumerate(arr):
            arr[idx]["class"] = one_hot(outputClasses, arr[idx]["class"])

        data["FeaturesDim"] = len(arr[0]["features"])
        data["ClassesDim"] = len(outputClasses)

        shuffle(arr)
        dataSetSize = len(arr)
        cutPoint = int(dataSetSize * .9) # 10% test, 90% training
        data["TrainingData"] = arr[:cutPoint]
        data["TestData"] = arr[cutPoint:]

        print "Number of output classes {0}".format(data["ClassesDim"])
        print "Number of features {0}".format(data["FeaturesDim"])
        print "Traning data size: {0}".format(len(data["TrainingData"]))
        print "Test data size: {0}".format(len(data["TestData"]))
        
    return data

# Input: data and number of samples
# Output: tuple of random number of samples (input values, 1-hot output values)
def data_next(data, n): 
    shuffle(data["TrainingData"])
    toTrain = data["TrainingData"][:n]
    return ([val["features"] for val in toTrain], [val["class"] for val in toTrain])

# Input: data
# Output: testing data tuple (input values, 1-hot output values)
def data_test(data):
    toTest = data["TestData"]
    return ([val["features"] for val in toTest], [val["class"] for val in toTest])

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
    assert(len(argv) == 2)
    inputCSVFile = argv[0]
    outputModelFile = argv[1]
    print("Reading CSV file located at {0} and writing a model file located at {1}".format(inputCSVFile,
    outputModelFile))
# Import data
    data = data_read(inputCSVFile)

    inputDim = data["FeaturesDim"]
    outputDim = data["ClassesDim"]

# Model
    x = tf.placeholder(tf.float32, [None, inputDim])
    W = tf.Variable(tf.zeros([inputDim, outputDim]))
    b = tf.Variable(tf.zeros([outputDim]))
    y = tf.nn.softmax(tf.matmul(x, W) + b)

# Loss function
    y_ = tf.placeholder(tf.float32, [None, outputDim])
    cross_entropy = tf.reduce_mean(-tf.reduce_sum(y_ * tf.log(y), reduction_indices=[1]))

# Traning
    train_step = tf.train.GradientDescentOptimizer(0.01).minimize(cross_entropy)
    init = tf.initialize_all_variables()
    with tf.Session() as sess:
        sess.run(init)
        for i in range(2500):
            batch_xs, batch_ys = data_next(data,200)
            sess.run(train_step, feed_dict={x: batch_xs, y_: batch_ys})

            if i % 50 == 0:
                correct_prediction = tf.equal(tf.argmax(y,1), tf.argmax(y_,1))
                accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
                test_xs, test_ys = data_test(data)
                print("RESULT: {0}".format(sess.run(accuracy, feed_dict={x: test_xs, y_: test_ys})))
                sys.stdout.flush()

        with open(outputModelFile, "w") as f:
            f.truncate()
        dumpToFile("W", W.eval(), outputModelFile)
        dumpToFile("B", [b.eval()], outputModelFile)

if __name__ == "__main__":
    main(sys.argv[1:])
