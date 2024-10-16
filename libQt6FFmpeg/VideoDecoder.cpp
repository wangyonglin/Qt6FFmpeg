#include "VideoDecoder.h"

VideoDecoder::VideoDecoder(QObject *parent)
    : FFmpegDecoder{parent}
{
    this->videoswscaler= new VideoSwscaler(this);
    this->video_frame_queue=new FFmpegFrame(this);

    this->start();
}

VideoDecoder::~VideoDecoder()
{
    this->release();
    this->stop();
}

int VideoDecoder::running(FFmpegDemuxer * demuxer,QFFmpegPixelFormat pix_fmt)
{

    int ret = -1;
    this->videoswscaler->SetFormat(pix_fmt);
    this->demuxer=demuxer;
    if(!this->isRunning())return ret;
    if((ret=RunningDecoder(this->demuxer->ifmt_ctx, AVMEDIA_TYPE_VIDEO))<0){
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "RunningDecoder failed" << errmsg;
        return ret;
    }
    this->videoswscaler->ImageAllocate(this->dec_ctx);
    emit sigFirst(this->videoswscaler->dstImagedata,this->dec_ctx->width, this->dec_ctx->height);
    return 0;
}

void VideoDecoder::release()
{
    this->frameFinished=true;
    this->videoswscaler->ReleaseAll();
    this->ReleaseDecoder();
}

void VideoDecoder::loop()
{
    int ret = -1;
    if(state()==Running && !frameFinished){
        if (this->demuxer->synchronizer->get_video_synchronize() > this->demuxer->synchronizer->get_master_synchronize())
        {
            QThread::usleep(10);
            return;
        }

        if(!this->demuxer->video_pkt_queue->isEmpty()){
            if(this->video_frame_queue->size()  < 3){

                if((ret= this->DecodePacket(this->demuxer->video_pkt_queue,this->video_frame_queue)) <0 ){
                    char errmsg[AV_ERROR_MAX_STRING_SIZE];
                    av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
                    qDebug() << "running decoder failed [video]" << errmsg;
                    return ;
                }
            }else{
                QThread::usleep(200);
                return;
            }
        }

        if(!this->video_frame_queue->isEmpty()){
            AVFrame * yuvFrame= this->video_frame_queue->dequeue();
            if(yuvFrame){
                int64_t pts_time= (yuvFrame->pts == AV_NOPTS_VALUE) ? NAN : yuvFrame->pts;
                this->demuxer->synchronizer->video_synchronize(pts_time,this->st->start_time,this->st->time_base);

                videoswscaler->Converting(yuvFrame);
                emit newFrame();
                av_frame_free(&yuvFrame);
            }

        }
    }
}

void VideoDecoder::start(Priority pri)
{
 FFmpegThreader::start(pri);
}

void VideoDecoder::stop()
{
 FFmpegThreader::stop();
}

void VideoDecoder::pause()
{
    this->frameFinished=true;
    FFmpegThreader::pause();
    this->video_frame_queue->release();
}

void VideoDecoder::resume()
{
    this->frameFinished=false;
    FFmpegThreader::resume();
}




