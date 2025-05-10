#ifndef QT6PLAYER_H
#define QT6PLAYER_H

#include <QWidget>
#include "Qt6Packet.h"
#include "Qt6Frame.h"
#include "Qt6Demuxer.h"
#include "Qt6Synchronizer.h"
#include "Qt6YUV420PRenderer.h"
#include "Qt6Swscaler.h"
#include "Qt6DecoderVideo.h"
#include "Qt6DecoderAudio.h"

class Qt6Player : public Qt6YUV420PRenderer
{
    Q_OBJECT
public:
    explicit Qt6Player(QWidget *parent = nullptr);
    ~Qt6Player();
    void play(const QString &url);
public:
    Qt6Swscaler * swscaler;
    Qt6Demuxer *demux=nullptr;
    Qt6Packet *vpkt=nullptr;
    Qt6Packet *apkt=nullptr;
    Qt6Synchronizer *sync=nullptr;
    Qt6Frame *vframe;
    Qt6Frame *aframe;
    Qt6DecoderVideo *decode_video;
    Qt6DecoderAudio *decode_audio;
signals:
};

#endif // QT6PLAYER_H
