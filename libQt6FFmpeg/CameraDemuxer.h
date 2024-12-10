#ifndef CAMERADEMUXER_H
#define CAMERADEMUXER_H

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

#include "libavfilter/avfilter.h"
#include "libavfilter/buffersink.h"
#include "libavfilter/buffersrc.h"
}

#include <QObject>
namespace QtRockchip::Qt6FFmpeg {

typedef struct DemuxerContext{

    const AVInputFormat * iformat=NULL;
    AVDictionary *dict;
    QString url;
    int width=1920;
    int height=1080;
    int framerate=60;
    const AVCodec * video_decodec=NULL;
    int video_stream_index=-1;
    int audio_stream_index=-1;
    AVStream * video_stream=NULL;
}DemuxerContext;

class CameraDemuxer : public QObject
{
    Q_OBJECT
public:
    explicit CameraDemuxer(QObject *parent = nullptr);
    int Initialize();
    void Release();
    int OpenDemuxer(const QString & url);
    void CloseDemuxer();
    int CaptureDemuxer(AVPacket * pkt);
public:
    std::unique_ptr<DemuxerContext> ctx;
    AVFormatContext *ifmt_ctx= nullptr;
};
}
#endif // CAMERADEMUXER_H
