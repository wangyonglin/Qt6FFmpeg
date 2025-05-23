#include "Qt6Synchronizer.h"

Qt6Synchronizer::Qt6Synchronizer(QObject *parent)
    : QObject{parent}
{}

void Qt6Synchronizer::audio_synchronize(int64_t  pts_time,int64_t start_time,AVRational timebase)
{
    __audio_pts_time = (pts_time - start_time) * av_q2d(timebase) * 1000;
}
void Qt6Synchronizer::video_synchronize(int64_t  pts_time,int64_t start_time,AVRational timebase)
{
    __video_pts_time = (pts_time- start_time) * av_q2d(timebase) * 1000;
}
int64_t Qt6Synchronizer::get_audio_synchronize()
{
    return __audio_pts_time;
}
int64_t Qt6Synchronizer::get_video_synchronize()
{
    return __video_pts_time;
}
void Qt6Synchronizer::init_synchronize()
{
    QMutexLocker locker(&mutex);
    __locktime_root_flag=false;
    __locktime_start_time_ms=0;//synchronize_audio
    __audio_pts_time=0;
    __video_pts_time=0;

}
void Qt6Synchronizer::start_synchronize()
{
    QMutexLocker locker(&mutex);
    if (!__locktime_root_flag)
    {
        __locktime_start_time_ms = get_now_ms();
        __locktime_root_flag=true;
    }
}

int64_t Qt6Synchronizer::get_now_ms()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

int64_t Qt6Synchronizer::get_master_synchronize()
{
    QMutexLocker locker(&mutex);
    int64_t curr_time= get_now_ms();
    if (curr_time >__locktime_start_time_ms)
    {
        return  curr_time - __locktime_start_time_ms;
    }
    return 0;
}
