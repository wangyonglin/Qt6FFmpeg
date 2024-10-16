#ifndef AudioDecoder_H
#define AudioDecoder_H
#include <QPointer>
#include "FFmpegDecoder.h"
#include "FFmpegPacket.h"
#include "FFmpegFrame.h"
#include "FFmpegResampler.h"
#include "FFmpegDemuxer.h"
#include "FFmpegSpeaker.h"

class AudioDecoder : public FFmpegDecoder
{
    Q_OBJECT
public:
    explicit AudioDecoder(QObject *parent = nullptr);
    ~AudioDecoder();
    int running( FFmpegDemuxer * demuxer);
    void release();

private:
    FFmpegFrame *audio_frame_queue=nullptr;
    FFmpegDemuxer * demuxer=nullptr;
    bool frameFinished= true;
    FFmpegResampler *ffmpegResample;
    Qt6FFmpeg::FFmpegSpeaker *ffmpegSpeaker;
public slots:
    virtual void start(Priority pri = InheritPriority);
    virtual void stop();
    virtual void pause();
    virtual void resume();


    // FFmpegThreader interface
protected:
    virtual void loop() override;
};
#endif // AudioDecoder_H
