#ifndef QT6PACKET_H
#define QT6PACKET_H
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>
#include <QThread>
#include <QObject>
#include "Qt6Object.h"
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
#endif // QT6PACKET_H
