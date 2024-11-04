#include "RGBAPlayer.h"
#include <QOpenGLExtraFunctions>
#include <QDebug>
#include <QImageReader>
#include <QResizeEvent>

QtRockchip::Qt6FFmpeg::RGBAPlayer::RGBAPlayer(QWidget *parent)
    :QOpenGLWidget(parent),
    demuxer(new FFmpegDemuxer(this)),
    audio_decoder(new AudioDecoder(this)),
    video_decoder(new VideoDecoder(this))
{
    connect(demuxer,&FFmpegDemuxer::reject,this,&Qt6FFmpeg::RGBAPlayer::rejectCallback);
    connect(this->video_decoder,&VideoDecoder::sigFirst,[=](uint8_t * data[],int w,int h){
        dstImageData =*data;
        dstImageWidth=w;
        dstImageHeight=h;
    });
    connect(this->video_decoder,&VideoDecoder::newFrame,this,&Qt6FFmpeg::RGBAPlayer::refresh);
}


void QtRockchip::Qt6FFmpeg::RGBAPlayer::play(const QString &url)
{
    // demuxer->pause();
    // audio_decoder->pause();
    // video_decoder->pause();
    demuxer->synchronizer->init_synchronize();
    if(demuxer->demuxing(url)<0){
        return;
    }
    if(audio_decoder->execution(demuxer)<0){
        return;
    }
    //AV_PIX_FMT_RGBA
    if(video_decoder->execution(demuxer,AV_PIX_FMT_RGBA)<0){
        return;
    }
    // audio_decoder->resume();
    // video_decoder->resume();
    // demuxer->resume();
}

void QtRockchip::Qt6FFmpeg::RGBAPlayer::pause()
{
    demuxer->pause();
    audio_decoder->pause();
    video_decoder->pause();
}

void QtRockchip::Qt6FFmpeg::RGBAPlayer::resume()
{
    audio_decoder->resume();
    video_decoder->resume();
    demuxer->resume();
}

void QtRockchip::Qt6FFmpeg::RGBAPlayer::stop()
{
    audio_decoder->release();
    video_decoder->release();
    demuxer->release();
}

void QtRockchip::Qt6FFmpeg::RGBAPlayer::rejectCallback(int err)
{
    emit reject(err);
}



QtRockchip::Qt6FFmpeg::RGBAPlayer::~RGBAPlayer()
{    demuxer->release();
    audio_decoder->release();
    video_decoder->release();
}

void QtRockchip::Qt6FFmpeg::RGBAPlayer::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f,1.0f);  // 设置背景色

    // 创建着色器程序
    shaderProgram = new QOpenGLShaderProgram(this);
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex.vert");
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment.frag");
    shaderProgram->link();

    // 设置纹理
    texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture->setMinificationFilter(QOpenGLTexture::Linear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::ClampToEdge);

    // 创建 OpenGL 纹理
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    finished=true;
}

void QtRockchip::Qt6FFmpeg::RGBAPlayer::resizeGL(int w, int h)
{
    if(h<=0) h=1;
    glViewport(0,0,w,h);
}

void QtRockchip::Qt6FFmpeg::RGBAPlayer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (!currentFrame.isNull()) {
        // 将视频帧数据上传到纹理
        QImage glImage = currentFrame.convertToFormat(QImage::Format_RGBA8888);
        if(!glImage.isNull()){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glImage.width(), glImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glImage.bits());
        }
        // 启用纹理和绘制四边形
        glBindTexture(GL_TEXTURE_2D, textureID);
        glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
}

void QtRockchip::Qt6FFmpeg::RGBAPlayer::refresh()
{
    if(finished){
        QImage glImage(dstImageData,dstImageWidth,dstImageHeight,QImage::Format_RGBA8888);
        currentFrame = glImage.copy();
        this->update();
    }
}

