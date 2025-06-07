#ifndef QT6FFMPEGPROCESS_H
#define QT6FFMPEGPROCESS_H

#include <QObject>
#include "Qt6FFmpegContext.h"
#include "Qt6Swscaler.h"
#include "Qt6Speaker.h"
#include "Qt6Resampler.h"
#include "Qt6Syncthing.h"
#include "Qt6Packet.h"
#include "Qt6Frame.h"

class Qt6FFmpegProcess : public QThread
{
    Q_OBJECT
public:
    explicit Qt6FFmpegProcess(QObject *parent = nullptr);
    ~Qt6FFmpegProcess();
    void start();
    void stop();

private:
    //init
    Qt6Swscaler *sws=nullptr;
    Qt6Speaker * speaker=nullptr;
    Qt6Resampler *resampler=nullptr;
    Qt6FFmpegContext *fctx=nullptr;
    Qt6Syncthing * syncth=nullptr;
    void init();
    //demux

    void onDemuxHandle();
    bool demux_flag=false;

    //audio
    bool audio_flag=false;
    Qt6Frame * audio_frame=nullptr;
    Qt6Packet * audio_packet=nullptr;
    void onAudioHandle();

    //video
    bool video_flag=false;
    Qt6Packet * video_packet=nullptr;
    Qt6Frame * video_frame=nullptr;
    void onVideoHandle();
signals:
public slots:
    void url(const QString &url);
signals:
    void refresh(uint8_t *, const QSize size);

    // QThread interface
protected:
    virtual void run() override;
};

#endif // QT6FFMPEGPROCESS_H
