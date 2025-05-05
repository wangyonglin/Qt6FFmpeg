#include "Qt6FFmpegThread.h"

Qt6FFmpegThread::Qt6FFmpegThread(QObject *parent)
    : QThread(parent),
    pauseFlag(false),
    stopFlag(false)
{}

Qt6FFmpegThread::State Qt6FFmpegThread::state() const
{
    State s = Stoped;
    if (!QThread::isRunning())
    {
        s = Stoped;
    }
    else if (QThread::isRunning() && pauseFlag)
    {
        s = Paused;
    }
    else if (QThread::isRunning() && (!pauseFlag))
    {
        s = Running;
    }
    return s;
}

void Qt6FFmpegThread::start(QThread::Priority pri)
{
    QThread::start(pri);
}



void Qt6FFmpegThread::stop()
{
    if (QThread::isRunning())
    {
        stopFlag = true;
        condition.wakeAll();
        QThread::quit();
        QThread::wait();
    }
}

void Qt6FFmpegThread::pause()
{
    if (QThread::isRunning())
    {
        pauseFlag = true;
    }
}

void Qt6FFmpegThread::resume()
{
    if (QThread::isRunning())
    {
        pauseFlag = false;
        condition.wakeAll();
    }
}

void Qt6FFmpegThread::run()
{
    //qDebug() << "enter thread : " << QThread::currentThreadId();
    while (!stopFlag)
    {

        loop(); // do something...

        if (pauseFlag)
        {
            mutex.lock();
            condition.wait(&mutex);
            mutex.unlock();
        }
    }
    pauseFlag = false;
    stopFlag = false;

    //qDebug() << "exit thread : " << QThread::currentThreadId();
}


