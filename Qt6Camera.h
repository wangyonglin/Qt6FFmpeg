#ifndef QT6CAMERA_H
#define QT6CAMERA_H
#include <QString>
#include <QObject>
#include "Qt6CameraContext.h"
#include "Qt6YUV420PRenderer.h"
#include "Qt6CameraProcess.h"
#include "Qt6Swscaler.h"
#include "Qt6Object.h"
#include "Qt6Renderer.h"
class Qt6Camera : public Qt6Renderer
{
    Q_OBJECT
public:
    explicit Qt6Camera(QWidget *parent = nullptr);
    ~Qt6Camera();
    Qt6CameraProcess *camera_process= nullptr;
public slots:
    void play(const QString &url);
    void setting(const QSize &size,int framerate=25);
    void stop();
};

#endif // QT6CAMERA_H
