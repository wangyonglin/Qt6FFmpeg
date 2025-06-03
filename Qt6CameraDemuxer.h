#ifndef QT6CAMERADEMUXER_H
#define QT6CAMERADEMUXER_H
#include <QObject>
#include <QString>
#include <QDebug>
#include "Qt6Object.h"


class QT6FFMPEG_EXPORT Qt6CameraDemuxer : public QObject
{
    Q_OBJECT
public:
    explicit Qt6CameraDemuxer(QObject *parent = nullptr);
    bool initFFmpegAll();
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

#endif // QT6CAMERADEMUXER_H
