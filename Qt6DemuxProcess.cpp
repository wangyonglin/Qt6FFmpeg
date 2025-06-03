#include "Qt6DemuxProcess.h"

Qt6DemuxProcess::Qt6DemuxProcess(QObject *parent)
    : Qt6DemuxContext{parent}

{
    connect(this,&Qt6DemuxContext::finish,this,&Qt6DemuxProcess::finish,Qt::DirectConnection);
    m_pkt=av_packet_alloc();
}

Qt6DemuxProcess::~Qt6DemuxProcess()
{
    av_packet_free(&m_pkt);
}

// void Qt6DemuxProcess::start()
// {
//     m_exitloop=false;
//     Qt6DemuxContext::start();
// }

// void Qt6DemuxProcess::stop()
// {
//     m_exitloop=true;
//     Qt6DemuxContext::stop();
// }


void Qt6DemuxProcess::finish()
{
    //Qt6DemuxContext::stop();
}


void Qt6DemuxProcess::loopping()
{
   // qDebug() << metaObject()->className();

    if (state() && !m_exitloop) {
        if(config()->getAudioPacketSize()>24 || config()->getVideoPacketSize() >24){
            return;
        }
        if(read(&m_pkt)){
            if (m_pkt->stream_index == av_find_best_stream(config()->getFormatContext(), AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0))
            {
                config()->addAudioPacket(m_pkt);
                // qDebug() << "audio_pkt_queue" << config()->getAudioPacketSize();
            }
            else if(m_pkt->stream_index == av_find_best_stream(config()->getFormatContext(), AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0))
            {
                config()->addVideoPacket(m_pkt);
                // qDebug() << "video_pkt_queue" << config()->getVideoPacketSize();
            }
        }
    }

}

void Qt6DemuxProcess::opening()
{
    config()->start_synchronize();
}


