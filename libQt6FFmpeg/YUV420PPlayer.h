#ifndef YUV420PPLAYER_H
#define YUV420PPLAYER_H

#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLWidget>
#include "AudioDecoder.h"
#include "FFmpegDemuxer.h"
#include "VideoDecoder.h"
#include "FFmpegParameters.h"
#include "libQt6FFmpeg_global.h"

namespace Qt6FFmpeg {
class LIBQT6FFMPEG_EXPORT YUV420PPlayer : public QOpenGLWidget,public QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit YUV420PPlayer(QWidget *parent = nullptr);
    ~YUV420PPlayer();
    void play(const QString & url);
    void pause();
    void resume();
    void stop();


private:

    FFmpegDemuxer *demuxer=nullptr;
    AudioDecoder *audio_decoder=nullptr;
    VideoDecoder *video_decoder=nullptr;

    QOpenGLShaderProgram m_program;
    QOpenGLBuffer vbo;
    int idY,idU,idV;
    int width,height;
   // uchar* ptr;
    uint8_t *ptr;
signals:
    void reject(int err);
    //void resolve();
public slots:
   // void resolveCallback();
    void rejectCallback(int err);

    // QOpenGLWidget interface
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
};

}


#endif // YUV420PPLAYER_H
