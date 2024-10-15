// #ifndef QFFMPEGMANAGER_H
// #define QFFMPEGMANAGER_H

// #include "QFFmpegHeaders.h"
// #include <QWidget>
// #include "FFmpegPacket.h"
// #include "FFmpegFrame.h"
// #include "FFmpegSynchronizer.h"

// class FFmpegParameters : public FFmpegSynchronizer
// {
//     Q_OBJECT
// public:
//     explicit FFmpegParameters(QWidget *parent = nullptr);
// public:
//     FFmpegPacket *audio_pkt_queue=nullptr;
//     FFmpegPacket *video_pkt_queue=nullptr;
//     FFmpegFrame *audio_frame_queue=nullptr;
//     FFmpegFrame *video_frame_queue=nullptr;
//     const    AVCodec * codec=nullptr;
//     QString url;
//     AVFormatContext *ifmt_ctx=nullptr;
//     int audio_stream_index=-1;
//     int video_stream_index=-1;
//     AVCodecParameters * audio_codecpar=nullptr;
//     AVCodecParameters * video_codecpar=nullptr;
//     AVRational video_pts_base = {0,0};
//     AVRational audio_pts_base = {0,0};
//     int64_t audio_pts_begin = 0;
//     int64_t video_pts_begin = 0;
//     AVCodecContext *audio_codec_ctx=nullptr;
//     AVCodecContext * video_codec_ctx=nullptr;
//     struct SwrContext *swr_ctx;
//     int64_t dst_ch_layout =AV_CH_LAYOUT_STEREO;
//     int dst_rate=44100;
//     enum AVSampleFormat dst_sample_fmt=AV_SAMPLE_FMT_S16;
//     uint8_t** src_data_;
//     uint8_t** dst_data_;
//     int src_nb_channels, dst_nb_channels;
//     int src_linesize, dst_linesize;
//     int src_nb_samples_, dst_nb_samples_;
//     enum AVSampleFormat dst_sample_fmt_;
//     enum AVSampleFormat src_sample_fmt_;
//     int data_size=0;
//     //Video Readerer
//     int YUV420BufferSize=0;
//     uchar *YUV420Buffer= nullptr;
//     bool isFirst = true;
//     int base_cache = 4;
// private:
//     QMutex mutex;
//     QWaitCondition waitCondition;
// };

// #endif // QFFMPEGMANAGER_H
