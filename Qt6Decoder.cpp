#include "Qt6Decoder.h"


Qt6Decoder::Qt6Decoder(QObject *parent)
    : Qt6Thread{parent}
{}

int Qt6Decoder::decodeing(AVFormatContext *fmt_ctx, enum AVMediaType type)
{
    int ret=-1;
    if(!fmt_ctx)return ret;
    ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not find %s stream in input file '%s'\n",
                av_get_media_type_string(type));
        return ret;
    } else {

        this->st = fmt_ctx->streams[ret];

        /* find decoder for the stream */
        this->dec = avcodec_find_decoder(this->st->codecpar->codec_id);
        if (!this->dec) {
            fprintf(stderr, "Failed to find %s codec\n",
                    av_get_media_type_string(type));
            return AVERROR(EINVAL);
        }

        /* Allocate a codec context for the decoder */
        this->dec_ctx = avcodec_alloc_context3(this->dec);
        if (!this->dec_ctx) {
            fprintf(stderr, "Failed to allocate the %s codec context\n",
                    av_get_media_type_string(type));
            return AVERROR(ENOMEM);
        }
        /* Copy codec parameters from input stream to output codec context */
        if ((ret = avcodec_parameters_to_context(this->dec_ctx, this->st->codecpar)) < 0) {
            fprintf(stderr, "Failed to copy %s codec parameters to decoder context\n",
                    av_get_media_type_string(type));
            return ret;
        }

        /* Init the decoders */
        if ((ret = avcodec_open2(this->dec_ctx, this->dec, NULL)) < 0) {
            fprintf(stderr, "Failed to open %s codec\n",
                    av_get_media_type_string(type));
            return ret;
        }
    }

    return 0;
}


void Qt6Decoder::ReleaseDecoder(){
    if(this->dec_ctx){
        avcodec_free_context(& this->dec_ctx);
        this->dec_ctx=nullptr;
    }
}
int Qt6Decoder::depacket(Qt6Packet *pkt_queue, Qt6Frame *frame_queue)
{
    int ret = -1;
    if(pkt_queue->isEmpty() && (!this->dec_ctx))return -1;
    AVPacket * pkt= pkt_queue->dequeue();
    if(!pkt)return ret;
    ret = avcodec_send_packet(this->dec_ctx, pkt);
    av_packet_free(&pkt);
    if (ret < 0)
    {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "avcodec_send_packet failed" << errmsg;
        return ret;
    }
    AVFrame* frame = av_frame_alloc();
    if(!frame)return ret;
    ret = avcodec_receive_frame(this->dec_ctx, frame);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
        return ret;
    }else if (ret < 0) {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "avcodec_receive_frame failed" << errmsg;
        return ret;
    }
    frame_queue->enqueue(frame);
    av_frame_free(&frame);
    return 0;
}
