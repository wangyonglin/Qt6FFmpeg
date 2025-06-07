#include "Qt6CameraDemuxer.h"
#include <QDebug>


Qt6CameraDemuxer::Qt6CameraDemuxer(QObject *parent)
    :QObject{parent}
{
    initFFmpegAll();
}

bool Qt6CameraDemuxer::initFFmpegAll()
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

void Qt6CameraDemuxer::release()
{
     avformat_free_context(ifmt_ctx);
}


int Qt6CameraDemuxer::open(const QString &url)
{
    int ret =-1;
    av_dict_set(&dict, "video_size", QString("%1x%2").arg(width).arg(height).toUtf8().constData(),0);
    av_dict_set(&dict, "framerate", QString::number(framerate).toUtf8().constData(),0);

    ifmt_ctx = avformat_alloc_context();
    if (!ifmt_ctx) {
        av_log(NULL, AV_LOG_FATAL, "Could not allocate context.\n");
        return  AVERROR(ENOMEM);
    }
    ret=avformat_open_input(&ifmt_ctx,url.toUtf8().constData(),iformat,&dict);
    av_dict_free(&dict);
    if(ret<0)
    {
        avformat_free_context(ifmt_ctx);
        char errmsg[AV_ERROR_MAX_STRING_SIZE]={0x00};
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        av_log(NULL, AV_LOG_FATAL, "Input not open failed: %s\n", errmsg);
        return ret;
    }
    if((ret = avformat_find_stream_info(this->ifmt_ctx, NULL)) < 0) {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "avformat_find_stream_info failed : %s\n" << errmsg;
        avformat_close_input(&this->ifmt_ctx);
        return ret;
    }
    av_dump_format(this->ifmt_ctx, 0, url.toLocal8Bit().data(), 0);

    // find Video Stream Index
    int count = ifmt_ctx->nb_streams;
    for (int i=0; i<count; ++i)
    {
        if (ifmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            video_stream_index = i;
            break;
        }else if(ifmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO){
            audio_stream_index = i;
            break;
        }
    }

    return 0;

}



int Qt6CameraDemuxer::read(AVPacket * pkt)
{
    if(ifmt_ctx){
        return av_read_frame(ifmt_ctx,pkt);
    }
    return -1;
}

