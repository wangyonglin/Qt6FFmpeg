TEMPLATE = lib
QT       += core opengl
QT += gui core gui multimedia opengl widgets openglwidgets

TEMPLATE = lib
DEFINES += LIBQT6FFMPEG_LIBRARY
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#INCLUDEPATH += $$PWD/include
#LIBS += -L$$PWD/lib/linux/aarch64 -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lswresample -lswscale -Wl,-Bsymbolic
#INCLUDEPATH +=$$PWD/../libQt6FFmpeg/include
CONFIG += debug_and_release
linux-g++* {
    message(Compling with linux-g++)
    CONFIG(debug, debug|release){
        message(Debug build)
        TARGET = Qt6FFmpeg
        # INCLUDEPATH += -I/usr/local/include
        # LIBS += -L/usr/local/lib/librockchip_mpp.a
        # LIBS += -L/usr/local/lib/libavdevice.a
        # LIBS += -L/usr/local/lib/libavformat.a
        # LIBS += -L/usr/local/lib/libavutil.a
        # LIBS += -L/usr/local/lib/libavfilter.a
        # LIBS += -L/usr/local/lib/libswscale.a
        # LIBS += -L/usr/local/lib/libswresample.a
        # LIBS += -L/usr/local/lib/libavcodec.a
        # LIBS += -L/usr/local/lib/librga.a
        # LIBS += -pthread -lm -latomic -llzma -lz -Wl,-Bsymbolic
        release

    }

    CONFIG(release, debug|release){
        message(Release build)
        TARGET = Qt6FFmpeg
    }

    #DEFINES += CSM_TARGET_LINUX_GL
}
SOURCES += \
    AudioDecoder.cpp \
    FFmpegDecoder.cpp \
    FFmpegDemuxer.cpp \
    FFmpegFrame.cpp \
    FFmpegPacket.cpp \
    FFmpegParameters.cpp \
    FFmpegResampler.cpp \
    FFmpegSpeaker.cpp \
    FFmpegSynchronizer.cpp \
    FFmpegThreader.cpp \
    RGBAPlayer.cpp \
    VideoDecoder.cpp \
    VideoSwscaler.cpp \
    YUV420PPlayer.cpp

HEADERS += \
    AudioDecoder.h \
    FFmpegDecoder.h \
    FFmpegDemuxer.h \
    FFmpegFrame.h \
    FFmpegPacket.h \
    FFmpegParameters.h \
    FFmpegResampler.h \
    FFmpegSpeaker.h \
    FFmpegSynchronizer.h \
    FFmpegThreader.h \
    QFFmpegHeaders.h \
    RGBAPlayer.h \
    VideoSwscaler.h \
    YUV420PPlayer.h \
    libQt6FFmpeg_global.h \
    VideoDecoder.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    OpenGLFFmpegPlayer.fsh \
    OpenGLFFmpegPlayer.vsh \
    README.md

RESOURCES += \
    resources.qrc
