## Speex 

[TOC]

Speex是一套主要针对语音的开源免费，无专利保护的音频压缩格式。 需要注意的是：
- Speex只能提供压缩音频流，不能提供文件存储、音频播放、暂停、等动作。
- Speex的提供了降噪和回声消除，但是现在降噪和回声消除被单独作为一个独立的库下载，个人猜测是可能可以独立使用的，因为现在出了 Opus 库，根据测试效果，Opus各频段的压缩都超过了Speex，而且也是免费开源的。而且Speex官方也推荐使用Opus。唯一的问题是Opus没有降噪和回音消除的部分，Speex很可能将部分作为单独部分来支持Opus的音频也是可能的。具体待求证。
- Speex 压缩过的内容只是一个数据流，在一些情况下可能并不是直接传输数据集流。需要存储文件的话，可以使用Ogg文件格式存储。



## 如何编译Speex 

如果是在操作系统上使用，下载并编译安装就可以使用指令安装。因为Speex支持的处理器架构还是很多的：
(1)x86 & x86-64
(2)Power
(3)SPARC
(4)ARM
(5)Blackfin
(6)Coldfire (68k family)
(7)TI C54xx & C55xx
(8)TI C6xxx
(9)TriMedia (experimental)

能够运行的系统包括：
(1)Linux
(2)uCLinux
(3)MacOS X
(4)BSD
(5)其它 UNIX/POSIX 变种
(6)塞班
源代码目录在README.xxx文件里包括了在一定的架构和操作系统编译的信息。

***这也是为什么Speex能在众多平台编译使用的原因，如果发现你的平台Speex不支持，可能移植就要花费很多时间去修改代码了。***

#### 下载
需要注意的是，最新的版本的 Speex 压缩/解压缩 和 降噪和去回声两个包下载的。
https://www.speex.org/downloads/

- Speex Source Code 是用于压缩和和解压缩的内容，最新版是1.2rc2。
- Speex DSP Source Code 是用于降噪和回声消除的包，最新版是1.2rc3。

虽然网上有好多讲解Speex使用的帖子，都是面向于某个方面或者时间有点久了，可能有一些变换，后面我会提到我遇到的问题和解决办法。当然最好的办法就是察觉源文档。下载后，两个包里都有一个doc/manual.pdf 的文档，是一样的。该文档主要是Speex的使用，当然再点的文档更齐全一些。例如，如果需要存储文件的内容，需要查看
https://www.speex.org/docs/manual/speex-manual/node8.html

#### 编译
在桌面上的我并没有去编译使用。如果有这方面需要再测试补充。



## 安卓上面使用

在安卓上面使用需要Java和C 语言互调，这是Java语言提供的一种机制，还是比较麻烦的，具体的使用笔记单独放在了[JNI](/jni.md)中，这里只说明JNI在安卓中的是使用以及 Speex 的编译。

#### 配置 JNI 环境
JNI 使用中好多内容都是需要指令生成代码或者改动后频繁编译的，如果每次都使用指令来完后还是比较繁琐的，虽然 Android Studio 没有提供原生的一键使用，但是可以配置External Toolsd的方式开快捷执行。先了解 [JNI](/jni.md) 编程的JAVA指令对之后的配置很有帮助。

##### 下载NKD 

要实现我们的jni，首先一定要先下载NDK，NDK是什么？
- NDK是一系列工具的集合
- NDK提供了一份稳定、功能有限的API头文件声明
- NDK的发布，使“Java+C”的开发方式终于转正，成为官方支持的开发方式
- NDK将使Android平台支持C开发的开端

那么我们下载NDK，有两种下载方式：

- 这是Google官方下载 点击下载NDK；
- 通过SDK Manger下载

点击SDK Manger -> SDK Tools -> NDK -> 点击Apply

![](/assets/2137700-1f9dcaa58e35a815.png)

##### Android Studio配置JNI 工具

###### 配置生成 .h 头文件的的指令

Android Studio （菜单栏最左边）-> Preferences [或者command+,直接打开]-> Tools -> External Tools -> 点击➕号进行添加：

![](/assets/1226849-fc511b1906b58971.png)
点击+号之后，打开Macros 配置宏命令界面，如下图：
![](/assets/1226849-ef87cb38f9355763.png)

javah 指令的配置如下：

Program:  $JDKPath$\bin\javah.exe 
Parameters: -classpath . -jni -o $ModuleFileDir$/src/main/jni/$Prompt$  $FileClass$ 
Working directory:  $ModuleFileDir$\src\main\java 
![](/assets/屏幕快照 2018-01-05 下午6.04.49.png)

参数有依赖文件的话需要加入依赖库
-bootclasspath$ModuleSdkPath$/platforms/android-21/android.jar

###### 编译指令

Program: ~/Library/Android/sdk/ndk-bundle/build/ndk-build
Parameters: 如果想要强制重新编译: -B 如果想要清理: clean
Working dicrectory: $ProjectFileDir$/app/src/main
或者 
Working directory: $ModuleFileDir$\src\main

![](/assets/屏幕快照 2018-01-05 下午7.06.02.png)

![](/assets/屏幕快照 2018-01-05 下午7.15.57.png)

** 注意，上面的的Program最好打开目录选择，手动输入容易出错**


2.配置完成就可以使用javah 、ndk-build、ndk-build clean这三个命令了，那么在哪里使用这些命令，鼠标点击Moudule中的一个文件，右键就能找到：请看下图

![](/assets/1226849-13747f6eedf18067.png)


#### Speex 配置

##### 在 Module 中添加JNI 目录
 
![](/assets/1226849-cf21dea66559c79e.png)

