#include "Qt6RGBARenderer.h"
#include <QOpenGLExtraFunctions>
#include <QDebug>
#include <QImageReader>
#include <QResizeEvent>

Qt6RGBARenderer::Qt6RGBARenderer(QWidget *parent)
    :QOpenGLWidget(parent)
{

}

Qt6RGBARenderer::~Qt6RGBARenderer()
{

}


void Qt6RGBARenderer::initializeGL()
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

void Qt6RGBARenderer::resizeGL(int w, int h)
{
    if(h<=0) h=1;
    glViewport(0,0,w,h);
}

void Qt6RGBARenderer::paintGL()
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

void Qt6RGBARenderer::refresh()
{
    if(finished){
        QImage glImage(dstImageData,dstImageWidth,dstImageHeight,QImage::Format_RGBA8888);
        currentFrame = glImage.copy();
        this->update();
    }
}

