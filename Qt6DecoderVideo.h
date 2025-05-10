#ifndef QT6DECODERVIDEO_H
#define QT6DECODERVIDEO_H
#include "Qt6Decoder.h"
#include "Qt6Thread.h"
#include "Qt6Demuxer.h"
#include "Qt6Frame.h"
#include "Qt6Synchronizer.h"
#include "Qt6Swscaler.h"
#include <QObject>

class Qt6DecoderVideo : public Qt6Decoder
{
    Q_OBJECT
public:
  typedef  enum {
        QT6_VIDEO_YUV420P,
        QT6_VIDEO_RGBA
    }Qt6PixelFormat;
    explicit Qt6DecoderVideo(QObject *parent = nullptr,Qt6PixelFormat = QT6_VIDEO_YUV420P);
    bool start(Qt6Demuxer *demux,Qt6Synchronizer *sync,Qt6Packet * pkt,Qt6Frame *frame);
signals:

    // Qt6Thread interface
protected:
    virtual void loop() override;
signals:
    void signalYUV420P(uint8_t * yuvdata,int yuvwidth,int yuvheight);
private:
    Qt6Swscaler *swscaler;
    Qt6Demuxer *demux;
    Qt6Synchronizer *sync;
    Qt6Packet * packet;
    Qt6Frame *frame;
    Qt6PixelFormat fmt=QT6_VIDEO_YUV420P;
    bool frameFinished= true;
};



#endif // QT6DECODERVIDEO_H
