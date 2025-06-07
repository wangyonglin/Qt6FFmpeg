#include "Qt6Syncthing.h"

Qt6Syncthing::Qt6Syncthing(QObject *parent)
    : QObject{parent}
{}

void Qt6Syncthing::audio_synchronize(AVFrame * frame,int64_t start_time,AVRational timebase)
{
    if(!frame)return;
    int64_t pts_time= (frame->pts == AV_NOPTS_VALUE) ? NAN : frame->pts;
    __audio_pts_time = (pts_time - start_time) * av_q2d(timebase) * 1000;
}
void Qt6Syncthing::video_synchronize(AVFrame * frame,int64_t start_time,AVRational timebase)
{
    if(!frame)return;
    int64_t pts_time= (frame->pts == AV_NOPTS_VALUE) ? NAN : frame->pts;
    __video_pts_time = (pts_time- start_time) * av_q2d(timebase) * 1000;
}
int64_t Qt6Syncthing::get_audio_synchronize()
{
    return __audio_pts_time;
}
int64_t Qt6Syncthing::get_video_synchronize()
{
    return __video_pts_time;
}
void Qt6Syncthing::init_synchronize()
{
    QMutexLocker locker(&mutex);
    __locktime_root_flag=false;
    __locktime_start_time_ms=0;//synchronize_audio
    __audio_pts_time=0;
    __video_pts_time=0;

}
void Qt6Syncthing::start_synchronize()
{
    QMutexLocker locker(&mutex);
    if (!__locktime_root_flag)
    {
        __locktime_start_time_ms = get_now_ms();
        __locktime_root_flag=true;
    }
}

int64_t Qt6Syncthing::get_now_ms()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}



int64_t Qt6Syncthing::get_master_synchronize()
{
    QMutexLocker locker(&mutex);
    int64_t curr_time= get_now_ms();
    if (curr_time >__locktime_start_time_ms)
    {
        return  curr_time - __locktime_start_time_ms;
    }
    return 0;
}
