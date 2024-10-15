#ifndef QFFMPEGFRAME_H
#define QFFMPEGFRAME_H
#include "QFFmpegHeaders.h"
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>
#include <QThread>

class FFmpegFrame : public QObject
{
    Q_OBJECT
public:
    explicit FFmpegFrame(QObject *parent = nullptr);

    void enqueue(AVFrame *frame);
    AVFrame* dequeue();
    void release();
    int size();
    bool isEmpty();
    bool isFulled();

private:
    QQueue<AVFrame*> frameQueue;
    QMutex mutex;
    QWaitCondition waitCondition;
    int this_maxSize=100;
};

#endif // QFFMPEGFRAME_H
