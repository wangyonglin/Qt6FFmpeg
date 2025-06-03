#ifndef QT6SOUNDCONTEXT_H
#define QT6SOUNDCONTEXT_H

#include "Qt6Object.h"
#include "Qt6Process.h"
#include "Qt6ConfigContext.h"
class Qt6SoundContext : public Qt6Process
{
    Q_OBJECT
public:
    explicit Qt6SoundContext(QObject *parent = nullptr);
    bool open();
    bool receive(AVPacket *pkt,AVFrame**frame);
    QSize getSize();
    void close();
    void init(Qt6ConfigContext * config);
    Qt6ConfigContext * config();
    QMutex m_mutex;
    QWaitCondition m_condition;
    Qt6ConfigContext * m_config;
};

#endif // QT6SOUNDCONTEXT_H
