package com.pandarow.chinese.util.speex;

import android.util.Log;

import java.io.File;
import java.lang.ref.WeakReference;
import java.util.concurrent.LinkedBlockingQueue;

/**
 * Created by yaowen on 2017/12/29.
 */

public class WriteSpeexOggFileRunnable implements Runnable {
    private final static String TAG = "WriteSpeexOggFile";

    private final File mFile;
    private SpeexWriteClient mSpeexWriteClient = new SpeexWriteClient();
    private volatile boolean mIsRecording;
    private ProcessedData mData;
    private LinkedBlockingQueue<ProcessedData> mDataQueue;

    private WeakReference<WriteSpeexOggListener> mWriteSpeexOggListener;

    public static int sWritePackageSize = 1024;

    public WriteSpeexOggFileRunnable(File file, WriteSpeexOggListener listener) {
        this.mFile = file;
        this.mWriteSpeexOggListener = new WeakReference<>(listener);
        mDataQueue = new LinkedBlockingQueue<>();
    }

    public  void run() {
        Log.d(TAG, "write thread running");
        mSpeexWriteClient.start(this.mFile, SpeexWriteClient.MODE_NB, SpeexWriteClient.SAMPLERATE_16000, true);
        mIsRecording = true;
        while (this.isRecording()) {
            try {
                mData = mDataQueue.take();
            } catch (InterruptedException e) {
                Log.e(TAG, Log.getStackTraceString(e));
                mData = null;
            }
            if (mData == sStopFlagData) {
                mIsRecording = false;
            } else if (mData != null) {
                Log.d(TAG, "mData size=" + mData.size);
                mSpeexWriteClient.writePacket(mData.processed, mData.size);
            }
        }
        mSpeexWriteClient.stop();

        if (mWriteSpeexOggListener.get() != null) {
            mWriteSpeexOggListener.get().onWriteSpeexOggFileFinished(this.mFile);
        }
        Log.d(TAG, "write thread exit");
    }

    public  boolean putData(final byte[] buf, int size) {
        ProcessedData data = new ProcessedData(buf, size);
        try {
            mDataQueue.put(data);
        } catch (InterruptedException e) {
            Log.e(TAG, Log.getStackTraceString(e));
            return false;
        }
        return true;
    }

    public  void stop() {
        try {
            mDataQueue.put(sStopFlagData);
        } catch (InterruptedException e) {
            Log.e(TAG, Log.getStackTraceString(e));
        }
    }

    public void setWriteFileListener(WriteSpeexOggListener listener) {
        mWriteSpeexOggListener = new WeakReference(listener);
    }

    public  File getOutputFile() {
        return this.mFile;
    }

    public  boolean isRecording() {
        return mIsRecording;
    }

    public interface WriteSpeexOggListener {
        void onWriteSpeexOggFileFinished(File file);
    }

    static class ProcessedData {
        ProcessedData(byte[] buf, int size) {
            if (buf != null)
                System.arraycopy(buf, 0, this.processed, 0, size);
            this.size = size;
        }

        private int size;
        private byte[] processed = new byte[sWritePackageSize];
    }

    private static final ProcessedData sStopFlagData = new ProcessedData(null, -1);
}
