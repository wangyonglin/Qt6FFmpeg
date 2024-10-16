#include "AudioDecoder.h"



AudioDecoder::AudioDecoder(QObject *parent)
    : FFmpegDecoder(parent)
{
    ffmpegSpeaker= new  Qt6FFmpeg::FFmpegSpeaker(this);
    ffmpegResample = new FFmpegResampler(this);
    this->audio_frame_queue=new FFmpegFrame(this);

    this->start();
}

AudioDecoder::~AudioDecoder()
{
    this->release();
    this->stop();
}

int AudioDecoder::running(FFmpegDemuxer *demuxer)
{
    this->demuxer=demuxer;
    int ret =0;
    if((ret=RunningDecoder(this->demuxer->ifmt_ctx, AVMEDIA_TYPE_AUDIO))<0){
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "RunningDecoder failed" << errmsg;
        return ret;
    }

    ffmpegResample->init(this->dec_ctx,AV_CH_LAYOUT_STEREO,44100,AV_SAMPLE_FMT_S16);
    // int data_size = av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleFormat(QAudioFormat::SampleFormat::Int16);
    ffmpegSpeaker->init(format);
    ffmpegSpeaker->create();
}

void AudioDecoder::release()
{
    this->frameFinished=true;
    this->ReleaseDecoder();
    ffmpegResample->release();
}
void AudioDecoder::start(Priority pri)
{
    FFmpegThreader::start(pri);
}

void AudioDecoder::stop()
{
    FFmpegThreader::stop();
}
void AudioDecoder::pause()
{
    this->frameFinished=true;
    FFmpegThreader::pause();
    this->audio_frame_queue->release();   
}

void AudioDecoder::resume()
{
    this->frameFinished=false;
    FFmpegThreader::resume();
}





void AudioDecoder::loop()
{
    int ret = -1;
    if(state()==Running && !frameFinished){
        if (this->demuxer->synchronizer->get_audio_synchronize() > this->demuxer->synchronizer->get_master_synchronize())
        {
            QThread::usleep(10);
            return;
        }
        if(!this->demuxer->audio_pkt_queue->isEmpty()){
            if(this->audio_frame_queue->size() <3){
                if((ret= DecodePacket(this->demuxer->audio_pkt_queue,this->audio_frame_queue)) <0 ){
                    char errmsg[AV_ERROR_MAX_STRING_SIZE];
                    av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
                    qDebug() << "running decoder failed [audio]" << errmsg;
                    return ;
                }
            }else{
                QThread::usleep(200);
                return;
            }
        }

        if(!this->audio_frame_queue->isEmpty()){
            AVFrame * frame= this->audio_frame_queue->dequeue();
            if(frame){
                int64_t pts_time= (frame->pts == AV_NOPTS_VALUE) ? NAN : frame->pts;
                this->demuxer->synchronizer->audio_synchronize(pts_time,this->st->start_time,this->st->time_base);
                QByteArray bytes= ffmpegResample->convert(frame);
                ffmpegSpeaker->write(bytes);
                av_frame_free(&frame);
            }

        }
    }
}








