#include "Qt6Player.h"

Qt6Player::Qt6Player(QWidget *parent)
    : Qt6Renderer{parent}
{

    ffmpeg_process = new Qt6FFmpegProcess();
    connect(ffmpeg_process,&Qt6FFmpegProcess::refresh,this,&Qt6Renderer::refresh,Qt::DirectConnection);
}

Qt6Player::~Qt6Player()
{
    ffmpeg_process->stop();

}

void Qt6Player::play(const QString &url)
{
    ffmpeg_process->url(url);
    ffmpeg_process->start();
}
