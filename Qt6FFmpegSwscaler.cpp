#include "Qt6FFmpegSwscaler.h"
#include <QDebug>

Qt6FFmpegSwscaler::Qt6FFmpegSwscaler(QObject *parent)
    : QObject{parent}
{

}

Qt6FFmpegSwscaler::~Qt6FFmpegSwscaler()
{
    av_freep(&imageframe[0]);
}


int Qt6FFmpegSwscaler::Qt6FFmpegSwscaler::scale2qyuv420p(AVCodecContext *ctx, AVFrame *frame)
{

    int ret=-1;

    if((!ctx) || (ctx->width<=0) || (ctx->height<=0)){
        return -1;
    }
    if((!frame) || (frame->width<=0) || (frame->height<=0)){
        return -1;
    }
    imagewidth=ctx->width;
    imageheight=ctx->height;

    /* buffer is going to be written to rawvideo file, no alignment */
    if (av_image_alloc(imageframe, linesizes,
                       this->imagewidth, this->imageheight, AV_PIX_FMT_YUV420P, 1) < 0) {
        fprintf(stderr, "Could not allocate destination image\n");
        return -1;
    }

    // qDebug("The address of ptr is: %p", imageframe);
    SwsContext* sws_ctx = sws_getContext(frame->width,
                                         frame->height,
                                         static_cast<enum AVPixelFormat>(frame->format),
                                         imagewidth,
                                         imageheight,
                                         AV_PIX_FMT_YUV420P,
                                         SWS_BILINEAR,
                                         nullptr,
                                         nullptr,
                                         nullptr);


    if (!sws_ctx) {
        qDebug() << "sws_getContext failed.";
        return -1;
    }
    if((ret = sws_scale(sws_ctx, frame->data, frame->linesize, 0, imageheight, imageframe, linesizes))<0){
        sws_freeContext(sws_ctx);
        return -1;
    }

    sws_freeContext(sws_ctx);
    if (ret != frame->height) {
        qDebug() << "sws_scale failed.";
        return -1;
    }


    return 0;
}

uint8_t *Qt6FFmpegSwscaler::data()
{
    return *imageframe;
}

int Qt6FFmpegSwscaler::width()
{
    return imagewidth;
}

int Qt6FFmpegSwscaler::height()
{
    return imageheight;
}


