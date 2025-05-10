#ifndef QT6DECODER_H
#define QT6DECODER_H

#include <QObject>
#include "Qt6Core.h"
#include "Qt6Thread.h"
#include "Qt6Frame.h"
#include "Qt6Packet.h"
class Qt6Decoder : public Qt6Thread
{
    Q_OBJECT
public:
    explicit Qt6Decoder(QObject *parent = nullptr);
    int decodeing(AVFormatContext *fmt_ctx, enum AVMediaType type);
    void ReleaseDecoder();
    int depacket(Qt6Packet *pkts, Qt6Frame *frames);
public:
    const AVCodec * dec=nullptr;
    AVCodecContext * dec_ctx=nullptr;
    AVStream *st;
    int stream_index =-1;

signals:
};

#endif // QT6DECODER_H
