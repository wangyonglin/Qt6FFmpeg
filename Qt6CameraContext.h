#ifndef QT6CAMERACONTEXT_H
#define QT6CAMERACONTEXT_H
#include <QObject>
#include <QString>
#include <QDebug>
#include <QSize>
#include "Qt6Object.h"


class QT6FFMPEG_EXPORT Qt6CameraContext : public QObject
{
    Q_OBJECT
public:
    explicit Qt6CameraContext(QObject *parent = nullptr);
    bool initFFmpegAll();
    void url(const QString &url);

public:
    const AVInputFormat * iformat=NULL;
    AVDictionary *dict;

    int width=1280;
    int height=720;
    int framerate=25;

    QMutex __mutex;
    QWaitCondition __condition;
    QString __url;
    AVFormatContext *__ifmt_ctx=nullptr;
    AVStream * __audio_stream=nullptr;
    AVStream * __video_stream=nullptr;
    AVCodecContext * __audio_dctx=nullptr;
    AVCodecContext * __video_dctx=nullptr;
    int __audio_stream_id=-1;
    int __video_stream_id=-1;
    Qt6Result demux_read(AVPacket **pkt,AVFormatContext *ifmt_ctx);
    Qt6Result decode_receive(AVFrame **frame, AVCodecContext *dctx, AVPacket *pkt);
    AVCodecContext *video_dctx();
    AVCodecContext *audio_dctx();
    AVStream *video_stream();
    AVStream *audio_stream();
    void close();
    void decode_close(AVCodecContext **avctx);
    void demux_close(AVFormatContext **ifmt_ctx);
    int audio_sid();
    int video_sid();
    AVFormatContext *ifmtctx();
public slots:
    void open();
protected slots:
    int decode_open(AVCodecContext **dctx, AVStream **stream, AVFormatContext *ifmt_ctx, AVMediaType media_type);

private slots:
    AVFormatContext *demux_open(const QString &url);
};

#endif // QT6CAMERACONTEXT_H
