#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <jni.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <android/log.h>
#include <speex_util.h>
#include <speex/speex.h>
#include <speex/speex_types.h>
#include "process_set.h"
#include <string.h>
#include <stdlib.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "speex", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "speex", __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "speex", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "speex", __VA_ARGS__))


// SpeexPreprocessState *st;
/*帧的大小在这个例程中是一个固定的值,但它并不是必须这样*/
int frame_size = 160;
int audio_rate = 16000;
SpeexPreprocessState *global_preprocess_state = NULL;

JNIEXPORT jint JNICALL Java_com_pandarow_chinese_util_speex_Speex_cancelNoiseInit(JNIEnv* env,jobject this,jint frame_size_j, jint record_rate)
{
    frame_size = frame_size_j;
    audio_rate = record_rate;

    global_preprocess_state = speex_preprocess_state_init(frame_size, record_rate);
    spx_int32_t denoise = 1;
    //SPEEX_PREPROCESS_SET_DENOISE 打开(1) 或者关闭(2)降噪 (spx_int32_t)
    speex_preprocess_ctl(global_preprocess_state, SPEEX_PREPROCESS_SET_DENOISE, &denoise); //降噪
    spx_int32_t noiseSuppress = -200;//负的32位整数
    //SPEEX_PREPROCESS_SET_NOISE_SUPPRESS 设置噪音的最大衰减分贝(negative spx_int32_t)
    speex_preprocess_ctl(global_preprocess_state, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &noiseSuppress); //设置噪声的dB

    spx_int32_t agc = 1;
    //打开(1)或者关闭(2)自动增益(AGC)(spx_int32_t)
    speex_preprocess_ctl(global_preprocess_state, SPEEX_PREPROCESS_SET_AGC, &agc);//增益
    spx_int32_t level = 65535;
    //默认值是8000(spx_int32_t: 0<-->32768),如果默认值不够大，就设置大一些。
    speex_preprocess_ctl(global_preprocess_state, SPEEX_PREPROCESS_SET_AGC_LEVEL, &level);

    int vad = 1;
    int vadProbStart = 80;
    int vadProbContinue = 65;
    speex_preprocess_ctl(global_preprocess_state, SPEEX_PREPROCESS_SET_VAD, &vad); //静音检测
    speex_preprocess_ctl(global_preprocess_state, SPEEX_PREPROCESS_SET_PROB_START, &vadProbStart);
    //Set probability required for the VAD to <a href="http://lib.csdn.net/base/go" class='replace_word' title="Go知识库" target='_blank' style='color:#df3434; font-weight:bold;'>Go</a> from silence to voice
    speex_preprocess_ctl(global_preprocess_state, SPEEX_PREPROCESS_SET_PROB_CONTINUE,
                                    &vadProbContinue); //Set probability required for the VAD to

    if (global_preprocess_state == NULL) {
         LOGE("speex_preprocess_state_init failed!");
         return 1;
    } else {
         return 0;
    }
}

JNIEXPORT jint JNICALL Java_com_pandarow_chinese_util_speex_Speex_cancelNoisePreprocess(JNIEnv* env,jobject this,jbyteArray buffer, jint length)
{

    //openEchoCancel(16000, 160, 5000);
    char * inbuffer = (char *)(*env)->GetByteArrayElements(env,buffer, 0);

    short *in = (short *)inbuffer;
    int32_t pcmval;

    //int vad = speex_preprocess_run(st, in);

    //(*env)->ReleaseByteArrayElements(env,buffer, (jbyte *)inbuffer, 0);
    //uint8_t level = 10;
    //float multiplier = 2/*tan(level/100.0)*/;
    //for (int index = 0; index < length / 2; index++) {
    //    pcmval = in[index] * multiplier;
    //    if (pcmval < 30000 && pcmval > -30000) {
    //        in[index] = pcmval;
    //    } else if (pcmval > 30000) {
    //        in[index] = 30000;
    //    } else if (pcmval < -30000) {
    //        in[index] = -30000;
    //    }
    //}
    //int denoise = 1;
    //int noiseSuppress = -25;
    //speex_preprocess_ctl(state, SPEEX_PREPROCESS_SET_DENOISE, &denoise);
    //speex_preprocess_ctl(state, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &noiseSuppress);
    //return vad;
    return 0;
}

