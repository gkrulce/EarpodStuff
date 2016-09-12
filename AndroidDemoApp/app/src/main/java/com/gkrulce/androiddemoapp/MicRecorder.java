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
    private AudioRecord mic;
    private VolumeChangeEvent event;
    private Thread backgroundThread = null;
    private JEarpodModel model_;

    MicRecorder(VolumeChangeEvent event) {
        this.event = event;
        model_ = new JEarpodModel("nnSimple.model");
        System.loadLibrary("EarpodModel");
        mic = new AudioRecord(MediaRecorder.AudioSource.MIC, sampleRate_, AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT, sampleRate_);
        if (mic.getState() == AudioRecord.STATE_INITIALIZED) {
            Log.i(TAG, "RECORDING FROM MIC");
        } else {
            Log.e(TAG, "NOT RECORDING FROM MIC");
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
            short[] buffer = new short[frameSize_];
            if(mic.getState() != mic.STATE_INITIALIZED) {
                Log.v(TAG, "Can't record from uninitialized mic!");
                return;
            }
            mic.startRecording();
            while (!backgroundThread.isInterrupted()) {
                mic.read(buffer, 0, frameSize_);
                Log.v(TAG, "Read some audio");
                JEarpodModel.Token[] tokens = model_.read(buffer);
                for(int i = 0; i < tokens.length; ++i) {
                    if(tokens[i] == JEarpodModel.Token.VOLUME_DOWN) {
                        event.onVolumeDown();
                    }

                    if(tokens[i] == JEarpodModel.Token.VOLUME_UP) {
                        event.onVolumeUp();
                    }
                }
            }
            mic.release();
        }
    }
}