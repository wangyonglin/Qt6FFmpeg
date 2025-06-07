#include "Qt6FFmpegProcess.h"
#include <QDebug>
Qt6FFmpegProcess::Qt6FFmpegProcess(QObject *parent)
    : QThread{parent}
{
    audio_packet=new Qt6Packet(parent);
    video_packet=new Qt6Packet(parent);
    audio_frame=new Qt6Frame(parent);
    video_frame=new Qt6Frame(parent);
    fctx = new Qt6FFmpegContext(parent);
    sws=new  Qt6Swscaler(parent);
    speaker=new Qt6Speaker(parent);
    resampler=new Qt6Resampler(parent);
    syncth=new Qt6Syncthing(parent);
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleFormat(QAudioFormat::SampleFormat::Int16);
    speaker->init(format);
    speaker->create();
}

Qt6FFmpegProcess::~Qt6FFmpegProcess()
{
    audio_packet->deleteLater();
    video_packet->deleteLater();
    audio_frame->deleteLater();
    video_frame->deleteLater();
    fctx->deleteLater();
    sws->deleteLater();
    speaker->deleteLater();
    resampler->deleteLater();
    syncth->deleteLater();

}

void Qt6FFmpegProcess::start()
{

    init();
    demux_flag=true;
    audio_flag=true;
    video_flag=true;
    syncth->init_synchronize();
    if(!QThread::isRunning()){
        QThread::start();
    }

}

void Qt6FFmpegProcess::stop()
{
    QThread::wait();
    QThread::quit();
}

void Qt6FFmpegProcess::init()
{
    demux_flag=false;
    audio_flag=false;
    video_flag=false;
    audio_packet->release();
    video_packet->release();
    audio_frame->release();
    video_frame->release();
    if(QThread::isRunning()){
        QThread::wait();
        QThread::quit();
    }
}

void Qt6FFmpegProcess::onAudioHandle()
{

    if(audio_flag){
        if (syncth->get_audio_synchronize() > syncth->get_master_synchronize())
        {
            return;
        }
        AVFrame *frame=av_frame_alloc();
        if(frame){
            if(!audio_packet->isEmpty() && (audio_frame->size()<64)){
                AVPacket*tpkt= audio_packet->dequeue();
                if(tpkt){
                    av_frame_unref(frame);

                    Qt6Result ret=fctx->decode_receive(&frame,fctx->audio_dctx(),tpkt);
                    if(ret==Finished){
                        audio_flag=false;
                    }else if(ret==Successfuled){
                        audio_frame->enqueue(frame);
                    }
                }


            }

            if(!audio_frame->isEmpty()){
                AVFrame * sampleframe= audio_frame->dequeue();
                if(sampleframe){
                    syncth->audio_synchronize(sampleframe,fctx->audio_stream()->start_time,fctx->audio_stream()->time_base);
                    QByteArray bytes= resampler->convert(sampleframe);
                    speaker->write(bytes);
                    av_frame_free(&sampleframe);
                }
            }
            av_frame_free(&frame);
        }

    }

}

void Qt6FFmpegProcess::onVideoHandle()
{

    if(video_flag){
        if (syncth->get_video_synchronize() > syncth->get_master_synchronize())
        {
            return;
        }
        AVFrame* frame=av_frame_alloc();
        if(frame){
            if(!video_packet->isEmpty() && (video_frame->size()<64)){
                AVPacket* tpkt=video_packet->dequeue();
                if(tpkt){
                    av_frame_unref(frame);
                    Qt6Result ret=fctx->decode_receive(&frame,fctx->video_dctx(),tpkt);
                    if(ret==Finished){
                        video_flag=false;
                    }else if(ret==Successfuled){
                        video_frame->enqueue(frame);
                    }
                    av_packet_free(&tpkt);
                }
            }
            if(!video_frame->isEmpty()){
                AVFrame* scaleframe= video_frame->dequeue();
                if(scaleframe){
                    syncth->video_synchronize(scaleframe,fctx->video_stream()->start_time,fctx->video_stream()->time_base);
                    if(sws->scale2qrgbaex(scaleframe,QSize(scaleframe->width,scaleframe->height))>=0){

                        emit refresh(sws->data(),
                                     QSize(scaleframe->width,scaleframe->height));
                    }
                    av_frame_free(&scaleframe);
                }
            }
            av_frame_free(&frame);
        }

    }

}

void Qt6FFmpegProcess::onDemuxHandle()
{
    syncth->start_synchronize();
    if(demux_flag){
        AVPacket* pkt =av_packet_alloc();
        if(pkt){

            if(audio_packet->size()<64 || video_packet->size() <64){
                Qt6Result ret=fctx->demux_read(&pkt);
                if(ret==Finished){
                    demux_flag=false;
                }else if(ret==Successfuled){
                    if(pkt->stream_index==fctx->audio_sid()){
                        audio_packet->enqueue(pkt);
                    }else if (pkt->stream_index==fctx->video_sid()){
                        video_packet->enqueue(pkt);
                    }
                }
            }
            av_packet_free(&pkt);
        }
    }
}

void Qt6FFmpegProcess::url(const QString &url)
{
    fctx->url(url);
}



void Qt6FFmpegProcess::run()
{


    fctx->open();
    resampler->init(fctx->audio_dctx(),AV_CH_LAYOUT_STEREO,44100,AV_SAMPLE_FMT_S16);
    while(demux_flag || audio_flag || video_flag){

        //demux ...
        onDemuxHandle();
        //audio decode
        onAudioHandle();
        //video
        onVideoHandle();
    }
    fctx->close();


}


