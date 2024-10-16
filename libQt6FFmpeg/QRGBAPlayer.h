#ifndef QRGBAPLAYER_H
#define QRGBAPLAYER_H


#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
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
class LIBQT6FFMPEG_EXPORT QRGBAPlayer : public QOpenGLWidget,public QOpenGLFunctions
{
    Q_OBJECT
public:

    explicit QRGBAPlayer(QWidget *parent = nullptr);
    ~QRGBAPlayer();
    void play(const QString & url);
    void pause();
    void resume();
    void stop();

public slots:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
private:
    FFmpegDemuxer *demuxer=nullptr;
    AudioDecoder *audio_decoder=nullptr;
    VideoDecoder *video_decoder=nullptr;
    QOpenGLShaderProgram *shaderProgram;
    QOpenGLTexture *texture;
    GLuint textureID;
    QImage currentFrame;
    uchar *dstImageData=nullptr;
    int dstImageWidth=800;
    int dstImageHeight=600;
    bool finished = false;
signals:
    void reject(int err);//void resolve();
public slots:
    void rejectCallback(int err);
    void refresh();

};

}

#endif // QRGBAPLAYER_H
