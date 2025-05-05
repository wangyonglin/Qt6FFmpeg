#include "Qt6FFmpegDecoder.h"
#include <QDebug>
Qt6FFmpegDecoder::Qt6FFmpegDecoder(QObject *parent)
    : QObject{parent}
{
}


bool Qt6FFmpegDecoder::initialize()
{
    frame = av_frame_alloc();
    if(frame){

        return true;
    }
    return false;
}

void Qt6FFmpegDecoder::release()
{
    if(frame){
       av_frame_free(&frame);
    }
}


int Qt6FFmpegDecoder::open(AVFormatContext *fmt_ctx, enum AVMediaType type)
{
    int ret=-1;
    if(!fmt_ctx)return ret;
    ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
    if (ret < 0) {
        // fprintf(stderr, "Could not find %s stream in input file '%s'\n",
        //         av_get_media_type_string(type));
        return ret;
    } else {

        stream = fmt_ctx->streams[ret];

        /* find decoder for the stream */
        codec = avcodec_find_decoder( stream->codecpar->codec_id);
        if (!codec) {
            fprintf(stderr, "Failed to find %s codec\n",
                    av_get_media_type_string(type));
            return AVERROR(EINVAL);
        }

        /* Allocate a codec context for the decoder */
        codec_ctx = avcodec_alloc_context3(codec);
        if (!codec_ctx) {
            fprintf(stderr, "Failed to allocate the %s codec context\n",
                    av_get_media_type_string(type));
            return AVERROR(ENOMEM);
        }
        /* Copy codec parameters from input stream to output codec context */
        if ((ret = avcodec_parameters_to_context(codec_ctx,  stream->codecpar)) < 0) {
            fprintf(stderr, "Failed to copy %s codec parameters to decoder context\n",
                    av_get_media_type_string(type));
            return ret;
        }

        /* Init the decoders */
        if ((ret = avcodec_open2(codec_ctx, this->codec, NULL)) < 0) {
            fprintf(stderr, "Failed to open %s codec\n",
                    av_get_media_type_string(type));
            return ret;
        }
    }

    return 0;
}




AVFrame* Qt6FFmpegDecoder::read(AVPacket * pkt){

    if(!pkt)return NULL;
    if(codec_ctx){
        if(!codec_ctx)return NULL;
    }
    int ret =-1;
    ret = avcodec_send_packet(codec_ctx, pkt);
    if (ret < 0)
    {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "avcodec_send_packet failed" << errmsg;
        return NULL;
    }
    AVFrame* frame = av_frame_alloc();
    if(!frame)return NULL;
    ret = avcodec_receive_frame(codec_ctx, frame);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
        av_frame_free(&frame);
        return NULL;
    }else if (ret < 0) {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "avcodec_receive_frame failed" << errmsg;
        av_frame_free(&frame);
        return NULL;
    }
    return frame;
}