JNIEXPORT jint JNICALL Java_com_pandarow_chinese_util_speex_Speex_cancelNoiseDestroy(JNIEnv* env,jobject this)
{
   //if(st != NULL)
       //speex_preprocess_state_destroy(st);
   //st = NULL;
   return 1;
}


//int openEchoCancel(int sampleRate, int exampleFrame, int tailFrame) {
    /*
     * SPEEX_frame_size 每一个数据包的帧数, 数据包最好是20到30毫秒之间
     * TAIL 要消除的样本的回声帧数（对应与100~500ms之间的帧数）,这个参数的设置将影响回声消除模块的处理效果。
     * return 先创建的回声消除器的状态
     */
//    SpeexEchoState *echo_state = speex_echo_state_init(exampleFrame, tailFrame); // 回声消除初始化
//    if (echo_state == NULL) {
        // LOG("speex_echo_state_init failed");
//        return -3;
//    }
//    speex_echo_ctl(echo_state, SPEEX_ECHO_SET_SAMPLING_RATE, &sampleRate);
//    return 0;
//}


/*
 * Class:     com_cyril_speexnoisecancel_Speex
 * Method:    encode
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_pandarow_chinese_util_speex_Speex_encode
        (JNIEnv *env_p, jobject jObj, jstring pcm_path_j,jstring speex_path_j) {
    //const char *pcm_patha = env_p->GetStringUTFChars(pcm_path_j, 0);
    const char *pcm_path = (char *)(*env_p)->GetStringUTFChars(env_p, pcm_path_j, 0); // 输入文件路径
    const char *speex_path = (char *)(*env_p)->GetStringUTFChars(env_p, speex_path_j, 0); // 输出文件路径
    //const int frame_size = 160; /*帧的大小在这个例程中是一个固定的值,但它并不是必须这样*/

    FILE *pcm_file, *speex_file;
    pcm_file = fopen(pcm_path, "r");
    if (pcm_file == NULL) {
        LOGE("%s open failed", pcm_path);
        return 1;
    }
    speex_file = fopen(speex_path, "wb");
    if (speex_file == NULL) {
        LOGE("%s open failed", speex_path);
        fclose(pcm_file);
        return 2;
    }
    enableDenoize(16000);
    /*只支持8000Hz窄带（Narrow Band）、16000Hz宽带（Wide Band）、32000Hz超宽带（Ultra Wide Band）
    的三种带模式进行编解码，不支持其他采样频率。 */
    /*新建一个编码状态在宽宽模式下的状态*/
    void *state = speex_encoder_init(&speex_nb_mode);
    //设置质量为8(15kbps)
    int tmp = 8;
    speex_encoder_ctl(state, SPEEX_SET_QUALITY, &tmp);

    /*保存字节因此他们可以被speex常规读写*/
    //初始化结构使他们保存数据,用于speex常规读写保存数据
    SpeexBits bits;
    speex_bits_init(&bits);
    short in_buf[frame_size];
    float input_buf[frame_size];
    int nbBytes; // 返回编码后的字节数
    char cbits[200];
    while (1) {
        //读入一帧16bits的声音
        fread(in_buf, sizeof(short), frame_size, pcm_file);
        if (feof(pcm_file)) break;
        //把16bits的值转化为float,以便speex库可以在上面工作
        for (int i = 0; i < frame_size; i++)
            input_buf[i] = in_buf[i];

        //清空这个结构体里所有的字节,以便我们可以编码一个新的帧
        speex_bits_reset(&bits);
        //对帧进行编码
        speex_encode(state, input_buf, &bits);
        //把bits拷贝到一个char型数组
        nbBytes = speex_bits_write(&bits, cbits, 200);

        //首先写出帧的大小,这是sampledec文件需要的一个值,但是你的应用程序中可能不一样
        fwrite(&nbBytes, sizeof(int), 1, speex_file);

        //写出压缩后的数组
        fwrite(cbits, 1, nbBytes, speex_file);
    }
    //释放编码器状态量
    speex_encoder_destroy(state);
    //释放bit_packing结构
    speex_bits_destroy(&bits);
    fclose(pcm_file);
    fclose(speex_file);
    (*env_p)->ReleaseStringUTFChars(env_p, pcm_path_j, pcm_path);
    (*env_p)->ReleaseStringUTFChars(env_p, speex_path_j, speex_path);
    return 0;
}

