#ifndef QT6SOUNDPROCESS_H
#define QT6SOUNDPROCESS_H

#include <QObject>
#include "Qt6Speaker.h"
#include "Qt6Resampler.h"
#include "Qt6Process.h"
#include "Qt6SoundContext.h"
class Qt6SoundProcess : public Qt6SoundContext
{
    Q_OBJECT
public:
    explicit Qt6SoundProcess(QObject *parent = nullptr);
    ~Qt6SoundProcess();
    // void start();
    // void stop();
    bool m_exitloop=true;
private:

    Qt6Speaker * speaker=nullptr;
    Qt6Resampler *resampler=nullptr;


    // Qt6Process interface
protected:
    virtual void loopping() override;

    // Qt6Process interface
protected:
    virtual void opening() override;
};

#endif // QT6SOUNDPROCESS_H
