#ifndef FFMPEGDECODER_H
#define FFMPEGDECODER_H

#include <QObject>
#include "QFFmpegHeaders.h"
#include "FFmpegThreader.h"
#include "FFmpegFrame.h"
#include "FFmpegPacket.h"
class FFmpegDecoder : public FFmpegThreader
{
    Q_OBJECT
public:
    explicit FFmpegDecoder(QObject *parent = nullptr);
    int decodeing(AVFormatContext *fmt_ctx, enum AVMediaType type);
    void ReleaseDecoder();
    int depacket(FFmpegPacket *pkts, FFmpegFrame *frames);
public:
    const AVCodec * dec=nullptr;
    AVCodecContext * dec_ctx=nullptr;
    AVStream *st;
    int stream_index =-1;

signals:
};

#endif // FFMPEGDECODER_H
