#ifndef FFmpegDemuxer_H
#define FFmpegDemuxer_H
#include "FFmpegThreader.h"
#include <QObject>
#include "QFFmpegHeaders.h"
#include "FFmpegSynchronizer.h"
#include "FFmpegFrame.h"
#include "FFmpegPacket.h"
class FFmpegDemuxer : public FFmpegThreader
{
    Q_OBJECT
public:
    explicit FFmpegDemuxer(QObject *parent = nullptr);
    ~FFmpegDemuxer();
    int running(const QString & url);
    void release();
public:
    FFmpegSynchronizer *synchronizer=nullptr;
    AVFormatContext *ifmt_ctx=nullptr;
    FFmpegPacket *audio_pkt_queue=nullptr;
    FFmpegPacket *video_pkt_queue=nullptr;

    int audio_stream_index=-1;
    int video_stream_index=-1;
    AVCodecParameters * video_codecpar=nullptr;
    bool frameFinished=true;

public slots:
    virtual void start(Priority pri = InheritPriority);
    virtual void stop();
    virtual void pause();
    virtual void resume();


private:
    virtual void loop() override;
signals:
    void reject(int err);
};

#endif // FFmpegDemuxer_H
