#ifndef QT6FFMPEGDEMUXER_H
#define QT6FFMPEGDEMUXER_H
#include <QObject>
#include <QString>
#include <QDebug>
#include <Qt6FFmpegCore.h>
#include "Qt6FFmpeg_global.h"



class QT6FFMPEG_EXPORT Qt6FFmpegDemuxer : public QObject
{
    Q_OBJECT
public:
    explicit Qt6FFmpegDemuxer(QObject *parent = nullptr);
    bool initialize();
    void release();
    int open(const QString & url);
    int read(AVPacket * pkt);
public:
    AVFormatContext *ifmt_ctx= nullptr;
    const AVInputFormat * iformat=NULL;
    AVDictionary *dict;

    int width=1280;
    int height=720;
    int framerate=25;
    const AVCodec * video_decodec=NULL;
    int video_stream_index=-1;
    int audio_stream_index=-1;
    AVStream * video_stream=NULL;
};

#endif // QT6FFMPEGDEMUXER_H
