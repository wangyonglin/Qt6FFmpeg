#include "CameraCapturer.h"
#include "qdebug.h"

CameraCapturer::CameraCapturer(QObject *parent)
    : FFmpegThreader{parent}
{

    demux=new QtRockchip::Qt6FFmpeg::CameraDemuxer(parent);
    video_decoder= new QtRockchip::Qt6FFmpeg::CameraDecoder(parent);
    videoswscaler= new VideoSwscaler(this);
    demux->Initialize();
    video_decoder->Initialize();
}



void CameraCapturer::OpenDeviceCapture(const QString &url)
{
    int ret =0;
    ret=demux->OpenDemuxer(url);
    video_decoder->OpenDecoder(demux->ifmt_ctx,AVMEDIA_TYPE_VIDEO);
    videoswscaler->SetFormat(AV_PIX_FMT_YUV420P);
    videoswscaler->ImageAllocate(video_decoder->ctx->codec_ctx);
     emit sigFirst(this->videoswscaler->dstImagedata,video_decoder->ctx->codec_ctx->width, video_decoder->ctx->codec_ctx->height);
    if(ret>=0){
        start();
    }
}

void CameraCapturer::StopDeviceCapture()
{
    stop();
    video_decoder->CloseDecoder();
    demux->CloseDemuxer();
}




void CameraCapturer::loop()
{
    AVPacket pkt;
    if(demux->CaptureDemuxer(&pkt)==0){
        if (pkt.stream_index == demux->ctx->video_stream_index)
        {
            AVFrame* frame= video_decoder->CaptureDecoder(&pkt);
            if(frame){
                videoswscaler->Converting(frame);
                emit newFrame();
                av_frame_free(&frame);
            }

        }
        av_packet_unref(&pkt);
    }
}
