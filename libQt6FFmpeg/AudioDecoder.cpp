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
    // this->audio_codec_ctx = avcodec_alloc_context3(NULL);

    // this->audio_stream_index=av_find_best_stream(this->demuxer->ifmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);

    // if( this->audio_stream_index>=0)
    // {
    //     this->audio_codecpar= this->demuxer->ifmt_ctx->streams[ this->audio_stream_index]->codecpar;
    //     this->audio_pts_base= this->demuxer->ifmt_ctx->streams[ this->audio_stream_index]->time_base;
    //     this->audio_pts_begin =  this->demuxer->ifmt_ctx->streams[ this->audio_stream_index]->start_time;
    // }
    // if((ret=avcodec_parameters_to_context(this->audio_codec_ctx, this->audio_codecpar)) < 0) {
    //     char errmsg[AV_ERROR_MAX_STRING_SIZE];
    //     av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
    //     qDebug() << tr("Failed to copy %0 codec parameters to decoder context").arg(errmsg);
    //     avcodec_free_context(&this->audio_codec_ctx);
    //     return ret;
    // }
    // // h264
    // // h264_qsv  AV_CODEC_ID_H264
    // //    avcodec_find_decoder_by_name()

    // //    if(AV_CODEC_ID_H264 == codec_ctx_->codec_id)
    // //        codec = avcodec_find_decoder_by_name("h264_qsv");
    // //    else

    // if(!(audio_codec = avcodec_find_decoder(this->audio_codec_ctx->codec_id))) {
    //     qDebug() << "avcodec_find_decoder failed";
    //     avcodec_free_context(&this->audio_codec_ctx);
    //     return ret;
    // }

    // if((ret = avcodec_open2(this->audio_codec_ctx, audio_codec, NULL)) < 0) {
    //     char errmsg[AV_ERROR_MAX_STRING_SIZE];
    //     av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
    //     qDebug() << "avcodec_open2 failed [audio]" << errmsg;
    //     avcodec_free_context(&this->audio_codec_ctx);
    //     return ret;
    // }


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

    if(state()==Running && !frameFinished){
        if (this->demuxer->synchronizer->get_audio_synchronize() > this->demuxer->synchronizer->get_master_synchronize())
        {
            QThread::usleep(10);
            return;
        }
        if(!this->demuxer->audio_pkt_queue->isEmpty()){
            if(this->audio_frame_queue->size() <3){
                decoder(this->dec_ctx,this->demuxer->audio_pkt_queue,this->audio_frame_queue);
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





void AudioDecoder::decoder(AVCodecContext *codec_ctx,FFmpegPacket *pkt_queue, FFmpegFrame *frame_queue)
{
    if(!codec_ctx)return;
    if(pkt_queue->isEmpty())return;
    AVPacket * pkt= pkt_queue->dequeue();
    if(!pkt)return;
    int ret = avcodec_send_packet(codec_ctx, pkt);
    av_packet_free(&pkt);
    if (ret < 0)
    {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "avcodec_send_packet failed" << errmsg;
        return ;
    }
    AVFrame* frame = av_frame_alloc();
    if(!frame)return;
    ret = avcodec_receive_frame(codec_ctx, frame);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
        return ;
    }else if (ret < 0) {
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "avcodec_receive_frame failed" << errmsg;
        return ;
    }
    frame_queue->enqueue(frame);
    av_frame_free(&frame);
}



