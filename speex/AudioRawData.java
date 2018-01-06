package com.pandarow.chinese.util.speex;

/**
 * Created by yaowen on 2017/12/29.
 */

public class AudioRawData {
    public final short[] data;
    public final int len;

    public AudioRawData(short[] data, int len) {
        this.data = data;
        this.len = len;
    }

    public int size() {
        return data == null ? 0 : (data.length < len ? data.length : len);
    }
}
