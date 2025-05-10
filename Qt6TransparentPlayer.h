#ifndef QT6TRANSPARENTPLAYER_H
#define QT6TRANSPARENTPLAYER_H

#include <QWidget>
#include "Qt6Packet.h"
#include "Qt6Frame.h"
#include "Qt6Demuxer.h"
#include "Qt6Synchronizer.h"
#include "Qt6RGBARenderer.h"
#include "Qt6Swscaler.h"
#include "Qt6DecoderVideo.h"
#include "Qt6DecoderAudio.h"
#include "Qt6Core.h"

class Qt6TransparentPlayer : public Qt6RGBARenderer
{
    Q_OBJECT
public:
    explicit Qt6TransparentPlayer(QWidget *parent = nullptr);
    ~Qt6TransparentPlayer();
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
};

#endif // QT6TRANSPARENTPLAYER_H
