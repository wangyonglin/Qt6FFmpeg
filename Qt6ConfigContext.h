#ifndef QT6CONFIGCONTEXT_H
#define QT6CONFIGCONTEXT_H
#include "Qt6Object.h"
#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include "Qt6Packet.h"
#include "Qt6Frame.h"
#include "Qt6Process.h"

class Qt6ConfigContext : public QObject
{
    Q_OBJECT
public:
    explicit Qt6ConfigContext(QObject *parent = nullptr);
    AVFormatContext * getFormatContext();
    void createFormatContext(AVFormatContext *ifmt_ctx);
    QString getURL();
    int getFindBeseStream(AVMediaType type);
   int  getAudioPacketSize();
    int  getVideoPacketSize();
   void addAudioPacket(AVPacket *packet);
    void addVideoPacket(AVPacket *packet);
    AVPacket *getAudioPacket();
    AVPacket *getVideoPacket();
    bool isAudioPacketEmpty();
    bool isVideoPacketEmpty();
    int getAudioFrameSize();
    int getVideoFrameSize();
    void addAudioFrame(AVFrame *frame);
    void addVideoFrame(AVFrame *frame);
    AVFrame * getAudioFrame();
     AVFrame * getVideoFrame();
    bool isAudioFrameEmpty();
    bool isVideoFrameEmpty();
    AVCodecContext *getAudioCodecContext();
    AVCodecContext *getVideoCodecContext();
    void init(const QString &url);
    AVStream * getAudioStream();
    AVStream * getVideoStream();

    bool m_exitloop=true;
    AVFormatContext *m_ifmt_ctx=nullptr;
    int m_audio_stream_id=-1;
    AVStream * m_audio_stream=nullptr;
    const AVCodec * m_audio_dec=nullptr;
    AVCodecContext * m_audio_dec_ctx=nullptr;
    int m_video_stream_id=-1;
    AVStream * m_video_stream=nullptr;
    const AVCodec * m_video_dec=nullptr;
    AVCodecContext * m_video_dec_ctx=nullptr;
    QString m_url;

private:

    //
    Qt6Packet * m_audio_packet=nullptr;
    Qt6Packet * m_video_packet=nullptr;
    Qt6Frame * m_audio_frame=nullptr;
    Qt6Frame * m_video_frame=nullptr;
    //
public:
    //音视频同步
    void init_synchronize();
    int64_t get_master_synchronize();
    void start_synchronize();
    int64_t get_audio_synchronize();
    int64_t get_video_synchronize();
    void audio_synchronize(AVFrame *frame, int64_t start_time, AVRational timebase);
    void video_synchronize(AVFrame *frame, int64_t start_time, AVRational timebase);

private:
    int64_t __audio_pts_time=0;
    int64_t __video_pts_time=0;
    bool __locktime_root_flag =false;
    int64_t __locktime_start_time_ms =0;
    QMutex mutex;
    QWaitCondition waitCondition;
    int64_t get_now_ms();

};

#endif // QT6CONFIGCONTEXT_H
