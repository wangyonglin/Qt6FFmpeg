#ifndef QFFMPEGTOOL_H
#define QFFMPEGTOOL_H

#include <QObject>
#include <QImage>
#include "QFFmpegHeaders.h"
#include "libQt6FFmpeg_global.h"
namespace Qt6FFmpeg {
class LIBQT6FFMPEG_EXPORT QFFmpegTool : public QObject
{
    Q_OBJECT
public:
    explicit QFFmpegTool(QObject *parent = nullptr);
    static QImage AVFrame2RGBA(AVFrame *frame);
signals:
};
}


#endif // QFFMPEGTOOL_H
