#include "Qt6TransparentPlayer.h"




Qt6TransparentPlayer::Qt6TransparentPlayer(QWidget *parent)
    : Qt6RGBARenderer{parent}
{
    vpkt=new Qt6Packet(this);
    apkt= new Qt6Packet(this);
    demux= new Qt6Demuxer(this);
    sync= new Qt6Synchronizer(this);
    vframe= new Qt6Frame(this);
    aframe = new Qt6Frame(this);
    decode_video= new Qt6DecoderVideo(this,QT6_VIDEO_RGBA);
    decode_audio= new Qt6DecoderAudio(this);
    connect(decode_video,&Qt6DecoderVideo::refresh,this,&Qt6RGBARenderer::refresh,Qt::DirectConnection);
}

Qt6TransparentPlayer::~Qt6TransparentPlayer()
{
    decode_video->stop();
    decode_audio->stop();
    demux->stop();
    demux->release();
    vframe->release();
    aframe->release();
    vpkt->release();
    apkt->release();
}

void Qt6TransparentPlayer::play(const QString &url)
{
    Q_UNUSED(url);
    demux->start(url,sync,vpkt,apkt);
    decode_video->start(demux,sync,vpkt,vframe);
    decode_audio->start(demux,sync,apkt,aframe);

}
