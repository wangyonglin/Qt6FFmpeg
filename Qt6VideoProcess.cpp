#include "Qt6VideoProcess.h"
Qt6VideoProcess::Qt6VideoProcess(QObject *parent)
    : Qt6VideoContext{parent},

    swscaler(new Qt6Swscaler(this))

{

}


// void Qt6VideoProcess::start()
// {
//     m_exitloop=false;
//     Qt6VideoContext::start();
// }

// void Qt6VideoProcess::stop()
// {
//     m_exitloop=true;
//     Qt6VideoContext::stop();
// }



void Qt6VideoProcess::loopping()
{
    AVFrame* frame=av_frame_alloc();
    if(state() && !m_exitloop){
        int ret=-1;
        if (config()->get_video_synchronize() > config()->get_master_synchronize())
        {
            QThread::usleep(10);
            return;
        }

        if(!config()->isVideoPacketEmpty() && (config()->getVideoFrameSize()<24)){
            AVPacket* __pkt=config()->getVideoPacket();
            if(__pkt){
                av_frame_unref(frame);
                if(receive(__pkt,&frame)){
                    config()->addVideoFrame(frame);
                }
                av_packet_free(&__pkt);
            }
        }

        if(!config()->isVideoFrameEmpty()){
            AVFrame* scaleframe= config()->getVideoFrame();
            if(scaleframe){
                config()->video_synchronize(scaleframe,config()->getVideoStream()->start_time,config()->getVideoStream()->time_base);
                if(ret=swscaler->scale2qrgbaex(scaleframe,QSize(scaleframe->width,scaleframe->height))>=0){

                    emit refresh(swscaler->data(),
                                 QSize(scaleframe->width,scaleframe->height));
                }
                av_frame_free(&scaleframe);
            }
        }
        if(state())return;
    }
    av_frame_free(&frame);
}

void Qt6VideoProcess::opening()
{

}
