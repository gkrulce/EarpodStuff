package com.gkrulce.androiddemoapp;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    final static String TAG = "MainActivity";
    private MicRecorder recorder;
    Toast cToast;
    private Handler mHandler = new Handler(Looper.getMainLooper()) {
        @Override
        public void handleMessage(Message inputMessage) {
            String message = (String) inputMessage.obj;
            if(inputMessage.what == 0) {
                if(cToast != null) {
                    cToast.cancel();
                }
                Log.v(TAG, message);
                Context context = getApplicationContext();
                int duration = Toast.LENGTH_SHORT;
                cToast = Toast.makeText(context, message, duration);
                cToast.show();
            }else {
                throw new RuntimeException("Message had error code.");
            }
        }
    };
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    @Override
    protected void onStart() {
        super.onStart();
        ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.RECORD_AUDIO}, 420);
    }

    @Override
    protected void onStop() {
        super.onStop();
        if(recorder != null) {
            recorder.stop();
            recorder = null;
        }
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

    private class VolumeChangeListener implements VolumeChangeEvent {

        @Override
        public void onVolumeUp() {
            mHandler.obtainMessage(0, "Volume Up!").sendToTarget();
        }

        @Override
        public void onVolumeDown() {
            mHandler.obtainMessage(0, "Volume Down!").sendToTarget();
        }

        @Override
        public void onError() {
            mHandler.obtainMessage(0, "Error!").sendToTarget();
        }
    }

}
