//
// Created by Yaowen on 2017/12/12.
//

#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <com_example_yaowen_speextext_JniTest.h>

#define  LOG_TAG    "【C_LOG】"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

JNIEXPORT jstring JNICALL Java_com_example_yaowen_speextext_JniTest_getString(JNIEnv *env, jobject thiz)
{
    LOGI("调用 C getStringFromJNI() 方法\n");
    char str[] = "Hello Java! 我是一只来自C世界的Dog，汪!!!";
    return env->NewStringUTF(str);
}

JNIEXPORT void JNICALL Java_com_dgk_jnitest_MainActivity_setStringToJNI (JNIEnv* env, jobject thiz, jstring str){
    LOGI("调用 C setStringFromJNI() 方法\n");
    const char* string = env->GetStringUTFChars(str, NULL);
    LOGI("%s\n", string);
    env->ReleaseStringUTFChars(str, string);
}


