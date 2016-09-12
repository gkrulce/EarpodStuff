package com.gkrulce.androiddemoapp;

/**
 * Created by gkrulce on 9/11/16.
 */
public class JEarpodModel {
    enum Token {
        VOLUME_UP,
        VOLUME_DOWN,
        NOISE
    }

    public JEarpodModel() {
    }

    public Token[] read(short[] samples) {
        int[] jniRet = readJni(samples);
        Token[] ret = new Token[jniRet.length];
        for(int i = 0; i < jniRet.length; ++i) {
            // These mappings match those in jni/jearpodmodel.cpp
            switch(jniRet[i]) {
                case 0:
                    ret[i] = Token.VOLUME_UP;
                    break;
                case 1:
                    ret[i] = Token.VOLUME_DOWN;
                    break;
                case 2:
                    ret[i] = Token.NOISE;
                    break;
                default:
                    throw new RuntimeException("ID not recognized: " + jniRet[i]);
            }
        }
        return ret;
    }

    private native int[] readJni(short[] samples);

}
