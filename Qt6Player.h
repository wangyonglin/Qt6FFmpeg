#ifndef QT6PLAYER_H
#define QT6PLAYER_H

#include <QWidget>
#include "Qt6Renderer.h"
#include "Qt6Swscaler.h"
#include "Qt6FFmpegProcess.h"

class Qt6Player : public Qt6Renderer
{
    Q_OBJECT
public:
    explicit Qt6Player(QWidget *parent = nullptr);
    ~Qt6Player();
    void play(const QString &url);
public:
    Qt6FFmpegProcess *ffmpeg_process=nullptr;
signals:
    void start(QThread::Priority pri = QThread::InheritPriority);
    void stop();
};

#endif // QT6PLAYER_H
