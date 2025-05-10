#ifndef QT6DEMUXER_H
#define QT6DEMUXER_H

#include <QObject>
#include "Qt6Core.h"
#include "Qt6Packet.h"
#include "Qt6Synchronizer.h"
#include "Qt6Thread.h"

class QT6FFMPEG_EXPORT Qt6Demuxer : public Qt6Thread
{
    Q_OBJECT
public:
    explicit Qt6Demuxer(QObject *parent = nullptr);

    // Qt6Thread interface
    int start(const QString &url, Qt6Synchronizer *sync,Qt6Packet *apkt,Qt6Packet *vpkt);
    void release();
protected:
    virtual void loop() override;
public:
    Qt6Synchronizer *av_sync=nullptr;
    AVFormatContext *ifmt_ctx=nullptr;
    Qt6Packet *audio_pkt_queue=nullptr;
    Qt6Packet *video_pkt_queue=nullptr;

    int audio_stream_index=-1;
    int video_stream_index=-1;
    AVCodecParameters * video_codecpar=nullptr;
    bool frameFinished=true;
};



#endif // QT6DEMUXER_H
