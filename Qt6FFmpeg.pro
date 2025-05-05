QT += core opengl
QT += gui multimedia  widgets openglwidgets
TEMPLATE = lib
DEFINES += QT6FFMPEG_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target


HEADERS += \
    Qt6CameraHandler.h \
    Qt6CameraPlayer.h \
    Qt6FFmpegCore.h \
    Qt6FFmpegDecoder.h \
    Qt6FFmpegDemuxer.h \
    Qt6FFmpegSwscaler.h \
    Qt6FFmpegThread.h \
    Qt6FFmpegYUV420P.h \
    Qt6FFmpeg_global.h

SOURCES += \
    Qt6CameraHandler.cpp \
    Qt6CameraPlayer.cpp \
    Qt6FFmpegCore.cpp \
    Qt6FFmpegDecoder.cpp \
    Qt6FFmpegDemuxer.cpp \
    Qt6FFmpegSwscaler.cpp \
    Qt6FFmpegThread.cpp \
    Qt6FFmpegYUV420P.cpp

DISTFILES += \
    include/Qt6FFmpegDemuxer \
    include/Qt6FFmpegThread \
    include/Qt6FFmpegYUV420P
