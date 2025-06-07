#include "Qt6Camera.h"

Qt6Camera::Qt6Camera(QWidget *parent)
    : Qt6Renderer{parent}
{
    camera_process = new Qt6CameraProcess(parent);
    connect(camera_process,&Qt6CameraProcess::refresh,this,&Qt6Renderer::refresh,Qt::DirectConnection);
}

Qt6Camera::~Qt6Camera()
{
    camera_process->deleteLater();
}

void Qt6Camera::play(const QString &url)
{
    camera_process->url(url);
    camera_process->start();
}

void Qt6Camera::setting(const QSize &size,int framerate)
{
    camera_process->setting(size,framerate);
}

void Qt6Camera::stop()
{
    camera_process->stop();
}




