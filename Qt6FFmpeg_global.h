#ifndef QT6FFMPEG_GLOBAL_H
#define QT6FFMPEG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QT6FFMPEG_LIBRARY)
#define QT6FFMPEG_EXPORT Q_DECL_EXPORT
#else
#define QT6FFMPEG_EXPORT Q_DECL_IMPORT
#endif

#endif // QT6FFMPEG_GLOBAL_H
