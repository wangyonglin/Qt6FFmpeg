#include "Qt6CameraProcess.h"

Qt6CameraProcess::Qt6CameraProcess(QObject *parent)
    : QThread{parent}
{
    cctx= new Qt6CameraContext(this);
    sws=new  Qt6Swscaler(parent);
}

Qt6CameraProcess::~Qt6CameraProcess()
{
    stop();
    sws->deleteLater();
    cctx->deleteLater();
}

void Qt6CameraProcess::url(const QString &url)
{
    cctx->url(url);
}

void Qt6CameraProcess::start()
{
    demux_flag=true;
    video_flag=true;
    QThread::start();
}

void Qt6CameraProcess::stop()
{
    demux_flag=false;
    video_flag=false;
    QThread::wait();
    QThread::quit();
}

void Qt6CameraProcess::setting(const QSize &size,int framerate)
{
    cctx->width=size.width();
    cctx->height=size.height();
    cctx->framerate=framerate;
}


void Qt6CameraProcess::run()
{
    cctx->open();
    AVFrame* frame = av_frame_alloc();
    AVPacket* pkt =av_packet_alloc();
    while(demux_flag  || video_flag){
        if(frame && pkt){
                Qt6Result ret=cctx->demux_read(&pkt,cctx->ifmtctx());
                if(ret==Finished){
                   // demux_flag=false;
                }else if(ret==Successfuled){
                    av_frame_unref(frame);

                    Qt6Result ret=cctx->decode_receive(&frame,cctx->video_dctx(),pkt);
                    if(ret==Finished){
                       // video_flag=false;
                    }else if(ret==Successfuled){

                        if(sws->scale2qrgbaex(frame,QSize(frame->width,frame->height))>=0){
                            emit refresh(sws->data(),
                                         QSize(frame->width,frame->height));
                        }
                    }
                }
        }
    }
    cctx->close();
    av_frame_free(&frame);
    av_packet_free(&pkt);
}
