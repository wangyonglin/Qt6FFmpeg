#include "Qt6VideoContext.h"

Qt6VideoContext::Qt6VideoContext(QObject *parent)
    : Qt6Process{parent}
{}


bool Qt6VideoContext::receive(AVPacket *pkt,AVFrame** frame)
{
    QMutexLocker locker(&m_mutex);
    int ret = -1;
    //AVPacket * pkt=nullptr;
    if(!pkt && !*frame)return false;
    ret = avcodec_send_packet(config()->getVideoCodecContext(), pkt);
    if (ret < 0)
    {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "avcodec_send_packet failed" << errmsg;
        return false;
    }
    //AVFrame* frame = av_frame_alloc();
    ret = avcodec_receive_frame(config()->getVideoCodecContext(), *frame);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
        return false;
    }else if (ret < 0) {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "avcodec_receive_frame failed" << errmsg;
        return false;
    }

    //av_frame_free(&frame);
    return true;
}






void Qt6VideoContext::close()
{
    QMutexLocker locker(&m_mutex);
    // if(config()->m_video_dec_ctx){
    //     avcodec_free_context(&config()->m_video_dec_ctx);
    // }
}

void Qt6VideoContext::init(Qt6ConfigContext *config)
{
    Qt6Process::stop();
    //close();
    m_config=config;
}

Qt6ConfigContext *Qt6VideoContext::config()
{
    return m_config;
}



bool Qt6VideoContext::open()
{
    QMutexLocker locker(&m_mutex);
    if(!config()->getFormatContext() )return false;
    qDebug() <<  av_get_media_type_string(AVMEDIA_TYPE_VIDEO);
    // const AVCodec * dec=nullptr;
    // AVCodecContext * dec_ctx=nullptr;
    config()->m_video_stream_id = av_find_best_stream(config()->getFormatContext(), AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (config()->m_video_stream_id < 0) {
        fprintf(stderr, "Could not find %s stream in input file '%s'\n",
                av_get_media_type_string(AVMEDIA_TYPE_VIDEO));
        return false;
    } else {

        config()->m_video_stream = m_config->getFormatContext()->streams[config()->m_video_stream_id];
        /* find decoder for the stream */
        config()->m_video_dec = avcodec_find_decoder(config()->m_video_stream->codecpar->codec_id);
        if (! config()->m_video_dec) {
            fprintf(stderr, "Failed to find %s codec\n",
                    av_get_media_type_string(AVMEDIA_TYPE_VIDEO));
            return false;
        }

        /* Allocate a codec context for the decoder */
        config()->m_video_dec_ctx = avcodec_alloc_context3( config()->m_video_dec);
        if (!config()->m_video_dec_ctx) {
            fprintf(stderr, "Failed to allocate the %s codec context\n",
                    av_get_media_type_string(AVMEDIA_TYPE_VIDEO));
            return false;
        }
        /* Copy codec parameters from input stream to output codec context */
        if (( avcodec_parameters_to_context(config()->m_video_dec_ctx, config()->m_video_stream->codecpar)) < 0) {
            fprintf(stderr, "Failed to copy %s codec parameters to decoder context\n",
                    av_get_media_type_string(AVMEDIA_TYPE_VIDEO));
            return false;
        }

        /* Init the decoders */
        if ((avcodec_open2(config()->m_video_dec_ctx, config()->m_video_dec, NULL)) < 0) {
            fprintf(stderr, "Failed to open %s codec\n",
                    av_get_media_type_string(AVMEDIA_TYPE_VIDEO));
            return false;
        }
    }

    return true;
}
