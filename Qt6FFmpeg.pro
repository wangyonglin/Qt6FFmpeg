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
    Qt6ConfigContext.h \
    Qt6DemuxContext.h \
    Qt6DemuxProcess.h \
    Qt6Frame.h \
    Qt6Object.h \
    Qt6Packet.h \
    Qt6Player.h \
    Qt6Process.h \
    Qt6Renderer.h \
    Qt6Resampler.h \
    Qt6SoundContext.h \
    Qt6SoundProcess.h \
    Qt6Speaker.h \
    Qt6Swscaler.h \
    Qt6VideoContext.h \
    Qt6VideoProcess.h \
    Qt6YUV420PRenderer.h \
    include/Qt6Camera \
    include/Qt6Player \
    include/Qt6TransparentPlayer

SOURCES += \
    Qt6Camera.cpp \
    Qt6CameraDecoder.cpp \
    Qt6CameraDemuxer.cpp \
    Qt6CameraHandler.cpp \
    Qt6ConfigContext.cpp \
    Qt6DemuxContext.cpp \
    Qt6DemuxProcess.cpp \
    Qt6Frame.cpp \
    Qt6Object.cpp \
    Qt6Packet.cpp \
    Qt6Player.cpp \
    Qt6Process.cpp \
    Qt6Renderer.cpp \
    Qt6Resampler.cpp \
    Qt6SoundContext.cpp \
    Qt6SoundProcess.cpp \
    Qt6Speaker.cpp \
    Qt6Swscaler.cpp \
    Qt6VideoContext.cpp \
    Qt6VideoProcess.cpp \
    Qt6YUV420PRenderer.cpp



