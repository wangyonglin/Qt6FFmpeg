#include "VideoDecoder.h"

VideoDecoder::VideoDecoder(QObject *parent)
    : FFmpegDecoder{parent}
{

    this->video_frame_queue=new FFmpegFrame(this);
    this->start();
}

VideoDecoder::~VideoDecoder()
{
    this->release();
    this->stop();
}

int VideoDecoder::running(FFmpegDemuxer * demuxer)
{
    int ret = -1;
    this->demuxer=demuxer;
    if(!this->isRunning())return ret;
    if((ret=RunningDecoder(this->demuxer->ifmt_ctx, AVMEDIA_TYPE_VIDEO))<0){
        char errmsg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "RunningDecoder failed" << errmsg;
        return ret;
    }

    // this->video_codec_ctx = avcodec_alloc_context3(NULL);
    // if(!this->video_codec_ctx)return ret;

    // this->video_stream_index=av_find_best_stream(this->demuxer->ifmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);

    // if( this->video_stream_index>=0)
    // {
    //     this->video_codecpar= this->demuxer->ifmt_ctx->streams[ this->video_stream_index]->codecpar;
    //     this->video_pts_base= this->demuxer->ifmt_ctx->streams[ this->video_stream_index]->time_base;
    //     this->video_pts_begin =  this->demuxer->ifmt_ctx->streams[ this->video_stream_index]->start_time;
    // }

    // if((ret=avcodec_parameters_to_context(this->video_codec_ctx, this->video_codecpar)) < 0) {
    //     char errmsg[AV_ERROR_MAX_STRING_SIZE];
    //     av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
    //     qDebug() << "avcodec_parameters_to_context failed" << errmsg;
    //     avcodec_free_context(&this->video_codec_ctx);
    //     return ret;
    // }

    // if(!(this->video_codec = avcodec_find_decoder(this->video_codec_ctx->codec_id))) {
    //     qDebug() << "avcodec_find_decoder failed";
    //     avcodec_free_context(&this->video_codec_ctx);
    //     return ret;
    // }


    // if((ret = avcodec_open2(this->video_codec_ctx, this->video_codec, NULL)) < 0) {
    //     char errmsg[AV_ERROR_MAX_STRING_SIZE];
    //     av_make_error_string(errmsg,AV_ERROR_MAX_STRING_SIZE, ret);
    //     qDebug() << "avcodec_open2 failed [video]" << errmsg;
    //     avcodec_free_context(& this->video_codec_ctx);
    //     return ret;
    // }
    this->YUV420Size = av_image_get_buffer_size(
        AV_PIX_FMT_YUV420P,
        this->dec_ctx->width,
        this->dec_ctx->height,1);
    if(this->YUV420Data){
        av_free(this->YUV420Data);
        this->YUV420Data=nullptr;
    }
    this->YUV420Data = (unsigned char *)av_malloc(this->YUV420Size*sizeof(uchar));
    emit sigFirst(this->YUV420Data,this->dec_ctx->width,this->dec_ctx->height);
    return 0;
}

void VideoDecoder::release()
{
    this->frameFinished=true;
    this->ReleaseDecoder();
}





void VideoDecoder::loop()
{

    if(state()==Running && !frameFinished){
        if (this->demuxer->synchronizer->get_video_synchronize() > this->demuxer->synchronizer->get_master_synchronize())
        {
            QThread::usleep(10);
            return;
        }

        if(!this->demuxer->video_pkt_queue->isEmpty()){
            if(this->video_frame_queue->size()  < 3){
                this->decoder(this->dec_ctx,this->demuxer->video_pkt_queue,this->video_frame_queue);
            }else{
                QThread::usleep(200);
                return;
            }
        }

        if(!this->video_frame_queue->isEmpty()){
            AVFrame * yuvFrame= this->video_frame_queue->dequeue();
            if(yuvFrame){
                int64_t pts_time= (yuvFrame->pts == AV_NOPTS_VALUE) ? NAN : yuvFrame->pts;
                this->demuxer->synchronizer->video_synchronize(pts_time,this->st->start_time,this->st->time_base);
                int bytes =0;
                for(int i=0;i<this->dec_ctx->height;i++){
                    memcpy( this->YUV420Data+bytes,yuvFrame->data[0]+yuvFrame->linesize[0]*i,this->dec_ctx->width);
                    bytes+=this->dec_ctx->width;
                }

                int u=this->dec_ctx->height>>1;
                for(int i=0;i<u;i++){
                    memcpy( this->YUV420Data+bytes,yuvFrame->data[1]+yuvFrame->linesize[1]*i,this->dec_ctx->width/2);
                    bytes+=this->dec_ctx->width/2;
                }

                for(int i=0;i<u;i++){
                    memcpy( this->YUV420Data+bytes,yuvFrame->data[2]+yuvFrame->linesize[2]*i,this->dec_ctx->width/2);
                    bytes+=this->dec_ctx->width/2;
                }

                emit newFrame();

                av_frame_free(&yuvFrame);
            }

        }
    }
}


void VideoDecoder::decoder(AVCodecContext *codec_ctx,FFmpegPacket *pkt_queue, FFmpegFrame *frame_queue)
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

void VideoDecoder::start(Priority pri)
{
 FFmpegThreader::start(pri);
}

void VideoDecoder::stop()
{
 FFmpegThreader::stop();
}

void VideoDecoder::pause()
{
    this->frameFinished=true;
    FFmpegThreader::pause();
    this->video_frame_queue->release();
}

void VideoDecoder::resume()
{
    this->frameFinished=false;
    FFmpegThreader::resume();
}



