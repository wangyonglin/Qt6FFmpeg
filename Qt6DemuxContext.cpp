#include "Qt6DemuxContext.h"

Qt6DemuxContext::Qt6DemuxContext(QObject *parent)
    : Qt6Process(parent)
{}

void Qt6DemuxContext::init(Qt6ConfigContext *config)
{
    Qt6Process::stop();
   // close();
    m_config=config;
}


bool Qt6DemuxContext::open()
{
    QMutexLocker locker(&m_mutex); // 自动加锁
    int ret =-1;
    qDebug() << "URL:" <<m_config->getURL();
    if(m_config->getURL().isEmpty())return false;
    if(m_config->m_ifmt_ctx){
        avformat_close_input(&m_config->m_ifmt_ctx);
        m_config->m_ifmt_ctx=nullptr;
    }
    if((ret = avformat_open_input(&m_config->m_ifmt_ctx, m_config->getURL().toLocal8Bit().data(), NULL, NULL)) < 0) {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "avformat_open_input failed" << errmsg;
        return false;
    }

    if((ret = avformat_find_stream_info(m_config->m_ifmt_ctx, NULL)) < 0) {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "avformat_find_stream_info failed" << errmsg;
        avformat_close_input(&m_config->m_ifmt_ctx);
        return false;
    }
    av_dump_format(m_config->m_ifmt_ctx, 0, m_config->getURL().toLocal8Bit().data(), 0);
    return true;
}

bool Qt6DemuxContext::read(AVPacket** pkt)
{
    int ret =-1;
    QMutexLocker locker(&m_mutex);
    if(!m_config->m_ifmt_ctx)return false;
    av_packet_unref(*pkt);
    if ((ret = av_read_frame(m_config->m_ifmt_ctx, *pkt)) < 0) {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        if (ret == AVERROR_EOF)
        {
            //qDebug() << "Reached end of file" << errmsg;
            emit finish();
            av_packet_unref(*pkt);
            return false;
        }
        return false;
    }
    return true;
}

Qt6ConfigContext *Qt6DemuxContext::config()
{
    return m_config;
}



void Qt6DemuxContext::close()
{
    QMutexLocker locker(&m_mutex);
    // avformat_close_input(&m_config->m_ifmt_ctx);
    // m_config->m_ifmt_ctx=nullptr;

}





