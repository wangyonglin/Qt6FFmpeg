#ifndef QT6CAMERAPROCESS_H
#define QT6CAMERAPROCESS_H

#include <QObject>
#include <QThread>
#include "Qt6CameraContext.h"
#include "Qt6Swscaler.h"

class Qt6CameraProcess : public QThread
{
    Q_OBJECT
public:
    explicit Qt6CameraProcess(QObject *parent = nullptr);
    ~Qt6CameraProcess();
public slots:
     void url(const QString &url);
    void start();
     void stop();
    void setting(const QSize &size,int framerate);
private:
    Qt6CameraContext *cctx=nullptr;
    void onDemuxHandle();
    void onVideoHandle();
    bool demux_flag=false;
    bool video_flag=false;
    Qt6Swscaler *sws=nullptr;
signals:

    // QThread interface
protected:
    virtual void run() override;
signals:
    void refresh(uint8_t *, const QSize size);
    //void refresh(uint8_t *y, uint8_t *u, uint8_t *v, QSize size);
};

#endif // QT6CAMERAPROCESS_H
