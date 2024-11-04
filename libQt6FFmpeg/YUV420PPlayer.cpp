#include "YUV420PPlayer.h"
#include <QDebug>


QtRockchip::Qt6FFmpeg::YUV420PPlayer::YUV420PPlayer(QWidget *parent)
    :QOpenGLWidget(parent),
    demuxer(new FFmpegDemuxer(this)),
    audio_decoder(new AudioDecoder(this)),
    video_decoder(new VideoDecoder(this))
{


    connect(demuxer,&FFmpegDemuxer::reject,this,&Qt6FFmpeg::YUV420PPlayer::rejectCallback);
    connect(this->video_decoder,&VideoDecoder::sigFirst,[=](uint8_t * data[],int w,int h){
        ptr = *data;
        width = w;
        height = h;
    });
    connect(this->video_decoder,&VideoDecoder::newFrame,[=](){
        update();
    });


}

QtRockchip::Qt6FFmpeg::YUV420PPlayer::~YUV420PPlayer()
{

    demuxer->release();
    audio_decoder->release();
    video_decoder->release();

}
void QtRockchip::Qt6FFmpeg::YUV420PPlayer::play(const QString &url)
{

   demuxer->pause();
    audio_decoder->pause();
    video_decoder->pause();


    demuxer->synchronizer->init_synchronize();
    if(demuxer->demuxing(url)<0){
        return;
    }
    if(audio_decoder->execution(demuxer)<0){
        return;
    }
    if(video_decoder->execution(demuxer)<0){
        return;
    }



    audio_decoder->resume();
    video_decoder->resume();
    demuxer->resume();


}

void QtRockchip::Qt6FFmpeg::YUV420PPlayer::pause()
{
    demuxer->pause();
    audio_decoder->pause();
    video_decoder->pause();
}

void QtRockchip::Qt6FFmpeg::YUV420PPlayer::resume()
{
    audio_decoder->resume();
    video_decoder->resume();
    demuxer->resume();
}

void QtRockchip::Qt6FFmpeg::YUV420PPlayer::stop()
{


    audio_decoder->release();
    video_decoder->release();
    demuxer->release();
}



void QtRockchip::Qt6FFmpeg::YUV420PPlayer::initializeGL()
{
    initializeOpenGLFunctions();
    const char *vsrc =
        "attribute vec4 vertexIn; \
        attribute vec4 textureIn; \
        varying vec4 textureOut;  \
        void main(void)           \
    {                         \
            gl_Position = vertexIn; \
            textureOut = textureIn; \
    }";

        const char *fsrc =
        "varying vec4 textureOut;\n"
        "uniform sampler2D textureY;\n"
        "uniform sampler2D textureU;\n"
        "uniform sampler2D textureV;\n"
        "void main(void)\n"
        "{\n"
        "vec3 yuv; \n"
        "vec3 rgb; \n"
        "yuv.x = texture2D(textureY, textureOut.st).r; \n"
        "yuv.y = texture2D(textureU, textureOut.st).r - 0.5; \n"
        "yuv.z = texture2D(textureV, textureOut.st).r - 0.5; \n"
        "rgb = mat3( 1,       1,         1, \n"
        "0,       -0.39465,  2.03211, \n"
        "1.13983, -0.58060,  0) * yuv; \n"
        "gl_FragColor = vec4(rgb, 1); \n"
        "}\n";

    m_program.addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,vsrc);
    m_program.addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,fsrc);
    m_program.link();

    GLfloat points[]{
        -1.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, -1.0f,
        -1.0f, -1.0f,

        0.0f,0.0f,
        1.0f,0.0f,
        1.0f,1.0f,
        0.0f,1.0f
    };

    vbo.create();
    vbo.bind();
    vbo.allocate(points,sizeof(points));

    GLuint ids[3];
    glGenTextures(3,ids);
    idY = ids[0];
    idU = ids[1];
    idV = ids[2];
}

void QtRockchip::Qt6FFmpeg::YUV420PPlayer::resizeGL(int w, int h)
{
    if(h<=0) h=1;

    glViewport(0,0,w,h);
}

void QtRockchip::Qt6FFmpeg::YUV420PPlayer::paintGL()
{
    if(!ptr) return;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    m_program.bind();
    vbo.bind();
    m_program.enableAttributeArray("vertexIn");
    m_program.enableAttributeArray("textureIn");
    m_program.setAttributeBuffer("vertexIn",GL_FLOAT, 0, 2, 2*sizeof(GLfloat));
    m_program.setAttributeBuffer("textureIn",GL_FLOAT,2 * 4 * sizeof(GLfloat),2,2*sizeof(GLfloat));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,idY);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RED,width,height,0,GL_RED,GL_UNSIGNED_BYTE,ptr);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,idU);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RED,width >> 1,height >> 1,0,GL_RED,GL_UNSIGNED_BYTE,ptr + width*height);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,idV);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RED,width >> 1,height >> 1,0,GL_RED,GL_UNSIGNED_BYTE,ptr + width*height*5/4);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    m_program.setUniformValue("textureY",0);
    m_program.setUniformValue("textureU",1);
    m_program.setUniformValue("textureV",2);
    glDrawArrays(GL_QUADS,0,4);
    m_program.disableAttributeArray("vertexIn");
    m_program.disableAttributeArray("textureIn");
    m_program.release();
}


void QtRockchip::Qt6FFmpeg::YUV420PPlayer::rejectCallback(int err)
{
    emit reject(err);
}
