#ifndef VideoDecoder_H
#define VideoDecoder_H

#include "QFFmpegHeaders.h"
#include <QObject>
#include "FFmpegDecoder.h"
#include "FFmpegParameters.h"
#include "FFmpegDemuxer.h"
#include "VideoSwscaler.h"
class VideoDecoder : public FFmpegDecoder
{
    Q_OBJECT
public:
    explicit VideoDecoder(QObject *parent = nullptr);
    ~VideoDecoder();
    int running(FFmpegDemuxer * demuxer,QFFmpegPixelFormat pix_fmt=AV_PIX_FMT_YUV420P);
    void release();
    VideoSwscaler * videoswscaler=nullptr;
private:
    FFmpegFrame *video_frame_queue=nullptr;
    FFmpegDemuxer * demuxer=nullptr;
    bool frameFinished= true;
public slots:
    void start(Priority pri = InheritPriority);
    void stop();
    void pause();
    void resume();
signals:
    void sigFirst( uint8_t * data[],int w,int h);
    void newFrame();
    // FFmpegThreader interface
protected:
    virtual void loop() override;
};

#endif // VideoDecoder_H
