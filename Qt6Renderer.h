#ifndef QT6RGBARENDERER_H
#define QT6RGBARENDERER_H


#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLWidget>


#include "Qt6Core.h"


class QT6FFMPEG_EXPORT Qt6RGBARenderer : public QOpenGLWidget,public QOpenGLFunctions
{
    Q_OBJECT
public:

    explicit Qt6RGBARenderer(QWidget *parent = nullptr);
    ~Qt6RGBARenderer();


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
    void refresh(uint8_t * dstImageData,int dstImageWidth,int dstImageHeight);

};

#endif // QT6RGBARENDERER_H
