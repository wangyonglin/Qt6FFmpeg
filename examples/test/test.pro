QT       += core gui opengl widgets multimedia

# In Qt6 to use QOpenglWidget, we need add QT += openglwidgets.
greaterThan(QT_MAJOR_VERSION, 5){
    message(Building with Qt6 or Higher)
    QT += openglwidgets
}

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#



CONFIG += debug_and_release
linux-g++* {
    message(Compling with linux-g++)
    CONFIG(debug, debug|release){
        message(Debug build)
     INCLUDEPATH += $$PWD/../../libQt6FFmpeg/include
     LIBS += -L$$PWD/../../build/Desktop_Qt_6_7_3-Debug/libQt6FFmpeg -lQt6FFmpeg
     #LIBS += -L$$PWD/../../libQt6FFmpeg/lib  \
     LIBS += -L/opt/lib -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lswresample -lswscale
     LIBS += -lrga -lrockchip_mpp
     #-lssl -lcrypto -lx264 -ldrm
     LIBS += -pthread -lm -latomic -llzma -lz -Wl,-Bsymbolic -ldl
        release
    }

    CONFIG(release, debug|release){
        message(Release build)
    }

    DEFINES += CSM_TARGET_LINUX_GL
}


SOURCES += \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

