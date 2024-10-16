#ifndef VIDEOSWSCALER_H
#define VIDEOSWSCALER_H

#include <QObject>
#include <QImage>
#include "QFFmpegHeaders.h"
typedef enum AVPixelFormat QFFmpegPixelFormat;

class VideoSwscaler : public QObject
{
    Q_OBJECT
public:

    explicit VideoSwscaler(QObject *parent = nullptr);
    void ReleaseAll();
    int ImageAllocate(AVCodecContext *dec_ctx);
    uint8_t *dstImagedata[4];
    int  dstImagelinesize[4];
    int dstImagesize;
    int Converting(AVFrame *frame);
    int AVFrame2YUV420P(AVFrame *frame);
    int AVFrame2RGBA(AVFrame *frame);
    void SetFormat(QFFmpegPixelFormat  pix_fmt=AV_PIX_FMT_YUV420P);
private:
    AVCodecContext * dec_ctx;
    QFFmpegPixelFormat  dst_pix_fmt= AV_PIX_FMT_NONE;

    //enum AVPixelFormat src_pix_fmt = AV_PIX_FMT_YUV420P, dst_pix_fmt = AV_PIX_FMT_RGB24;


};

#endif // VIDEOSWSCALER_H
