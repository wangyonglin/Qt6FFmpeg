#ifndef QT6CAMERAPLAYER_H
#define QT6CAMERAPLAYER_H

#include <QWidget>
#include <Qt6FFmpegDemuxer.h>
#include <Qt6FFmpegYUV420P.h>
#include <Qt6FFmpegDecoder.h>
#include <Qt6FFmpegThread.h>
#include <Qt6CameraHandler.h>
class Qt6CameraPlayer : public Qt6FFmpegYUV420P
{
    Q_OBJECT
public:
    explicit Qt6CameraPlayer(QWidget *parent = nullptr);
    ~Qt6CameraPlayer();
    Qt6CameraHandler *handle;
public:
    void OpenCamera(const QString &url);
};

#endif // QT6CAMERAPLAYER_H
