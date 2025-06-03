#ifndef QT6FRAME_H
#define QT6FRAME_H
#include "Qt6Object.h"
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>
#include <QThread>
#include <QObject>

class Qt6Frame : public QObject
{
    Q_OBJECT
public:
    explicit Qt6Frame(QObject *parent = nullptr);
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

#endif // QT6FRAME_H
