#ifndef CAMERADECODER_H
#define CAMERADECODER_H

#include "QFFmpegHeaders.h"
#include <QObject>
namespace QtRockchip::Qt6FFmpeg {

typedef struct DecoderContext{
    AVCodecContext * codec_ctx=nullptr;
    const AVCodec * codec=nullptr;
    AVStream *stream=nullptr;
    AVFrame* frame =nullptr;
}DecoderContext;

class CameraDecoder : public QObject
{
    Q_OBJECT
public:
    explicit CameraDecoder(QObject *parent = nullptr);
    int Initialize();
    void Release();
    int OpenDecoder(AVFormatContext *fmt_ctx, AVMediaType type);
    void CloseDecoder();
    AVFrame* CaptureDecoder(AVPacket * pkt);
public:
    std::unique_ptr<DecoderContext> ctx;
signals:
};

}


#endif // CAMERADECODER_H
