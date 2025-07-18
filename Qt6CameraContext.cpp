#include "Qt6CameraContext.h"
#include <QDebug>
#include <QThread>

Qt6CameraContext::Qt6CameraContext(QObject *parent)
    :QObject{parent}
{
    initFFmpegAll();
}

bool Qt6CameraContext::initFFmpegAll()
{
    avdevice_register_all();
    avformat_network_init();

#if defined(Q_OS_WIN)
    iformat = av_find_input_format("dshow");            // Windows下如果没有则不能打开摄像头
#elif defined(Q_OS_LINUX)
    iformat = av_find_input_format("video4linux2");       // Linux也可以不需要就可以打开摄像头
#elif defined(Q_OS_MAC)
    iformat = av_find_input_format("avfoundation");
#endif
    if (!iformat) {
        av_log(NULL, AV_LOG_FATAL, "Unknown input format: %s\n", "video4linux2");
        return false;
    }
    return true;
}






AVCodecContext *Qt6CameraContext::audio_dctx()
{
    return __audio_dctx;
}

AVCodecContext *Qt6CameraContext::video_dctx()
{
    return __video_dctx;
}

int Qt6CameraContext::audio_sid()
{
    return __audio_stream_id;
}

int Qt6CameraContext::video_sid()
{
    return __video_stream_id;
}

AVStream *Qt6CameraContext::audio_stream()
{
    return __audio_stream;
}

AVStream *Qt6CameraContext::video_stream()
{
    return __video_stream;
}


void Qt6CameraContext::open() {

    qDebug() << "Processing URL in thread:" << QThread::currentThreadId();
    if(__ifmt_ctx= demux_open(__url)){
       __video_stream_id= decode_open(&__video_dctx,&__video_stream,__ifmt_ctx,AVMEDIA_TYPE_VIDEO);
        // __audio_stream_id= decode_open(&__audio_dctx,&__audio_stream,__ifmt_ctx,AVMEDIA_TYPE_AUDIO);
    }
  //  emit resultReady("Processed: " + __url);
}

void Qt6CameraContext::close()
{
    decode_close(&__video_dctx);
    decode_close(&__audio_dctx);
    demux_close(&__ifmt_ctx);
}

void Qt6CameraContext::decode_close(AVCodecContext **avctx){
    avcodec_free_context(avctx);
}


int Qt6CameraContext::decode_open(AVCodecContext ** dctx ,AVStream ** stream,AVFormatContext *ifmt_ctx,enum AVMediaType media_type)
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

AVFormatContext *Qt6CameraContext::ifmtctx()
{
    return __ifmt_ctx;
}
AVFormatContext* Qt6CameraContext::demux_open(const QString &url)
{
    AVFormatContext * ifmt_ctx=nullptr;
    int ret =-1;
    QMutexLocker locker(&__mutex); // 自动加锁
    if(url.isEmpty())return nullptr;
    AVDictionary *dict;
    av_dict_set(&dict, "video_size", QString("%1x%2").arg(width).arg(height).toUtf8().constData(),0);
    av_dict_set(&dict, "framerate", QString::number(framerate).toUtf8().constData(),0);

    ifmt_ctx = avformat_alloc_context();
    if (!ifmt_ctx) {
        av_log(NULL, AV_LOG_FATAL, "Could not allocate context.\n");
        return  ifmt_ctx;
    }
    if((ret = avformat_open_input(&ifmt_ctx, url.toLocal8Bit().data(), iformat,&dict)) < 0) {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "avformat_open_input failed" << errmsg;
        return ifmt_ctx;
    }
    av_dict_free(&dict);
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

void Qt6CameraContext::demux_close(AVFormatContext **ifmt_ctx)
{
    avformat_close_input(ifmt_ctx);
}


Qt6Result Qt6CameraContext::demux_read(AVPacket** pkt,AVFormatContext *ifmt_ctx)
{
    if(!ifmt_ctx)return NullPointerException;
    int ret =-1;
    QMutexLocker locker(&__mutex);
    av_packet_unref(*pkt);
    if ((ret = av_read_frame(ifmt_ctx, *pkt)) < 0) {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        if (ret == AVERROR_EOF)
        {
            qDebug() << "Reached end of file" << errmsg;
            av_packet_unref(*pkt);
            return Finished;
        }
        return ErrorException;
    }
    return Successfuled;
}

void Qt6CameraContext::url(const QString &url)
{
    __url =url;
}

Qt6Result Qt6CameraContext::decode_receive(AVFrame** frame,AVCodecContext *dctx,AVPacket *pkt)
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
