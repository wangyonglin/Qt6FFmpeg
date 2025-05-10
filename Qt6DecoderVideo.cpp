#include "Qt6DecoderVideo.h"

Qt6DecoderVideo::Qt6DecoderVideo(QObject *parent,Qt6PixelFormat fmt)
    : Qt6Decoder{parent},fmt(fmt)
{
    swscaler= new Qt6Swscaler(this);
}


bool Qt6DecoderVideo::start(Qt6Demuxer *demux,
                            Qt6Synchronizer *sync,
                            Qt6Packet *packet,
                            Qt6Frame *frame)
{
    int ret=0;
    this->demux=demux;
    this->sync=sync;
    this->packet=packet;
    this->frame=frame;

    if(Qt6Thread::isRunning()){
        Qt6Thread::stop();
    }
    if((ret=decodeing(demux->ifmt_ctx, AVMEDIA_TYPE_VIDEO))<0){
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "RunningDecoder failed" << errmsg;
        return false;
    }
    Qt6Decoder::start();
    frameFinished=false;
    return true;
}


void Qt6DecoderVideo::loop()
{
    int ret = -1;
    if(state()==Running && !frameFinished){
        if (sync->get_video_synchronize() > sync->get_master_synchronize())
        {
            QThread::usleep(10);
            return;
        }

        if(!packet->isEmpty()){
            if(frame->size()  < 3){

                if((ret= this->depacket(packet,frame)) <0 ){
                    char errmsg[AV_ERROR_MAX_STRING_SIZE];
                    av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
                    qDebug() << "video depacket " << errmsg;
                    return ;
                }
            }else{
                QThread::usleep(200);
                return;
            }
        }

        if(!frame->isEmpty()){
            AVFrame * yuvFrame= frame->dequeue();
            if(yuvFrame){
                int64_t pts_time= (yuvFrame->pts == AV_NOPTS_VALUE) ? NAN : yuvFrame->pts;
                sync->video_synchronize(pts_time,this->st->start_time,this->st->time_base);
                if(fmt==QT6_VIDEO_YUV420P){
                    if(swscaler->scale2qyuv420p(dec_ctx,yuvFrame)==0){
                        emit refresh(swscaler->data(),swscaler->width(),swscaler->height());
                    }
                }else if(fmt==QT6_VIDEO_RGBA){
                    if(swscaler->scale2rgba(dec_ctx,yuvFrame)==0){
                        emit refresh(swscaler->data(),swscaler->width(),swscaler->height());
                    }
                }

                av_frame_free(&yuvFrame);
            }

        }
    }
}

