#include "Qt6Thread.h"

Qt6Thread::Qt6Thread(QObject *parent)
    : QThread(parent),
    pauseFlag(false),
    stopFlag(false)
{}

Qt6Thread::State Qt6Thread::state() const
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

void Qt6Thread::start(QThread::Priority pri)
{
    QThread::start(pri);
}



void Qt6Thread::stop()
{
    if (QThread::isRunning())
    {
        stopFlag = true;
        condition.wakeAll();
        QThread::quit();
        QThread::wait();
    }
}

void Qt6Thread::pause()
{
    if (QThread::isRunning())
    {
        pauseFlag = true;
    }
}

void Qt6Thread::resume()
{
    if (QThread::isRunning())
    {
        pauseFlag = false;
        condition.wakeAll();
    }
}

void Qt6Thread::run()
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


