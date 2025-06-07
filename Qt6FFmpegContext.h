#ifndef QT6FFMPEGCONTEXT_H
#define QT6FFMPEGCONTEXT_H
#include "Qt6Object.h"
#include <QObject>
#include <QMutexLocker>
#include <QThread>

class Qt6FFmpegContext : public QObject
{
    Q_OBJECT
public:

    explicit Qt6FFmpegContext(QObject *parent = nullptr);
    AVCodecContext*audio_dctx();
    AVCodecContext*video_dctx();
    int audio_sid();
    int video_sid();
    AVStream * audio_stream();
    AVStream * video_stream();
    Qt6Result demux_read(AVPacket **pkt);
    Qt6Result decode_receive(AVFrame **frame, AVCodecContext *dctx, AVPacket *pkt);
public slots:
    void url(const QString &url);
    void open();
    void close();
private:
    AVFormatContext * demux_open(const QString &url);
    void demux_close(AVFormatContext **ifmt_ctx);
    int decode_open(AVCodecContext ** dctx ,AVStream ** stream,AVFormatContext *ifmt_ctx,enum AVMediaType media_type);
    void decode_close(AVCodecContext **avctx);
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
    bool __loop=false;
signals:
    void resultReady(const QString &url);
    void demux_finish();
};

#endif // QT6FFMPEGCONTEXT_H
