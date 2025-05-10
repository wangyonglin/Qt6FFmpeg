#ifndef QT6CAMERA_H
#define QT6CAMERA_H

#include <QWidget>
#include "Qt6CameraDemuxer.h"
#include "Qt6YUV420PRenderer.h"
#include "Qt6CameraDecoder.h"
#include "Qt6Thread.h"
#include "Qt6CameraHandler.h"
class Qt6Camera : public Qt6YUV420PRenderer
{
    Q_OBJECT
public:
    explicit Qt6Camera(QWidget *parent = nullptr);
    ~Qt6Camera();
    Qt6CameraHandler *handle;
public:
    void OpenCamera(const QString &url);
};

#endif // QT6CAMERA_H