JNIEXPORT jint JNICALL Java_com_pandarow_chinese_util_speex_Speex_decode
        (JNIEnv *env_p, jobject obj_j, jstring speex_name_jstr, jstring pcm_name_jstr) {
    const char *speex_path = (*env_p)->GetStringUTFChars(env_p, speex_name_jstr, 0);
    const char *pcm_path = (*env_p)->GetStringUTFChars(env_p, pcm_name_jstr, 0);

    FILE *speex_file, *pcm_file;
    speex_file = fopen(speex_path, "r");
    if (speex_file == NULL) {
        LOGE("%s open failed", speex_path);
        return 1;
    }
    pcm_file = fopen(pcm_path, "wb");
    if (pcm_file == NULL) {
        LOGE("%s open failed", pcm_path);
        fclose(speex_file);
        return 1;
    }
    /*保存编码的状态*/
    void *state = speex_decoder_init(&speex_nb_mode);;
    int tmp = 1; /*Set the perceptual enhancement on*/
    speex_decoder_ctl(state, SPEEX_SET_ENH, &tmp);
    float output[frame_size]; // speex解码后的数据
    short out[frame_size]; // 解码后用于输出的数据
    char cbits[200];
    SpeexBits bits; // 保存speex解码前的数据
    //初始化结构使他们保存数据
    speex_bits_init(&bits);
    int size = 0;
    while (1) {
        size = 0;
        fread(&size, sizeof(int), 1, speex_file);
        if (feof(speex_file)) {
            LOGI("文件解析完毕");
            break;
        }
        fread(cbits, 1, size, speex_file);

        //speex_bits_reset(&bits);
        //把读入的char数组拷贝到bits
        speex_bits_read_from(&bits, cbits, size);
        //将bits中的数据解码到output
        speex_decode(state, &bits, output);
        //把16bits的float转换short,以便pcm播放
        for (int i = 0; i < frame_size; i++)
            out[i] = output[i];

        fwrite(out, sizeof(short), frame_size, pcm_file);
    }
    //释放编码器状态量
    speex_decoder_destroy(state);
    //释放bit_packing结构
    speex_bits_destroy(&bits);
    fclose(speex_file);
    fclose(pcm_file);

    (*env_p)->ReleaseStringUTFChars(env_p, speex_name_jstr, speex_path);
    (*env_p)->ReleaseStringUTFChars(env_p, pcm_name_jstr, pcm_path);
    return 0;
}


JNIEXPORT jint JNICALL Java_com_pandarow_chinese_util_speex_Speex_en
        (JNIEnv *env_p, jobject obj_j, jstring pcm_path_j, jstring speex_path_j) {
    const char *pcm_path = (char *)(*env_p)->GetStringUTFChars(env_p, pcm_path_j, 0); // 输入文件路径
    const char *speex_path = (char *)(*env_p)->GetStringUTFChars(env_p, speex_path_j, 0); // 输出文件路径
    FILE *pcm_file, *speex_file;
    short in[frame_size];
    float input[frame_size];
    char cbits[200];
    int nbBytes;
    /*Holds the state of the encoder*/
    void *state;
    /*Holds bits so they can be read and written to by the Speex routines*/
    SpeexBits bits;
    int i, tmp;
    int multiplier = 3, pcmval = 0;
    for (int index = 0; index < frame_size; index++) {
        pcmval = in[index] * multiplier;
        if (pcmval < 32767 && pcmval > -32768) {
            in[index] = pcmval;
        } else if (pcmval > 32767) {
            in[index] = 32767;
        } else if (pcmval < -32768) {
            in[index] = -32768;
        }
    }
    SpeexPreprocessState *preprocess_state = enableDenoize(audio_rate); // 增益降噪设置
    /*Create a new encoder state in narrowband mode*/
    state = speex_encoder_init(&speex_nb_mode);
    /*Set the quality to 8 (15 kbps)*/
    tmp=8;
    speex_encoder_ctl(state, SPEEX_SET_QUALITY, &tmp);

    pcm_file = fopen(pcm_path, "r");
    speex_file = fopen(speex_path, "wb");
    /*Initialization of the structure that holds the bits*/
    speex_bits_init(&bits);
    //SpeexHeader speexHeader; // 文件头
    //writeSpeexHeader(&speexHeader, audio_rate, speex_nb_mode, 2, true, nframes)
    while (1)
    {
        /*Read a 16 bits/sample audio frame*/
        fread(in, sizeof(short), frame_size, pcm_file);
        if (feof(pcm_file))
            break;
        // 压缩之前降噪
        speex_preprocess_run(preprocess_state, (spx_int16_t *)in);
        /*Copy the 16 bits values to float so Speex can work on them*/
        for (i = 0; i < frame_size; i++)
            input[i] = in[i];
        /*Flush all the bits in the struct so we can encode a new frame*/
        speex_bits_reset(&bits);
        /*Encode the frame*/
        speex_encode(state, input, &bits);
        /*Copy the bits to an array of char that can be written*/
        nbBytes = speex_bits_write(&bits, cbits, 200);
        /*Write the size of the frame first. This is what sampledec expects but
        it’s likely to be different in your own application*/
        //fprintf(speex_file , "%d" , nbBytes);
        fwrite(&nbBytes, sizeof(int), 1, speex_file);
        /*Write the compressed data*/
        fwrite(cbits, 1, nbBytes, speex_file);
    }
    /*Destroy the encoder state*/
    speex_encoder_destroy(state);
    /*Destroy the bit-packing struct*/
    speex_bits_destroy(&bits);
    speex_preprocess_state_destroy(preprocess_state);
    fclose(pcm_file);
    fclose(speex_file);
    (*env_p)->ReleaseStringUTFChars(env_p, pcm_path_j, pcm_path);
    (*env_p)->ReleaseStringUTFChars(env_p, speex_path_j, speex_path);
    return 0;
}

