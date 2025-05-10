#include "Qt6Packet.h"

Qt6Packet::Qt6Packet(QObject *parent)
    : QObject{parent}
{}
void Qt6Packet::enqueue(AVPacket *packet) {
    QMutexLocker locker(&mutex);
    if(packetQueue.size() <= this_maxSize){
        AVPacket *__packet = av_packet_alloc();
        av_packet_move_ref(__packet, packet);
        packetQueue.enqueue(__packet);
        waitCondition.wakeAll();
    }
}

AVPacket* Qt6Packet::dequeue() {
    QMutexLocker locker(&mutex);
    while (packetQueue.isEmpty()) {
        waitCondition.wait(&mutex);

    }
    return packetQueue.dequeue();
}

void Qt6Packet::release() {
    QMutexLocker locker(&mutex);
    while (!packetQueue.isEmpty()) {
        AVPacket *frame = packetQueue.dequeue();
        if(frame){
            av_packet_free(&frame); // 释放帧内存
        }
    }
}

int Qt6Packet::size()
{
    return packetQueue.size();
}

bool Qt6Packet::isEmpty()
{
    return packetQueue.isEmpty();
}

bool Qt6Packet::isFulled()
{
    if(!packetQueue.isEmpty()){
        if(packetQueue.size()>=this_maxSize){
            return true;
        }
    }
    return false;
}
