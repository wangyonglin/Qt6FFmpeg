#include "Qt6SoundProcess.h"

Qt6SoundProcess::Qt6SoundProcess(QObject *parent)
    : Qt6SoundContext{parent},
    speaker(new Qt6Speaker(this)),
    resampler(new Qt6Resampler(this))
{
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleFormat(QAudioFormat::SampleFormat::Int16);
    speaker->init(format);
    speaker->create();
}

Qt6SoundProcess::~Qt6SoundProcess()
{

}

// void Qt6SoundProcess::start()
// {

//     m_exitloop=false;

//     Qt6SoundContext::start();
// }

// void Qt6SoundProcess::stop()
// {
//     m_exitloop=true;
//     Qt6SoundContext::stop();
// }




void Qt6SoundProcess::loopping()
{

    AVFrame *frame=av_frame_alloc();
    if(state() && !m_exitloop){
        if (config()->get_audio_synchronize() > config()->get_master_synchronize())
        {
            QThread::usleep(10);
            return;
        }

        if(!config()->isAudioPacketEmpty() && (config()->getAudioFrameSize()<24)){
            AVPacket*tmppacket= config()->getAudioPacket();
            if(tmppacket){
                if(receive(tmppacket,&frame)){
                    config()->addAudioFrame(frame);
                    av_packet_free(&tmppacket);
                }
            }


        }

        if(!config()->isAudioFrameEmpty()){
            AVFrame * sampleframe= config()->getAudioFrame();
            if(sampleframe){
                config()->audio_synchronize(sampleframe,config()->getAudioStream()->start_time,config()->getAudioStream()->time_base);

                QByteArray bytes= resampler->convert(sampleframe);
                speaker->write(bytes);
                av_frame_free(&sampleframe);
            }
        }
        if(state())return;
    }
    av_frame_free(&frame);


}

void Qt6SoundProcess::opening()
{
    resampler->init(config()->getAudioCodecContext(),AV_CH_LAYOUT_STEREO,44100,AV_SAMPLE_FMT_S16);
}
