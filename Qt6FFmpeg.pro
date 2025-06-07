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
    Qt6CameraContext.h \
    Qt6CameraProcess.h \
    Qt6FFmpegContext.h \
    Qt6FFmpegProcess.h \
    Qt6Frame.h \
    Qt6Object.h \
    Qt6Packet.h \
    Qt6Player.h \
    Qt6Renderer.h \
    Qt6Resampler.h \
    Qt6Speaker.h \
    Qt6Swscaler.h \
    Qt6Syncthing.h \
    Qt6Thread.h \
    Qt6YUV420PRenderer.h \
    include/Qt6Camera \
    include/Qt6Player \
    include/Qt6TransparentPlayer

SOURCES += \
    Qt6Camera.cpp \
    Qt6CameraContext.cpp \
    Qt6CameraProcess.cpp \
    Qt6FFmpegContext.cpp \
    Qt6FFmpegProcess.cpp \
    Qt6Frame.cpp \
    Qt6Object.cpp \
    Qt6Packet.cpp \
    Qt6Player.cpp \
    Qt6Renderer.cpp \
    Qt6Resampler.cpp \
    Qt6Speaker.cpp \
    Qt6Swscaler.cpp \
    Qt6Syncthing.cpp \
    Qt6Thread.cpp \
    Qt6YUV420PRenderer.cpp



