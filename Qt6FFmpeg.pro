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

# Default rules for deployment.
# qnx: target.path = /tmp/$${TARGET}/bin
# else: unix:!android: target.path = /opt/$${TARGET}/bin
# !isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    LICENSE \
    Qt6FFmpeg.pro.user \
    README.md \
    fragment.frag \
    vertex.vert

HEADERS += \
    Qt6Camera.h \
    Qt6CameraDecoder.h \
    Qt6CameraDemuxer.h \
    Qt6CameraHandler.h \
    Qt6Core.h \
    Qt6Decoder.h \
    Qt6DecoderAudio.h \
    Qt6DecoderVideo.h \
    Qt6Demuxer.h \
    Qt6FFmpeg_global.h \
    Qt6Frame.h \
    Qt6Packet.h \
    Qt6Player.h \
    Qt6RGBARenderer.h \
    Qt6Resampler.h \
    Qt6Speaker.h \
    Qt6Swscaler.h \
    Qt6Synchronizer.h \
    Qt6Thread.h \
    Qt6YUV420PRenderer.h \
    include/Qt6Camera

SOURCES += \
    Qt6Camera.cpp \
    Qt6CameraDecoder.cpp \
    Qt6CameraDemuxer.cpp \
    Qt6CameraHandler.cpp \
    Qt6Core.cpp \
    Qt6Decoder.cpp \
    Qt6DecoderAudio.cpp \
    Qt6DecoderVideo.cpp \
    Qt6Demuxer.cpp \
    Qt6Frame.cpp \
    Qt6Packet.cpp \
    Qt6Player.cpp \
    Qt6RGBARenderer.cpp \
    Qt6Resampler.cpp \
    Qt6Speaker.cpp \
    Qt6Swscaler.cpp \
    Qt6Synchronizer.cpp \
    Qt6Thread.cpp \
    Qt6YUV420PRenderer.cpp



