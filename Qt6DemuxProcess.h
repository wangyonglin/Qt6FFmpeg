#ifndef QT6DEMUXPROCESS_H
#define QT6DEMUXPROCESS_H

#include <QObject>
#include "Qt6Process.h"
#include "Qt6Packet.h"
#include "Qt6DemuxContext.h"

class Qt6DemuxProcess : public Qt6DemuxContext
{
    Q_OBJECT
public:
    explicit Qt6DemuxProcess(QObject *parent = nullptr);
    ~Qt6DemuxProcess();
// public slots:
//     void start();
//     void stop();

public:
    AVPacket *m_pkt=nullptr;
    bool m_exitloop =true;
public slots:
    void finish();
    // Qt6Process interface
protected:
    virtual void loopping() override;

    // Qt6Process interface
protected:
    virtual void opening() override;
};

#endif // QT6DEMUXPROCESS_H
