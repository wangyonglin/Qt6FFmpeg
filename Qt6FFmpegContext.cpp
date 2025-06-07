#include "Qt6FFmpegContext.h"
#include <QDebug>
#include <QThread>

Qt6FFmpegContext::Qt6FFmpegContext(QObject *parent)
    : QObject{parent}
{}

AVCodecContext *Qt6FFmpegContext::audio_dctx()
{
    return __audio_dctx;
}

AVCodecContext *Qt6FFmpegContext::video_dctx()
{
    return __video_dctx;
}

int Qt6FFmpegContext::audio_sid()
{
    return __audio_stream_id;
}

int Qt6FFmpegContext::video_sid()
{
return __video_stream_id;
}

AVStream *Qt6FFmpegContext::audio_stream()
{
    return __audio_stream;
}

AVStream *Qt6FFmpegContext::video_stream()
{
    return __video_stream;
}


void Qt6FFmpegContext::open() {

    qDebug() << "Processing URL in thread:" << QThread::currentThreadId();
    if(__ifmt_ctx= demux_open(__url)){
        __video_stream_id= decode_open(&__video_dctx,&__video_stream,__ifmt_ctx,AVMEDIA_TYPE_VIDEO);
        __audio_stream_id= decode_open(&__audio_dctx,&__audio_stream,__ifmt_ctx,AVMEDIA_TYPE_AUDIO);
    }
    emit resultReady("Processed: " + __url);
}

void Qt6FFmpegContext::close()
{
    decode_close(&__video_dctx);
    decode_close(&__audio_dctx);
    demux_close(&__ifmt_ctx);
}

void Qt6FFmpegContext::decode_close(AVCodecContext **avctx){
    avcodec_free_context(avctx);
}


int Qt6FFmpegContext::decode_open(AVCodecContext ** dctx ,AVStream ** stream,AVFormatContext *ifmt_ctx,enum AVMediaType media_type)
{
    QMutexLocker locker(&__mutex);
    int ret=-1;
    qDebug() <<  av_get_media_type_string(media_type);
    // const AVCodec * dec=nullptr;
    // AVCodecContext * dec_ctx=nullptr;
    ret = av_find_best_stream(ifmt_ctx, media_type, -1, -1, NULL, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not find %s stream in input file '%s'\n",
                av_get_media_type_string(media_type));
        return ret;
    } else {

        (*stream) = ifmt_ctx->streams[ret];
        /* find decoder for the stream */
       const AVCodec * dec = avcodec_find_decoder((*stream)->codecpar->codec_id);
        if (!dec) {
            fprintf(stderr, "Failed to find %s codec\n",
                    av_get_media_type_string(media_type));
            return -1;
        }

        /* Allocate a codec context for the decoder */
        (*dctx) = avcodec_alloc_context3(dec);
        if (!(*dctx)) {
            fprintf(stderr, "Failed to allocate the %s codec context\n",
                    av_get_media_type_string(media_type));
            return -2;
        }
        /* Copy codec parameters from input stream to output codec context */
        if (( avcodec_parameters_to_context((*dctx), (*stream)->codecpar)) < 0) {
            fprintf(stderr, "Failed to copy %s codec parameters to decoder context\n",
                    av_get_media_type_string(media_type));
            return -3;
        }

        /* Init the decoders */
        if ((avcodec_open2((*dctx),dec, NULL)) < 0) {
            fprintf(stderr, "Failed to open %s codec\n",
                    av_get_media_type_string(media_type));
            return -4;
        }
    }

    return ret;
}
AVFormatContext* Qt6FFmpegContext::demux_open(const QString &url)
{
    AVFormatContext * ifmt_ctx=nullptr;
    int ret =-1;
    QMutexLocker locker(&__mutex); // 自动加锁
    qDebug() << "URL:" <<url;
    if(url.isEmpty())return nullptr;
    if(ifmt_ctx){
        avformat_close_input(&ifmt_ctx);
        ifmt_ctx=nullptr;
    }
    if((ret = avformat_open_input(&ifmt_ctx, url.toLocal8Bit().data(), NULL, NULL)) < 0) {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "avformat_open_input failed" << errmsg;
        return ifmt_ctx;
    }

    if((ret = avformat_find_stream_info(ifmt_ctx, NULL)) < 0) {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "avformat_find_stream_info failed" << errmsg;
        avformat_close_input(&ifmt_ctx);
        return ifmt_ctx;
    }
    av_dump_format(ifmt_ctx, 0, url.toLocal8Bit().data(), 0);
    return ifmt_ctx;
}

void Qt6FFmpegContext::demux_close(AVFormatContext **ifmt_ctx)
{
    avformat_close_input(ifmt_ctx);
}


Qt6Result Qt6FFmpegContext::demux_read(AVPacket** pkt)
{
    int ret =-1;
    QMutexLocker locker(&__mutex);
    av_packet_unref(*pkt);
    if ((ret = av_read_frame(__ifmt_ctx, *pkt)) < 0) {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        if (ret == AVERROR_EOF)
        {
            //qDebug() << "Reached end of file" << errmsg;
            av_packet_unref(*pkt);
            return Finished;
        }
        return ErrorException;
    }
    return Successfuled;
}

void Qt6FFmpegContext::url(const QString &url)
{
     __url =url;
}

Qt6Result Qt6FFmpegContext::decode_receive(AVFrame** frame,AVCodecContext *dctx,AVPacket *pkt)
{
    QMutexLocker locker(&__mutex);
    int ret = -1;
    //AVPacket * pkt=nullptr;
    if(!pkt && !*frame)return NullPointerException;
    ret = avcodec_send_packet(dctx, pkt);
    if (ret < 0)
    {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "avcodec_send_packet failed" << errmsg;
        return ErrorException;
    }
    //AVFrame* frame = av_frame_alloc();
    ret = avcodec_receive_frame(dctx, *frame);
    if(ret == AVERROR_EOF){
        return Finished;
    }else if (ret == AVERROR(EAGAIN)){
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "avcodec_receive_frame" << errmsg;
        return ErrorException;
    }else if (ret < 0) {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "avcodec_receive_frame failed" << errmsg;
        return ErrorException;
    }else{
        return Successfuled;
    }
}
