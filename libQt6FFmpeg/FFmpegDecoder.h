#ifndef FFMPEGDECODER_H
#define FFMPEGDECODER_H

#include <QObject>
#include "QFFmpegHeaders.h"
#include "FFmpegThreader.h"
class FFmpegDecoder : public FFmpegThreader
{
    Q_OBJECT
public:
    explicit FFmpegDecoder(QObject *parent = nullptr);
    int RunningDecoder(AVFormatContext *fmt_ctx, enum AVMediaType type);
    void ReleaseDecoder();
    int DecodePacket(AVCodecContext *dec, const AVPacket *pkt);
public:
    const AVCodec * dec=nullptr;
    AVCodecContext * dec_ctx=nullptr;
    AVStream *st;
    int stream_index =-1;
signals:
};

#endif // FFMPEGDECODER_H
