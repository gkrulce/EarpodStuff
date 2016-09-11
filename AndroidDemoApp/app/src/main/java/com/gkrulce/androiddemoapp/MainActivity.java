package com.gkrulce.androiddemoapp;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

public class MainActivity extends AppCompatActivity {
    final static String TAG = "MainActivity";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        recorder = new MicRecorder(new VolumeChangeListener());
        recorder.start();
    }

    private MicRecorder recorder;

    private class VolumeChangeListener implements VolumeChangeEvent {

        @Override
        public void onVolumeUp() {
            Log.v(TAG, "Volume Up!");
        }

        @Override
        public void onVolumeDown() {
            Log.v(TAG, "Volume Down!");
        }

        @Override
        public void onError() {
            Log.v(TAG, "OnError");
        }
    }

}
