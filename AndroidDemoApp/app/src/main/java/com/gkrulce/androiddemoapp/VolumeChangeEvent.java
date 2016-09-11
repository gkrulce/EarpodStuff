package com.gkrulce.androiddemoapp;

/**
 * Created by gkrulce on 9/11/16.
 */
public interface VolumeChangeEvent {

    public void onVolumeUp();
    public void onVolumeDown();
    public void onError();
}