这时候后会多一个jin 目录，在 Module的 gradle中添加如下内容：
```
android {
    ...
    defaultConfig {
        ...
        ndk {
            moduleName "libspeex"  // 指定连接时 c 动态链接库的名字
            // 指定链接哪些架构的库，会根据用于不同架构的apk链接不同的类型
            abiFilters 'armeabi','arm64-v8a', 'x86', 'armeabi-v7a'
        }
        sourceSets.main {
            jni.srcDirs = []
            // 指定so库的位置，用于生成apk时链接时进apk，这里指定了两个。
            jniLibs.srcDirs = ['src/main/jniLibs', 'src/main/libs']

        }
    }
}

```

##### 指定该项目支持 NDK
在项目的 gradle.properties 中添加一行 android.useDeprecatedNdk=true。不过我发现我没有配置也是可以使用的。
![](/assets/2137700-b9d0627b27a7ba9e.png)

如果你是自己下载的NDK包解压的 你还需在local.properties 中添加NDK位置
ndk.dir=/Users/yaowen/Library/Android/sdk/ndk-bundle

#### 拷贝Speex 到项目
当然，你可以编译好so库，直接拷贝so库到项目，不过这里为了方便源代码的查看，修改还是使用最常用的直接拷贝的项目中。
##### Speex 压缩和解压缩库
speex 包下有个 libspeex 目录，全部拷贝到刚刚新建的jni目录下，
##### Speexdsp 降噪和去回声
speexdsp包下的libspeexdsp 也拷贝到 之前新建的jni目录下。

*** 同时两个包下都有一个 include 目录，将它们合并拷贝到 jin 目录下，合并的时候两个目录有一些是重名，使用 Android studio 的文件compare with 功能绝大部分都是相同的，所致我只保留了speex包中的一个，没有使用 speexdsp 中的 include 文件。 
拷贝之后是这样
![](/assets/屏幕快照 2018-01-05 下午8.52.05.png)


##### 编写编译文件

所有在Android 项目中使用jni的内容都要在项目中建立Android.mk 和 Application.mk 文件用于编译，如果你熟悉 make 并且编写过makefile文件，那么编写这个两个文件对于你来说很定非常轻松。

在 jni 目录下添加 Application.mk 并添加：
```makefile
# APP_MODULES := libspeex
# APP_ABI := all

# 如下指定了要生成的so库的运行架构， 如果确定不需要哪个架构，可以删除。
APP_ABI := armeabi armeabi-v7a  arm64-v8a x86
```

在 jni 目录下添加 Android.mk 文件，并添加，注意看里面的注释，在编译不同版本时，很容易出错，要去修改里面的内容 
```makefile
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libspeex  # 这个就是要生成的so库名称，也是在Module的 gradle 中 moduleName "libspeex" 指定的一致，不然找不到，无法链接进apk
LOCAL_CFLAGS = -DFIXED_POINT -DUSE_KISS_FFT -DEXPORT="" -UHAVE_CONFIG_H
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include // 指定speex的include目录地址，如果你拷贝的include 目录位置不同位置，需要在此修改。


# 这些是自己编写的c文件和libspeex和libspeexdsp目录下要编译链接的c文件，如果
# 你发现哪个文件找不到，如果你下载的版本的sppex有这个文件，就应该在这里添加上，
# 如果没有，就应该在此处删掉多余的。或者你只使用了一个压缩的，那就根本不需要指定
# 降噪的文件。
# 
# 注意，你自己编写的c 文件也要加, 中间不要加空行，你懂得
LOCAL_SRC_FILES :=\
libspeex/bits.c \
libspeex/cb_search.c \
libspeex/exc_10_16_table.c \
libspeex/exc_10_32_table.c \
libspeex/exc_20_32_table.c \
libspeex/exc_5_256_table.c \
libspeex/exc_5_64_table.c \
libspeex/exc_8_128_table.c \
libspeex/filters.c \
libspeex/gain_table.c \
libspeex/gain_table_lbr.c \
libspeex/hexc_10_32_table.c \
libspeex/hexc_table.c \
libspeex/high_lsp_tables.c \
libspeex/kiss_fft.c \
libspeex/kiss_fftr.c \
libspeex/lpc.c \
libspeex/lsp.c \
libspeex/lsp_tables_nb.c \
libspeex/ltp.c \
libspeex/modes.c \
libspeex/modes_wb.c \
libspeex/nb_celp.c \
libspeex/quant_lsp.c \
libspeex/sb_celp.c \
libspeex/smallft.c \
libspeex/speex.c \
libspeex/speex_callbacks.c \
libspeex/speex_header.c \
libspeex/stereo.c \
libspeex/vbr.c \
libspeex/vq.c \
libspeex/window.c \
speex_process.c \
./libspeexdsp/buffer.c \
./libspeexdsp/fftwrap.c \
./libspeexdsp/filterbank.c \
./libspeexdsp/jitter.c \
./libspeexdsp/mdf.c \
./libspeexdsp/preprocess.c \
./libspeexdsp/resample.c \
./libspeexdsp/scal.c



LOCAL_LDLIBS := -llog -lz

include $(BUILD_SHARED_LIBRARY)
```


#### 编写Java native 方法

将你要调用so库中方法 的java声明成native 方法。例如：

```java
public class Speex {


        // 这里需要注意，我在网上看到的so库的名字叫libspeex，这里就写libspeex
        // 但是，我在写的时候报错，提示的是找不到liblibspeex，当然找不到
        // 我之前写的生成的库名字是libspeex，不知道为什么自动给我添加了一个lib 字段。
        // 所以这里自然是去掉一个lib了。
        // 这里不一定写成静态代码块，
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
```