/*
 * Class:     com_example_yaowen_speextext_Speex
 * Method:    de
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_pandarow_chinese_util_speex_Speex_de
        (JNIEnv *env_p, jobject obj_j, jstring speex_name_jstr, jstring pcm_name_jstr){
    const char *speex_path = (*env_p)->GetStringUTFChars(env_p, speex_name_jstr, 0);
    const char *pcm_path = (*env_p)->GetStringUTFChars(env_p, pcm_name_jstr, 0);

    FILE *speex_file, *pcm_file;

    /*Holds the audio that will be written to file (16 bits per sample)*/
    short out[frame_size];
    /*Speex handle samples as float, so we need an array of floats*/
    float output[frame_size];
    char cbits[200];
    int nbBytes;
    /*Holds the state of the decoder*/
    void *state;
    /*Holds bits so they can be read and written to by the Speex routines*/
    SpeexBits bits;
    int i, tmp;

    /*Create a new decoder state in narrowband mode*/
    state = speex_decoder_init(&speex_nb_mode);
    /*Set the perceptual enhancement on*/
    tmp=1;
    speex_decoder_ctl(state, SPEEX_SET_ENH, &tmp);

    speex_file = fopen(speex_path, "r");
    pcm_file = fopen(pcm_path, "wb");

    /*Initialization of the structure that holds the bits*/
    speex_bits_init(&bits);
    while (1)
    {
        /*Read the size encoded by sampleenc, this part will likely be
        different in your application*/
        //fscanf(speex_file, "%d", &nbBytes);
        fread(&nbBytes, sizeof(int), 1, speex_file);

        if (feof(speex_file))
            break;

        /*Read the "packet" encoded by sampleenc*/
        fread(cbits, 1, nbBytes, speex_file);
        /*Copy the data into the bit-stream struct*/
        speex_bits_read_from(&bits, cbits, nbBytes);

        /*Decode the data*/
        speex_decode(state, &bits, output);

        /*Copy from float to short*/
        for (i=0;i<frame_size;i++)
            out[i]=output[i];

        /*Write the decoded audio to file*/
        fwrite(out, sizeof(short), frame_size, pcm_file);
    }

    /*Destroy the decoder state*/
    speex_decoder_destroy(state);
    /*Destroy the bit-stream truct*/
    speex_bits_destroy(&bits);
    fclose(speex_file);
    fclose(pcm_file);
    (*env_p)->ReleaseStringUTFChars(env_p, speex_name_jstr, speex_path);
    (*env_p)->ReleaseStringUTFChars(env_p, pcm_name_jstr, pcm_path);
    return 0;
}


