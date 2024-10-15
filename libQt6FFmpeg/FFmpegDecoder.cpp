#include "FFmpegDecoder.h"

FFmpegDecoder::FFmpegDecoder(QObject *parent)
    : FFmpegThreader{parent}
{}

int FFmpegDecoder::RunningDecoder(AVFormatContext *fmt_ctx, enum AVMediaType type)
{
    int ret;


    ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not find %s stream in input file '%s'\n",
                av_get_media_type_string(type));
        return ret;
    } else {
        this->stream_index = ret;
        this->st = fmt_ctx->streams[this->stream_index];

        /* find decoder for the stream */
        this->dec = avcodec_find_decoder(this->st->codecpar->codec_id);
        if (!this->dec) {
            fprintf(stderr, "Failed to find %s codec\n",
                    av_get_media_type_string(type));
            return AVERROR(EINVAL);
        }

        /* Allocate a codec context for the decoder */
        this->dec_ctx = avcodec_alloc_context3(this->dec);
        if (!this->dec_ctx) {
            fprintf(stderr, "Failed to allocate the %s codec context\n",
                    av_get_media_type_string(type));
            return AVERROR(ENOMEM);
        }

        /* Copy codec parameters from input stream to output codec context */
        if ((ret = avcodec_parameters_to_context(this->dec_ctx, this->st->codecpar)) < 0) {
            fprintf(stderr, "Failed to copy %s codec parameters to decoder context\n",
                    av_get_media_type_string(type));
            return ret;
        }

        /* Init the decoders */
        if ((ret = avcodec_open2(this->dec_ctx, this->dec, NULL)) < 0) {
            fprintf(stderr, "Failed to open %s codec\n",
                    av_get_media_type_string(type));
            return ret;
        }
    }

    return 0;
}


void FFmpegDecoder::ReleaseDecoder(){
    if(this->dec_ctx){
        avcodec_free_context(& this->dec_ctx);
        this->dec_ctx=nullptr;
    }
}
int FFmpegDecoder::DecodePacket(AVCodecContext *dec, const AVPacket *pkt){

        int ret = 0;
    AVFrame *frame;
        // submit the packet to the decoder
        ret = avcodec_send_packet(dec, pkt);
        if (ret < 0) {
            //fprintf(stderr, "Error submitting a packet for decoding (%s)\n", av_err2str(ret));
            return ret;
        }

        // get all the available frames from the decoder
        while (ret >= 0) {
            ret = avcodec_receive_frame(dec, frame);
            if (ret < 0) {
                // those two return values are special and mean there is no output
                // frame available, but there were no errors during decoding
                if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN))
                    return 0;

               // fprintf(stderr, "Error during decoding (%s)\n", av_err2str(ret));
                return ret;
            }

            // write the frame data to output file
            // if (dec->codec->type == AVMEDIA_TYPE_VIDEO)
            //     ret = output_video_frame(frame);
            // else
            //     ret = output_audio_frame(frame);

            av_frame_unref(frame);
            if (ret < 0)
                return ret;
        }

        return 0;

}
