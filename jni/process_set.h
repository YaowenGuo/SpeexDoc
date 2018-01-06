//
// Created by Yaowen on 2017/12/29.
//
#include <speex/speex_preprocess.h>
#ifndef SPEEXTEXT_PROSESS_SET_H_H
#define SPEEXTEXT_PROSESS_SET_H_H
SpeexPreprocessState * enableDenoize(int sampleRate);

typedef struct OggHeader {
    char name[8];
    char version[20];
    int version_id;
    int header_size;
    int rate;
    int mode;
    int mode_bitstream_version;
    int nb_channels;
    int bitrate;
    int frame_size;
    int vbr;
    int frames_per_packet;
    int extra_headers;
    int reserved1;
    int reserved2;
} SpeexHeader;
int encode(short in[], int in_length, char cbits[], int bit_length);
int decode(char codedBitArr[], int coded_size, short decodeArr[], int decodeArrLen);
void writeSpeexHeader(SpeexHeader *header, int rate, int mode, int channels, int vbr, int nframes);
#endif //SPEEXTEXT_PROSESS_SET_H_H
