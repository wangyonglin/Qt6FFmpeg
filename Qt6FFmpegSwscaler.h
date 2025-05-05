#ifndef QT6FFMPEGSWSCALER_H
#define QT6FFMPEGSWSCALER_H

#include <QObject>
#include <QImage>
#include <Qt6FFmpegCore.h>
#include "Qt6FFmpeg_global.h"

typedef enum AVPixelFormat QFFmpegPixelFormat;

class QT6FFMPEG_EXPORT Qt6FFmpegSwscaler : public QObject
{
    Q_OBJECT
public:
    explicit Qt6FFmpegSwscaler(QObject *parent = nullptr);
    ~Qt6FFmpegSwscaler();
    int scale2qyuv420p(AVCodecContext *ctx,AVFrame *frame);
    uint8_t * data();
    int width();
    int height();
private:
    uint8_t *imageframe[4];
    int linesizes[4];
    int imagewidth;
    int imageheight;
    AVCodecContext * dec_ctx;
    QFFmpegPixelFormat  dst_pix_fmt= AV_PIX_FMT_NONE;
    //enum AVPixelFormat src_pix_fmt = AV_PIX_FMT_YUV420P, dst_pix_fmt = AV_PIX_FMT_RGB24;
signals:

};

#endif // QT6FFMPEGSWSCALER_H
