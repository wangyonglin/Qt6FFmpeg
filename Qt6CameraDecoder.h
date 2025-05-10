#ifndef QT6CAMERADECODER_H
#define QT6CAMERADECODER_H
extern "C" {
#include "libavutil/avstring.h"
#include "libavutil/channel_layout.h"
#include "libavutil/eval.h"
#include "libavutil/mathematics.h"
#include "libavutil/pixdesc.h"
#include "libavutil/imgutils.h"
#include "libavutil/dict.h"
#include "libavutil/fifo.h"
#include "libavutil/parseutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/time.h"
#include "libavutil/bprint.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
#include "libavutil/tx.h"
#include "libswresample/swresample.h"
#include <libavcodec/avcodec.h>
#include "libavfilter/avfilter.h"
#include "libavfilter/buffersink.h"
#include "libavfilter/buffersrc.h"
}

#include <QObject>
#include "Qt6FFmpeg_global.h"


class QT6FFMPEG_EXPORT Qt6CameraDecoder : public QObject
{
    Q_OBJECT
public:
    explicit Qt6CameraDecoder(QObject *parent = nullptr);
    // bool initialize();
    // void release();
    int open(AVFormatContext *fmt_ctx, AVMediaType type);
    AVFrame* read(AVPacket * pkt);
public:
    AVCodecContext * codec_ctx=nullptr;
    const AVCodec * codec=nullptr;
    AVStream *stream=nullptr;
    AVFrame* frame =nullptr;
signals:
};



#endif // QT6CAMERADECODER_H
