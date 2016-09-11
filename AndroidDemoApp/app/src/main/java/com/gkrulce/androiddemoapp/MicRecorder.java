package com.gkrulce.androiddemoapp;

import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.util.Log;

/**
 * Created by gkrulce on 9/11/16.
 */
public class MicRecorder {
    private final static String TAG = "MicRecorder";
    private final static int frameSize_ = 2048;
    private final static int sampleRate_ = 44100;
    private AudioManager audioManager;
    private AudioRecord mic;
    private VolumeChangeEvent event;
    private Thread backgroundThread = null;

    MicRecorder(VolumeChangeEvent event) {
        mic = new AudioRecord(MediaRecorder.AudioSource.MIC, sampleRate_, AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT, 44100);
        this.event = event;
        if (mic.getState() == AudioRecord.STATE_INITIALIZED) {
            Log.i(TAG, "RECORDING FROM MIC");
        } else {
            Log.e(TAG, "NOT RECORDING FROM MIC");
            return;
        }
    }

    void start() {
        Log.v(TAG, "start()");
        if (backgroundThread == null) {
            backgroundThread = new RecordingThread();
            backgroundThread.start();
        }
    }

    void stop() {
        Log.v(TAG, "stop()");
        if (backgroundThread != null) {
            backgroundThread.interrupt();
            backgroundThread = null;
        }
    }

    class RecordingThread extends Thread {
        final static String TAG = "RecordingThread";

        @Override
        public void run() {
            mic = new AudioRecord(MediaRecorder.AudioSource.MIC, sampleRate_, AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT, 44100);
            if (mic.getState() == AudioRecord.STATE_INITIALIZED) {
                Log.i(TAG, "RECORDING FROM MIC");
            } else {
                Log.e(TAG, "NOT RECORDING FROM MIC");
                return;
            }
            short[] buffer = new short[frameSize_];
            mic.startRecording();
            while (!backgroundThread.isInterrupted()) {
                mic.read(buffer, 0, frameSize_);
                Log.v(TAG, "Read some audio");
                // Feed into NN
            }
            mic.release();
        }
    }
}