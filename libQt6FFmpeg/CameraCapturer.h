#ifndef CAMERACAPTURER_H
#define CAMERACAPTURER_H
#include "CameraDemuxer.h"
#include "CameraDecoder.h"
#include "VideoSwscaler.h"
#include <QObject>

#include "FFmpegThreader.h"



class CameraCapturer : public FFmpegThreader
{

    Q_OBJECT
public:
    explicit CameraCapturer(QObject *parent = nullptr);
    void OpenDeviceCapture(const QString &url);
    void StopDeviceCapture();
public slots:
signals:
    void sigFirst( uint8_t * data[],int w,int h);
    void newFrame();
private:
    QtRockchip::Qt6FFmpeg::CameraDemuxer *demux;
    QtRockchip::Qt6FFmpeg::CameraDecoder *video_decoder;
    VideoSwscaler *videoswscaler;
protected:
    virtual void loop() override;
};

#endif // CAMERACAPTURER_H
