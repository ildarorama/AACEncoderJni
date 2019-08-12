#include <iostream>
#include <ostream>
#include <fstream>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

int main() {
    avcodec_register_all();
    av_register_all();

    AVFormatContext *oc = avformat_alloc_context();
    AVCodec *audio_codec = avcodec_find_encoder(AV_CODEC_ID_AAC);

    AVStream *audio_st = avformat_new_stream(oc, audio_codec);
    if (!audio_st) {
        fprintf(stderr, "Could not allocate stream\n");
        exit(1);
    }


    audio_st->codec->sample_fmt  = AV_SAMPLE_FMT_FLTP;
    audio_st->codec->bit_rate    = 32000;
    audio_st->codec->sample_rate = 8000;
    audio_st->codec->profile=FF_PROFILE_AAC_LOW;
    audio_st->time_base = (AVRational){1, audio_st->codec->sample_rate };
    audio_st->codec->channels    = 1;
    audio_st->codec->channel_layout = AV_CH_LAYOUT_MONO;

    AVCodecContext *c = audio_st->codec;

    int ret = avcodec_open2(c, audio_codec, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not open audio codec: %s\n", av_err2str(ret));
        exit(1);
    }


    AVFrame *aframe = av_frame_alloc();

    int packet_size = 1024 * sizeof(float);

    uint8_t *data = static_cast<uint8_t *>(malloc(packet_size));

    avcodec_fill_audio_frame(aframe,1, AV_SAMPLE_FMT_FLTP,data,packet_size,0);


    std::ofstream f;
    f.open("file.bin", std::fstream::binary | std::fstream::out | std::fstream::ate);


    for(int k=0; k < 10; k++ ) {


        float *p = (float *) (data);
        for (int i = 0; i < packet_size; i++) {
            *(p + i) = 10 * sin(i);
        }

        avcodec_fill_audio_frame(aframe, 1, AV_SAMPLE_FMT_FLTP, data, packet_size, 0);

        ret = avcodec_send_frame(c, aframe);
        if (ret < 0) {
            fprintf(stderr, "Error encoding2222 audio frame: %s\n", av_err2str(ret));
            exit(1);
        }

        AVFrame *result = av_frame_alloc();
        AVPacket *packet = av_packet_alloc();
        ret = avcodec_receive_packet(c, packet);


        if (packet->size) {

        std::cout << "ERRR" << std::endl;
        f.write((char *) (packet->data), packet->size);
    }

        std::cout << "Size: " << packet->size << std::endl;
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            //std::cout << "avcodec_receive_frame: " << ret << std::endl;
            std::cout << "Skip...." << std::endl;
        }
    }


    f.close();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}