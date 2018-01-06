package com.pandarow.chinese.util.speex;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.util.Log;

import com.socks.library.KLog;

import java.lang.ref.WeakReference;
import java.util.concurrent.LinkedBlockingQueue;

/**
 * Created by yaowen on 2017/12/29.
 */

public class AudioRecorderRunnable implements Runnable{
    private static final String TAG = "AudioRecorderRunnable";

    public static final int SAMPLE_RATE = 16000; // 采样频率
    public static final int FRAME_SIZE = 160;
    private boolean mStopped = false;
    private LinkedBlockingQueue<AudioRawData> mBufferQueue;
    private WeakReference<AudioRecorderListener> mListener;
    private Thread mSpeexProcessThread;
    private ProcessSpeexRunnable mSpeexProcessRunnable;
    public AudioRecorderRunnable(AudioRecorderListener listener) {
        this.mListener = new WeakReference(listener);
    }

    @Override
    public  void run() {
        mStopped = false;
        if (mBufferQueue != null && mSpeexProcessRunnable != null) {
            mBufferQueue.clear();
            mSpeexProcessThread = new Thread(mSpeexProcessRunnable);
        }
        Log.e(TAG, "AudioRecorderRunnable is running.");
        android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_URGENT_AUDIO);
        AudioRecord recorder = null;
        short[][] buffers = new short[256][160];
        int ix = 0;
        //float gain = this.mGain;
        try {
            int n = AudioRecord.getMinBufferSize(SAMPLE_RATE, AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT);
            recorder = new AudioRecord(MediaRecorder.AudioSource.MIC /*VOICE_COMMUNICATION*/,
                    SAMPLE_RATE,
                    AudioFormat.CHANNEL_IN_MONO,
                    AudioFormat.ENCODING_PCM_16BIT,
                    n * 10);
            recorder.startRecording();
            recorder.setPositionNotificationPeriod(FRAME_SIZE); // 设置每次read()读取缓存是的长度。
            recorder.startRecording();
            if (mListener.get() != null)
                mListener.get().onRecordStart();
            if (mSpeexProcessThread != null) {
                mSpeexProcessThread.start();
            }
            while (!this.mStopped) {
                short[] buffer = buffers[ix++ % buffers.length];
                int numRead = recorder.read(buffer, 0, buffer.length);
//                String value = "";
//                for (int i = 0; i < numRead; i ++) {
//                    value += buffer[i] + ", ";
//                }
//                KLog.e("Process: bef size " + numRead + " value: " + value);

//                if (numRead > 0) {
//                    for (int i = 0; i < numRead; ++i)
//                        buffer[i] = (short) Math.min((int) (buffer[i] * gain), (int) Short.MAX_VALUE);
//                }
                if (mBufferQueue != null) {
                    this.mBufferQueue.offer(new AudioRawData(buffer, numRead));
                }
                if (mListener.get() != null) {
                    mListener.get().onRecoded(buffer, numRead);
                }
            }
        } catch (Throwable x) {
            Log.e(TAG, "Error reading voice audio", x);
        } finally {
            if (recorder != null) {
                recorder.stop();
                recorder.release();
            }

            if (mSpeexProcessRunnable != null) {
                mSpeexProcessRunnable.stop();
            }
            Log.d(TAG, "thread exit.");
        }
    }

    public ProcessSpeexRunnable setSpeexProcess(ProcessSpeexRunnable.ProcessSpeexListener listener) {
        mBufferQueue = new LinkedBlockingQueue<>();
        mSpeexProcessRunnable = new ProcessSpeexRunnable(mBufferQueue, listener);
        mSpeexProcessThread = new Thread(mSpeexProcessRunnable);
        return mSpeexProcessRunnable;
    }

    public  void stop() {
        this.mStopped = true;
        if (mSpeexProcessRunnable != null) {
            mSpeexProcessRunnable.stop();
        }
    }

    public interface AudioRecorderListener {
        void onRecordStart();
        void onRecoded(short[] data, int len);
    }
}
