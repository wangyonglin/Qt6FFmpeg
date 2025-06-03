#ifndef QT6OBJECT_H
#define QT6OBJECT_H
extern "C" {
#include <libavcodec/avcodec.h>
#include "libavutil/avstring.h"
#include "libavutil/channel_layout.h"
#include "libavutil/eval.h"
#include "libavutil/mathematics.h"
#include "libavutil/pixdesc.h"
#include "libavutil/imgutils.h"
#include "libavutil/dict.h"
#include "libavutil/fifo.h"
#include "libavutil/parseutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/time.h"
#include "libavutil/bprint.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
#include "libavutil/tx.h"
#include "libswresample/swresample.h"

#include "libavfilter/avfilter.h"
#include "libavfilter/buffersink.h"
#include "libavfilter/buffersrc.h"
}
#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QtCore/qglobal.h>

#if defined(QT6FFMPEG_LIBRARY)
#define QT6FFMPEG_EXPORT Q_DECL_EXPORT
#else
#define QT6FFMPEG_EXPORT Q_DECL_IMPORT
#endif



class Qt6Object : public QObject
{
    Q_OBJECT
public:
    explicit Qt6Object(QObject *parent = nullptr);

signals:
};

#endif // QT6OBJECT_H
