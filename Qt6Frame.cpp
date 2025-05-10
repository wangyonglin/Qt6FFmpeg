#include "Qt6Frame.h"

Qt6Frame::Qt6Frame(QObject *parent)
    : QObject{parent}
{}
void Qt6Frame::enqueue(AVFrame *frame) {
    QMutexLocker locker(&mutex);
    if(frame){
        if(frameQueue.size() <= this_maxSize){
            AVFrame *__frame = av_frame_alloc();
            av_frame_move_ref(__frame, frame);
            frameQueue.enqueue(__frame);
            waitCondition.wakeAll();
        }
    }

}

AVFrame* Qt6Frame::dequeue() {
    QMutexLocker locker(&mutex);
    while (frameQueue.isEmpty()) {
        waitCondition.wait(&mutex);

    }
    return frameQueue.dequeue();
}

void Qt6Frame::release() {
    QMutexLocker locker(&mutex);
    while (!frameQueue.isEmpty()) {
        AVFrame *frame = frameQueue.dequeue();
        if(frame){
            av_frame_free(&frame); // 释放帧内存
        }
    }
}

int Qt6Frame::size()
{
    return frameQueue.size();
}

bool Qt6Frame::isEmpty()
{
    return frameQueue.isEmpty();
}

bool Qt6Frame::isFulled()
{
    if(!frameQueue.isEmpty()){
        if(frameQueue.size()>=this_maxSize){
            return true;
        }
    }
    return false;
}
