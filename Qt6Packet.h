#ifndef FFmpegPacket_H
#define FFmpegPacket_H

#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>
#include <QThread>
#include "Qt6Core.h"


class Qt6Packet : public QObject
{
    Q_OBJECT
public:
    explicit Qt6Packet(QObject *parent = nullptr);
    void enqueue(AVPacket *packet);
    AVPacket* dequeue();
    void release();
    int size();
    bool isEmpty();
    bool isFulled();

private:
    QQueue<AVPacket*> packetQueue;
    QMutex mutex;
    QWaitCondition waitCondition;
    int this_maxSize=100;
};

#endif // FFmpegPacket_H
