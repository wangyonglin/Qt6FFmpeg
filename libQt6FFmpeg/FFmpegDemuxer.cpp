#include "FFmpegDemuxer.h"
FFmpegDemuxer::FFmpegDemuxer(QObject *parent)
    : FFmpegThreader{parent}
{
    this->synchronizer= new FFmpegSynchronizer(this);
    this->audio_pkt_queue=new FFmpegPacket(this);
    this->video_pkt_queue=new FFmpegPacket(this);

    this->start();
}

FFmpegDemuxer::~FFmpegDemuxer()
{
    this->release();
    this->stop();
}

int FFmpegDemuxer::demuxing(const QString & url)
{
    int ret =-1;
    if(url.isEmpty())return ret;
    if(this->isRunning()){

        if(this->ifmt_ctx)avformat_free_context(this->ifmt_ctx);
        this->ifmt_ctx=nullptr;
        if((ret = avformat_open_input(&this->ifmt_ctx, url.toLocal8Bit().data(), NULL, NULL)) < 0) {
            char errmsg[AV_ERROR_MAX_STRING_SIZE];
            av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
            qDebug() << "avformat_open_input failed" << errmsg;
            emit reject(ret);
            return ret;
        }

        if((ret = avformat_find_stream_info(this->ifmt_ctx, NULL)) < 0) {
            char errmsg[AV_ERROR_MAX_STRING_SIZE];
            av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
            qDebug() << "avformat_find_stream_info failed" << errmsg;
            avformat_close_input(&this->ifmt_ctx);
            emit reject(ret);
            return ret;
        }
        av_dump_format(this->ifmt_ctx, 0, url.toLocal8Bit().data(), 0);
        this->audio_stream_index=av_find_best_stream(this->ifmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
        this->video_stream_index=av_find_best_stream(this->ifmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
        resume();
        return 0;
    }
    return ret;
}

void FFmpegDemuxer::release()
{
    this->frameFinished=true;
    if(this->ifmt_ctx){
        avformat_close_input(&this->ifmt_ctx);
        this->ifmt_ctx=nullptr;
    }
    this->audio_pkt_queue->release();
    this->video_pkt_queue->release();
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
    this->frameFinished=true;
    FFmpegThreader::pause();
    this->audio_pkt_queue->release();
    this->video_pkt_queue->release();
}

void FFmpegDemuxer::resume()
{
    this->frameFinished=false;
    FFmpegThreader::resume();

}

void FFmpegDemuxer::loop()
{
    // QThread::sleep(3);
    //qDebug() << tr("state[%0] bool [%1]").arg(state()).arg(frameFinished);
    int ret =-1;
    if(state() ==Running && !frameFinished){

        this->synchronizer->start_synchronize();
        if(this->audio_pkt_queue->size() > 10 || this->video_pkt_queue->size() >10){
            QThread::usleep(200);
            return;
        }
        AVPacket* pkt=av_packet_alloc();
        av_packet_unref(pkt);

        if ((ret = av_read_frame(this->ifmt_ctx, pkt)) < 0) {
            char errmsg[AV_ERROR_MAX_STRING_SIZE];
            av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
            if (ret == AVERROR_EOF)
            {
                qDebug() << "Reached end of file" << errmsg;
                av_packet_unref(pkt);
                this->frameFinished=true;
                emit reject(ret);
                return;
            }
            else
            {
                qDebug() << "Error while reading frames" << errmsg;
                return;
            }
        }
        if (pkt->stream_index == this->audio_stream_index)
        {
            this->audio_pkt_queue->enqueue(pkt);
           // qDebug() << "audio_pkt_queue" << this->audio_pkt_queue->size();
        }
        else if(pkt->stream_index == this->video_stream_index)
        {
            this->video_pkt_queue->enqueue(pkt);
           // qDebug() << "video_pkt_queue" << this->video_pkt_queue->size();
        }
        av_packet_free(&pkt);
    }

}
