#include "Qt6Process.h"



Qt6Process::Qt6Process(QObject *parent)
    : QThread(parent),
    pauseFlag(false),
    stopFlag(false),
    firstFlag(false)
{}


Qt6Process::State Qt6Process::state() const
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

void Qt6Process::start(QThread::Priority pri)
{
    firstFlag=true;
    QThread::start(pri);
}



void Qt6Process::stop()
{
    if (QThread::isRunning())
    {
        stopFlag = true;
        condition.wakeAll();
        QThread::quit();
        QThread::wait();
    }
}

void Qt6Process::pause()
{
    if (QThread::isRunning())
    {
        pauseFlag = true;
    }
}

void Qt6Process::resume()
{
    firstFlag=true;
    if (QThread::isRunning())
    {
        pauseFlag = false;
        condition.wakeAll();
    }
}

void Qt6Process::run()
{
    //qDebug() << "enter thread : " << QThread::currentThreadId();

    while (!stopFlag)
    {
        if(firstFlag){
            opening();
            firstFlag=false;
        }
        loopping(); // do something...

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
