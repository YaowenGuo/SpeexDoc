package com.pandarow.chinese.util.speex;

import android.util.Log;
import com.socks.library.KLog;

import java.io.File;
import java.lang.ref.WeakReference;
import java.util.concurrent.LinkedBlockingQueue;

/**
 * Created by yaowen on 2017/12/29.
 */

public class ProcessSpeexRunnable implements Runnable {
    private static final String TAG = "ProcessSpeexRunnable";
    private final WeakReference<ProcessSpeexListener> mListener;

    private Speex mSpeexLib;
    private LinkedBlockingQueue<AudioRawData> mBufferQueue;
    private boolean mStopped = false;
    private static final AudioRawData sStopAudioData = new AudioRawData(null, -1);

    private int totByte;

    private Thread mSaveFileThread;
    private WriteSpeexOggFileRunnable mFileRunnable;

    public  ProcessSpeexRunnable(LinkedBlockingQueue<AudioRawData> queue, ProcessSpeexListener listener) {
        this.mBufferQueue = queue;
        this.mListener = new WeakReference<ProcessSpeexListener>(listener);

    }

    @Override
    public void run() {
        mStopped = false;
        init();
        if (mFileRunnable != null) {
            mSaveFileThread = new Thread(mFileRunnable);
            mSaveFileThread.start();
        }
/*        if (this.mStopped || mSpeexLib == null) {
            Log.w(TAG, "ProcessSpeexRunnable not runnable.");
            return;
        }*/
        try {
            while (this.mBufferQueue.size() != 0 || !this.mStopped) {
                AudioRawData data = null;
                try {
                    data = this.mBufferQueue.take();
                } catch (InterruptedException e) {
                    Log.e(TAG, "poll error: " + e.toString());
                    continue;
                }
                if (data == null) {
                    continue;
                } else if (data == sStopAudioData) {
                    this.mStopped = true;
                    continue;
                } else if (data.len > 0) {
                    process(data.data, data.size());
                }
            }
        } finally {
            if (mListener.get() != null) {
                mListener.get().onProcessSpeexFinish(/*mEncodedData*/);
            }

            this.clean();
            Log.d(TAG, "thread exit.");
        }
    }

    public void stop() {
        if (this.mBufferQueue == null) {
            this.mStopped = true;
        } else {
            try {
                this.mBufferQueue.put(sStopAudioData);
            } catch (InterruptedException e) {
                Log.e(TAG, "stop flag error: " + e.toString());
            }
        }

        if (mFileRunnable != null) {
            mFileRunnable.stop();
        }
    }

    private  void init() {
        mSpeexLib = new Speex();
        mSpeexLib.cancelNoiseInit();
    }

    private  void clean() {
        if (mSpeexLib != null) {
            mSpeexLib.close();
            mSpeexLib = null;
        }
    }


    private  void process(short[] buffer, int n) {
        byte[] encoded = new byte[n];
        String value = "";
        for (int i = 0; i < n; i ++) {
            value += buffer[i] + ", ";
        }
        KLog.e("Process: bef size " + n + " value: " + value);

        // 返回编码后的数据长度
        int encLen = this.mSpeexLib.encodeArray(buffer, n, encoded);

        totByte += encLen;
        Log.d(TAG, "process speex from " + (n * 2) + " byte to " + encLen + " byte");
        if (mFileRunnable != null) {
            mFileRunnable.putData(encoded, encLen);
        }
        if (mListener.get() != null) {
            mListener.get().onEncodedData(encoded, encLen);
        }

        String code = "";
        for (int i = 0; i < encLen; i++) {
            code += encoded[i] + ", ";
        }
        KLog.e("Process: aft size " + encLen + " value: " + code);


//        int decodedLen = mSpeexLib.decodeArray(encoded, encLen, buffer);
//
//        String decode = "";
//        for (int i = 0; i < decodedLen; i++) {
//            decode += buffer[i] + ", ";
//        }
//        KLog.e("Process: decode size " + decodedLen + " value: " + decode);
//        if (mListener.get() != null) {
//            mListener.get().onDecodedData(buffer, decodedLen);
//        }
    }

    public interface ProcessSpeexListener {
        void onEncodedData(byte[] data, int len);

        void onDecodedData(short[] data, int leb);

        void onProcessSpeexFinish(/*List<byte[]> data*/);
    }

    public WriteSpeexOggFileRunnable saveFile(File file, WriteSpeexOggFileRunnable.WriteSpeexOggListener listener) {
        mFileRunnable = new WriteSpeexOggFileRunnable(file, listener);
        return mFileRunnable;
    }
}
