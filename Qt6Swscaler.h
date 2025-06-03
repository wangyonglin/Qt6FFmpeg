#ifndef QT6SWSCALER_H
#define QT6SWSCALER_H

#include <QObject>
#include <QImage>
#include "Qt6Object.h"

typedef enum AVPixelFormat QFFmpegPixelFormat;

class QT6FFMPEG_EXPORT Qt6Swscaler : public QObject
{
    Q_OBJECT
public:
    explicit Qt6Swscaler(QObject *parent = nullptr);
    ~Qt6Swscaler();
    int scale2qrgbaex(AVFrame *frame,const QSize &size);
    int scale2rgba(AVCodecContext *ctx, AVFrame *frame);
    uint8_t * data();
    QSize getSize();
private:
    QSize m_image_size;
    uint8_t *imageframe[4];
    int linesizes[4];
    int imagewidth;
    int imageheight;
    AVCodecContext * dec_ctx;
    QFFmpegPixelFormat  dst_pix_fmt= AV_PIX_FMT_NONE;
    //enum AVPixelFormat src_pix_fmt = AV_PIX_FMT_YUV420P, dst_pix_fmt = AV_PIX_FMT_RGB24;
signals:

};

#endif // QT6SWSCALER_H
