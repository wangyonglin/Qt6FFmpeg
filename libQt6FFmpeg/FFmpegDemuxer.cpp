#include "FFmpegDemuxer.h"
FFmpegDemuxer::FFmpegDemuxer(QObject *parent)
    : FFmpegThreader{parent}
{

}
FFmpegManager* FFmpegDemuxer::init(FFmpegManager *manager){
    if(manager->url.isEmpty())return nullptr;
    if(manager->ifmt_ctx)avformat_free_context( manager->ifmt_ctx);
    manager->ifmt_ctx=nullptr;
    int ret = avformat_open_input(&manager->ifmt_ctx, manager->url.toLocal8Bit().data(), NULL, NULL);
    if(ret < 0) {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "avformat_open_input failed" << errmsg;
        emit reject(-1);
        return nullptr;
    }
    ret = avformat_find_stream_info(manager->ifmt_ctx, NULL);
    if(ret < 0) {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "avformat_find_stream_info failed" << errmsg;
        avformat_close_input(&manager->ifmt_ctx);
        emit reject(-2);
        return nullptr;
    }
    av_dump_format(manager->ifmt_ctx, 0, manager->url.toLocal8Bit().data(), 0);
    this->manager=manager;
    return manager;

}
void FFmpegDemuxer::release(FFmpegManager *manager){
    if(manager->ifmt_ctx){
        avformat_close_input(&manager->ifmt_ctx);
        manager->ifmt_ctx=nullptr;
    }
}

void FFmpegDemuxer::start(Priority pri)
{

    FFmpegThreader::start(pri);
}

void FFmpegDemuxer::stop()
{

    FFmpegThreader::stop();
}

void FFmpegDemuxer::pause()
{

    FFmpegThreader::pause();

}

void FFmpegDemuxer::resume()
{
    FFmpegThreader::resume();
}

void FFmpegDemuxer::loop()
{

    if(state() ==Running && !frameFinished){

        manager->start_synchronize();
        if(manager->audio_pkt_queue->size() > 10 || manager->video_pkt_queue->size() >10){
            QThread::usleep(200);
            return;
        }
        AVPacket* pkt=av_packet_alloc();
        av_packet_unref(pkt);
        int read_ret = av_read_frame(manager->ifmt_ctx, pkt);
        if (read_ret < 0) {
            char errmsg[AV_ERROR_MAX_STRING_SIZE];
            av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, read_ret);
            if (read_ret == AVERROR_EOF)
            {
                qDebug() << "Reached end of file" << errmsg;
                av_packet_unref(pkt);
                frameFinished=true;
                emit reject(0);
                return;
            }
            else
            {
                qDebug() << "Error while reading frames" << errmsg;
                return;
            }
        }
        if (pkt->stream_index == manager->audio_stream_index)
        {
            manager->audio_pkt_queue->enqueue(pkt);
        }
        else if(pkt->stream_index == manager->video_stream_index)
        {
            manager->video_pkt_queue->enqueue(pkt);
        }
        av_packet_free(&pkt);
    }

}
