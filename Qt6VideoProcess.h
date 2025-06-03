#ifndef QT6VIDEOPROCESS_H
#define QT6VIDEOPROCESS_H

#include <QObject>

#include "Qt6DemuxProcess.h"
#include "Qt6Frame.h"
#include "Qt6Swscaler.h"
#include "Qt6Speaker.h"
#include "Qt6Resampler.h"
#include "Qt6SoundContext.h"
#include "Qt6ConfigContext.h"
#include "Qt6VideoContext.h"

class Qt6VideoProcess : public Qt6VideoContext
{
    Q_OBJECT
public:
    explicit Qt6VideoProcess(QObject *parent = nullptr);

public slots:
    // void start();
    // void stop();
public:
    Qt6Swscaler * swscaler=nullptr;
    uint8_t * mdst_scale=nullptr;
    bool m_exitloop=true;
signals:
    void refresh(uint8_t *, const QSize size);
protected:
    virtual void loopping() override;

    // Qt6Process interface
protected:
    virtual void opening() override;
};

#endif // QT6VIDEOPROCESS_H
