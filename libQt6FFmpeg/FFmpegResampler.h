#ifndef FFmpegResampler_H
#define FFmpegResampler_H
extern "C" {
#include <libavutil/pixfmt.h>
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include <libavutil/opt.h>
#include <libavutil/channel_layout.h>
#include <libavutil/samplefmt.h>
#include <libavutil/time.h>
#include <libswresample/swresample.h>
}

#include <QObject>
#include <QDebug>

class FFmpegResampler :public QObject{
public:
    explicit FFmpegResampler(QObject *parent = nullptr);


    int init(AVCodecContext *dec_ctx, int64_t dst_ch_layout, int dst_rate, AVSampleFormat dst_sample_fmt);
    QByteArray convert(AVFrame *frame);
    void release();

private:
    struct SwrContext* swr_ctx=nullptr;
    uint8_t** src_data_=nullptr;
    uint8_t** dst_data_=nullptr;
    int src_nb_channels, dst_nb_channels;
    int src_linesize, dst_linesize;
    int src_nb_samples_, dst_nb_samples_;
    enum AVSampleFormat dst_sample_fmt_;
    enum AVSampleFormat src_sample_fmt_;
    int data_size=0;

};


#endif // FFmpegResampler_H
