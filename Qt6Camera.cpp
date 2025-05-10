#include "Qt6Camera.h"

Qt6Camera::Qt6Camera(QWidget *parent)
    : Qt6YUV420PRenderer{parent}
{
    handle=new Qt6CameraHandler(parent);
    connect(handle,&Qt6CameraHandler::signalYUV420P,this,&Qt6Camera::slotUpdate,Qt::DirectConnection);
}

Qt6Camera::~Qt6Camera()
{
    handle->stop();
}

void Qt6Camera::OpenCamera(const QString &url)
{
    handle->openUrl(url);
    handle->start();
}



