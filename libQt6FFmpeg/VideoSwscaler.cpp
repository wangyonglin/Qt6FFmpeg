#include "VideoSwscaler.h"
#include <QDebug>
VideoSwscaler::VideoSwscaler(QObject *parent)
    : QObject{parent}
{}
void VideoSwscaler::SetFormat(QFFmpegPixelFormat pix_fmt){
    this->dst_pix_fmt=pix_fmt;
}
void VideoSwscaler::ReleaseAll()
{
    av_freep(&dstImagedata[0]);
}
int VideoSwscaler::ImageAllocate(AVCodecContext *dec_ctx){

    this->dec_ctx =dec_ctx;

    int ret = -1;
    if((!dec_ctx) || (dec_ctx->width<=0) || (dec_ctx->height<=0)){
        return ret;
    }
    /* buffer is going to be written to rawvideo file, no alignment */
    if ((ret = av_image_alloc(dstImagedata, dstImagelinesize,
                              dec_ctx->width, dec_ctx->height, dst_pix_fmt, 1)) < 0) {
        fprintf(stderr, "Could not allocate destination image\n");
       return ret;
    }
    dstImagesize = ret;
    return 0;
}
int VideoSwscaler::Converting(AVFrame *frame)
{
    int ret =-1;
    if((!frame) || (frame->width<=0) || (frame->height<=0)){
        return ret;
    }
    SwsContext* sws_ctx = sws_getContext(frame->width,
                                         frame->height,
                                         static_cast<enum AVPixelFormat>(frame->format),
                                         frame->width,
                                         frame->height,
                                         static_cast<enum AVPixelFormat>(this->dst_pix_fmt),
                                         SWS_BILINEAR,
                                         nullptr,
                                         nullptr,
                                         nullptr);


    if (!sws_ctx) {
        qDebug() << "sws_getContext failed.";
        return ret;
    }
    if((ret = sws_scale(sws_ctx, frame->data, frame->linesize, 0, frame->height, dstImagedata, dstImagelinesize))<0){
        sws_freeContext(sws_ctx);
        return ret;
    }
    sws_freeContext(sws_ctx);
    if (ret != frame->height) {
        qDebug() << "sws_scale failed.";
        return ret;
    }
    return 0;

}
