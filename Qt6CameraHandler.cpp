#include "Qt6CameraHandler.h"

Qt6CameraHandler::Qt6CameraHandler(QObject *parent)
    : Qt6FFmpegThread{parent}
{
    camera_demuxer = new Qt6FFmpegDemuxer(parent);
    camera_demuxer->initialize();
    video_decoder = new Qt6FFmpegDecoder(parent);
    video_decoder->initialize();
    videoswscaler = new Qt6FFmpegSwscaler(parent);
}

void Qt6CameraHandler::openUrl(const QString &url)
{
    camera_demuxer->open(url);
    video_decoder->open(camera_demuxer->ifmt_ctx,AVMEDIA_TYPE_VIDEO);
}

void Qt6CameraHandler::loop()
{
        AVPacket pkt;
        if(camera_demuxer->read(&pkt)==0){
            if (pkt.stream_index == camera_demuxer->video_stream_index)
            {
                AVFrame* frame= video_decoder->read(&pkt);
                if(frame){
                  //QByteArray byteFrame= Qt6FFmpegSwscaler::scale2qyuv420p(video_decoder->codec_ctx,frame);
                    if(videoswscaler->scale2qyuv420p(video_decoder->codec_ctx,frame)==0){
                      emit signalYUV420P(videoswscaler->data(),videoswscaler->width(),videoswscaler->height());
                    }


                  //emit signalUpdate(byteFrame,frame->width,frame->height);
                av_frame_free(&frame);
                }

            }
            av_packet_unref(&pkt);
        }
        //QThread::sleep(1000);
}
