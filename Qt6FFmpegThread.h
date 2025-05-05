#ifndef QT6FFMPEGTHREAD_H
#define QT6FFMPEGTHREAD_H

#include <QThread>
#include <QObject>
#include <QDebug>
#include <QMutex>
#include <QWaitCondition>
#include "Qt6FFmpeg_global.h"

class QT6FFMPEG_EXPORT Qt6FFmpegThread: public QThread
{
    Q_OBJECT
public:
    explicit Qt6FFmpegThread(QObject *parent = nullptr);
    enum State
    {

        Stoped,     ///<停止状态，包括从未启动过和启动后被停止
        Running,    ///<运行状态
        Paused      ///<暂停状态
    };
    State state() const;
public slots:
    void start(Priority pri = InheritPriority);
    void stop();
    void pause();
    void resume();
protected:
    virtual void run() override final;
    virtual void loop()=0;
public:
    std::atomic_bool pauseFlag;
    volatile std::atomic_bool stopFlag;
    QMutex mutex;
    QWaitCondition condition;
};

#endif // QT6FFMPEGTHREAD_H
