QT += core opengl
QT += gui multimedia  widgets openglwidgets
TEMPLATE = lib
DEFINES += QT6FFMPEGCORE_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Qt6FFmpegCore.cpp \
    Qt6FFmpegThread.cpp \
    Qt6FFmpegYUV420P.cpp

HEADERS += \
    Qt6FFmpegCore_global.h \
    Qt6FFmpegCore.h \
    Qt6FFmpegThread.h \
    Qt6FFmpegYUV420P.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
