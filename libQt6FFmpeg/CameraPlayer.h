#ifndef CAMERAPLAYER_H
#define CAMERAPLAYER_H

#include <QWidget>
#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_1>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLWidget>
#include "libQt6FFmpeg_global.h"

#include "CameraCapturer.h"

namespace QtRockchip::Qt6FFmpeg {
class LIBQT6FFMPEG_EXPORT CameraPlayer : public QOpenGLWidget,public QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit CameraPlayer(QWidget *parent = nullptr);
    ~CameraPlayer();
    void play(const QString & url);
    void pause();
    void resume();
    void stop();
private:
    CameraCapturer *capture;
    QOpenGLShaderProgram m_program;
    QOpenGLBuffer vbo;
    int idY,idU,idV;
    int width,height;
    // uchar* ptr;
    uint8_t *ptr;
signals:
    void reject(int err);

public slots:
    void rejectCallback(int err);

    // QOpenGLWidget interface
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
};
}
#endif // CAMERAPLAYER_H
