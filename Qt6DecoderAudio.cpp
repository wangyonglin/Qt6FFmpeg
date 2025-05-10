#include "Qt6DecoderAudio.h"

Qt6DecoderAudio::Qt6DecoderAudio(QObject *parent)
    : Qt6Decoder{parent}
{
    resampler =new Qt6Resampler(parent);
    speaker= new Qt6Speaker(this);
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleFormat(QAudioFormat::SampleFormat::Int16);
    speaker->init(format);
    speaker->create();
}



int Qt6DecoderAudio::start(Qt6Demuxer *demux,
                           Qt6Synchronizer * sync,
                           Qt6Packet *packet,
                           Qt6Frame *frame)
{
    this->demux=demux;
    this->sync=sync;
    this->packet=packet;
    this->frame=frame;
    int ret =0;

    if(Qt6Thread::isRunning()){
        Qt6Thread::stop();
    }
    if((ret=decodeing(demux->ifmt_ctx, AVMEDIA_TYPE_AUDIO))<0){
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "RunningDecoder failed" << errmsg;
        return ret;
    }

    resampler->init(dec_ctx,AV_CH_LAYOUT_STEREO,44100,AV_SAMPLE_FMT_S16);
    // int data_size = av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
    // QAudioFormat format;
    // format.setSampleRate(44100);
    // format.setChannelCount(2);
    // format.setSampleFormat(QAudioFormat::SampleFormat::Int16);
    // ffmpegSpeaker->init(format);
    // ffmpegSpeaker->create();
    Qt6Decoder::start();
    frameFinished=false;
return 0;
}
void Qt6DecoderAudio::loop()
{
    int ret = -1;
    if(state()==Running && !frameFinished){
        if (sync->get_audio_synchronize() > sync->get_master_synchronize())
        {
            QThread::usleep(10);
            return;
        }
        if(!packet->isEmpty()){
            if(this->frame->size() <3){
                if((ret= depacket(packet,this->frame)) <0 ){
                    char errmsg[AV_ERROR_MAX_STRING_SIZE];
                    av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
                    qDebug() << "audio depacket " << errmsg;
                    return ;
                }
            }else{
                QThread::usleep(200);
                return;
            }
        }

        if(!frame->isEmpty()){
            AVFrame * sampleframe= this->frame->dequeue();
            if(sampleframe){
                int64_t pts_time= (sampleframe->pts == AV_NOPTS_VALUE) ? NAN : sampleframe->pts;
                sync->audio_synchronize(pts_time,this->st->start_time,this->st->time_base);
                QByteArray bytes= resampler->convert(sampleframe);
                speaker->write(bytes);
                av_frame_free(&sampleframe);
            }

        }
    }
}