SpeexPreprocessState * enableDenoize(int sampleRate) {
    SpeexPreprocessState *preprocess_state;
    /**
     * 音频处理器初始化 start
     */
    preprocess_state = speex_preprocess_state_init(frame_size, sampleRate);
    spx_int32_t denoise = 1;
    //SPEEX_PREPROCESS_SET_DENOISE 打开(1) 或者关闭(2)降噪 (spx_int32_t)
    speex_preprocess_ctl(preprocess_state, SPEEX_PREPROCESS_SET_DENOISE, &denoise); //降噪
    spx_int32_t noiseSuppress = -200;//负的32位整数
    //SPEEX_PREPROCESS_SET_NOISE_SUPPRESS 设置噪音的最大衰减分贝(negative spx_int32_t)
    speex_preprocess_ctl(preprocess_state, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &noiseSuppress); //设置噪声的dB

    spx_int32_t agc = 1;
    //打开(1)或者关闭(2)自动增益(AGC)(spx_int32_t)
    speex_preprocess_ctl(preprocess_state, SPEEX_PREPROCESS_SET_AGC, &agc);//增益
    spx_int32_t level = 65535;
    //默认值是8000(spx_int32_t: 0<-->32768),如果默认值不够大，就设置大一些。
    speex_preprocess_ctl(preprocess_state, SPEEX_PREPROCESS_SET_AGC_LEVEL, &level);

    int vad = 1;
    int vadProbStart = 80;
    int vadProbContinue = 65;
    speex_preprocess_ctl(preprocess_state, SPEEX_PREPROCESS_SET_VAD, &vad); //静音检测
    speex_preprocess_ctl(preprocess_state, SPEEX_PREPROCESS_SET_PROB_START, &vadProbStart);
    //Set probability required for the VAD to <a href="http://lib.csdn.net/base/go" class='replace_word' title="Go知识库" target='_blank' style='color:#df3434; font-weight:bold;'>Go</a> from silence to voice
    speex_preprocess_ctl(preprocess_state, SPEEX_PREPROCESS_SET_PROB_CONTINUE,
                                 &vadProbContinue); //Set probability required for the VAD to

    if (preprocess_state == NULL) {
         LOGE("speex_preprocess_state_init failed!");
    }
    return preprocess_state;
}


void writeSpeexHeader(SpeexHeader *header, int rate, int mode, int channels,
        int vbr, int nframes) {
    memset(header, 0, sizeof(SpeexHeader)); // 清空
    strncpy(header->name, "Speex", sizeof(header->name)); // 8字节name
    strncpy(header->version, "speex-1.2rc", sizeof(header->version)); // 20字节的speex编码器的版本
    header->version_id = 1;           // speex_version_id
    header->header_size = 80;         // Ogg的文件头固定是80字节
    header->rate = rate;              // 采样频率
    header->mode = mode;              // 压缩带宽模式(0=NB, 1=WB, 2=UWB)
    header->mode_bitstream_version = 4; // mode_bitstream_version
    header->nb_channels = channels;   // 数据采样通道 (1=mono 单声道, 2=stereo立体声)
    header->bitrate = -1;             // 不知道是什么
    header->frame_size = 160 << mode; // frame_size (NB=160, WB=320, UWB=640)
    header->vbr = vbr ? 1 : 0;        // EnableVBR 是否启用可变比特率（Variable Bit Rate）
    header->frames_per_packet = nframes; // frames_per_packet， 每个speex包的帧数量，如果一个包160帧，每帧16字节的位宽。则原始数据为320字节
}


JNIEXPORT jint JNICALL Java_com_pandarow_chinese_util_speex_Speex_encodeArray
        (JNIEnv *env_p, jobject obj_j, jshortArray pcm_arr_j, jint pcm_size_j, jbyteArray encode_arr_j) {
    jshort *pcm_arr = (*env_p)->GetShortArrayElements(env_p, pcm_arr_j, NULL);
    int pcm_length = (int)(*env_p)->GetArrayLength(env_p, pcm_arr_j);
    if (pcm_length >  pcm_size_j) {
        pcm_length = pcm_size_j;
    }
    jbyte *encoded_arr = (*env_p)->GetByteArrayElements(env_p, encode_arr_j, NULL);
    int encoded_arr_length = (*env_p)->GetArrayLength(env_p, encode_arr_j);
    int encoded_len = 0;
    SpeexPreprocessState *preprocess_state = enableDenoize(audio_rate); // 增益降噪设置
    // 压缩之前降噪
    speex_preprocess_run(preprocess_state, (spx_int16_t *)pcm_arr);
    encoded_len = encode(pcm_arr, pcm_length, (char *)encoded_arr, encoded_arr_length);

    speex_preprocess_state_destroy(preprocess_state);

    (*env_p)->ReleaseShortArrayElements(env_p, pcm_arr_j, pcm_arr, pcm_length);
    (*env_p)->ReleaseByteArrayElements(env_p, encode_arr_j, encoded_arr, encoded_arr_length);
    return encoded_len;
}


