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
#include <QMutex>
#include <QWaitCondition>
#include "Qt6Object.h"


class QT6FFMPEG_EXPORT Qt6YUV420PRenderer: public QOpenGLWidget,public QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Qt6YUV420PRenderer(QWidget *parent = nullptr);
private:
    QOpenGLShaderProgram m_program;
    QOpenGLBuffer vbo;
    int idY,idU,idV;
    QSize mdst_size;
    uint8_t* mdst_ydata;
    uint8_t* mdst_udata;
    uint8_t* mdst_vdata;

    QMutex mutex;
    QWaitCondition waitCondition;
public slots:
    //void refresh(uint8_t * data,QSize size);
    void refresh(uint8_t * y,uint8_t * u,uint8_t * v,QSize size);
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
};


#endif // QT6YUV420PRENDERER_H
