#ifndef QT6CAMERAHANDLER_H
#define QT6CAMERAHANDLER_H
#include <QString>
#include <QObject>
#include <Qt6FFmpegDemuxer.h>
#include <Qt6FFmpegYUV420P.h>
#include <Qt6FFmpegDecoder.h>
#include <Qt6FFmpegThread.h>
#include <Qt6FFmpegSwscaler.h>
#include <Qt6FFmpegCore.h>

class Qt6CameraHandler : public Qt6FFmpegThread
{
    Q_OBJECT
public:
    explicit Qt6CameraHandler(QObject *parent = nullptr);
    Qt6FFmpegDemuxer *camera_demuxer;
    Qt6FFmpegDecoder *video_decoder;
    Qt6FFmpegSwscaler *videoswscaler;
    void openUrl(const QString &url);
signals:
    void signalUpdate(QByteArray yuvdata, int yuvwidth, int yuvheight);
    void signalYUV420P(uint8_t * yuvdata,int yuvwidth,int yuvheight);
    // Qt6FFmpegThread interface
protected:
    virtual void loop() override;

};

#endif // QT6CAMERAHANDLER_H
