#include "Qt6CameraPlayer.h"

Qt6CameraPlayer::Qt6CameraPlayer(QWidget *parent)
    : Qt6FFmpegYUV420P{parent}
{
    handle=new Qt6CameraHandler(parent);
    connect(handle,&Qt6CameraHandler::signalYUV420P,this,&Qt6CameraPlayer::slotUpdate,Qt::DirectConnection);
}

Qt6CameraPlayer::~Qt6CameraPlayer()
{
    handle->stop();
}

void Qt6CameraPlayer::OpenCamera(const QString &url)
{
    handle->openUrl(url);
    handle->start();
}



