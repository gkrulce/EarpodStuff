package com.gkrulce.androiddemoapp;

import android.Manifest;
import android.content.pm.PackageManager;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

public class MainActivity extends AppCompatActivity {
    final static String TAG = "MainActivity";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.RECORD_AUDIO}, 420);
    }
    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           String permissions[], int[] grantResults) {
        switch (requestCode) {
            case 420: {
                if (grantResults.length > 0
                        && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    Log.v(TAG, "User granted permissions");
                    recorder = new MicRecorder(new VolumeChangeListener());
                    recorder.start();
                } else {
                    Log.e(TAG, "User denied permissions");
                }
                return;
            }
            default: {
                throw new RuntimeException("Permission not recognized!");
            }
        }
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
