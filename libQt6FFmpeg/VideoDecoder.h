#ifndef VideoDecoder_H
#define VideoDecoder_H

#include "QFFmpegHeaders.h"
#include <QObject>
#include "FFmpegDecoder.h"
#include "FFmpegParameters.h"
#include "FFmpegDemuxer.h"
class VideoDecoder : public FFmpegDecoder
{
    Q_OBJECT
public:
    explicit VideoDecoder(QObject *parent = nullptr);
    ~VideoDecoder();
    int running(FFmpegDemuxer * demuxer);
    void release();
private:
    FFmpegFrame *video_frame_queue=nullptr;
    // AVRational video_pts_base;
    // int64_t   video_pts_begin;
    FFmpegDemuxer * demuxer=nullptr;
    int YUV420Size=0;
    uchar *YUV420Data= nullptr;
    // const    AVCodec * video_codec=nullptr;
    // AVCodecContext * video_codec_ctx=nullptr;
    // AVCodecParameters *video_codecpar;
    // int video_stream_index =-1;
     bool frameFinished= true;
    AVFrame         *yuvFrame     = NULL;
    void decoder(AVCodecContext *codec_ctx, FFmpegPacket *pkt_queue, FFmpegFrame *frame_queue);

public slots:
    void start(Priority pri = InheritPriority);
    void stop();
    void pause();
    void resume();
signals:
    void sigFirst(uchar* p,int w,int h);
    void newFrame();


    // FFmpegThreader interface
protected:
    virtual void loop() override;
};

#endif // VideoDecoder_H
