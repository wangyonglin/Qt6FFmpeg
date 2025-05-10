#ifndef QT6CAMERAHANDLER_H
#define QT6CAMERAHANDLER_H
#include <QString>
#include <QObject>
#include "Qt6CameraDemuxer.h"
#include "Qt6YUV420PRenderer.h"
#include "Qt6CameraDecoder.h"
#include "Qt6Thread.h"
#include "Qt6Swscaler.h"
#include "Qt6Core.h"

class Qt6CameraHandler : public Qt6Thread
{
    Q_OBJECT
public:
    explicit Qt6CameraHandler(QObject *parent = nullptr);
    Qt6CameraDemuxer *camera_demuxer;
    Qt6CameraDecoder *video_decoder;
    Qt6Swscaler *videoswscaler;
    void openUrl(const QString &url);
signals:
    void signalUpdate(QByteArray yuvdata, int yuvwidth, int yuvheight);
    void signalYUV420P(uint8_t * yuvdata,int yuvwidth,int yuvheight);
    // Qt6FFmpegThread interface
protected:
    virtual void loop() override;

};

#endif // QT6CAMERAHANDLER_H
