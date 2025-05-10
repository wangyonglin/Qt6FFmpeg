#ifndef QT6YUV420PRENDERER_H
#define QT6YUV420PRENDERER_H
#include <QWidget>
#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_1>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLWidget>
#include "Qt6FFmpeg_global.h"


class QT6FFMPEG_EXPORT Qt6YUV420PRenderer: public QOpenGLWidget,public QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Qt6YUV420PRenderer(QWidget *parent = nullptr);
private:
    QOpenGLShaderProgram m_program;
    QOpenGLBuffer vbo;
    int idY,idU,idV;
    int width,height;
    uint8_t *ptr;
    QByteArray imgdata;
public slots:
    void slotUpdate(uint8_t * yuvdata,int yuvwidth,int yuvheight);
    // QOpenGLWidget interface
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
};


#endif // QT6YUV420PRENDERER_H
