#ifndef QT6DEMUXCONTEXT_H
#define QT6DEMUXCONTEXT_H

#include <QObject>
#include "Qt6ConfigContext.h"
#include "Qt6Process.h"

class Qt6DemuxContext : public Qt6Process
{
    Q_OBJECT
public:
    explicit Qt6DemuxContext(QObject *parent = nullptr);
    void init(Qt6ConfigContext * config);
    bool open();
    void close();
    bool read(AVPacket **pkt);
    Qt6ConfigContext * config();
private:
    Qt6ConfigContext * m_config=nullptr;
    QMutex m_mutex;
    QWaitCondition m_condition;
signals:
    void finish();
};



#endif // QT6DEMUXCONTEXT_H
