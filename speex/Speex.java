package com.pandarow.chinese.util.speex;

public class Speex {


	static {
		System.loadLibrary("speex");
	}


	public native int cancelNoiseInit(int frame_size,int sample_rate);
	public native int cancelNoisePreprocess(byte[] inbuffer, int length);
	public native int cancelNoiseDestroy();

	public native int encode(String pcmFile, String speexFile);
	public native int decode(String speexFile,String pcmFile);

	public native int en(String pcmFile, String speexFile);
	public native int de(String speexFile,String pcmFile);

    public void close() {
    }

	public native int encodeArray(short[] buffer, int n, byte[] encoded);

	public native int decodeArray(byte[] codeBuffer, int n, short[] buffer);

	public void cancelNoiseInit() {
		cancelNoiseInit(AudioRecorderRunnable.FRAME_SIZE, AudioRecorderRunnable.SAMPLE_RATE);
	}
}