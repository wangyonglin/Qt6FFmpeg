#include "Qt6Demuxer.h"


Qt6Demuxer::Qt6Demuxer(QObject *parent)
    :Qt6Thread{parent}
{

}
int Qt6Demuxer::start(const QString & url,
                      Qt6Synchronizer *sync,
                      Qt6Packet *apkt,
                      Qt6Packet *vpkt)
{

    int ret =-1;
    if(url.isEmpty())return ret;
    av_sync=sync;
    video_pkt_queue=apkt;
    audio_pkt_queue=vpkt;
    if(Qt6Thread::isRunning()){
        Qt6Thread::stop();
    }
    if(state()==Stoped){

        if(this->ifmt_ctx)avformat_free_context(this->ifmt_ctx);
        this->ifmt_ctx=nullptr;
        if((ret = avformat_open_input(&this->ifmt_ctx, url.toLocal8Bit().data(), NULL, NULL)) < 0) {
            char errmsg[AV_ERROR_MAX_STRING_SIZE];
            av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
            qDebug() << "avformat_open_input failed" << errmsg;
            return ret;
        }

        if((ret = avformat_find_stream_info(this->ifmt_ctx, NULL)) < 0) {
            char errmsg[AV_ERROR_MAX_STRING_SIZE];
            av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
            qDebug() << "avformat_find_stream_info failed" << errmsg;
            avformat_close_input(&this->ifmt_ctx);
            return ret;
        }
        av_dump_format(this->ifmt_ctx, 0, url.toLocal8Bit().data(), 0);
        this->audio_stream_index=av_find_best_stream(this->ifmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
        this->video_stream_index=av_find_best_stream(this->ifmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
        Qt6Thread::start();
        frameFinished=false;
        return 0;
    }
    return ret;
}

void Qt6Demuxer::release()
{
    this->frameFinished=true;
    if(this->ifmt_ctx){
        avformat_close_input(&this->ifmt_ctx);
        this->ifmt_ctx=nullptr;
    }
    this->audio_pkt_queue->release();
    this->video_pkt_queue->release();
}
void Qt6Demuxer::loop()
{
    int ret =-1;
    if(state() ==Running && !frameFinished){

        this->av_sync->start_synchronize();
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
                //qDebug() << "audio_pkt_queue" << this->audio_pkt_queue->size();
        }
        else if(pkt->stream_index == this->video_stream_index)
        {
            this->video_pkt_queue->enqueue(pkt);
            //  qDebug() << "video_pkt_queue" << this->video_pkt_queue->size();
        }
        av_packet_free(&pkt);
    }
}
