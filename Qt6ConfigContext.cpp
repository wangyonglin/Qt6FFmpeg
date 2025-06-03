#include "Qt6ConfigContext.h"

Qt6ConfigContext::Qt6ConfigContext(QObject *parent)
    : QObject{parent}
{
    m_audio_packet=new Qt6Packet(parent);
    m_video_packet=new Qt6Packet(parent);
    m_audio_frame=new Qt6Frame(parent);
    m_video_frame=new Qt6Frame(parent);
}


AVFormatContext *Qt6ConfigContext::getFormatContext()
{
    return m_ifmt_ctx;
}

void Qt6ConfigContext::createFormatContext(AVFormatContext *ifmt_ctx)
{
    m_ifmt_ctx=ifmt_ctx;
}

void Qt6ConfigContext::init(const QString &url)
{
    m_audio_packet->release();
    m_video_packet->release();
    m_audio_frame->release();
    m_video_frame->release();
    m_url=url;
    init_synchronize();
}

QString Qt6ConfigContext::getURL()
{
    return m_url;
}

int Qt6ConfigContext::getFindBeseStream(enum AVMediaType type)
{
    return av_find_best_stream(getFormatContext(), type, -1, -1, NULL, 0);
}

int Qt6ConfigContext::getAudioPacketSize()
{
    return m_audio_packet->size();
}

int Qt6ConfigContext::getVideoPacketSize()
{
    return m_video_packet->size();
}

void Qt6ConfigContext::addAudioPacket(AVPacket *packet)
{
    m_audio_packet->enqueue(packet);
}

void Qt6ConfigContext::addVideoPacket(AVPacket *packet)
{
    m_video_packet->enqueue(packet);
}
AVPacket* Qt6ConfigContext::getAudioPacket()
{
    return m_audio_packet->dequeue();
}
AVPacket* Qt6ConfigContext::getVideoPacket()
{
   return  m_video_packet->dequeue();

}
bool  Qt6ConfigContext::isAudioPacketEmpty()
{
    return m_audio_packet->isEmpty();
}
bool  Qt6ConfigContext::isVideoPacketEmpty()
{
    return m_video_packet->isEmpty();
}

int Qt6ConfigContext::getAudioFrameSize()
{
    return m_audio_frame->size();
}

int Qt6ConfigContext::getVideoFrameSize()
{
    return m_video_frame->size();
}

void Qt6ConfigContext::addAudioFrame(AVFrame *frame)
{
    m_audio_frame->enqueue(frame);
}

void Qt6ConfigContext::addVideoFrame(AVFrame *frame)
{
    m_video_frame->enqueue(frame);
}

AVFrame *Qt6ConfigContext::getAudioFrame()
{
    return m_audio_frame->dequeue();
}

AVFrame *Qt6ConfigContext::getVideoFrame()
{
    return m_video_frame->dequeue();
}

bool Qt6ConfigContext::isAudioFrameEmpty()
{
    return m_audio_frame->isEmpty();
}

bool Qt6ConfigContext::isVideoFrameEmpty()
{
    return m_video_frame->isEmpty();
}

AVCodecContext *Qt6ConfigContext::getAudioCodecContext()
{
    return m_audio_dec_ctx;
}

AVCodecContext *Qt6ConfigContext::getVideoCodecContext()
{
    return m_video_dec_ctx;
}

AVStream *Qt6ConfigContext::getAudioStream()
{
    return m_audio_stream;
}

AVStream *Qt6ConfigContext::getVideoStream()
{
    return m_video_stream;
}
//音视频同步

void Qt6ConfigContext::audio_synchronize(AVFrame * frame,int64_t start_time,AVRational timebase)
{
    if(!frame)return;
    int64_t pts_time= (frame->pts == AV_NOPTS_VALUE) ? NAN : frame->pts;
    __audio_pts_time = (pts_time - start_time) * av_q2d(timebase) * 1000;
}
void Qt6ConfigContext::video_synchronize(AVFrame * frame,int64_t start_time,AVRational timebase)
{
    if(!frame)return;
    int64_t pts_time= (frame->pts == AV_NOPTS_VALUE) ? NAN : frame->pts;
    __video_pts_time = (pts_time- start_time) * av_q2d(timebase) * 1000;
}
int64_t Qt6ConfigContext::get_audio_synchronize()
{
    return __audio_pts_time;
}
int64_t Qt6ConfigContext::get_video_synchronize()
{
    return __video_pts_time;
}
void Qt6ConfigContext::init_synchronize()
{
    QMutexLocker locker(&mutex);
    __locktime_root_flag=false;
    __locktime_start_time_ms=0;//synchronize_audio
    __audio_pts_time=0;
    __video_pts_time=0;

}
void Qt6ConfigContext::start_synchronize()
{
    QMutexLocker locker(&mutex);
    if (!__locktime_root_flag)
    {
        __locktime_start_time_ms = get_now_ms();
        __locktime_root_flag=true;
    }
}

int64_t Qt6ConfigContext::get_now_ms()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}



int64_t Qt6ConfigContext::get_master_synchronize()
{
    QMutexLocker locker(&mutex);
    int64_t curr_time= get_now_ms();
    if (curr_time >__locktime_start_time_ms)
    {
        return  curr_time - __locktime_start_time_ms;
    }
    return 0;
}
