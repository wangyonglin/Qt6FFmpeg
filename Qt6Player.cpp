#include "Qt6Player.h"

Qt6Player::Qt6Player(QWidget *parent)
    : Qt6Renderer{parent},m_config(new Qt6ConfigContext(parent))
{
    m_demux_process= new Qt6DemuxProcess();
    m_video_process=new Qt6VideoProcess();
    m_audio_process=new Qt6SoundProcess();
    connect(m_video_process,&Qt6VideoProcess::refresh,this,&Qt6Renderer::refresh,Qt::DirectConnection);

    connect(this,&Qt6Player::start,m_demux_process,&Qt6DemuxProcess::start,Qt::DirectConnection);

    connect(this,&Qt6Player::start,m_video_process,&Qt6VideoProcess::start,Qt::DirectConnection);
    connect(this,&Qt6Player::start,m_audio_process,&Qt6SoundProcess::start,Qt::DirectConnection);

    connect(this,&Qt6Player::stop,m_demux_process,&Qt6DemuxProcess::stop,Qt::DirectConnection);
    connect(this,&Qt6Player::stop,m_video_process,&Qt6VideoProcess::stop,Qt::DirectConnection);
    connect(this,&Qt6Player::stop,m_audio_process,&Qt6SoundProcess::stop,Qt::DirectConnection);

}

Qt6Player::~Qt6Player()
{
    emit stop();
}

void Qt6Player::play(const QString &url)
{
    m_demux_process->m_exitloop=true;
    m_video_process->m_exitloop=true;
    m_audio_process->m_exitloop=true;
    m_video_process->close();
    m_audio_process->close();
    m_demux_process->close();
    m_config->init(url);
    m_demux_process->init(m_config);
    m_audio_process->init(m_config);
    m_video_process->init(m_config);

    m_demux_process->open();
    m_video_process->open();
    m_audio_process->open();

    m_demux_process->m_exitloop=false;
    m_video_process->m_exitloop=false;
    m_audio_process->m_exitloop=false;
    emit start();
    // m_demux_process->start();
    // m_video_process->start();
    // m_audio_process->start();
}
