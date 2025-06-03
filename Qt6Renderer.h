#ifndef QT6RENDERER_H
#define QT6RENDERER_H


#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLWidget>


#include "Qt6ConfigContext.h"


class QT6FFMPEG_EXPORT Qt6Renderer : public QOpenGLWidget,public QOpenGLFunctions
{
    Q_OBJECT
public:

    explicit Qt6Renderer(QWidget *parent = nullptr);
    ~Qt6Renderer();


public slots:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
private:
    QOpenGLShaderProgram *shaderProgram;
    QOpenGLTexture *texture;
    GLuint textureID;
    QImage currentFrame;
    // uchar *dstImageData=nullptr;
    // int dstImageWidth=800;
    // int dstImageHeight=600;
    bool finished = false;
signals:
public slots:
    void refresh(uint8_t * dstImageData,const QSize & size);

};

#endif // QT6RENDERER_H
