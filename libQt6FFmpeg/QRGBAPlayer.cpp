#include "QRGBAPlayer.h"
#include <QOpenGLExtraFunctions>
#include <QDebug>
#include <QImageReader>
#include <QResizeEvent>
Qt6FFmpeg::QRGBAPlayer::QRGBAPlayer(QWidget *parent)
    :QOpenGLWidget(parent),
    demuxer(new FFmpegDemuxer(this)),
    audio_decoder(new AudioDecoder(this)),
    video_decoder(new VideoDecoder(this))
{
    connect(demuxer,&FFmpegDemuxer::reject,this,&Qt6FFmpeg::QRGBAPlayer::rejectCallback);
    connect(this->video_decoder,&VideoDecoder::sigFirst,[=](uint8_t * data[],int w,int h){
        dstImageData =*data;
        dstImageWidth=w;
        dstImageHeight=h;
    });
    connect(this->video_decoder,&VideoDecoder::newFrame,[&](){
        QImage frame(dstImageData,dstImageWidth,dstImageHeight,QImage::Format_RGBA8888);
        currentFrame=frame.copy();
        this->update();
    });
}


void Qt6FFmpeg::QRGBAPlayer::play(const QString &url)
{
    demuxer->pause();
    audio_decoder->pause();
    video_decoder->pause();
    demuxer->synchronizer->init_synchronize();
    if(demuxer->running(url)<0){
        return;
    }
    if(audio_decoder->running(demuxer)<0){
        return;
    }
    //AV_PIX_FMT_RGBA
    if(video_decoder->running(demuxer,AV_PIX_FMT_RGBA)<0){
        return;
    }
    audio_decoder->resume();
    video_decoder->resume();
    demuxer->resume();
}

void Qt6FFmpeg::QRGBAPlayer::pause()
{
    demuxer->pause();
    audio_decoder->pause();
    video_decoder->pause();
}

void Qt6FFmpeg::QRGBAPlayer::resume()
{
    audio_decoder->resume();
    video_decoder->resume();
    demuxer->resume();
}

void Qt6FFmpeg::QRGBAPlayer::stop()
{
    audio_decoder->release();
    video_decoder->release();
    demuxer->release();
}

void Qt6FFmpeg::QRGBAPlayer::rejectCallback(int err)
{
    emit reject(err);
}

void Qt6FFmpeg::QRGBAPlayer::resizeEvent(QResizeEvent *event)
{
    resizeGL(event->size().width(),event->size().height());
}


Qt6FFmpeg::QRGBAPlayer::~QRGBAPlayer()
{
    demuxer->release();
    audio_decoder->release();
    video_decoder->release();
}

void Qt6FFmpeg::QRGBAPlayer::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0, 0.0, 0.0, 1.0);  // 设置背景色

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



}

void Qt6FFmpeg::QRGBAPlayer::resizeGL(int w, int h)
{
   // if(h<=0) h=1;
    //glViewport(0,0,w,h);
    qDebug() << tr("width:%0 height:%1").arg(w).arg(h);
    QImage frame= currentFrame.scaled(w*4, h*4).scaled(w,h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    currentFrame=frame.copy();
}

void Qt6FFmpeg::QRGBAPlayer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (!currentFrame.isNull()) {
        // 创建 OpenGL 纹理
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // 设置纹理参数
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // 将视频帧数据上传到纹理
        QImage glImage = currentFrame.convertToFormat(QImage::Format_RGBA8888);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glImage.width(), glImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glImage.bits());

        // 启用纹理和绘制四边形
        glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        // 删除纹理
        glDeleteTextures(1, &textureID);
    }

}


