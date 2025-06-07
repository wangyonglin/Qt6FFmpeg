#ifndef QT6SYNCTHING_H
#define QT6SYNCTHING_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include "Qt6Object.h"
class Qt6Syncthing : public QObject
{
    Q_OBJECT
public:
    explicit Qt6Syncthing(QObject *parent = nullptr);

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

#endif // QT6SYNCTHING_H