JNIEXPORT jint JNICALL Java_com_pandarow_chinese_util_speex_Speex_decodeArray
        (JNIEnv *env_p, jobject obj_j, jbyteArray encode_arr_j, jint encode_len_j, jshortArray pcm_arr_j) {
    jshort *pcm_arr = (*env_p)->GetShortArrayElements(env_p, pcm_arr_j, NULL);
    int pcm_length = (int)(*env_p)->GetArrayLength(env_p, pcm_arr_j);
    jbyte *encoded_arr = (*env_p)->GetByteArrayElements(env_p, encode_arr_j, NULL);
    int encoded_arr_length = (*env_p)->GetArrayLength(env_p, encode_arr_j);
    if (encoded_arr_length >  encode_len_j) {
        encoded_arr_length = encode_len_j;
    }
    LOGE("Decode: srcLen %d, desLen %d，%d, %d, %d", encoded_arr_length, pcm_length, encoded_arr[0], encoded_arr[1], encoded_arr[2]);
    int decodeLen = decode((char*)encoded_arr, encoded_arr_length, pcm_arr, pcm_length);
    LOGE("Decode: srcLen %d, desLen %d, %d, %d, %d", encoded_arr_length, pcm_length, pcm_arr[0], pcm_arr[1], pcm_arr[2]);
    (*env_p)->ReleaseShortArrayElements(env_p, pcm_arr_j, pcm_arr, pcm_length);
    (*env_p)->ReleaseByteArrayElements(env_p, encode_arr_j, encoded_arr, encoded_arr_length);

    return decodeLen;
}


int encode(short in[], int in_length, char cbits[], int bit_length) {
    frame_size = in_length;
    float* input = malloc(sizeof(float) * in_length);
    int nbBytes;
    /*Holds the state of the encoder*/
    void *state;
    /*Holds bits so they can be read and written to by the Speex routines*/
    SpeexBits bits;
    int i, tmp = 8;/*Set the quality to 8 (15 kbps)*/

    /*Create a new encoder state in narrowband mode*/
    state = speex_encoder_init(&speex_nb_mode);

    speex_encoder_ctl(state, SPEEX_SET_QUALITY, &tmp);

    /*Initialization of the structure that holds the bits*/
    speex_bits_init(&bits);

    /*Copy the 16 bits values to float so Speex can work on them*/
    for (i = 0; i < frame_size && i < in_length; i++)
        input[i] = in[i] * 3;
    /*Flush all the bits in the struct so we can encode a new frame*/
    speex_bits_reset(&bits);
    /*Encode the frame*/
    speex_encode(state, input, &bits);
    /*Copy the bits to an array of char that can be written*/
    nbBytes = speex_bits_write(&bits, cbits, 200);
    /*Destroy the encoder state*/
    speex_encoder_destroy(state);
    /*Destroy the bit-packing struct*/
    speex_bits_destroy(&bits);
    free(input);
    return nbBytes;
}

/*
* out : Holds the audio that will be written to file (16 bits per sample)
* output : Speex handle samples as float, so we need an array of floats
*/
int decode(char codedBitArr[], int coded_size, short decodeArr[], int decodeArrLen) {
    //char cbits[200];
    //int nbBytes;
    float* output  = (float*)malloc(sizeof(float) * decodeArrLen);
    /*Holds the state of the decoder*/
    /*Create a new decoder state in narrowband mode*/
    void *state = speex_decoder_init(&speex_nb_mode);
    /*Set the perceptual enhancement on*/
    int tmp=1;
    speex_decoder_ctl(state, SPEEX_SET_ENH, &tmp);

    /*Holds bits so they can be read and written to by the Speex routines*/
    SpeexBits bits;
    /*Initialization of the structure that holds the bits*/
    speex_bits_init(&bits);


    /*Copy the data into the bit-stream struct*/
    speex_bits_read_from(&bits, codedBitArr, coded_size);
    LOGE("Decode: coded_size %d", coded_size);
    /*Decode the data*/
    speex_decode(state, &bits, output);

    /*Copy from float to short*/
    for (int i = 0; i < frame_size; i++)
         decodeArr[i]=output[i];

    /*Destroy the decoder state*/
    speex_decoder_destroy(state);
    /*Destroy the bit-stream truct*/
    speex_bits_destroy(&bits);
    free(output);
    return frame_size;
}