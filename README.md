# Qt6FFmpeg
Qt6.7.3&amp;FFmpeg6.1

### examples
```
#include <QApplication>
#include <Qt6Camera>
#include <Qt6Player>
#include <Qt6TransparentPlayer>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    #透明通道RGBA
    Qt6TransparentPlayer w;
    # YUV420P
    #Qt6Player w;

    w.play("https://cesium.com/public/SandcastleSampleData/big-buck-bunny_trailer.mp4");
    w.show();
    return a.exec();
}
```

### project.pro

```
QT += core opengl
QT += gui multimedia  widgets openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += /home/wangyonglin/github/Qt6FFmpeg/include
LIBS += -L/home/wangyonglin/github/Qt6FFmpeg/build/Desktop_Qt_6_7_3-Debug -lQt6FFmpeg
LIBS += -L/opt/lib -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lswresample -lswscale
SOURCES += \
    main.cpp \
    Widget.cpp

HEADERS += \
    Widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
```

#

```CONFIG(debug, debug|release){  //处理debug
win32{
}
unix{
    contains(QT_ARCH, arm64){
                    message("arm64")
                    }else{
                    message("x86")
                    }
}
}else{     //处理release
win32{
}
unix{
}
```
    

```
    Qt6DemuxContext
        
    Qt6SoundContext
    Qt6VideoContext
```
