#ifndef QT6PLAYER_H
#define QT6PLAYER_H

#include <QWidget>
#include "Qt6DemuxProcess.h"

#include "Qt6Renderer.h"
#include "Qt6Swscaler.h"
#include "Qt6VideoProcess.h"
#include "Qt6SoundProcess.h"
#include "Qt6ConfigContext.h"

class Qt6Player : public Qt6Renderer
{
    Q_OBJECT
public:
    explicit Qt6Player(QWidget *parent = nullptr);
    ~Qt6Player();
    void play(const QString &url);
public:
    Qt6ConfigContext *m_config=nullptr;
    Qt6DemuxProcess *m_demux_process= nullptr;
    Qt6SoundProcess *m_audio_process=nullptr;
    Qt6VideoProcess *m_video_process=nullptr;
   // Qt6VideoProcess *m_video_process=nullptr;
signals:
    void start(QThread::Priority pri = QThread::InheritPriority);
    void stop();
};

#endif // QT6PLAYER_H
